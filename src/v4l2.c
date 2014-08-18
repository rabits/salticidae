#include "v4l2.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

static int xioctl(int fd, int request, void *arg) {
	int r;
	
	do {
		r = ioctl(fd, request, arg);	
	} while (-1 == r && EINTR == errno);
	
	return r;
}

v4l2_device_t* v4l2_create_device(const char* device_name) {
	v4l2_device_t* device = (v4l2_device_t*)malloc(sizeof(v4l2_device_t));
	
	strncpy(device->name, device_name, sizeof(device->name));
	
	return device;
}

void v4l2_destroy_device(v4l2_device_t* device) {
	free(device);
}

int v4l2_open_device(v4l2_device_t* device) {
	struct stat st;
	
	//identify device
	if (-1 == stat(device->name, &st)) {
		fprintf(stderr, "Cannot identify device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "Unrecognized device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	//open device
	device->fd = open(device->name, O_RDWR | O_NONBLOCK, 0);
	
	if (-1 == device->fd) {
		fprintf(stderr, "Unable to open device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	struct v4l2_capability cap;
	
	//query capabilities
	if (-1 == xioctl(device->fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "Error: %s is no V4L2 device\n", device->name);
		} else {
			fprintf(stderr, "Error querying capabilities on device: %s\n", device->name);
		}
		
		return V4L2_STATUS_ERROR;
	}
	
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "Error: %s is no video capture device\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "Error: %s does not support streaming\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	//init mmap
	struct v4l2_requestbuffers req;
	
	CLEAR(req);
	
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	
	if (-1 == xioctl(device->fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "Error: %s does not support memory mapping\n", device->name);
		} else {
			fprintf(stderr, "Error requesting buffers on device: %s\n", device->name);
		}
		
		return V4L2_STATUS_ERROR;
	}
	
	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	//allocate buffers
	device->buffers = (v4l2_buffer_t*)calloc(req.count, sizeof(v4l2_buffer_t));
	
	if (!device->buffers) {
		fprintf(stderr, "Out of memory");
		return V4L2_STATUS_ERROR;
	}
	
	//map buffers
    unsigned int i;
    for (i = 0; i < req.count; i++) {
		struct v4l2_buffer buf;
		
		CLEAR(buf);
		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if (-1 == xioctl(device->fd, VIDIOC_QUERYBUF, &buf)) {
			fprintf(stderr, "Unable to query buffers on %s\n", device->name);
			return V4L2_STATUS_ERROR;
		}
		
		device->buffers[i].length = buf.length;
		device->buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, device->fd, buf.m.offset);
		
		if (MAP_FAILED == device->buffers[i].start) {
			fprintf(stderr, "Unable to map buffers on %s\n", device->name);
			return V4L2_STATUS_ERROR;
		}		
	}
	
	device->num_buffers = req.count;
	
	return V4L2_STATUS_OK;
}

int v4l2_close_device(v4l2_device_t *device) {
    unsigned int i;
    for (i = 0; i < device->num_buffers; i++) {
		if (-1 == munmap(device->buffers[i].start, device->buffers[i].length)) {
			fprintf(stderr, "Unable to unmap buffers on %s\n", device->name);
			return V4L2_STATUS_ERROR;
		}
	}
	
	if (-1 == close(device->fd)) {
		fprintf(stderr, "Unable to close device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	return V4L2_STATUS_OK;
}

size_t v4l2_get_buffer_size(v4l2_device_t* device) {
	if (device->num_buffers > 0) {
		return device->buffers[0].length;
	} else {
		return 0;
	}
}

int v4l2_set_format(v4l2_device_t* device, v4l2_format_t* format) {
	struct v4l2_format fmt;
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = format->width;
	fmt.fmt.pix.height = format->height;
	fmt.fmt.pix.pixelformat = format->pixel_format;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	
	if (-1 == xioctl(device->fd, VIDIOC_S_FMT, &fmt)) {
		fprintf(stderr, "Could not set format on device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	} else {
		return V4L2_STATUS_OK;
	}
}

int v4l2_get_format(v4l2_device_t* device, v4l2_format_t* format) {
	struct v4l2_format fmt;
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	if (-1 == xioctl(device->fd, VIDIOC_G_FMT, &fmt)) {
		fprintf(stderr, "Could not get format on device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	format->width = fmt.fmt.pix.width;
	format->height = fmt.fmt.pix.height;
	format->pixel_format = fmt.fmt.pix.pixelformat;
	
	return V4L2_STATUS_OK;
}

int v4l2_start_capture(v4l2_device_t *device) {
	//allocate data buffer
	device->data = (unsigned char*)malloc(device->buffers[0].length);
	
	//queue buffers
    unsigned int i;
    for (i = 0; i < device->num_buffers; i++) {
		struct v4l2_buffer buf;
		
		CLEAR(buf);
		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if (-1 == xioctl(device->fd, VIDIOC_QBUF, &buf)) {
			fprintf(stderr, "Unable to queue buffers on device: %s\n", device->name);
			return V4L2_STATUS_ERROR;
		}
	}
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	//turn on stream
	if (-1 == xioctl(device->fd, VIDIOC_STREAMON, &type)) {
		fprintf(stderr, "Unable to turn on stream on device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	return V4L2_STATUS_OK;
}

int v4l2_stop_capture(v4l2_device_t *device) {
	//free data buffer
	free(device->data);
	
	//turn off stream
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	if (-1 == xioctl(device->fd, VIDIOC_STREAMOFF, &type)) {
		fprintf(stderr, "Unable to turn off stream on device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	return V4L2_STATUS_OK;
}

int v4l2_grab_frame(v4l2_device_t *device) {
	struct v4l2_buffer frame_buffer;
	
	//dequeue buffer
	CLEAR(frame_buffer);

	frame_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	frame_buffer.memory = V4L2_MEMORY_MMAP;
	
	if (-1 == xioctl(device->fd, VIDIOC_DQBUF, &frame_buffer)) {
		return V4L2_STATUS_ERROR;
	}
	
	memcpy(device->data, device->buffers[frame_buffer.index].start, frame_buffer.bytesused);

	//requeue buffer
	if (-1 == xioctl(device->fd, VIDIOC_QBUF, &frame_buffer)) {
		fprintf(stderr, "Could not requeue buffer on device: %s\n", device->name);
		return V4L2_STATUS_ERROR;
	}
	
	return V4L2_STATUS_OK;
}

void v4l2_copy_frame(v4l2_device_t *device, unsigned char* dest) {
	memcpy(dest, device->data, device->buffers[0].length);
}
