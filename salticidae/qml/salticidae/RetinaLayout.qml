import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {
    id: layer

    Layout.maximumWidth: parent.width / parent.columns
    Layout.maximumHeight: parent.height / parent.rows
    Layout.fillWidth: true
    Layout.fillHeight: true

    color: "#222"

    property alias retina_grid: retina_layout

    function tree() {
        var node = this.retina_grid
        var data = []
        for( var c in node.children ) {
            if( node.children[c].objectName === '' ) {
                data.push('Retina')
            } else {
                var d = {}
                d["Layout " + node.children[c].objectName] = node.children[c].tree()
                data.push(d)
            }
        }

        return data
    }

    function showTree() {
        console.log(JSON.stringify(layer.tree(), undefined, 2))
    }

    GridLayout {
        id: retina_layout
        anchors.fill: parent

        rows: 0
        columns: 0
        rowSpacing: 0
        columnSpacing: 0

        function createLayout(object_parent) {
            if( object_parent === undefined )
                object_parent = this
            var component = Qt.createComponent("RetinaLayout.qml");
            if (component.status === Component.Ready) {
                var sprite = component.createObject(object_parent, {})
            } else {
                console.error("Unable to create component:\n  " + component.errorString())
            }
            sprite.objectName = (parseInt(object_parent.parent.objectName) + 1) + "_" + object_parent.children.length
            return sprite
        }

        function createRetina(object_parent) {
            if( object_parent === undefined )
                object_parent = this
            var component = Qt.createComponent("Retina.qml");
            if (component.status === Component.Ready) {
                var sprite = component.createObject(object_parent, {})
            } else {
                console.error("Unable to create component:\n  " + component.errorString())
            }
            return sprite
        }

        function addRetina(layout, type) {
            // We need to create layout for every retina to prevent removing items in place
            var layout_obj
            if( retina_layout.children.length < 1 ) {
                // Add first children
                retina_layout.columns = 1
                retina_layout.rows = 1
                if( (layer.parent.parent === null) || (layer.parent.parent.retina_grid === undefined) ) {
                    // Special for the first layout - we need to create another one
                    layout_obj = retina_layout.createLayout()
                    layout_obj.retina_grid.addRetina()
                } else {
                    retina_layout.createRetina()
                }
            } else if( ((type !== 'vertical') && (retina_layout.rows === 1)) ||
                       ((type === 'vertical') && (retina_layout.columns === 1)) ) {
                // Add next childrens if layout already have requested vertical or horizontal retinas
                if( type !== 'vertical' ) {
                    // Add horizontal by default
                    retina_layout.columns += 1
                } else {
                    // Add vertical
                    retina_layout.rows += 1
                }
                layout_obj = retina_layout.createLayout()
                layout_obj.retina_grid.addRetina()
            } else {
                // If layout doesn't have requested line type of retinas - use children layout
                var retina_obj = layout.childAt(0,0)
                layout_obj = layout.createLayout()
                retina_obj.parent = layout_obj.retina_grid
                layout_obj.retina_grid.columns = 1
                layout_obj.retina_grid.rows = 1
                layout.addRetina(layout_obj.retina_grid, type)
            }
        }
    }
}
