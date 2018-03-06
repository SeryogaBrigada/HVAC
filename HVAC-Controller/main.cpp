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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "dataprovider.h"
#include "systemtray.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QLatin1String("Nekrasov and Company"));
    QCoreApplication::setApplicationName(QLatin1String("HVAC Controller"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String(":/Images/tray-icon.png")));

    qmlRegisterType<DataProvider>("io.qt.dataprovider", 1, 0, "DataProvider");

    SystemTray sysTray;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("systemTray", &sysTray);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
