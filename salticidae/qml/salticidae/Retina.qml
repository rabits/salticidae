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

    color: "#000"
    border.color: "#11ffffff"
    border.width: 1

    VideoOutput {
        id: vo_l
        anchors.fill: parent
    }
    Text {
        id: message
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
            color: "#22ffffff"
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
                    hide_header.stop()
                    header.state = "visible"
                }
                onExited: {
                    hide_header.restart()
                }

                Text {
                    id: retina_source

                    anchors {
                        top: parent.top
                        left: parent.left
                        bottom: parent.bottom
                        right: duplicate_vertical.left
                        leftMargin: 5
                    }

                    font {
                        family: "monospace"
                        pixelSize: header.height * 0.7
                    }
                    color: "#44ffffff"
                    clip: true
                    elide: Text.ElideRight

                    text: retina.source
                }

                Button {
                    id: duplicate_vertical

                    anchors.right: duplicate_horizontal.left
                    anchors.rightMargin: 5

                    height: parent.height
                    width: height

                    color: "#77444444"
                    caption: "V"

                    onClicked: retina.parent.parent.parent.addRetina(retina.parent, 'vertical')
                    onEntered: { header.state = 'visible'; hide_header.stop() }
                }
                Button {
                    id: duplicate_horizontal

                    anchors.right: remove.left
                    anchors.rightMargin: 5

                    height: parent.height
                    width: height

                    color: "#77444444"
                    caption: "H"

                    onClicked: retina.parent.parent.parent.addRetina(retina.parent)
                    onEntered: { header.state = 'visible'; hide_header.stop() }
                }
                Button {
                    id: remove

                    anchors.right: parent.right

                    height: parent.height
                    width: height

                    color: "#77660000"
                    caption: "X"

                    onClicked: console.log("Not implemented yet")
                    onEntered: { header.state = 'visible'; hide_header.stop() }
                }
            }

            Timer {
                id: hide_header

                interval: 3000
                running: false
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
                    }
                }
            ]
        }
    }
}
