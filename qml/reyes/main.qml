import QtQuick 2.1
import QtMultimedia 5.2
import org.rabits.eyes 1.0

Rectangle {
    width: 1280
    height: 720
    Rectangle {
        width: 720
        height: 480
        //Camera {
        //    id: camera
        //}

        VideoOutput {
            id: vo_l
            anchors.fill: parent
            //source: camera
            source: app.camera("/dev/video0")
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("State:"+app.camera("/dev/video0").state);
            console.log("Status:"+app.camera("/dev/video0").status);
            app.camera("/dev/video0").start();
            console.log("State:"+app.camera("/dev/video0").state);
            console.log("Status:"+app.camera("/dev/video0").status);
            //Qt.quit();
        }
    }
}
