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

#ifndef UDPCLIENTSERVER_H
#define UDPCLIENTSERVER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE

class UDPClientServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPClientServer(QObject *parent = nullptr);
    ~UDPClientServer() override;

    enum IRCommands {
        Mode_Off,
        Mode_On,
        CoolMode_20,
        CoolMode_21,
        CoolMode_22,
        CoolMode_23,
        CoolMode_24,
        CoolMode_25,
        CoolMode_26,
        CoolMode_27,
        CoolMode_28,
        CoolMode_29,
        CoolMode_30,
        HeatMode_20,
        HeatMode_21,
        HeatMode_22,
        HeatMode_23,
        HeatMode_24,
        HeatMode_25,
        HeatMode_26,
        HeatMode_27,
        HeatMode_28,
        HeatMode_29,
        HeatMode_30
    };

    enum Direction {
        Client,
        Server
    };

    enum ServerCommands {
        GetInfo,
        GetTemp,
        SetTargetTemp,
        ProcessCommand
    };

    struct ResponseHead
    {
        qint8   direction;
        qint8   serverCommand;
        QString roomName;
        QChar   phase;
    };

    struct InfoData
    {
        QString conditionerName;
        quint32 power;
    };

    struct TemperatureData
    {
        qint8 currentTemp;
        qint8 targetTemp;
    };

    struct CommandData
    {
        qint8 lastIRCommand;
        bool  commandStatus;
    };

    void sendRequest(const QString &room, qint8 command, qint8 commandData);

private:
    QUdpSocket *udpSocket = nullptr;

private slots:
    void processPendingDatagrams();

signals:
    void receiveResponce(QByteArray &ba);

public slots:
};

#endif // UDPCLIENTSERVER_H
