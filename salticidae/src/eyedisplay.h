#ifndef EYEDISPLAY_H
#define EYEDISPLAY_H

#include <QAbstractVideoSurface>

#include <protovideo.h>
#include <prototransform.h>

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

    Q_INVOKABLE ProtoVideo* getSource();

    Q_INVOKABLE void addTransform(QString name);
    Q_INVOKABLE ProtoTransform* getTransform(QString name);
    Q_INVOKABLE void deleteTransform(QString name);
    Q_INVOKABLE QStringList getTransforms();

public slots:
    void start();
    void stop();

private slots:
    void present(QImage image);

private:
    void closeSurface();

    QAbstractVideoSurface* _vs;

    bool      _connected;
    ProtoVideo* _eye;

    QList<ProtoTransform*> _transforms;
};

#endif // EYEDISPLAY_H
