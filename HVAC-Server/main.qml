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
import QtQuick.Controls 2.3
import QtQuick.Window 2.3

ApplicationWindow {
    id: application
    visible: true
    x: (Screen.desktopAvailableWidth - width)/2
    y: (Screen.desktopAvailableHeight - height)/2
    width: 800
    height: 480
    minimumWidth: 800
    minimumHeight: 480

    title: "HVAC Server"

    onClosing: application.hide()

    Connections {
        target: systemTray
        onSignalShowHide: {
            application.setVisible( !application.visibility )
        }

        onSignalQuit: {
            Qt.quit()
        }
    }


    Page1 {
        anchors.fill: parent
    }
}
