#ifndef EYEDISPLAY_H
#define EYEDISPLAY_H

#include <QAbstractVideoSurface>

#include <protoeye.h>

class EyeDisplay
    : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )

public:
    explicit EyeDisplay(QObject *parent = 0);
    ~EyeDisplay();

    // Interface for VideoOutput
    QAbstractVideoSurface* videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface* surface);

    // Connect Eye as source of pictures
    Q_INVOKABLE void setSource(QString url);

    Q_INVOKABLE ProtoEye* getSource();

public slots:
    void start();
    void stop();

private slots:
    void present(QImage image);

private:
    void closeSurface();

    QAbstractVideoSurface* _vs;

    bool      _connected;
    ProtoEye* _eye;
};

#endif // EYEDISPLAY_H
