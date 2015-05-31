#include "eyedisplay.h"

#include <QVideoSurfaceFormat>

#include "pluginmanager.h"
#include "eyes.h"

EyeDisplay::EyeDisplay(QObject *parent)
    : QObject(parent)
    , _vs(NULL)
    , _connected(false)
    , _eye(NULL)
{
}

EyeDisplay::~EyeDisplay()
{
    stop();
    qDeleteAll(_transforms);
    delete _vs;
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
    _eye = PluginManager::video(QUrl(url));
}

ProtoVideo* EyeDisplay::getSource()
{
    return _eye;
}

void EyeDisplay::addTransform(QString name)
{
    _transforms.append(PluginManager::transform(name));
}

ProtoTransform *EyeDisplay::getTransform(QString name)
{
    ProtoTransform *transform = NULL;

    foreach( ProtoTransform *t, _transforms ) {
        if( t->name() == name ) {
            transform = t;
            break;
        }
    }

    return transform;
}

void EyeDisplay::deleteTransform(QString name)
{
    ProtoTransform *transform = getTransform(name);
    _transforms.removeOne(transform);
    delete transform;
}

QStringList EyeDisplay::getTransforms()
{
    QStringList transforms;

    foreach( ProtoTransform *t, _transforms ) {
        transforms.append(t->name());
    }

    return transforms;
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
        _connected = connect(_eye, &ProtoVideo::present, this, &EyeDisplay::present);

    closeSurface();
    _vs->start(QVideoSurfaceFormat(_eye->size(), QVideoFrame::Format_RGB32));

    _eye->start();
}

void EyeDisplay::stop()
{
    qDebug() << "[Salticidae] Stopping showing video";

    if( _eye ) {
        if( _connected )
            _connected = ! disconnect(_eye, &ProtoVideo::present, this, &EyeDisplay::present);
        _eye->stop();
    }

    closeSurface();
}

void EyeDisplay::present(QImage image)
{
    //qDebug() << "[DEBUG] Present & save image";
    foreach( ProtoTransform *transform, _transforms ) {
        image = transform->process(image);
    }
    if( _vs->nativeResolution() != image.size() ) {
        _vs->stop();
        _vs->start(QVideoSurfaceFormat(image.size(), QVideoFrame::Format_RGB32));
    }
    _vs->present(QVideoFrame(image));

    /*// TODO: Simple image recording
    QDateTime dt = QDateTime::currentDateTime();
    QDir dir = QDir(Eyes::I()->setting("recorder/write_dir").toString() + "/" + dt.toString("yyMMdd_hhmm"));
    dir.mkpath(".");
    image.save(dir.path() + "/" + dt.toString("yyMMdd_hhmmss_zzz") + ".jpg", "JPG");*/
}
