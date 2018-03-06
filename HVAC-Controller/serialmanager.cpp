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

#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    connect(&m_serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialManager::handleError);
    connect(&m_timer, &QTimer::timeout, this, &SerialManager::findConnectionPort);
}

SerialManager::~SerialManager()
{
}

void SerialManager::findConnectionPort()
{
    for (const QSerialPortInfo &serialPortInfo : QSerialPortInfo::availablePorts()) {
        m_serial.setPort(serialPortInfo);
        m_serial.setBaudRate(QSerialPort::Baud115200);

        if (m_serial.open(QIODevice::ReadWrite)) {
            bool sended;
            QByteArray response = sendCommand('i', nullptr, 100, &sended);

            if (sended && response.contains("HVAC")) {
                emit connectionChanged(true);
                m_timer.stop();
                return;
            } else {
                m_serial.close();
            }
        }
    }

    m_timer.start(2000);
    emit connectionChanged(false);
}

void SerialManager::handleError(QSerialPort::SerialPortError error)
{
    if (QSerialPort::ResourceError == error) {
        if (m_serial.isOpen())
            m_serial.close();

        m_timer.start(3000);
        emit connectionChanged(false);
    }
}

QByteArray SerialManager::sendCommand(char command, const QByteArray &data, int timeout, bool *status)
{
    QByteArray array;
    array.reserve(481);
    *status = false;

    if (!m_serial.isOpen())
        return nullptr;

    array.append(command);
    array.append(data);

    m_serial.write(array);
    array.clear();

    if (m_serial.waitForReadyRead(timeout)) {
        array.append(m_serial.readAll());

        while(m_serial.waitForReadyRead(100))
            array.append(m_serial.readAll());

        if (array.back() == command) {
            array.chop(1);
            *status = true;
        }
    }
    return array;
}
