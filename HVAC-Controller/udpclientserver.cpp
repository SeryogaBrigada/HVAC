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

#include <QUdpSocket>
#include <QDataStream>
#include "udpclientserver.h"
#include "dataprovider.h"

#include <QDebug>

UDPClientServer::UDPClientServer(QObject *parent, DataProvider *provider) : QObject(parent)
  , m_provider(provider)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UDPClientServer::processPendingDatagrams);

    m_lastCommandData.lastIRCommand = 0;
    m_lastCommandData.commandStatus = false;
}

UDPClientServer::~UDPClientServer()
{
}

void UDPClientServer::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        parseDatagram(datagram);
    }
}

void UDPClientServer::parseDatagram(QByteArray &ba)
{
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_7);

    ResponseHead head;

    in >> head.direction;

    if (head.direction == Direction::Client)
        return;

    in >> head.serverCommand >> head.roomName;

    //Set outgoing direction
    head.direction = Direction::Client;

    QByteArray m_responseData;
    QDataStream out(&m_responseData, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_5_7);

    if (head.serverCommand == ServerCommands::GetInfo) {
        sendInfoDatagram();
        return;
    }

    if (head.roomName != m_provider->roomName())
        return;

    head.phase = m_provider->phaseLoad().at(0);

    // Write datagram head
    out << head.direction << head.serverCommand << head.roomName << head.phase;

    TemperatureData tempData;

    switch (head.serverCommand) {
    case ServerCommands::SetTargetTemp:
        in >> tempData.targetTemp;
        m_provider->setTargetTemp(QString::number(tempData.targetTemp));
        Q_FALLTHROUGH();
    case ServerCommands::GetTemp:
        tempData.currentTemp = m_provider->getCurrentTempIntValue();
        tempData.targetTemp = static_cast<qint8>(m_provider->targetTemp().toInt());

        out << tempData.currentTemp << tempData.targetTemp;
        break;

    case ServerCommands::ProcessCommand:
        in >> m_lastCommandData.lastIRCommand;
        m_provider->replayCommand(m_lastCommandData.lastIRCommand,
                                  &m_lastCommandData.commandStatus);

        out << m_lastCommandData.lastIRCommand << m_lastCommandData.commandStatus;
        break;

    default:
        break;
    }

    udpSocket->writeDatagram(m_responseData, m_responseData.size(),
                             QHostAddress::Broadcast, 45454);
}

void UDPClientServer::sendInfoDatagram()
{
    if (m_provider->roomName().isEmpty() ||
            m_provider->phaseLoad().isEmpty() ||
            m_provider->conditionerName().isEmpty() ||
            m_provider->powerLoad().isEmpty())
        return;

    ResponseHead head;
    head.direction = Direction::Client;
    head.serverCommand = ServerCommands::GetInfo;
    head.roomName = m_provider->roomName();
    head.phase = m_provider->phaseLoad().at(0);

    InfoData data;
    data.conditionerName = m_provider->conditionerName();
    data.power = m_provider->powerLoad().toUInt();

    QByteArray m_responseData;
    QDataStream out(&m_responseData, QIODevice::WriteOnly);

    out << head.direction << head.serverCommand << head.roomName << head.phase;
    out << data.conditionerName << data.power;

    TemperatureData tempData;
    tempData.currentTemp = m_provider->getCurrentTempIntValue();
    tempData.targetTemp = static_cast<qint8>(m_provider->targetTemp().toInt());

    out << tempData.currentTemp << tempData.targetTemp;
    out << m_lastCommandData.lastIRCommand << m_lastCommandData.commandStatus;

    udpSocket->writeDatagram(m_responseData, m_responseData.size(),
                             QHostAddress::Broadcast, 45454);
}


