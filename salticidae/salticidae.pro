QT += multimedia

# Add more folders to ship with the application, here
qml.source = qml/salticidae
qml.target = qml
DEPLOYMENTFOLDERS = qml

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

SOURCES += \
    src/main.cpp \
    src/eyes.cpp \
    src/pluginmanager.cpp

HEADERS += \
    src/eyes.h \
    src/pluginmanager.h

include(../plugins/include.pri)

!android {
    # Copy target to root directory
    QMAKE_POST_LINK = cp -lf "$${TARGET}" ../$${TARGET}.bin
}

RESOURCES += \
    resources.qrc
