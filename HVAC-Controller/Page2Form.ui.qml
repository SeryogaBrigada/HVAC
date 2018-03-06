import QtQuick 2.9
import QtQuick.Controls 2.3

Rectangle {
    id: rectangle
    width: 416
    height: 424
    color: "#8cadc5"
    property alias listView: listView
    property alias textFieldPhase: textFieldPhase
    property alias textFieldPower: textFieldPower
    property alias textFieldModel: textFieldModel
    property alias textFieldRoomNo: textFieldRoomNo

    TextField {
        id: textFieldRoomNo
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.top: parent.top
        anchors.topMargin: 6
        width: (parent.width / 2) - 8
        placeholderText: qsTr("Room")
        text: dataProvider.roomName
        font.pointSize: 12
    }

    TextField {
        id: textFieldModel
        anchors.top: textFieldRoomNo.top
        anchors.left: textFieldRoomNo.right
        anchors.leftMargin: 4
        anchors.bottom: textFieldRoomNo.bottom
        width: textFieldRoomNo.width
        placeholderText: qsTr("Conditioner model")
        text: dataProvider.conditionerName
        font.pointSize: textFieldRoomNo.font.pointSize
    }

    TextField {
        id: textFieldPower
        anchors.top: textFieldRoomNo.bottom
        anchors.topMargin: 4
        anchors.left: textFieldRoomNo.left
        anchors.right: textFieldRoomNo.right
        placeholderText: qsTr("Power load, W")
        validator: RegExpValidator {
            regExp: /[0-9]+/
        }
        text: dataProvider.powerLoad
        font.pointSize: textFieldRoomNo.font.pointSize
    }

    TextField {
        id: textFieldPhase
        anchors.top: textFieldPower.top
        anchors.bottom: textFieldPower.bottom
        anchors.left: textFieldModel.left
        anchors.right: textFieldModel.right
        placeholderText: qsTr("Power phase (A,B,C)")
        validator: RegExpValidator {
            regExp: /[A-C]/
        }
        text: dataProvider.phaseLoad
        font.pointSize: textFieldRoomNo.font.pointSize
    }

    Text {
        id: textIR
        anchors.top: textFieldPower.bottom
        anchors.topMargin: 6
        text: qsTr("IR commands")
        anchors.horizontalCenter: parent.horizontalCenter
        fontSizeMode: Text.Fit
        font.pointSize: textFieldRoomNo.font.pointSize
    }

    ScrollView {
        anchors.left: parent.left
        anchors.top: textIR.bottom
        anchors.topMargin: 6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        clip: true
        font.pointSize: textFieldRoomNo.font.pointSize

        ListView {
            id: listView
            spacing: 4
            cacheBuffer: 24
            clip: true

            model: ButtonModel {
            }
        }
    }
}
