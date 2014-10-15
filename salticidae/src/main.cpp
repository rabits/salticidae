#include <QGuiApplication>
#include <QtQml>

#include "qtquick2applicationviewer.h"
#include "src/eyes.h"

int main(int argc, char *argv[])
{
    qDebug("[Salticidae] Init Salticidae v%s", PROJECT_VERSION);

    QCoreApplication::setOrganizationName("Rabits");
    QCoreApplication::setOrganizationDomain("rabits.org");
    QCoreApplication::setApplicationName("salticidae");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QGuiApplication app(argc, argv);

    Eyes::registerQmlTypes();
    PluginManager::initPlugins();

    QtQuick2ApplicationViewer viewer;

    Eyes::I()->initContext(viewer, &app);

    qDebug("[Salticidae] Loading qml");
    viewer.setSource(QUrl("qrc:/qml/salticidae/main.qml"));
    QObject::connect(viewer.engine(), SIGNAL(quit()), &viewer, SLOT(close()));

    Eyes::I()->initRoot(viewer);

    qDebug("[Salticidae] Init done, starting");

    viewer.showExpanded();
    return app.exec();
}
