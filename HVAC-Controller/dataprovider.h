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

#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include <QSettings>
#include <QTimer>

class UDPClientServer;
class SerialManager;


class DataProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentTemp READ currentTemp NOTIFY currentTempChanged)
    Q_PROPERTY(QString targetTemp READ targetTemp NOTIFY targetTempChanged)

    Q_PROPERTY(QString roomName READ roomName NOTIFY roomNameChanged)
    Q_PROPERTY(QString conditionerName READ conditionerName NOTIFY conditionerNameChanged)
    Q_PROPERTY(QString powerLoad READ powerLoad NOTIFY powerLoadChanged)
    Q_PROPERTY(QString phaseLoad READ phaseLoad NOTIFY phaseLoadChanged)

    Q_PROPERTY(bool connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)

public:
    explicit DataProvider(QObject *parent = nullptr);
    ~DataProvider() override;

    bool connectionStatus() const;

    Q_INVOKABLE bool getCommandStatus(int index) const;
    int getCommandLength(int index) const;
    QByteArray getCommandData(int index) const;

    void setCommandStatus(int index, bool saved);
    void setCommandLength(int index, uint8_t length);
    void setCommandData(int index, const QByteArray &data);

    void replayCommand(int index, bool *status);
    void setTargetTemp(const QString &value);

    QString currentTemp() const;
    QString targetTemp() const;

    qint8 getCurrentTempIntValue() const;

    QString roomName() const;
    QString conditionerName() const;

    QString powerLoad() const;
    QString phaseLoad() const;

    Q_INVOKABLE void setRoomName(const QString &name);
    Q_INVOKABLE void setConditionerName(const QString &name);

    Q_INVOKABLE void setPowerLoad(const QString &load);
    Q_INVOKABLE void setPhaseLoad(const QString &load);

    double m_currentTempValue;

private:
    QSettings       *m_settings  = nullptr;
    SerialManager   *s_manager   = nullptr;
    UDPClientServer *m_UDPserver = nullptr;

    QTimer m_timer;
    bool   m_isConnected;

    enum DeviceCommands {
        Info        = 'i',
        Read        = 'r',
        Write       = 'w',
        CurrentTemp = 't',
        Replay      = 'p'
    };

Q_SIGNALS:
    void roomNameChanged();
    void conditionerNameChanged();
    void powerLoadChanged();
    void phaseLoadChanged();

    void currentTempChanged();
    void targetTempChanged();

    void connectionStatusChanged();

public Q_SLOTS:
    void commandPressed(int index);
    void commandReset(int index);
    void serialSignalHandler(bool status);
    void asyncEnableChanged(bool enable);

};

#endif // DATAPROVIDER_H
