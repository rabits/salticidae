#include <QDebug>

#include "eyes.h"

Eyes* Eyes::s_pInstance = NULL;

Eyes::Eyes(QObject *parent) :
    QObject(parent)
  , m_settings()
  , m_translator()
  , m_eyes()
{
    qDebug("[Salticidae] Init app");

    if( m_settings.value("preferences/locale").isNull() )
        m_settings.setValue("preferences/locale", QLocale::system().name());

    qDebug("[Salticidae] Init eyes");

    //QByteArray s;
    //foreach( s, Eye::availableDevices() )
    //    qDebug() << "[Salticidae] Found camera: " << s;
}

Eyes::~Eyes()
{
    qDebug("[Salticidae] Self destroying");
}

void Eyes::initContext(QtQuick2ApplicationViewer &viewer, QGuiApplication *app)
{
    qDebug("[Salticidae] Init context");

    m_context = viewer.rootContext();
    m_app = app;

    m_context->setContextProperty("app", this);

    setLocale(setting("preferences/locale").toString());
    m_app->installTranslator(&m_translator);
}

void Eyes::setLocale(QString locale)
{
    qDebug() << "[Salticidae] Changing locale to " << locale;
    if( ! m_translator.load("tr_" + locale, ":/") )
    {
        m_translator.load("tr_en", ":/");
        setting("preferences/locale", "en");
    }
}

void Eyes::initRoot(QtQuick2ApplicationViewer& viewer)
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
