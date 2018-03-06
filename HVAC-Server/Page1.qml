/****************************************************************************
**
** Copyright (C) 2018 Sergey Kovalenko <seryoga.engineering@gmail.com>
**
** This file is part of HVAC Server.
** HVAC Server is free software: you can redistribute
** it and/or modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation, either version 3 of
** the License, or (at your option) any later version.
**
** HVAC Server is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the
** GNU General Public License along with HVAC Server.
** If not, see http://www.gnu.org/licenses/.
**
****************************************************************************/

import QtQuick 2.9

Page1Form {

    totalPowerValue: DataProvider.totalPower

    phaseAPowerLabelText: DataProvider.phaseAPower
    phaseBPowerLabelText: DataProvider.phaseBPower
    phaseCPowerLabelText: DataProvider.phaseCPower

    textFieldphaseAThreshold {
        text: DataProvider.phaseALoadThreshold
        onFocusChanged: DataProvider.setPhaseALoadThreshold(textFieldphaseAThreshold.text)
    }

    textFieldphaseBThreshold {
        text: DataProvider.phaseBLoadThreshold
        onFocusChanged: DataProvider.setPhaseBLoadThreshold(textFieldphaseBThreshold.text)
    }

    textFieldphaseCThreshold {
        text: DataProvider.phaseCLoadThreshold
        onFocusChanged: DataProvider.setPhaseCLoadThreshold(textFieldphaseCThreshold.text)
    }


    listViewPhaseA {
        model: modelPhaseA

        delegate: RoomDelegate {
            onCheckedChanged: if (!checked) DataProvider.setNewTargetTemp(roomName, spinBoxValue)
        }
    }

    listViewPhaseB {
        model: modelPhaseB
        delegate: listViewPhaseA.delegate
    }

    listViewPhaseC {
        model: modelPhaseC
        delegate: listViewPhaseA.delegate
    }
}
