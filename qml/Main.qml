import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Controls as Controls

Window {
    id: mainWindow
    width: 350
    height: 200
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint

    Rectangle {
        anchors.fill: parent
        color: "#1C1C21"
        radius: 5
        border.color: "#28282A"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            RowLayout {
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.fillWidth: true
                Layout.preferredHeight: 42
                spacing: 0
                Button {
                    icon: "qrc:/svg/cog.svg"
                    onClicked: console.log("Config")
                }
                Text {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 42
                    text: "Elgato Lights Manager Linux"
                    color: "#fff"
                    font.pointSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    DragHandler {
                        target: null
                        onActiveChanged: if (active) mainWindow.startSystemMove()
                    }
                }
                Button {
                    icon: "qrc:/svg/close.svg"
                    onClicked: {
                        mainWindow.hide();
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#28282A"
            }
            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.fillHeight: true

                orientation: ListView.Vertical
                spacing: 10
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                model: lightsModel.items
                delegate: Loader {
                    active: true
                    source: "elements/KeyLight.qml" // TODO Add more types of lights
                    width: listView.width - (verticalScrollBar.visible ? verticalScrollBar.width : 0)
                    onLoaded: {
                        item.light = modelData
                        item.lastElement = index === lightsModel.items.length - 1
                    }
                }

                Controls.ScrollBar.vertical: Controls.ScrollBar {
                    id: verticalScrollBar
                    policy: Controls.ScrollBar.AsNeeded
                }
            }
        }
    }
}
