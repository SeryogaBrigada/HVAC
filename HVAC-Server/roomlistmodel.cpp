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

#include "roomlistmodel.h"

RoomListModel::RoomListModel(QObject *parent) : QAbstractListModel(parent)
{
}

RoomListModel::~RoomListModel()
{
}

int RoomListModel::rowCount(const QModelIndex &) const
{
    return m_rooms.count();
}

QVariant RoomListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case RoomRole::RoomNameRole: return m_rooms.at(index.row()).roomName;
        case RoomRole::ModelNameRole: return m_rooms.at(index.row()).conditionerModelName;
        case RoomRole::PowerRole: return m_rooms.at(index.row()).powerIntValue;
        case RoomRole::CurrentTempRole: return m_rooms.at(index.row()).currentTempIntValue;
        case RoomRole::TargetTempRole: return m_rooms.at(index.row()).targetTempIntValue;
        case RoomRole::LastCommandRole: return m_rooms.at(index.row()).lastIRCommand;
        case RoomRole::RunningRole: return m_rooms.at(index.row()).isRunning;
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> RoomListModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { RoomRole::RoomNameRole, "roomName" },
        { RoomRole::ModelNameRole, "modelName" },
        { RoomRole::PowerRole, "power" },
        { RoomRole::CurrentTempRole, "currentTemp" },
        { RoomRole::TargetTempRole, "targetTemp" },
        { RoomRole::LastCommandRole, "lastIRCommand" },
        { RoomRole::RunningRole, "isRunning" }
    };
    return roles;
}

QVariantMap RoomListModel::get(int row) const
{
    const Room m_room = m_rooms.value(row);
    return { {"roomName", m_room.roomName},
        {"modelName", m_room.conditionerModelName},
        {"power", m_room.powerIntValue},
        {"currentTemp", m_room.currentTempIntValue},
        {"targetTemp", m_room.targetTempIntValue},
        {"lastIRCommand", m_room.lastIRCommand},
        {"isRunning", m_room.isRunning} };
}

void RoomListModel::append(const QString &roomName, const QString &conditionerModelName,
                           quint32 powerConsumption, qint8 currentTemp, qint8 targetTemp,
                           qint8 lastIRCommand, bool isRunning)
{
    // Replace existing item
    for (int i = 0; i < m_rooms.count(); i++) {
        if (m_rooms.at(i).roomName.contains(roomName)) {
            set(i, roomName, conditionerModelName, powerConsumption,
                currentTemp, targetTemp, lastIRCommand, isRunning);
            return;
        }
    }

    int row = 0;
    while (row < m_rooms.count() && roomName > m_rooms.at(row).roomName)
        ++row;

    beginInsertRows(QModelIndex(), row, row);
    m_rooms.insert(row, { roomName, conditionerModelName, powerConsumption,
                          currentTemp, targetTemp, lastIRCommand, isRunning });
    endInsertRows();
}

void RoomListModel::set(int row, const QString &roomName, const QString &conditionerModelName,
                        quint32 powerConsumption, qint8 currentTemp, qint8 targetTemp,
                        qint8 lastIRCommand, bool isRunning)
{
    if (row < 0 || row >= m_rooms.count())
        return;

    m_rooms.replace(row, { roomName, conditionerModelName, powerConsumption,
                           currentTemp, targetTemp, lastIRCommand, isRunning });

    dataChanged(index(row, 0), index(row, 0), { RoomRole::RoomNameRole,
                                                RoomRole::ModelNameRole,
                                                RoomRole::PowerRole,
                                                RoomRole::CurrentTempRole,
                                                RoomRole::TargetTempRole,
                                                RoomRole::LastCommandRole,
                                                RoomRole::RunningRole });
}

void RoomListModel::remove(int row)
{
    if (row < 0 || row >= m_rooms.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_rooms.removeAt(row);
    endRemoveRows();
}

void RoomListModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, (m_rooms.count() - 1));
    m_rooms.clear();
    endRemoveRows();
}

void RoomListModel::updateTemp(const QString &roomName, int currentTemp, int targetTemp)
{
    for (int row = 0; row < m_rooms.count(); row++) {
        if (m_rooms.at(row).roomName.contains(roomName)) {

            Room tmp_room = m_rooms.value(row);
            tmp_room.currentTempIntValue = static_cast<qint8>(currentTemp);
            tmp_room.targetTempIntValue  = static_cast<qint8>(targetTemp);

            m_rooms.replace(row, tmp_room);
            dataChanged(index(row, 0), index(row, 0), { RoomRole::CurrentTempRole,
                                                        RoomRole::TargetTempRole });
        }
    }
}

int RoomListModel::getLastIRCommand(int row) const
{
    if (row < 0 || row >= m_rooms.count())
        return -1;

    return m_rooms.at(row).lastIRCommand;
}

void RoomListModel::setLastIRCommandByName(const QString &roomName, int command)
{
    for (int row = 0; row < m_rooms.count(); row++) {
        if (m_rooms.at(row).roomName.contains(roomName)) {

            Room tmp_room = m_rooms.value(row);
            tmp_room.lastIRCommand = static_cast<qint8>(command);
            tmp_room.isRunning = command > 0;

            m_rooms.replace(row, tmp_room);
            dataChanged(index(row, 0), index(row, 0), { RoomRole::LastCommandRole,
                                                        RoomRole::RunningRole });
        }
    }
}

int RoomListModel::getRowCount() const
{
    return m_rooms.count();
}

QString RoomListModel::getRoomName(int row) const
{
    if (row < 0 || row >= m_rooms.count())
        return nullptr;

    return m_rooms.at(row).roomName;
}

qint8 RoomListModel::getTargetTemp(int row) const
{
    if (row < 0 || row >= m_rooms.count())
        return -1;

    return m_rooms.at(row).targetTempIntValue;
}

qint8 RoomListModel::getCurrentTemp(int row) const
{
    if (row < 0 || row >= m_rooms.count())
        return -1;

    return m_rooms.at(row).currentTempIntValue;
}

int RoomListModel::getPower(int row) const
{
    if (row < 0 || row >= m_rooms.count())
        return -1;

    return static_cast<int>(m_rooms.at(row).powerIntValue);
}
