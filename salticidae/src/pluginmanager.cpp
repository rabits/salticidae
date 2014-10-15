#include "pluginmanager.h"

#include <QPluginLoader>

QList<ProtoEye*> PluginManager::s_eyes_plugins = QList<ProtoEye*>();
QMap<QUrl, ProtoEye*> PluginManager::s_eyes = QMap<QUrl, ProtoEye*>();

void PluginManager::loadPlugin(QObject *plugin, QString plugin_name)
{
    ProtoPlugin *proto_plugin = dynamic_cast<ProtoPlugin*>(plugin);
    if( plugin ) {
        qDebug() << "[Salticidae]   found plugin type:" << proto_plugin->type() << "name:" << proto_plugin->name() << "version:" << proto_plugin->version();
        if( proto_plugin->type().compare("eye") == 0 ) {
            ProtoEye *plugin_typed = qobject_cast<ProtoEye*>(plugin);
            if( plugin_typed ) {
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
    qDebug("[Salticidae] Init plugins");

    // Registering dummy types
    registerQmlPluginTypes();

    // Internal plugins is not supported right now
    /*qDebug("[Salticidae] Loading internal plugins");
    foreach( QObject *plugin, QPluginLoader::staticInstances() ) {
        loadPlugin(plugin);
    }*/

    QDir plugins_dir = QDir(qApp->applicationDirPath());
    plugins_dir.cd("plugins");
    qDebug() << "[Salticidae] Loading external plugins from" << plugins_dir.path();
    foreach (QString file_name, plugins_dir.entryList(QStringList("libplugin-*.so"), QDir::Files)) {
        QPluginLoader loader(plugins_dir.absoluteFilePath(file_name));
        loadPlugin(loader.instance(), file_name);
    }
}

void PluginManager::registerQmlPluginTypes()
{
    qmlRegisterUncreatableType<ProtoEye>(ProtoEye_iid, 1, 0, "Plugin", "Eye Plugin Interface");
}

ProtoEye* PluginManager::eye(QUrl url)
{
    if( ! s_eyes.contains(url) ) {
        qDebug() << "[Salticidae] Getting new instance of plugin for" << url;

        foreach (ProtoEye *plugin, s_eyes_plugins) {
            if( plugin->isSupported(url) ) {
                s_eyes.insert(url, plugin->instance(url));
                break;
            }
        }
    }

    return s_eyes.value(url);
}

QList<QUrl> PluginManager::sources()
{
    QList<QUrl> out;
    foreach (ProtoEye *plugin, s_eyes_plugins) {
        qDebug() << "[Salticidae] Retrieving sources from" << plugin->name();
        out.append(plugin->sources());
    }

    return out;
}

QStringList PluginManager::schemes()
{
    QStringList out;
    foreach (ProtoEye *plugin, s_eyes_plugins) {
        qDebug() << "[Salticidae] Retrieving schemes from" << plugin->name();
        out.append(plugin->schemes());
    }

    return out;
}
