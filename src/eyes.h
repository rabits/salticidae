#ifndef EYES_H
#define EYES_H

#include <QObject>
#include <QSettings>
#include <QtQml/QQmlContext>
#include <QtGui/QGuiApplication>
#include <QTranslator>

#include <qplatformdefs.h>

#include "qtquick2applicationviewer.h"
#include "src/eye.h"

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

    QList<Eye*> m_eyes;

public:
    inline static Eyes* I() { if( s_pInstance == NULL ) s_pInstance = new Eyes(); return s_pInstance; }
    inline static void  destroyI() { delete s_pInstance; }

    void initContext(QtQuick2ApplicationViewer &viewer, QGuiApplication *app);
    void setLocale(QString locale);
    void initRoot(QtQuick2ApplicationViewer &viewer);

    QVariant setting(QString key, QString value = "");

    Q_INVOKABLE Eye* camera(QString device) {
        if( m_eyes.isEmpty() ) {
            Eye* eye = new Eye(device.toLocal8Bit());
            m_eyes.append(eye);
        }
        return m_eyes.last();
    }

signals:
    void settingChanged(QString key);

public slots:

};

#endif // EYES_H
