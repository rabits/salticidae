QT += multimedia svg

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
    src/pluginmanager.cpp \
    src/eyedisplay.cpp

HEADERS += \
    src/eyes.h \
    src/pluginmanager.h \
    src/eyedisplay.h

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/xml/device_filter.xml \
    salticidae.svg

RESOURCES += \
    resources.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

include(../plugins/include.pri)

icon_svg = $$PWD/salticidae.svg
android {
    # Generate icons
    ldpi.size = 36x36
    mdpi.size = 48x48
    hdpi.size = 72x72
    xhdpi.size = 96x96
    xxhdpi.size = 144x144
    xxxhdpi.size = 192x192
    resolutions = ldpi mdpi hdpi xhdpi xxhdpi xxxhdpi
    for(res, resolutions) {
        icon_$${res}.target = icon_$$res
        icon_$${res}.commands = mkdir -p ../android-build/res/drawable-$$res && convert -background none -density 1200 -resize $$eval($${res}.size) "$${icon_svg}" ../android-build/res/drawable-$$res/icon.png
        QMAKE_EXTRA_TARGETS += icon_$$res
        POST_TARGETDEPS += icon_$$res
    }
}
!android {
    # Copy target to root directory
    QMAKE_POST_LINK = cp -lf "$${TARGET}" ../$${TARGET}.bin
}
