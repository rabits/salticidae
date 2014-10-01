import QtQuick 2.2

Rectangle {
    id: message

    function show(msg) {
        if( msg !== "" )
            text.text = msg
        text.opacity = 1.0
    }
    function hide() {
        text.opacity = 0.0
    }

    color: "#00000000"

    Text {
        id: text
        anchors.fill: parent
        clip: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font {
            bold: true
            family: "monospace"
            pointSize: (parent.width / 30) + 10
        }
        color: "#22ffffff"
        style: Text.Outline
        styleColor: "#44000000"
        text: qsTr("No source selected")

        opacity: 1.0

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Timer {
        id: timer

        interval: 3000
        running: true
        repeat: true

        onTriggered: message.hide()
    }
}
