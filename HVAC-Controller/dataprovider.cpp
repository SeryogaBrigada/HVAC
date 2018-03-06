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

#include <QDir>
#include <QCoreApplication>
#include "dataprovider.h"
#include "serialmanager.h"
#include "udpclientserver.h"

DataProvider::DataProvider(QObject *parent) : QObject(parent),
    m_currentTempValue(0), m_isConnected(false)
{
    m_settings = new QSettings(QCoreApplication::applicationDirPath()
                               + QDir::separator() + "settings.ini",
                               QSettings::IniFormat);

    m_UDPserver = new UDPClientServer(this, this);
    s_manager   = new SerialManager(this);

    connect(s_manager, &SerialManager::connectionChanged, this, &DataProvider::serialSignalHandler);
    s_manager->findConnectionPort();

    connect(&m_timer, &QTimer::timeout, [&](){
        bool status;
        QByteArray tmp = s_manager->sendCommand(DeviceCommands::CurrentTemp, nullptr, 1500, &status);

        if ( status && (tmp.count() == 2) ) {
            uint8_t byteL = static_cast<uint8_t>(tmp.at(0));
            uint8_t byteH = static_cast<uint8_t>(tmp.at(1));
            uint16_t temp;
            bool m_tempIsNegative = false;

            if (byteH & (1<<8)) {
                m_tempIsNegative = true;
                temp = byteH | byteL;
                temp = ~temp + 1;

                byteL = static_cast<uint8_t>(temp);
                byteH = temp >> 8;
            }
            temp = static_cast<uint16_t>( (byteL >> 4) | ((byteH & 7) << 4) );
            m_currentTempValue = temp + (byteL & 15) * 0.0625 - 3.25; // Correction -3.25 degrees

            if (m_tempIsNegative)
                m_currentTempValue = -m_currentTempValue;

            emit currentTempChanged();
        }
    });

    m_timer.start(5000);
}

DataProvider::~DataProvider()
{
}

void DataProvider::asyncEnableChanged(bool enable)
{
    if (enable) {
        m_timer.stop();
    } else {
        m_timer.start(5000);
        m_UDPserver->sendInfoDatagram();
    }
}


bool DataProvider::connectionStatus() const
{
    return m_isConnected;
}

void DataProvider::serialSignalHandler(bool status)
{
    m_isConnected = status;
    emit connectionStatusChanged();
}


/*!
 * For Page 1
 */
QString DataProvider::currentTemp() const
{
    return QString::number(m_currentTempValue, 'f', 2);
}

QString DataProvider::targetTemp() const
{
    return m_settings->value("TargetTemperature", "23").toString();
}

void DataProvider::setTargetTemp(const QString &value)
{
    m_settings->setValue("TargetTemperature", value);
    m_settings->sync();
    emit targetTempChanged();
}

qint8 DataProvider::getCurrentTempIntValue() const
{
    qint8 temp;
    if (m_currentTempValue > 0)
        temp = static_cast<qint8>(m_currentTempValue + 0.5);
    else
        temp = static_cast<qint8>(m_currentTempValue - 0.5);

    return temp;
}

/*!
 * For Page 2
 */
QString DataProvider::roomName() const
{
    return m_settings->value("RoomName", "").toString();
}

QString DataProvider::conditionerName() const
{
    return m_settings->value("ConditionerName", "").toString();
}

QString DataProvider::powerLoad() const
{
    return m_settings->value("PowerLoad", "").toString();
}

QString DataProvider::phaseLoad() const
{
    return m_settings->value("PhaseLoad", "").toString();
}

/*!
 * \brief ListView command handler
 * \param index
 */
void DataProvider::commandPressed(int index)
{
    bool commandSaved = false;

    // Receive and save command
    if (!getCommandStatus(index)) {
        QByteArray data = s_manager->sendCommand(DeviceCommands::Read,
                                                 nullptr, 7000, &commandSaved);
        if (commandSaved) {
            setCommandLength(index, static_cast<quint8>(data.at(0)));
            setCommandData(index, data);
            setCommandStatus(index, commandSaved);
        }
    }
}

void DataProvider::replayCommand(int index, bool *status)
{
    s_manager->sendCommand(DeviceCommands::Write, getCommandData(index), 500, status);
}

void DataProvider::commandReset(int index)
{
    m_settings->remove(QString("Command_%1").arg(index));
    m_settings->sync();
}


void DataProvider::setRoomName(const QString &name)
{
    if (name.isEmpty())
        return;

    m_settings->setValue("RoomName", name);
    m_settings->sync();

    emit roomNameChanged();
}

void DataProvider::setConditionerName(const QString &name)
{
    if (name.isEmpty())
        return;

    m_settings->setValue("ConditionerName", name);
    m_settings->sync();

    emit conditionerNameChanged();
}

void DataProvider::setPowerLoad(const QString &load)
{
    if (load.isEmpty())
        return;

    m_settings->setValue("PowerLoad", load);
    m_settings->sync();

    emit powerLoadChanged();
}

void DataProvider::setPhaseLoad(const QString &load)
{
    if (load.isEmpty())
        return;

    m_settings->setValue("PhaseLoad", load);
    m_settings->sync();

    emit phaseLoadChanged();
}

bool DataProvider::getCommandStatus(int index) const
{
    return m_settings->value(QString("Command_%1/status").arg(index), false).toBool();
}


int DataProvider::getCommandLength(int index) const
{
    return m_settings->value(QString("Command_%1/length").arg(index), 0).toInt();
}


QByteArray DataProvider::getCommandData(int index) const
{
    return QByteArray::fromBase64(m_settings->value(QString("Command_%1/data")
                                                    .arg(index), "").toByteArray());
}

void DataProvider::setCommandStatus(int index, bool saved)
{
    m_settings->setValue(QString("Command_%1/status").arg(index), saved);
    m_settings->sync();
}

void DataProvider::setCommandLength(int index, uint8_t length)
{
    m_settings->setValue(QString("Command_%1/length").arg(index), length);
    m_settings->sync();
}

void DataProvider::setCommandData(int index, const QByteArray &data)
{
    m_settings->setValue(QString("Command_%1/data").arg(index), data.toBase64());
    m_settings->sync();
}


