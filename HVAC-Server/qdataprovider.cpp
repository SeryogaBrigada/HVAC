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

#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include "qdataprovider.h"
#include "roomlistmodel.h"
#include "udpclientserver.h"

QDataProvider::QDataProvider(QObject *parent, RoomListModel *modelA,
                             RoomListModel *modelB, RoomListModel *modelC) :
    QObject(parent),
    m_modelPhaseA(modelA),
    m_modelPhaseB(modelB),
    m_modelPhaseC(modelC)
{
    m_settings = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator()
                               + "settings.ini", QSettings::IniFormat);

    m_phaseAThreshold = m_settings->value(QLatin1String("PhaseAThreshold_kW"), 0).toInt();
    m_phaseBThreshold = m_settings->value(QLatin1String("PhaseBThreshold_kW"), 0).toInt();
    m_phaseCThreshold = m_settings->value(QLatin1String("PhaseCThreshold_kW"), 0).toInt();

    m_powerManagerPeriod = m_settings->value(QLatin1String("PowerManagerPeriod_min")
                                             , 20).toInt() * 60 * 1000;

    m_updateTimerPeriod  = m_settings->value(QLatin1String("UpdatePeriod_sec")
                                             , 10).toInt() * 1000;

    m_switchPeriod       = m_settings->value(QLatin1String("SwitchPeriod_sec")
                                             , 2).toInt() * 1000;

    m_settings->setValue(QLatin1String("PowerManagerPeriod_min")
                         , m_powerManagerPeriod / (60 * 1000));
    m_settings->setValue(QLatin1String("UpdatePeriod_sec")
                         , m_updateTimerPeriod / 1000);
    m_settings->setValue(QLatin1String("SwitchPeriod_sec")
                         , m_switchPeriod / 1000);

    m_server = new UDPClientServer(this);
    connect(m_server, &UDPClientServer::receiveResponce, this, &QDataProvider::processResponce);

    m_powerTaskTimer  = new QTimer(this);
    m_senderTaskTimer = new QTimer(this);
    m_updateTimer     = new QTimer(this);

    connect(m_powerTaskTimer, &QTimer::timeout, this, &QDataProvider::powerTask);
    connect(m_senderTaskTimer, &QTimer::timeout, this, &QDataProvider::senderTask);
    connect(m_updateTimer, &QTimer::timeout, [&](){
        auto calculatePower = [](RoomListModel *model){
            int m_power = 0;

            for (int i = 0; i < model->getRowCount(); ++i) {
                if (model->getLastIRCommand(i) > 0)
                    m_power += model->getPower(i);
            }
            return m_power;
        };

        m_phaseAPower = calculatePower(m_modelPhaseA);
        m_phaseBPower = calculatePower(m_modelPhaseB);
        m_phaseCPower = calculatePower(m_modelPhaseC);

        powerChangedSignals();
        m_server->sendRequest("Any", UDPClientServer::ServerCommands::GetInfo, 0);
    });

    powerTask();
}

QDataProvider::~QDataProvider()
{
}

void  QDataProvider::senderTask()
{
    auto processQueue = [&](QQueue<QPair<QString, qint8>> *queue){
        if (!queue->isEmpty()) {
            auto m_pair = queue->dequeue();
            m_server->sendRequest(m_pair.first, UDPClientServer::ServerCommands::ProcessCommand,
                                  m_pair.second);
        }
    };

    processQueue(&m_queuePhaseA);
    processQueue(&m_queuePhaseB);
    processQueue(&m_queuePhaseC);

    if (m_queuePhaseA.isEmpty() && m_queuePhaseB.isEmpty() && m_queuePhaseC.isEmpty()) {
        m_senderTaskTimer->stop();
        m_powerTaskTimer->start(m_lastPowerPeriod);
        m_updateTimer->start(m_updateTimerPeriod);
    }
}

void QDataProvider::powerTask()
{
    // Update room list
    static int stage = 1;

    if (!stage) {
        stage = 1;
        m_powerTaskTimer->stop();
        m_updateTimer->stop();

        auto disableConditioners = [](RoomListModel *model, QQueue<QPair<QString, qint8>> *queue){
            for (int index = 0; index < model->getRowCount(); index++)
                queue->enqueue({model->getRoomName(index), IRCommands::Mode_Off});
        };

        disableConditioners(m_modelPhaseA, &m_queuePhaseA);
        disableConditioners(m_modelPhaseB, &m_queuePhaseB);
        disableConditioners(m_modelPhaseC, &m_queuePhaseC);

        m_lastPowerPeriod = 2000;
        m_senderTaskTimer->start(1000);
        return;

    } else if (1 == stage) {
        stage = 2;
        m_modelPhaseA->removeAll();
        m_modelPhaseB->removeAll();
        m_modelPhaseC->removeAll();

        m_powerTaskTimer->start(5000);
        m_server->sendRequest("Any", UDPClientServer::ServerCommands::GetInfo, 0);
        return;
    }
    stage = 0;

    m_lastPowerPeriod = m_powerManagerPeriod;

    // Calculate how many conditioners can be enabled
    static int lastPhaseAIndex = 0;
    static int lastPhaseBIndex = 0;
    static int lastPhaseCIndex = 0;

    auto calculatePower = [](RoomListModel *model, int threshold,
            int *lastIndex, QQueue<QPair<QString, qint8>> *queue){

        int m_count = model->getRowCount();

        if (!m_count)
            return;

        int m_idx = (*lastIndex >= m_count) ? 0 : *lastIndex;
        int m_power = model->getPower(m_idx);
        bool done = false;

        while (!done) {
            if (m_power > threshold) {
                done = true;
                break;
            }
            queue->enqueue({model->getRoomName(m_idx), (model->getTargetTemp(m_idx) - 18)});

            m_idx++;
            m_idx    = (m_idx >= m_count) ? 0 : m_idx;
            m_power += model->getPower(m_idx);

            if (*lastIndex == m_idx) {
                done = true;
                break;
            }
        }

        *lastIndex = m_idx;
    };

    calculatePower(m_modelPhaseA, m_phaseAThreshold * 1000,
                   &lastPhaseAIndex, &m_queuePhaseA);

    calculatePower(m_modelPhaseB, m_phaseBThreshold * 1000,
                   &lastPhaseBIndex, &m_queuePhaseB);

    calculatePower(m_modelPhaseC, m_phaseCThreshold * 1000,
                   &lastPhaseCIndex, &m_queuePhaseC);

    m_senderTaskTimer->start(m_switchPeriod);
}

