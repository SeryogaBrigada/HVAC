import QtQuick 2.9
import QtQuick.Controls 2.3

Item {
    width: 416
    height: 424

    property alias targetTempLabelText: targetTempLabel.text
    property alias currentTempLabelText: currentTempLabel.text
    property alias modelLabelText: modelLabel.text
    property alias roomLabelText: roomLabel.text

    property bool isConnected: false

    state: isConnected ? "Connected" : "Disconnected"

    Image {
        id: image
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        source: "Images/background.jpg"
    }

    Text {
        id: textRoomNumber
        x: 8
        y: 157
        width: 186
        height: 43
        text: qsTr("Room")
        font.pointSize: 16
        textFormat: Text.PlainText
        fontSizeMode: Text.Fit
        verticalAlignment: Text.AlignVCenter
        z: 1
    }

    Text {
        id: textModel
        x: 8
        y: 206
        width: 186
        height: 43
        text: qsTr("Model")
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: textCurrentTemp
        x: 8
        y: 255
        width: 186
        height: 43
        text: qsTr("Current temp.")
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: textTargetTemp
        x: 8
        y: 304
        width: 186
        height: 43
        text: qsTr("Target temp.")
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: roomLabel
        x: 200
        y: 157
        width: 208
        height: 43
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: modelLabel
        x: 200
        y: 206
        width: 208
        height: 43
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: 14
    }

    Text {
        id: currentTempLabel
        x: 200
        y: 255
        width: 208
        height: 43
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: targetTempLabel
        x: 200
        y: 304
        width: 208
        height: 43
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        z: 1
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }

    Text {
        id: statusLabel
        x: 99
        y: 8
        width: 118
        height: 24
        color: "#00ff00"
        text: qsTr("Connected")
        styleColor: "#000000"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        fontSizeMode: Text.Fit
        font.pointSize: textRoomNumber.font.pointSize
    }
    states: [
        State {
            name: "Disconnected"

            PropertyChanges {
                target: textRoomNumber
                color: "#929292"
            }

            PropertyChanges {
                target: textModel
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: textCurrentTemp
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: textTargetTemp
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: roomLabel
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: modelLabel
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: currentTempLabel
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: targetTempLabel
                color: textRoomNumber.color
            }

            PropertyChanges {
                target: statusLabel
                color: "#ff1010"
                text: qsTr("Disconnected")
            }
        }
    ]
}
