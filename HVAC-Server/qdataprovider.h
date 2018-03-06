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

#ifndef QDATAPROVIDER_H
#define QDATAPROVIDER_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QQueue>
#include <QPair>

class RoomListModel;
class UDPClientServer;

class QDataProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString totalPower READ totalPower NOTIFY totalPowerChanged)

    Q_PROPERTY(QString phaseAPower READ phaseAPower NOTIFY phaseAPowerChanged)
    Q_PROPERTY(QString phaseBPower READ phaseBPower NOTIFY phaseBPowerChanged)
    Q_PROPERTY(QString phaseCPower READ phaseCPower NOTIFY phaseCPowerChanged)

    Q_PROPERTY(QString phaseALoadThreshold READ phaseALoadThreshold
               NOTIFY phaseALoadThresholdChanged)
    Q_PROPERTY(QString phaseBLoadThreshold READ phaseBLoadThreshold
               NOTIFY phaseBLoadThresholdChanged)
    Q_PROPERTY(QString phaseCLoadThreshold READ phaseCLoadThreshold
               NOTIFY phaseCLoadThresholdChanged)

public:
    explicit QDataProvider(QObject *parent = nullptr,
                           RoomListModel *modelA = nullptr,
                           RoomListModel *modelB = nullptr,
                           RoomListModel *modelC = nullptr);
    ~QDataProvider() override;

    QString totalPower() const;

    QString phaseAPower() const;
    QString phaseBPower() const;
    QString phaseCPower() const;

    QString phaseALoadThreshold() const;
    QString phaseBLoadThreshold() const;
    QString phaseCLoadThreshold() const;

    Q_INVOKABLE void setPhaseALoadThreshold(const QString &data);
    Q_INVOKABLE void setPhaseBLoadThreshold(const QString &data);
    Q_INVOKABLE void setPhaseCLoadThreshold(const QString &data);

    Q_INVOKABLE void setNewTargetTemp(const QString &roomName, qint8 value);

private:
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

    QSettings       *m_settings = nullptr;
    UDPClientServer *m_server   = nullptr;

    RoomListModel *m_modelPhaseA = nullptr;
    RoomListModel *m_modelPhaseB = nullptr;
    RoomListModel *m_modelPhaseC = nullptr;

    QTimer *m_powerTaskTimer  = nullptr;
    QTimer *m_senderTaskTimer = nullptr;
    QTimer *m_updateTimer     = nullptr;

    QQueue<QPair<QString, qint8>> m_queuePhaseA;
    QQueue<QPair<QString, qint8>> m_queuePhaseB;
    QQueue<QPair<QString, qint8>> m_queuePhaseC;

    int m_phaseAPower = 0;
    int m_phaseBPower = 0;
    int m_phaseCPower = 0;

    int m_phaseAThreshold;
    int m_phaseBThreshold;
    int m_phaseCThreshold;

    int m_powerManagerPeriod;
    int m_updateTimerPeriod;
    int m_lastPowerPeriod;
    int m_switchPeriod;

    void powerTask();
    void senderTask();
    void powerChangedSignals();

signals:
    void totalPowerChanged();

    void phaseAPowerChanged();
    void phaseBPowerChanged();
    void phaseCPowerChanged();

    void phaseALoadThresholdChanged();
    void phaseBLoadThresholdChanged();
    void phaseCLoadThresholdChanged();

private slots:
    void processResponce(QByteArray &responce);

public slots:

};

#endif // QDATAPROVIDER_H
