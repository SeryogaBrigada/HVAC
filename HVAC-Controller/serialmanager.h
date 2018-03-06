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

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager() override;

    void findConnectionPort();
    QByteArray sendCommand(char command, const QByteArray &data, int timeout,
                           bool *status = nullptr);


private:
    QSerialPort m_serial;
    QTimer      m_timer;

Q_SIGNALS:
    void connectionChanged(bool status);

public Q_SLOTS:
    void handleError(QSerialPort::SerialPortError error);

};

#endif // SERIALMANAGER_H
