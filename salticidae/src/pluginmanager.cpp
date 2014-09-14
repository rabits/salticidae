#include "pluginmanager.h"

#include <QPluginLoader>

QList<ProtoEye*> PluginManager::s_eyes_plugins = QList<ProtoEye*>();

void PluginManager::loadPlugin(QObject *plugin, QString plugin_name)
{
    ProtoPlugin *proto_plugin = dynamic_cast<ProtoPlugin*>(plugin);
    if( plugin ) {
        qDebug() << "[Salticidae]   found plugin type:" << proto_plugin->type() << "name:" << proto_plugin->name() << "version:" << proto_plugin->version();
        if( proto_plugin->type().compare("eye") == 0 ) {
            ProtoEye *plugin_typed = qobject_cast<ProtoEye*>(plugin);
            if( plugin_typed ) {
                plugin_typed->registerQmlType();
                s_eyes_plugins.append(plugin_typed);
                qDebug() << "[Salticidae]     loading success";
            } else {
                qDebug() << "[Salticidae]     loading failure";
            }
        } else {
            qDebug() << "[Salticidae]     skipped - unknown plugin type" << proto_plugin->type();
        }
    } else {
        qDebug() << "[Salticidae]   found & skipped unrecognized plugin" << plugin_name;
    }
}

void PluginManager::initPlugins()
{
    // Registering dummy types
    registerQmlType();

    qDebug("[Salticidae] Loading internal plugins");
    foreach (QObject *plugin, QPluginLoader::staticInstances()) {
        loadPlugin(plugin);
    }

    QDir plugins_dir = QDir(qApp->applicationDirPath());
    plugins_dir.cdUp();
    plugins_dir.cd("plugins");
    qDebug() << "[Salticidae] Loading external plugins from " << plugins_dir.path();
    foreach (QString file_name, plugins_dir.entryList(QStringList("libplugin-*.so"), QDir::Files)) {
        QPluginLoader loader(plugins_dir.absoluteFilePath(file_name));
        loadPlugin(loader.instance(), file_name);
    }
}

ProtoEye* PluginManager::eye(QUrl url)
{
    foreach (ProtoEye *plugin, s_eyes_plugins) {
        if( plugin->schemes().contains(url.scheme()) )
            return plugin;
    }

    return NULL;
}

QList<QUrl> PluginManager::sources()
{
    QList<QUrl> out;
    foreach (ProtoEye *plugin, s_eyes_plugins) {
        out.append(plugin->sources());
    }

    return out;
}

QStringList PluginManager::schemes()
{
    QStringList out;
    foreach (ProtoEye *plugin, s_eyes_plugins) {
        out.append(plugin->schemes());
    }

    return out;
}