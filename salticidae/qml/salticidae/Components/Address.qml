import QtQuick 2.2

Rectangle {
    id: address

    property color color
    property color colorSub: Qt.lighter(address.color, 1.0)
    property color colorBorder: Qt.darker(address.color, 2.0)
    property color colorText: Qt.tint(address.color, "#55ffffff")

    signal clicked
    signal entered

    border.color: address.colorBorder
    radius: 4 * screenScale

    state: "nofocus"

    gradient: Gradient {
        GradientStop {
            id: gradientStop0
            position: 0
            color: address.color
        }
        GradientStop {
            id: gradientStop1
            position: 1
            color: address.colorSub
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true
        propagateComposedEvents: true

        onEntered: {
            address.state = "focus";
            address.entered()
        }
        onExited:  address.state = "nofocus"

        onPressed:  address.state = "pressed"
        onReleased: address.state = "focus"

        onClicked: address.clicked()
    }

    Text {
        id: retina_source
        anchors {
            fill: parent
            leftMargin: 5
            rightMargin: 5
        }

        font {
            family: "monospace"
            pixelSize: header.height * 0.7
        }
        color: colorText
        clip: true
        elide: Text.ElideRight

        text: retina.source
    }

    /*ListView {
        id: list
    }*/

    transitions: [
        Transition {
            to: "nofocus"
            ParallelAnimation {
                ColorAnimation { target: gradientStop0; property: "color"; to: address.color; duration: 200 }
                ColorAnimation { target: gradientStop1; property: "color"; to: address.colorSub; duration: 200 }
                ColorAnimation { target: address; property: "border.color"; to: address.colorBorder; duration: 200 }
            }
        },
        Transition {
            to: "focus"
            ParallelAnimation {
                ColorAnimation { target: gradientStop0; property: "color"; to: Qt.lighter(address.color); duration: 200 }
                ColorAnimation { target: gradientStop1; property: "color"; to: Qt.lighter(address.colorSub); duration: 200 }
                ColorAnimation { target: address; property: "border.color"; to: Qt.lighter(address.colorBorder, 2.0); duration: 200 }
            }
        },
        Transition {
            to: "pressed"
            ParallelAnimation {
                ColorAnimation { target: gradientStop0; property: "color"; to: Qt.darker(address.color, 2.0); duration: 50 }
                ColorAnimation { target: gradientStop1; property: "color"; to: Qt.darker(address.colorSub, 2.0); duration: 50 }
            }
        }
    ]
}
