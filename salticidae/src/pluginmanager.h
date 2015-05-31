#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtQml>

#include <protovideo.h>
#include <prototransform.h>

class PluginManager
{
public:
    virtual ~PluginManager() {}

    static ProtoVideo* video(QUrl url);
    static ProtoTransform* transform(QString name);

    // Get list of found supported sources by loaded plugins
    static QList<QUrl> sources();

    // Get list of supported schemes by loaded plugins
    static QStringList schemes();

    // Get list of available transform plugins
    static QStringList transforms();

    // Init & load internal and external plugins
    static void initPlugins();

    // Register plugin QML types
    static void registerQmlPluginTypes();

    static QMap<QUrl, ProtoVideo*> s_videos;

private:
    static QList<ProtoVideo*> s_video_plugins;
    static QList<ProtoTransform*> s_transform_plugins;

    // Load found plugin
    static void loadPlugin(QObject *plugin, QString path = "internal plugin");
};

#endif // PLUGINMANAGER_H
