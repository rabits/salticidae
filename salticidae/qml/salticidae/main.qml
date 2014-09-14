import QtQuick 2.2

Rectangle {
    width: 1280
    height: 720
    color: "#000"

    RetinaLayout {
        id: layout
        anchors.fill: parent
        objectName: "0"

        Component.onCompleted: layout.retina_grid.addRetina()
    }
}
