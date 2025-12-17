import QtQuick

Rectangle {
    id: root
    radius: 3
    color: "transparent"
    width: 30
    height: 30
    property string text: ""
    property url icon: ""
    signal clicked

    Loader {
        anchors.centerIn: parent
        sourceComponent: root.text === ""
            ? iconComponent
            : textComponent
    }

    Component {
        id: iconComponent
        Image {
            source: root.icon
            width: 24
            height: 24
            fillMode: Image.PreserveAspectFit
        }
    }

    Component {
        id: textComponent
        Text {
            anchors.fill: parent
            color: "#fff"
            font.pointSize: 20
            text: root.text

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }



    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()

        hoverEnabled: true
        onEntered: parent.color = Qt.rgba(255, 255, 255, 0.05)
        onExited: parent.color = "transparent"
    }
}