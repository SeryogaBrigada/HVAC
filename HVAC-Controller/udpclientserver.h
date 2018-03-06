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

#ifndef UDPCLIENTSERVER_H
#define UDPCLIENTSERVER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE
class DataProvider;

class UDPClientServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPClientServer(QObject *parent = nullptr, DataProvider *provider = nullptr);
    ~UDPClientServer() Q_DECL_OVERRIDE;

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

    void sendInfoDatagram();

private:
    DataProvider *m_provider = nullptr;
    QUdpSocket   *udpSocket  = nullptr;

    CommandData m_lastCommandData;

    void parseDatagram(QByteArray &ba);

public Q_SLOTS:
    void processPendingDatagrams();
};

#endif // UDPCLIENTSERVER_H
