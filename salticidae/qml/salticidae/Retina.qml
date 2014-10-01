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

    property color headerColor: Qt.rgba(Math.random(), Math.random(), Math.random(), 0.2)

    color: "#000"
    border.color: "#11ffffff"
    border.width: 1

    VideoOutput {
        id: video_output
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
        onClicked: {
            parent.focus = true
            header.show()
        }
        onExited: header.hide()

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

            property bool visible_locked: false
            function show() {
                hide_header.restart()
                header.state = "visible"
            }
            function hide() {
                if( ! header.visible_locked )
                    header.state = "hidden"
            }
            function visibleLock(value) {
                header.visible_locked = value
            }

            MouseArea {
                id: header_mouse
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: {
                    header.show()
                    hide_header.stop()
                }
                onExited: {
                    hide_header.restart()
                }
                onClicked: {
                    header.show()
                    hide_header.stop()
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
                    textPixelSize: header.height * 0.7

                    onEntered: {
                        header.show()
                        hide_header.stop()
                    }
                    onClicked: hide_header.stop()

                    onSelectedSource: {
                        if( video_output.source !== app.eye(source) ) {
                            if( video_output.source != null ) {
                                video_output.source.stop()
                            }

                            video_output.source = app.eye(source)
                        }

                        if( video_output.source == null ) {
                            message.show("Can't display source:<br/>" + source)
                            return
                        }
                        video_output.source.start()
                        message.show("Selected source:<br/>" + source)
                    }
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
                        onEntered: {
                            header.show()
                            hide_header.stop()
                        }
                    }
                    Button {
                        id: duplicate_horizontal

                        height: parent.height
                        width: height

                        color: "#77444444"
                        caption: "∣"

                        onClicked: retina.parent.parent.parent.addRetina(retina.parent)
                        onEntered: {
                            header.show()
                            hide_header.stop()
                        }
                    }
                    Button {
                        id: remove

                        height: parent.height
                        width: height

                        color: "#77660000"
                        caption: "X"

                        onClicked: console.log("Not implemented yet")
                        onEntered: {
                            header.show()
                            hide_header.stop()
                        }
                    }
                }
            }

            Timer {
                id: hide_header

                interval: 3000
                running: true
                repeat: true

                onTriggered: header.hide()
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
        header.show()
    }
}
