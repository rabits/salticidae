#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtQml>

#include <protoeye.h>

class PluginManager
{
public:
    virtual ~PluginManager() {}

    static ProtoEye* eye(QUrl url);

    // Get list of found supported sources by loaded plugins
    static QList<QUrl> sources();

    // Get list of supported schemes by loaded plugins
    static QStringList schemes();

    // Init & load internal and external plugins
    static void initPlugins();

    // Register plugin QML types
    static void registerQmlPluginTypes();

    static QMap<QUrl, ProtoEye*> s_eyes;

private:
    static QList<ProtoEye*> s_eyes_plugins;

    // Load found plugin
    static void loadPlugin(QObject *plugin, QString path = "internal plugin");
};

#endif // PLUGINMANAGER_H
