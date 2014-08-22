# Version
VERSION='1.0'
VERSTR='\\"$${VERSION}\\"'
DEFINES += PROJECT_VERSION=\"$${VERSTR}\"

QT += multimedia

#QMAKE_CFLAGS += -std=c99

# Add more folders to ship with the application, here
folder_01.source = qml/salticidae
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

SOURCES += \
    src/main.cpp \
    src/eyes.cpp \
    src/eye.cpp \
    src/v4l2.c

HEADERS += \
    src/eyes.h \
    src/eye.h \
    src/v4l2.h