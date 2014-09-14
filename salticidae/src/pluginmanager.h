#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

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

    // Register dummy QML objects
    static void registerQmlType() {
        qmlRegisterType<QObject>(ProtoEye_iid, 1, 0, "Dummy" );
    }

private:
    static QList<ProtoEye*> s_eyes_plugins;

    // Load found plugin
    static void loadPlugin(QObject *plugin, QString path = "internal plugin");
};

#endif // PLUGINMANAGER_H
