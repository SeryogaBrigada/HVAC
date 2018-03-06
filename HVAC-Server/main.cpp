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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "systemtray.h"
#include "qdataprovider.h"
#include "roomlistmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QLatin1String("Nekrasov and Company"));
    QCoreApplication::setApplicationName(QLatin1String("HVAC Controller Server"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(QLatin1String(":/Images/tray-icon.png")));

    SystemTray m_sysTray;

    RoomListModel m_modelPhaseA;
    RoomListModel m_modelPhaseB;
    RoomListModel m_modelPhaseC;

    QDataProvider m_provider(nullptr, &m_modelPhaseA, &m_modelPhaseB, &m_modelPhaseC);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QLatin1String("systemTray"),   &m_sysTray);
    engine.rootContext()->setContextProperty(QLatin1String("modelPhaseA"),  &m_modelPhaseA);
    engine.rootContext()->setContextProperty(QLatin1String("modelPhaseB"),  &m_modelPhaseB);
    engine.rootContext()->setContextProperty(QLatin1String("modelPhaseC"),  &m_modelPhaseC);
    engine.rootContext()->setContextProperty(QLatin1String("DataProvider"), &m_provider);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
