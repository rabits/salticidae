import QtQuick 2.2
import QtMultimedia 5.2
import QtQuick.Layouts 1.1

import org.rabits.salticidae.plugins.eye 1.0
import "Components"

Rectangle {
    id: retina

    Layout.maximumWidth: parent.width / parent.columns
    Layout.maximumHeight: parent.height / parent.rows
    Layout.fillWidth: true
    Layout.fillHeight: true

    property string source: "no source"
    property color headerColor: Qt.rgba(Math.random(), Math.random(), Math.random(), 0.2)

    color: "#000"
    border.color: "#11ffffff"
    border.width: 1

    VideoOutput {
        id: vo_l
        anchors.fill: parent
    }
    Message {
        id: message
        anchors.fill: parent
    }

    MouseArea {
        id: retina_mouse
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            hide_header.restart()
            header.state = "visible"
        }
        onExited: {
            header.state = "hidden"
        }

        //onClicked: {
            //app.eye("/dev/video4").startCapture();
            //Qt.quit();
        //}

        Rectangle {
            id: header
            opacity: 0
            height: 30 * screenScale
            color: headerColor
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 1
            }

            MouseArea {
                id: header_mouse
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: {
                    header.state = "visible"
                    hide_header.stop()
                }
                onExited: {
                    hide_header.restart()
                }

                Address {
                    id: header_address
                    anchors {
                        top: parent.top
                        left: parent.left
                        bottom: parent.bottom
                        right: header_buttons.left
                        margins: 2
                        leftMargin: 5
                        rightMargin: 5
                    }

                    color: "#44444444"

                    onEntered: { header.state = 'visible'; hide_header.stop() }
                }

                Row {
                    id: header_buttons
                    anchors.right: parent.right
                    height: parent.height
                    spacing: 5

                    Button {
                        id: duplicate_vertical

                        height: parent.height
                        width: height

                        color: "#77444444"
                        caption: "―"

                        onClicked: retina.parent.parent.parent.addRetina(retina.parent, 'vertical')
                        onEntered: { header.state = 'visible'; hide_header.stop() }
                    }
                    Button {
                        id: duplicate_horizontal

                        height: parent.height
                        width: height

                        color: "#77444444"
                        caption: "∣"

                        onClicked: retina.parent.parent.parent.addRetina(retina.parent)
                        onEntered: { header.state = 'visible'; hide_header.stop() }
                    }
                    Button {
                        id: remove

                        height: parent.height
                        width: height

                        color: "#77660000"
                        caption: "X"

                        onClicked: console.log("Not implemented yet")
                        onEntered: { header.state = 'visible'; hide_header.stop() }
                    }
                }
            }

            Timer {
                id: hide_header

                interval: 3000
                running: true
                repeat: true

                onTriggered: header.state = "hidden"
            }

            state: "hidden"

            transitions: [
                Transition {
                    to: "hidden"
                    ParallelAnimation {
                        NumberAnimation { target: header; property: "opacity"; to: 0.0; duration: 200 }
                        onStarted: hide_header.stop()
                    }
                },
                Transition {
                    to: "visible"
                    ParallelAnimation {
                        NumberAnimation { target: header; property: "opacity"; to: 1.0; duration: 200 }
                        onStarted: hide_header.restart()
                    }
                }
            ]
        }
    }

    Component.onCompleted: {
        header.state = "visible"
    }
}
