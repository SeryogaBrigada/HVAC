import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#aff1ff"
    property alias phaseCPowerLabelText: phaseCPowerLabel.text
    property alias phaseBPowerLabelText: phaseBPowerLabel.text
    property alias phaseAPowerLabelText: phaseAPowerLabel.text
    property alias listViewPhaseC: listViewPhaseC
    property alias listViewPhaseB: listViewPhaseB
    property alias listViewPhaseA: listViewPhaseA
    property alias textFieldphaseCThreshold: textFieldphaseCThreshold
    property alias textFieldphaseBThreshold: textFieldphaseBThreshold
    property alias textFieldphaseAThreshold: textFieldphaseAThreshold
    property string totalPowerValue: "0"

    gradient: Gradient {
        GradientStop {
            position: 0.00
            color: "#aff1ff"
        }
        GradientStop {
            position: 1.00
            color: "#c4c4c4"
        }
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Label {
            id: label
            text: qsTr("Total Power Load: ") + totalPowerValue + qsTr(" kW")
            font.pointSize: 16
            topPadding: 10
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        }

        RowLayout {
            id: rowLayout
            width: 100
            height: 400
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            spacing: 10

            Rectangle {
                id: rectangle1
                width: 80
                height: 400
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#abff62"
                radius: 25

                ColumnLayout {
                    id: columnLayout1
                    anchors.rightMargin: 5
                    anchors.leftMargin: 5
                    anchors.bottomMargin: 5
                    anchors.topMargin: 5
                    anchors.fill: parent

                    Label {
                        id: label10
                        text: qsTr("Phase A")
                        font.pointSize: 16
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Grid {
                        id: grid
                        width: 100
                        height: 400
                        Layout.fillWidth: true
                        rows: 2
                        columns: 2

                        Label {
                            id: label1
                            width: grid.width - phaseAPowerLabel.width
                            height: 35
                            Layout.fillWidth: true
                            color: "#282b26"
                            text: qsTr("Power, kW")
                            padding: 6
                            font.pointSize: 13
                        }

                        Label {
                            id: phaseAPowerLabel
                            width: grid.width / 3
                            height: textFieldphaseAThreshold.height
                            padding: label1.padding
                            font.pointSize: label1.font.pointSize
                        }

                        Label {
                            id: label3
                            width: label1.width
                            height: textFieldphaseAThreshold.height
                            text: qsTr("Threshold, kW")
                            padding: label1.padding
                            font.pointSize: label1.font.pointSize
                        }

                        TextField {
                            id: textFieldphaseAThreshold
                            width: phaseAPowerLabel.width
                            height: label1.height
                            padding: label1.padding
                            font.pointSize: label1.font.pointSize
                            validator: RegExpValidator {
                                regExp: /\d{1,5}, [0-9]+/
                            }
                        }
                    }

                    ListView {
                        id: listViewPhaseA
                        width: grid.width
                        height: 300
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                    }
                }
            }

            Rectangle {
                width: 80
                height: 400
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#f2ff58"
                radius: rectangle1.radius

                ColumnLayout {
                    id: columnLayout2
                    anchors.rightMargin: 5
                    anchors.leftMargin: 5
                    anchors.bottomMargin: 5
                    anchors.topMargin: 5
                    anchors.fill: parent

                    Label {
                        id: label11
                        text: qsTr("Phase B")
                        font.pointSize: label10.font.pointSize
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Grid {
                        id: grid1
                        width: 100
                        height: 400
                        Layout.fillWidth: true
                        columns: 2
                        rows: 2

                        Label {
                            id: label4
                            width: label1.width
                            height: label1.height
                            text: label1.text
                            padding: label1.padding
                            font.pointSize: label1.font.pointSize
                        }

                        Label {
                            id: phaseBPowerLabel
                            width: phaseAPowerLabel.width
                            height: phaseAPowerLabel.height
                            padding: phaseAPowerLabel.padding
                            font.pointSize: label1.font.pointSize
                        }

                        Label {
                            id: label6
                            width: label3.width
                            height: label3.height
                            text: label3.text
                            padding: label3.padding
                            font.pointSize: label1.font.pointSize
                        }

                        TextField {
                            id: textFieldphaseBThreshold
                            width: textFieldphaseAThreshold.width
                            height: textFieldphaseAThreshold.height
                            padding: textFieldphaseAThreshold.padding
                            font.pointSize: label1.font.pointSize
                            validator: RegExpValidator {
                                regExp: /\d{1,5}, [0-9]+/
                            }
                        }
                    }

                    ListView {
                        id: listViewPhaseB
                        width: grid1.width
                        height: 300
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                    }
                }
            }

            Rectangle {
                width: 80
                height: 400
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#ff8762"
                radius: rectangle1.radius

                ColumnLayout {
                    id: columnLayout3
                    anchors.rightMargin: 5
                    anchors.leftMargin: 5
                    anchors.bottomMargin: 5
                    anchors.topMargin: 5
                    anchors.fill: parent

                    Label {
                        id: label12
                        text: qsTr("Phase C")
                        font.pointSize: label10.font.pointSize
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Grid {
                        id: grid2
                        width: 100
                        height: 400
                        Layout.fillWidth: true
                        columns: 2
                        rows: 2
                        Label {
                            id: label7
                            width: label1.width
                            height: label1.height
                            text: label1.text
                            padding: label1.padding
                            font.pointSize: label1.font.pointSize
                        }

                        Label {
                            id: phaseCPowerLabel
                            width: phaseAPowerLabel.width
                            height: phaseAPowerLabel.height
                            padding: phaseAPowerLabel.padding
                            font.pointSize: label1.font.pointSize
                        }

                        Label {
                            id: label9
                            width: label3.width
                            height: label3.height
                            text: label3.text
                            padding: label3.padding
                            font.pointSize: label1.font.pointSize
                        }

                        TextField {
                            id: textFieldphaseCThreshold
                            width: textFieldphaseAThreshold.width
                            height: textFieldphaseAThreshold.height
                            padding: textFieldphaseAThreshold.padding
                            font.pointSize: label1.font.pointSize
                            validator: RegExpValidator {
                                regExp: /\d{1,5}, [0-9]+/
                            }
                        }
                    }

                    ListView {
                        id: listViewPhaseC
                        width: grid2.width
                        height: 300
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                    }
                }
            }
        }
    }
}
