#ifndef EYES_H
#define EYES_H

#include <QObject>
#include <QSettings>
#include <QtQml/QQmlContext>
#include <QtGui/QGuiApplication>
#include <QTranslator>

#include <QtQuick/QQuickView>
#include <qplatformdefs.h>

#include "pluginmanager.h"

class Eyes : public QObject
{
    Q_OBJECT
private:
    explicit Eyes(QObject *parent = 0);
    ~Eyes();

    static Eyes *s_pInstance;

    QQuickItem  *m_root_object;
    QQmlContext *m_context;

    QSettings m_settings;

    QTranslator      m_translator;
    QGuiApplication *m_app;

    QMap<QUrl, ProtoEye*> m_eyes;

    QList<QUrl> m_sources;
    QStringList m_schemes;

public:
    inline static Eyes* I() { if( s_pInstance == NULL ) s_pInstance = new Eyes(); return s_pInstance; }
    inline static void  destroyI() { delete s_pInstance; }

    void initContext(QQuickView &viewer, QGuiApplication *app);
    void setLocale(QString locale);
    void initRoot(QQuickView &viewer);

    Q_INVOKABLE QVariant setting(QString key, QString value = "");

    Q_INVOKABLE ProtoEye* eye(QString url);

    Q_INVOKABLE QList<QUrl> availableSources();
    Q_INVOKABLE void updateSources();

    Q_INVOKABLE QStringList availableSchemes();
    Q_INVOKABLE void updateSchemes();

signals:
    void settingChanged(QString key);

public slots:

};

#endif // EYES_H
