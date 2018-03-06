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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QWidget>
#include <QSystemTrayIcon>

class SystemTray : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = nullptr);
    ~SystemTray() override;

signals:
    void signalShowHide();
    void signalQuit();

private:
    QSystemTrayIcon *trayIcon = nullptr;

public slots:
};

#endif // SYSTEMTRAY_H
