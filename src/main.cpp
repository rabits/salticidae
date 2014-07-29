#include <QGuiApplication>
#include <QtQml>
#include "qtquick2applicationviewer.h"

#include "src/eyes.h"

int main(int argc, char *argv[])
{
    qDebug("[rEyes] Init rEyes v%s", PROJECT_VERSION);

    QCoreApplication::setOrganizationName("Rabits");
    QCoreApplication::setOrganizationDomain("rabits.org");
    QCoreApplication::setApplicationName("reyes");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Eye>("org.rabits.eyes", 1, 0, "Eye");

    QtQuick2ApplicationViewer viewer;

    Eyes::I()->initContext(viewer, &app);

    qDebug("[rEyes] Loading qml");
    viewer.setMainQmlFile(QStringLiteral("qml/reyes/main.qml"));
    QObject::connect(viewer.engine(), SIGNAL(quit()), &viewer, SLOT(close()));

    Eyes::I()->initRoot(viewer);

    qDebug("[rEyes] Init done, starting");

    viewer.showExpanded();
    return app.exec();
}
