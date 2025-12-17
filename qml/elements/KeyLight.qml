import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import ElgatoLightsManagerLinux

ColumnLayout {
    Layout.fillWidth: true
    property Light light
    property bool lastElement

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: 15
        spacing: 15

        OnOffButton {
            checked: light.enabled
            onToggled: function(checked) {
                light.setEnabled(checked)
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                text: light.name
                font.bold: true
                color: "#fff"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Image {
                    source: "qrc:/svg/low-brightness.svg"
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    fillMode: Image.PreserveAspectFit
                }
                Slider {
                    from: 0
                    to: 100
                    Layout.fillWidth: true
                    value: light.brightness
                    onMoved: light.setBrightness(value)
                }
                Image {
                    source: "qrc:/svg/high-brightness.svg"
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    fillMode: Image.PreserveAspectFit
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Image {
                    source: "qrc:/svg/low-temperature.svg"
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    fillMode: Image.PreserveAspectFit
                }
                Slider {
                    from: 143
                    to: 344
                    Layout.fillWidth: true
                    value: light.colorTemperature
                    onMoved: light.setColorTemperature(value)
                }
                Image {
                    source: "qrc:/svg/high-temperature.svg"
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    Rectangle {
        Layout.fillWidth: true
        height: 1
        color: "#28282A"
        visible: !lastElement
    }
}
