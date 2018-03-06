import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

ItemDelegate {
    id: delegate
    checkable: true

    property alias spinBoxValue: spinBox.value

    contentItem: ColumnLayout {}

    ColumnLayout {
        spacing: 10

        RowLayout {
            Label {
                id: roomLabel
                text: roomName
                font.pointSize: 12
                font.bold: true
                Layout.fillWidth: true
                color: isRunning ? "#000000" : "#808080"
            }

            Label {
                text: currentTemp + "°C"
                font.pointSize: 12
                Layout.fillWidth: true
                color: roomLabel.color
            }
        }

        GridLayout {
            id: grid
            visible: false
            columns: 2
            rowSpacing: 5
            columnSpacing: 5
            Layout.leftMargin: 20

            Label {
                text: qsTr("Model:")
            }

            Label {
                text: modelName
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Power:")
            }

            Label {
                text: power + " W"
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Target temp.:")
            }

            SpinBox {
                id: spinBox
                stepSize: 1
                to: 30
                from: 20
                value: targetTemp
            }
        }
    }

    states: [
        State {
            name: "expanded"
            when: delegate.checked

            PropertyChanges {
                target: grid
                visible: true
            }

            PropertyChanges {
                target: delegate
                height: grid.height + roomLabel.height + 10
            }
        }
    ]
}
