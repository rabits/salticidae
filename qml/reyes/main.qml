import QtQuick 2.1
import QtMultimedia 5.2
import org.rabits.eyes 1.0

Rectangle {
    width: 1280
    height: 720
    Rectangle {
        width: 1440
        height: 960
        color: "#000"
        VideoOutput {
            id: vo_l
            anchors.fill: parent
            source: app.camera("/dev/video4")
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            app.camera("/dev/video4").startCapture();
            //Qt.quit();
        }
    }
}
