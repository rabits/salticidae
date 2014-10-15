#include "eyedisplay.h"

#include <QVideoSurfaceFormat>

#include "pluginmanager.h"

EyeDisplay::EyeDisplay(QObject *parent)
    : QObject(parent)
    , _vs(NULL)
    , _connected(false)
    , _eye(NULL)
{
}

QAbstractVideoSurface* EyeDisplay::videoSurface() const
{
    return _vs;
}

void EyeDisplay::setVideoSurface(QAbstractVideoSurface* surface)
{
    closeSurface();
    _vs = surface;
}

void EyeDisplay::setSource(QString url)
{
    qDebug() << "[Salticidae] Set eye for display";
    stop();
    _eye = PluginManager::eye(QUrl(url));
}

ProtoEye* EyeDisplay::getSource()
{
    return _eye;
}

void EyeDisplay::closeSurface()
{
    if( _vs && _vs->isActive() ) {
        _vs->stop();
    }
}

void EyeDisplay::start()
{
    qDebug() << "[Salticidae] Starting showing video";

    if( ! _eye ) {
        qDebug() << "[Salticidae]   error: eye is not set";
        return;
    }

    if( ! _connected )
        _connected = connect(_eye, &ProtoEye::present, this, &EyeDisplay::present);

    closeSurface();
    _vs->start(QVideoSurfaceFormat(_eye->size(), QVideoFrame::Format_RGB32));

    _eye->start();
}

void EyeDisplay::stop()
{
    qDebug() << "[Salticidae] Stopping showing video";

    if( _eye ) {
        if( _connected )
            _connected = ! disconnect(_eye, &ProtoEye::present, this, &EyeDisplay::present);
        _eye->stop();
    }

    closeSurface();
}

void EyeDisplay::present(QImage *image)
{
    _vs->present(QVideoFrame(*image));
}
