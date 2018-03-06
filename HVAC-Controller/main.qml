/****************************************************************************
**
** Copyright (C) 2018 Sergey Kovalenko <seryoga.engineering@gmail.com>
**
** This file is part of HVAC Controller.
** HVAC Controller is free software: you can redistribute
** it and/or modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation, either version 3 of
** the License, or (at your option) any later version.
**
** HVAC Controller is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the
** GNU General Public License along with HVAC Controller.
** If not, see http://www.gnu.org/licenses/.
**
****************************************************************************/

import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.3

import io.qt.dataprovider 1.0

ApplicationWindow {
    id: root
    visible: false
    width: 416
    height: 424

    minimumWidth: width
    minimumHeight: height

    maximumWidth: width
    maximumHeight: height

    title: "HVAC Controller"

    //Prevent Application closing
    onClosing: root.hide()

    Connections {
        target: systemTray
        onSignalShowHide: {
            root.setVisible( !(root.visibility) )
        }

        onSignalQuit: {
            Qt.quit()
        }
    }

    DataProvider {
        id: provider
        onConnectionStatusChanged: systemTray.connectionChanged(connectionStatus)
    }


    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        onCurrentIndexChanged: provider.asyncEnableChanged(currentIndex === 1)

        Page1Form {
            isConnected: provider.connectionStatus
            roomLabelText: provider.roomName
            modelLabelText: provider.conditionerName
            currentTempLabelText: provider.currentTemp + " °C"
            targetTempLabelText: provider.targetTemp + " °C"
        }

        Page2Form {
            textFieldRoomNo.text: provider.roomName
            textFieldModel.text: provider.conditionerName
            textFieldPower.text: provider.powerLoad
            textFieldPhase.text: provider.phaseLoad

            textFieldRoomNo.onFocusChanged: provider.setRoomName(textFieldRoomNo.text)
            textFieldModel.onFocusChanged: provider.setConditionerName(textFieldModel.text)
            textFieldPower.onFocusChanged: provider.setPowerLoad(textFieldPower.text)
            textFieldPhase.onFocusChanged: provider.setPhaseLoad(textFieldPhase.text)

            listView.enabled: provider.connectionStatus
            listView.delegate: SwitchDelegate {
                id: delegate
                text: name
                font.pixelSize: 16
                width: root.width

                onClicked: {
                    provider.commandPressed(index)
                    checked = provider.getCommandStatus(index)
                }

                Dialog {
                    id: dialogReset
                    property int idx: -1

                    title: qsTr("Reset Command?")
                    modal: true
                    focus: true
                    x: parent.width / 2 - width / 2
                    y: -42
                    height: 100
                    width: 200
                    standardButtons: Dialog.Ok | Dialog.Cancel
                    onAccepted: {
                        provider.commandReset(idx)
                        delegate.checked = false
                    }
                }

                onPressAndHold: {
                    dialogReset.idx = index
                    dialogReset.open()
                }

                Component.onCompleted: {
                    checked = provider.getCommandStatus(index)
                }
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        font.pointSize: 12

        TabButton {
            text: qsTr("Main")
        }

        TabButton {
            text: qsTr("Settings")
        }
    }
}
