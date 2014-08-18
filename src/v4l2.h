#ifndef _V4L2_H
#define _V4L2_H

#include <stdlib.h>
#include <linux/videodev2.h>

#define V4L2_STATUS_OK 1
#define V4L2_STATUS_ERROR -1

typedef struct {
	void *start;
	size_t length;
} v4l2_buffer_t;

typedef struct {
	char name[32];
	int fd;
	unsigned int num_buffers;
	v4l2_buffer_t* buffers;
	unsigned char* data;
} v4l2_device_t;

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int pixel_format;
} v4l2_format_t;

v4l2_device_t* v4l2_create_device(const char* device_name);
void v4l2_destroy_device(v4l2_device_t *device);

int v4l2_open_device(v4l2_device_t *device);
int v4l2_close_device(v4l2_device_t *device);

size_t v4l2_get_buffer_size(v4l2_device_t* device);

int v4l2_set_format(v4l2_device_t* device, v4l2_format_t* format);
int v4l2_get_format(v4l2_device_t* device, v4l2_format_t* format);

int v4l2_start_capture(v4l2_device_t *device);
int v4l2_stop_capture(v4l2_device_t *device);

int v4l2_grab_frame(v4l2_device_t *device);

void v4l2_copy_frame(v4l2_device_t *device, unsigned char* dest);

#endif