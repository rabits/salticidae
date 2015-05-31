import QtQuick 2.2
import QtMultimedia 5.2
import QtQuick.Layouts 1.1

import org.rabits.salticidae 1.0
import org.rabits.salticidae.plugins.video 1.0
import org.rabits.salticidae.plugins.transform 1.0
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
        source: EyeDisplay {
            id: eyedisplay
        }
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
                        eyedisplay.setSource(source)

                        if( eyedisplay.getSource() === null ) {
                            message.show("Can't display source:<br/>" + source)
                            return
                        }
                        eyedisplay.start()
                        message.show("Selected source:<br/>" + source)
                    }
                }

                Row {
                    id: header_buttons
                    anchors.right: parent.right
                    height: parent.height
                    spacing: 5

                    Button {
                        id: transforms_list_button

                        height: parent.height
                        width: height

                        color: "#77444488"
                        caption: "T"

                        onClicked: transforms.trigger()
                        onEntered: {
                            header.show()
                            hide_header.stop()
                        }

                        Rectangle {
                            id: transforms
                            anchors.top: parent.bottom
                            width: header_buttons.width
                            height: retina.height - header.height

                            visible: false
                            color: "#88444444"
                            clip: true

                            function trigger() {
                                visible = !visible
                                if( visible )
                                    transforms.update()
                            }

                            function update() {
                                transforms_list.model.clear()

                                var transforms = eyedisplay.getTransforms()
                                var available = app.availableTransforms()

                                for( var i in transforms ) {
                                    transforms_list.model.append({
                                        online: true,
                                        name: transforms[i]
                                    })
                                    available = available.filter(function(e){ return e !== transforms[i] })
                                }
                                for( var j in available ) {
                                    transforms_list.model.append({
                                        online: false,
                                        name: available[j]
                                    })
                                }
                            }

                            ListView {
                                id: transforms_list
                                anchors.fill: parent

                                model: ListModel {}

                                delegate: Rectangle {
                                    id: delegate
                                    anchors {
                                        left: parent.left
                                        right: parent.right
                                    }
                                    height: 30 * screenScale
                                    color: online ? "#8800aa00" : "#88888888"

                                    Item {
                                        id: transform_name
                                        anchors{
                                            top: parent.top
                                            left: parent.left
                                            right: parent.right
                                        }
                                        height: parent.height

                                        Text {
                                            anchors{
                                                top: parent.top
                                                bottom: parent.bottom
                                                left: parent.left
                                                leftMargin: 10
                                            }
                                            verticalAlignment: Text.AlignVCenter

                                            text: name
                                            color: "#fff"
                                        }

                                        MouseArea {
                                            id: transforms_list_mouse_area
                                            anchors.fill: parent

                                            hoverEnabled: true
                                            propagateComposedEvents: true

                                            onClicked: {
                                                if( ! online )
                                                    eyedisplay.addTransform(name)
                                                else
                                                    eyedisplay.deleteTransform(name)

                                                transforms.update()
                                            }
                                        }

                                        Rectangle {
                                            radius: 4 * screenScale
                                            anchors.fill: parent
                                            color: "#11ffffff"
                                            visible: transforms_list_mouse_area.pressed
                                        }

                                        Button {
                                            id: transform_config_button
                                            anchors.right: parent.right

                                            height: parent.height
                                            width: height

                                            color: "#99448888"
                                            caption: "C"
                                            enabled: online

                                            onClicked: {
                                                if( delegate.state === "" ) {
                                                    delegate.state = "config"
                                                    var t = eyedisplay.getTransform(name)
                                                    if( t ) {
                                                        var configs = t.getConfig()
                                                        transform_config.model.clear()
                                                        for( var i in configs ) {
                                                            transform_config.model.append
                                                                    ({
                                                                         trans: name,
                                                                         name: i,
                                                                         value: configs[i],
                                                                         type: typeof(configs[i])
                                                                    })
                                                        }
                                                    } else
                                                        console.error("ERROR: unable to find required transform " + name)
                                                } else
                                                    delegate.state = ""
                                            }

                                            onEntered: {
                                                header.show()
                                                hide_header.stop()
                                            }
                                        }
                                    }

                                    ListView {
                                        id: transform_config
                                        anchors {
                                            top: transform_name.bottom
                                            left: parent.left
                                            right: parent.right
                                        }
                                        height: childrenRect.height
                                        interactive: false

                                        visible: false
                                        model: ListModel {}

                                        delegate: Rectangle {
                                            id: config_delegate
                                            anchors {
                                                left: parent.left
                                                right: parent.right
                                                leftMargin: 10
                                            }
                                            height: 30 * screenScale

                                            property var changed_value: value

                                            function save() {
                                                var conf = {}
                                                conf[name] = changed_value
                                                eyedisplay.getTransform(trans).setConfig(conf)
                                            }

                                            Text {
                                                anchors{
                                                    top: parent.top
                                                    bottom: parent.bottom
                                                    left: parent.left
                                                    leftMargin: 10
                                                }
                                                verticalAlignment: Text.AlignVCenter

                                                text: name
                                                color: "#fff"
                                            }

                                            MouseArea {
                                                id: config_delegate_mouse_area
                                                anchors.fill: parent

                                                hoverEnabled: true
                                                propagateComposedEvents: true
                                            }

                                            Rectangle {
                                                radius: 4 * screenScale
                                                anchors.fill: parent
                                                color: "#11ffffff"
                                                visible: config_delegate_mouse_area.pressed
                                            }

                                            TextInput {
                                                id: config_delegate_text_value
                                                anchors {
                                                    top: parent.top
                                                    right: parent.right
                                                    topMargin: parent.height / 2 - height / 2
                                                }
                                                visible: false
                                                width: parent.width / 2

                                                color: "#fff"
                                            }

                                            states: [
                                                State {
                                                    name: "boolean"
                                                    when: type === "boolean"
                                                    PropertyChanges { target: config_delegate; color: changed_value ? "#3a3" : "#333" }
                                                    PropertyChanges { target: config_delegate_mouse_area; onClicked: {
                                                            changed_value = ! changed_value
                                                            save()
                                                        }}
                                                },
                                                State {
                                                    name: "number"
                                                    when: type === "number"
                                                    PropertyChanges { target: config_delegate; color: "#333" }
                                                    PropertyChanges { target: config_delegate_text_value; visible: true; text: changed_value; onTextChanged: {
                                                            var val = parseInt(config_delegate_text_value.text)
                                                            if( config_delegate_text_value.text === "" )
                                                                val = 0
                                                            if( ! isNaN(val) ) {
                                                                if( changed_value !== val ) {
                                                                    changed_value = val
                                                                    save()
                                                                }
                                                            }
                                                        }}
                                                },
                                                State {
                                                    name: "string"
                                                    when: type === "string"
                                                    PropertyChanges { target: config_delegate; color: "#333" }
                                                    PropertyChanges { target: config_delegate_text_value; visible: true; text: changed_value; onTextChanged: {
                                                            changed_value = config_delegate_text_value.text
                                                            save()
                                                        }}
                                                }
                                            ]
                                        }
                                    }

                                    states: [
                                        State {
                                            name: "config"
                                            PropertyChanges { target: transform_config; visible: true }
                                            PropertyChanges { target: transform_name; height: 30 * screenScale }
                                            PropertyChanges { target: delegate; height: transform_name.height + transform_config.height }
                                        }
                                    ]
                                }
                            }
                        }
                    }

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
