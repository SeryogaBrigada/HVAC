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

#ifndef ROOMLISTMODEL_H
#define ROOMLISTMODEL_H

#include <QAbstractListModel>

class RoomListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    RoomListModel(QObject *parent = nullptr);
    ~RoomListModel() override;

    enum RoomRole {
        RoomNameRole = Qt::DisplayRole,
        ModelNameRole = Qt::UserRole,
        PowerRole,
        CurrentTempRole,
        TargetTempRole,
        LastCommandRole,
        RunningRole
    };
    Q_ENUM(RoomRole)

    int rowCount(const QModelIndex & = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE void append(const QString &roomName, const QString &conditionerModelName,
                            quint32 powerConsumption, qint8 currentTemp, qint8 targetTemp,
                            qint8 lastIRCommand, bool isRunning);

    Q_INVOKABLE void set(int row, const QString &roomName, const QString &conditionerModelName,
                         quint32 powerConsumption, qint8 currentTemp, qint8 targetTemp,
                         qint8 lastIRCommand, bool isRunning);

    Q_INVOKABLE void remove(int row);
    void removeAll();

    QString getRoomName(int row) const;
    qint8 getTargetTemp(int row) const;
    qint8 getCurrentTemp(int row) const;
    int     getPower(int row) const;

    int  getRowCount() const;
    int  getLastIRCommand(int row) const;
    void setLastIRCommandByName(const QString &roomName, int command);
    void updateTemp(const QString &roomName, int currentTemp, int targetTemp);

private:
    struct Room {
        QString roomName;
        QString conditionerModelName;
        quint32 powerIntValue;
        qint8 currentTempIntValue;
        qint8 targetTempIntValue;
        qint8 lastIRCommand;
        bool isRunning;
    };

    QList<Room> m_rooms;
};

#endif // ROOMLISTMODEL_H
