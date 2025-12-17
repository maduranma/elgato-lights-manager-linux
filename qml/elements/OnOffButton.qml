import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property bool checked: false
    signal toggled(bool checked)

    width: 32
    height: 32

    Rectangle {
        width: 32
        height: 32
        radius: 16
        color: root.checked ? "#fff" : "#999"

        Rectangle {
            anchors.centerIn: parent
            width: 28
            height: 28
            radius: 14
            color: "#1C1C21"

            Rectangle {
                anchors.centerIn: parent
                width: 24
                height: 24
                radius: 12
                color: root.checked ? "#fff" : "#1C1C21"

                Image {
                    anchors.centerIn: parent
                    width: 18
                    height: 18
                    id: icon
                    source: root.checked ? "qrc:/svg/power-black.svg" : "qrc:/svg/power-white.svg"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.toggled(!root.checked)
        }
        cursorShape: Qt.PointingHandCursor
    }
}
