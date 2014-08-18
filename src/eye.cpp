#include "eye.h"

#include <QtQml>

#include <QDebug>

#define SAT(c) \
    if (c & (~255)) { if (c < 0) c = 0; else c = 255; }

#define LIMIT(x)  ( (x) > 0xffff ? 0xff : ( (x) <= 0xff ? 0 : ( (x) >> 8 ) ) )

/*#include <QDir>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>*/

void Eye::registerQmlType()
{
    qmlRegisterType<Eye>("org.rabits.eyes", 1, 0, "Eye" );
}

Eye::Eye(QObject *parent)
    : QObject(parent)
    , _timer_id(0)
    , _vs(NULL)
    , _device(NULL)
    , _frame(NULL)
    , _rgb_frame(NULL)
{
    _device = v4l2_create_device("/dev/video0");
    v4l2_open_device(_device);

    v4l2_format_t _format;
    v4l2_get_format(_device, &_format);

    _width = _format.width;
    _height = _format.height;

    qDebug() << "Created eye width: " << _width << " height: " << _height;

    _frame = new unsigned char[v4l2_get_buffer_size(_device)];
    _rgb_frame = new unsigned char[_width * _height * 3];
}

Eye::Eye(const QByteArray &device, QObject *parent)
    : QObject(parent)
    , _timer_id(0)
    , _vs(NULL)
    , _device(NULL)
    , _frame(NULL)
    , _rgb_frame(NULL)
{
    _device = v4l2_create_device(device.data());
    v4l2_open_device(_device);

    v4l2_format_t _format;
    v4l2_get_format(_device, &_format);

    _width = _format.width;
    _height = _format.height;

    QString fourcc = "";
    fourcc += _format.pixel_format & 0xff;
    fourcc += (_format.pixel_format >> 8) & 0xff;
    fourcc += (_format.pixel_format >> 16) & 0xff;
    fourcc += (_format.pixel_format >> 24) & 0xff;
    qDebug() << "Created eye width: " << _width << " height: " << _height << " FourCC: " << fourcc;

    _frame = new unsigned char[v4l2_get_buffer_size(_device)];
    _rgb_frame = new unsigned char[_width * _height * 4]; // RGB32
}

Eye::~Eye()
{
    stopCapture();
    if( _device != NULL ) {
        v4l2_close_device(_device);
        v4l2_destroy_device(_device);
    }
    delete[] _frame;
    delete[] _rgb_frame;
    closeSurface();
}

QAbstractVideoSurface* Eye::videoSurface() const
{
    return _vs;
}

void Eye::setVideoSurface(QAbstractVideoSurface* surface)
{
    closeSurface();
    _vs = surface;
}

void Eye::closeSurface()
{
    if( _vs && _vs->isActive() )
        _vs->stop();
}

void Eye::startCapture()
{
    if ( _timer_id == 0 ) {
        qDebug() << "Starting capture";
        closeSurface();
        _vsformat = QVideoSurfaceFormat(QSize(_width, _height), QVideoFrame::Format_RGB32);
        _vs->start(_vsformat);
        v4l2_start_capture(_device);
        _timer_id = startTimer(0);
    }
}

void Eye::stopCapture()
{
    if ( _timer_id != 0 ) {
        qDebug() << "Stopping capture";
        killTimer(_timer_id);
        v4l2_stop_capture(_device);
        closeSurface();
    }
}

void Eye::timerEvent(QTimerEvent*)
{
    if( ! _vs )
        return;
    v4l2_grab_frame(_device);
    v4l2_copy_frame(_device, _frame);
    ccvt_yuyv(_width, _height, _frame, _rgb_frame);

    QImage screenImage = QImage(_rgb_frame, _width, _height, QImage::Format_RGB32);

    qDebug() << "Working...";

    _vs->present(QVideoFrame(screenImage));
    screenImage.save(QString("/sdcard/image.bmp"));
}

void Eye::ccvt_yuyv(int width, int height, const unsigned char *src, unsigned char *dst)
{
    int line, col;
    int y, yy;
    int u, v;
    int vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;

    py = src;
    pu = src + 1;
    pv = src + 3;

    y = *py;
    yy = y << 8;
    u = *pu - 128;
    ug = 88 * u;
    ub = 454 * u;
    v = *pv - 128;
    vg = 183 * v;
    vr = 359 * v;

    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = LIMIT(yy + vr);
            g = LIMIT(yy - ug - vg);
            b = LIMIT(yy + ub );
            *dst++ = r;
            *dst++ = g;
            *dst++ = b;
            *dst++ = 0;
            py += 2;
            y = *py;
            yy = y << 8;
            if ( (col & 1) == 1) {
                pu += 4; // skip yvy every second y
                pv += 4; // skip yuy every second y
            }
            u = *pu - 128;
            ug = 88 * u;
            ub = 454 * u;
            v = *pv - 128;
            vg = 183 * v;
            vr = 359 * v;
        } // ..for col
    } /* ..for line */
}

/*
QList<QByteArray> Eye::m_rawDevices;
QStringList Eye::m_rawDescriptions;

QList<QByteArray> Eye::availableDevices()
{
    if( m_rawDevices.isEmpty() )
        updateRawDevices();

    return QCamera::availableDevices() + m_rawDevices;
}

QString Eye::deviceDescription(const QByteArray &device)
{
    QString desc = QCamera::deviceDescription(device);

    if( ! desc.isEmpty() ) {
        return desc;
    }

    if (m_rawDevices.isEmpty())
        updateRawDevices();

    for (int i=0; i<m_rawDevices.count(); i++)
        if (m_rawDevices[i] == device)
            return m_rawDescriptions[i];

    return QString();
}

void Eye::updateRawDevices()
{
    m_rawDevices.clear();
    m_rawDescriptions.clear();

    QDir devDir("/dev");
    devDir.setFilter(QDir::System);

    QFileInfoList entries = devDir.entryInfoList(QStringList() << "video*");

    foreach (const QFileInfo &entryInfo, entries) {
        int fd = ::open(entryInfo.filePath().toLatin1().constData(), O_RDWR );
        if (fd == -1)
            continue;

        bool isCamera = false;

        v4l2_input input;
        memset(&input, 0, sizeof(input));
        for (; ::ioctl(fd, VIDIOC_ENUMINPUT, &input) >= 0; ++input.index) {
            if (input.type == V4L2_INPUT_TYPE_CAMERA || input.type == 0) {
                isCamera = ::ioctl(fd, VIDIOC_S_INPUT, input.index) != 0;
                break;
            }
        }

        if (isCamera) {
            // find out its driver "name"
            QString name;
            struct v4l2_capability vcap;
            memset(&vcap, 0, sizeof(struct v4l2_capability));

            if (ioctl(fd, VIDIOC_QUERYCAP, &vcap) != 0)
                name = entryInfo.fileName();
            else
                name = QString((const char*)vcap.card);
            //qDebug() << "found camera: " << name;

            m_rawDevices.append(entryInfo.filePath().toLocal8Bit());
            m_rawDescriptions.append(name);
        }
        ::close(fd);
    }
}
*/
