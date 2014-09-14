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

INCLUDEPATH  += ../plugins/include