void QDataProvider::processResponce(QByteArray &responce)
{
    QDataStream in(&responce, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_7);

    UDPClientServer::ResponseHead    m_head;
    UDPClientServer::InfoData        m_infoData;
    UDPClientServer::TemperatureData m_tempData;
    UDPClientServer::CommandData     m_commandData;

    in >> m_head.direction >> m_head.serverCommand >> m_head.roomName >> m_head.phase;

    RoomListModel *m_model = nullptr;

    if (QLatin1Char('A') == m_head.phase) {
        m_model = m_modelPhaseA;
    } else if (QLatin1Char('B') == m_head.phase) {
        m_model = m_modelPhaseB;
    } else {
        m_model = m_modelPhaseC;
    }

    switch (m_head.serverCommand) {
    case UDPClientServer::ServerCommands::GetInfo:
        in >> m_infoData.conditionerName >> m_infoData.power;
        in >> m_tempData.currentTemp >> m_tempData.targetTemp;
        in >> m_commandData.lastIRCommand >> m_commandData.commandStatus;

        if (!m_commandData.commandStatus)
            m_commandData.lastIRCommand = 0;

        m_model->append(m_head.roomName, m_infoData.conditionerName, m_infoData.power,
                        m_tempData.currentTemp, m_tempData.targetTemp,
                        m_commandData.lastIRCommand, m_commandData.lastIRCommand);
        break;

    case UDPClientServer::ServerCommands::GetTemp:
    case UDPClientServer::ServerCommands::SetTargetTemp:
        in >> m_tempData.currentTemp >> m_tempData.targetTemp;

        m_model->updateTemp(m_head.roomName, m_tempData.currentTemp, m_tempData.targetTemp);
        break;

    case UDPClientServer::ServerCommands::ProcessCommand:
        in >> m_commandData.lastIRCommand >> m_commandData.commandStatus;

        if (!m_commandData.commandStatus)
            m_commandData.lastIRCommand = 0;

        m_model->setLastIRCommandByName(m_head.roomName, m_commandData.lastIRCommand);
        break;

    default:
        break;
    }
}

void QDataProvider::setNewTargetTemp(const QString &roomName, qint8 value)
{
    m_server->sendRequest(roomName, UDPClientServer::ServerCommands::SetTargetTemp, value);
}

void QDataProvider::powerChangedSignals()
{
    emit phaseAPowerChanged();
    emit phaseBPowerChanged();
    emit phaseCPowerChanged();
    emit totalPowerChanged();
}

QString QDataProvider::totalPower() const
{
    return QString::number((m_phaseAPower + m_phaseBPower + m_phaseCPower) / 1000.0, 'f', 1);
}

QString QDataProvider::phaseAPower() const
{
    return QString::number(m_phaseAPower / 1000.0, 'f', 1);
}

QString QDataProvider::phaseBPower() const
{
    return QString::number(m_phaseBPower / 1000.0, 'f', 1);
}

QString QDataProvider::phaseCPower() const
{
    return QString::number(m_phaseCPower / 1000.0, 'f', 1);
}

QString QDataProvider::phaseALoadThreshold() const
{
    return QString::number(m_phaseAThreshold);
}

QString QDataProvider::phaseBLoadThreshold() const
{
    return QString::number(m_phaseBThreshold);
}

QString QDataProvider::phaseCLoadThreshold() const
{
    return QString::number(m_phaseCThreshold);
}

void QDataProvider::setPhaseALoadThreshold(const QString &data)
{
    m_settings->setValue("PhaseAThreshold_kW", data);
    m_phaseAThreshold = data.toInt();
    emit phaseALoadThresholdChanged();
}

void QDataProvider::setPhaseBLoadThreshold(const QString &data)
{
    m_settings->setValue("PhaseBThreshold_kW", data);
    m_phaseBThreshold = data.toInt();
    emit phaseBLoadThresholdChanged();
}

void QDataProvider::setPhaseCLoadThreshold(const QString &data)
{
    m_settings->setValue("PhaseCThreshold_kW", data);
    m_phaseCThreshold = data.toInt();
    emit phaseCLoadThresholdChanged();
}
