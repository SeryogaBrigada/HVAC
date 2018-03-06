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

#include <QUdpSocket>
#include <QDataStream>
#include "udpclientserver.h"

UDPClientServer::UDPClientServer(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UDPClientServer::processPendingDatagrams);
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

        if (datagram.startsWith(Direction::Client))
            emit receiveResponce(datagram);
    }
}

void UDPClientServer::sendRequest(const QString &room, qint8 command, qint8 commandData)
{
    QByteArray m_request;
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    out << quint8(Direction::Server) << command << room;

    switch (command) {
    case ServerCommands::SetTargetTemp:
    case ServerCommands::ProcessCommand:
        out << commandData;
        break;
    default:
        break;
    }

    udpSocket->writeDatagram(m_request, m_request.size(), QHostAddress::Broadcast, 45454);
}

