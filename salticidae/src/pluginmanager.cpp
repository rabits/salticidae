#include "pluginmanager.h"

#include <QPluginLoader>

QList<ProtoVideo*> PluginManager::s_video_plugins = QList<ProtoVideo*>();
QList<ProtoTransform*> PluginManager::s_transform_plugins = QList<ProtoTransform*>();

QMap<QUrl, ProtoVideo*> PluginManager::s_videos = QMap<QUrl, ProtoVideo*>();

void PluginManager::loadPlugin(QObject *plugin, QString plugin_name)
{
    ProtoPlugin *proto_plugin = dynamic_cast<ProtoPlugin*>(plugin);
    if( plugin ) {
        qDebug() << "[Salticidae]   found plugin type:" << proto_plugin->type() << "name:" << proto_plugin->name() << "version:" << proto_plugin->version();
        if( proto_plugin->type().compare("video") == 0 ) {
            ProtoVideo *plugin_typed = qobject_cast<ProtoVideo*>(plugin);
            if( plugin_typed ) {
                s_video_plugins.append(plugin_typed);
                qDebug() << "[Salticidae]     loading success";
            } else
                qDebug() << "[Salticidae]     loading failure";
        } else if( proto_plugin->type().compare("transform") == 0 ) {
            ProtoTransform *plugin_typed = qobject_cast<ProtoTransform*>(plugin);
            if( plugin_typed ) {
                s_transform_plugins.append(plugin_typed);
                qDebug() << "[Salticidae]     loading success";
            } else
                qDebug() << "[Salticidae]     loading failure";
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
    foreach( QString file_name, plugins_dir.entryList(QStringList("libsalticidae-plugin-*.so"), QDir::Files) ) {
        QPluginLoader loader(plugins_dir.absoluteFilePath(file_name));
        loadPlugin(loader.instance(), file_name);
    }
}

void PluginManager::registerQmlPluginTypes()
{
    qmlRegisterUncreatableType<ProtoVideo>(ProtoVideo_iid, 1, 0, "Plugin", "Video Plugin Interface");
    qmlRegisterUncreatableType<ProtoTransform>(ProtoTransform_iid, 1, 0, "Plugin", "Transform Plugin Interface");
}

ProtoVideo* PluginManager::video(QUrl url)
{
    if( ! s_videos.contains(url) ) {
        qDebug() << "[Salticidae] Getting new instance of video plugin for" << url;

        foreach( ProtoVideo *plugin, s_video_plugins ) {
            if( plugin->isSupported(url) ) {
                s_videos.insert(url, plugin->instance(url));
                break;
            }
        }
    }

    return s_videos.value(url);
}

ProtoTransform* PluginManager::transform(QString name)
{
    ProtoTransform *plugin = NULL;

    foreach( ProtoTransform *t, s_transform_plugins ) {
        if( t->name() == name ) {
            plugin = t->instance();
            break;
        }
    }

    return plugin;
}

QList<QUrl> PluginManager::sources()
{
    QList<QUrl> out;
    foreach( ProtoVideo *plugin, s_video_plugins ) {
        qDebug() << "[Salticidae] Retrieving sources from" << plugin->name();
        out.append(plugin->sources());
    }

    return out;
}

QStringList PluginManager::schemes()
{
    QStringList out;
    foreach( ProtoVideo *plugin, s_video_plugins ) {
        qDebug() << "[Salticidae] Retrieving schemes from" << plugin->name();
        out.append(plugin->schemes());
    }

    return out;
}

QStringList PluginManager::transforms()
{
    QStringList out;
    foreach( ProtoTransform *plugin, s_transform_plugins ) {
        out.append(plugin->name());
    }

    return out;
}
