#ifndef EYE_H
#define EYE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

extern "C" {
    #include "v4l2.h"
}

class Eye
    : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )

public:
    static void registerQmlType();

    Eye(QObject *parent = 0);
    Eye(const QByteArray& device, QObject *parent = 0);
    ~Eye();

    QAbstractVideoSurface* videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface* surface);

public slots:
    void startCapture();
    void stopCapture();

protected:
    void timerEvent(QTimerEvent*);

private:
    void closeSurface();

    int _timer_id;

    QAbstractVideoSurface* _vs;
    QVideoSurfaceFormat    _vsformat;

    v4l2_device_t* _device;

    int _width, _height;
    unsigned char *_frame;
    unsigned char *_rgb_frame;

    void ccvt_yuyv(int width, int height, const unsigned char *src, unsigned char *dst);
};

#endif // EYE_H
