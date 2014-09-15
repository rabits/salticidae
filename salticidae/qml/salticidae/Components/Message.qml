import QtQuick 2.2

Rectangle {
    id: message

    color: "#00000000"

    Text {
        id: header
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
    }
}
