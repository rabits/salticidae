#include <QDebug>
#include <QScreen>

#include "eyedisplay.h"
#include "eyes.h"

Eyes* Eyes::s_pInstance = NULL;

Eyes::Eyes(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_translator()
{
    qDebug("[Salticidae] Init app");

    if( m_settings.value("preferences/locale").isNull() )
        m_settings.setValue("preferences/locale", QLocale::system().name());
    if( m_settings.value("recorder/write_dir").isNull() ) {
        QDir dir = QDir(QDir::homePath() + "/salticidae/recorder");
        dir.mkpath(".");
        m_settings.setValue("recorder/write_dir", dir.path());
    }
}

Eyes::~Eyes()
{
    qDebug("[Salticidae] Self destroying");
}

void Eyes::initContext(QQuickView &viewer, QGuiApplication *app)
{
    qDebug("[Salticidae] Init context");

    m_context = viewer.rootContext();
    m_app = app;

    m_context->setContextProperty("app", this);
    m_context->setContextProperty("screenScale",
        QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio() / 100);

    setLocale(setting("preferences/locale").toString());
    m_app->installTranslator(&m_translator);
}

void Eyes::setLocale(QString locale)
{
    qDebug() << "[Salticidae] Set locale to" << locale;
    if( ! m_translator.load("tr_" + locale, ":/") )
    {
        m_translator.load("tr_en", ":/");
        setting("preferences/locale", "en");
    }
}

void Eyes::initRoot(QQuickView& viewer)
{
    m_root_object = viewer.rootObject();
}

QVariant Eyes::setting(QString key, QString value)
{
    if( ! value.isEmpty() )
    {
        m_settings.setValue(key, value);
        if( key.contains(QString("preferences/")) )
            emit settingChanged(key);
    }

    return m_settings.value(key);
}

QList<QUrl> Eyes::availableSources()
{
    if( m_sources.isEmpty() )
        updateSources();

    return m_sources;
}

void Eyes::updateSources()
{
    qDebug("[Salticidae] Updating sources");
    m_sources = PluginManager::sources();
}

QStringList Eyes::availableSchemes()
{
    if( m_schemes.isEmpty() )
        updateSchemes();

    return m_schemes;
}

void Eyes::updateSchemes()
{
    qDebug("[Salticidae] Updating schemes");
    m_schemes = PluginManager::schemes();
}

QStringList Eyes::availableTransforms()
{
    qDebug("[Salticidae] Getting transforms");
    return PluginManager::transforms();
}

void Eyes::registerQmlTypes()
{
    qmlRegisterType<EyeDisplay>("org.rabits.salticidae", 1, 0, "EyeDisplay");
}
