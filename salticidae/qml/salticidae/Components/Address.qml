import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    id: address

    property color color
    property color colorSub: Qt.lighter(address.color, 1.0)
    property color colorBorder: Qt.darker(address.color, 2.0)
    property color colorText: Qt.tint(address.color, "#55ffffff")

    property int textPixelSize: 12

    signal clicked
    signal entered

    signal selectedSource(string source)

    border.color: address.colorBorder
    radius: 4 * screenScale

    state: "nofocus"

    gradient: Gradient {
        GradientStop {
            id: gradient_stop0
            position: 0
            color: address.color
        }
        GradientStop {
            id: gradient_stop1
            position: 1
            color: address.colorSub
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent

        hoverEnabled: true
        propagateComposedEvents: true

        onEntered: {
            address.state = "focus";
            address.entered()
        }
        onExited:  address.state = "nofocus"

        TextInput {
            id: retina_source
            anchors {
                fill: parent
                leftMargin: 5
                rightMargin: 5
            }
            font {
                family: "monospace"
                pixelSize: textPixelSize
            }
            verticalAlignment: TextInput.AlignVCenter
            color: colorText
            clip: true
            selectByMouse: true
            onCursorPositionChanged: address.clicked()
            onFocusChanged: {
                retina_source_mouse_area.enabled = !focus
                list.show(focus)
            }

            text: "no source"

            onEditingFinished: {
                address.selectedSource(this.text)
                Qt.inputMethod.hide();
            }

            MouseArea {
                id: retina_source_mouse_area
                anchors.fill: parent
                propagateComposedEvents: true
                acceptedButtons: Qt.LeftButton

                onPressed: {
                    retina_source.focus = true
                    retina_source.selectAll()
                }
            }
        }
    }

    Rectangle {
        id: list
        anchors {
            top: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 0
        clip: true

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        function show(vis) {
            height = (vis ? 100 : 0) * screenScale
            if( vis ) {
                Qt.inputMethod.hide();
                if( list_view.model.count === 0 ) {
                    console.log(list_view.model.count)
                    update()
                }
            }
        }
        function update() {
            list_view.model.clear()

            var sources = app.availableSources()
            for( var i in sources ) {
                list_view.model.append({
                    type: "url",
                    info: sources[i]
                })
            }

            var schemes = app.availableSchemes()
            for( var i in schemes ) {
                list_view.model.append({
                    type: "scheme",
                    info: schemes[i] + ":"
                })
            }
        }

        gradient: Gradient {
            GradientStop {
                id: gradient_stop2
                position: 0
                color: address.color
            }
            GradientStop {
                id: gradient_stop3
                position: 1
                color: address.colorSub
            }
        }

        ListView {
            id: list_view
            anchors.fill: parent

            delegate: Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                height: 30 * screenScale
                color: "#11ffffff"
                Text {
                    anchors{
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                        leftMargin: 10
                    }
                    verticalAlignment: Text.AlignVCenter

                    text: info
                    color: type == "url" ? "#fff" : "#0f0"
                }

                MouseArea {
                    id: list_mouse_area
                    anchors.fill: parent

                    hoverEnabled: true
                    propagateComposedEvents: true

                    onClicked: {
                        list.show(false)

                        retina_source.selectAll()
                        retina_source.remove(retina_source.selectionStart, retina_source.selectionEnd)
                        retina_source.select(0,0)
                        retina_source.insert(0, info)

                        if( type === "url" ) {
                            address.selectedSource(info)
                            retina_source.focus = false
                        }
                    }
                }

                Rectangle {
                    radius: 4 * screenScale
                    anchors.fill: parent
                    color: "#11ffffff"
                    visible: list_mouse_area.pressed
                }
            }

            model: ListModel {}
        }
    }

    transitions: [
        Transition {
            to: "nofocus"
            ParallelAnimation {
                ColorAnimation { target: gradient_stop0; property: "color"; to: address.color; duration: 200 }
                ColorAnimation { target: gradient_stop1; property: "color"; to: address.colorSub; duration: 200 }
                ColorAnimation { target: address; property: "border.color"; to: address.colorBorder; duration: 200 }
            }
        },
        Transition {
            to: "focus"
            ParallelAnimation {
                ColorAnimation { target: gradient_stop0; property: "color"; to: Qt.lighter(address.color); duration: 200 }
                ColorAnimation { target: gradient_stop1; property: "color"; to: Qt.lighter(address.colorSub); duration: 200 }
                ColorAnimation { target: address; property: "border.color"; to: Qt.lighter(address.colorBorder, 2.0); duration: 200 }
            }
        }
    ]
}
