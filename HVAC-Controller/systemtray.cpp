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

#include <QMenu>
#include "systemtray.h"

SystemTray::SystemTray(QWidget *parent) : QWidget(parent)
{
    QAction * viewWindow = new QAction(tr("Show/Hide"), this);
    QAction * quitAction = new QAction(tr("Quit"), this);

    connect(viewWindow, &QAction::triggered, this, &SystemTray::signalShowHide);
    connect(quitAction, &QAction::triggered, this, &SystemTray::signalQuit);

    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(viewWindow);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
       if (QSystemTrayIcon::ActivationReason::DoubleClick == reason)
           emit signalShowHide();
    });

//  <a href="https://icons8.com/icon/50103/Remote-Control">Remote control icon credits</a>
    trayIcon->setIcon(QIcon(QLatin1String(":/Images/tray-icon.png")));

    trayIcon->setToolTip(QLatin1String("HVAC Controller"));
    trayIcon->show();
}

SystemTray::~SystemTray()
{
}

void SystemTray::connectionChanged(bool status)
{
    if (m_connectionStatus == status)
        return;

    m_connectionStatus = status;

    trayIcon->showMessage(QLatin1String("HVAC Controlled"),
                          status ? tr("Connected") : tr("Disconnected"),
                          QSystemTrayIcon::Information, 2500);
}
