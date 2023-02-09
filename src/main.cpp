/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Studio Components.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QObject>
#include <QQmlContext>

#include "app_environment.h"
#include "import_qml_plugins.h"
#include "steam_api.h"


#include "localfiles.h"
#include "globaldata.h"
#include "exit.h"
#include "cmodslistfile.h"
#include "cmodslistfilling.h"
#include "modpackslist.h"
#include "qtgeneralbackend.h"

class steam
{
public:
    steam()
    {
        if(SteamAPI_Init())
        {
            CSteamTools steamOperations;
            SharedSteamToolsObj->LoadItemsToQuery();
            SharedSteamToolsObj->LoadItemsDataFromQuery();
            std::cout << "SteamAPI Loaded" << std::endl;
        }
    };
    ~steam()
    {
        SteamAPI_Shutdown();
        std::cout << "SteamApi Destroyed" << std::endl;
    }
};


void settingsLoading()
{
    localFiles temp;
    if(temp.findLocalFolder())
    {
        std::string settingsPath = temp.localWh3Path + "\\settings.txt";
        if(temp.findLocalDataTxt(settingsPath))
        {
            temp.loadLocalSettings();
        }
    }
}

int main(int argc, char *argv[])
{
    std::cout << "1" << std::endl;
    set_qt_environment();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    cexit exit_obj;
    engine.rootContext()->setContextProperty("exit_obj", &exit_obj);


    localFiles localFilesObj;
    engine.rootContext()->setContextProperty("localFilesObj", &localFilesObj);

    settingsLoading();
    {
        steam runSteam;
    }

    QtGeneralBackend qtGeneralBackendObj;
    engine.rootContext()->setContextProperty("qtGeneralBackendObj", &qtGeneralBackendObj);

    qmlRegisterType<CModsListFile>("CModsList", 1, 0, "CModsListFile");
    qmlRegisterUncreatableType<cmodslistfilling>("CModsList", 1, 0, "Cmodslistfilling", QString("Sometthing"));

    cmodslistfilling Objcmodslistfilling;
    engine.rootContext()->setContextProperty("Objcmodslistfilling", &Objcmodslistfilling);

    qmlRegisterType<ModpacksList>("ModPacksList", 1, 0, "ModpacksList");
    qmlRegisterUncreatableType<ModpacksContent>("ModPacksList", 1, 0, "ModpacksContent", QString("Sometthing"));

    ModpacksContent ObjModpacksContent;
    engine.rootContext()->setContextProperty("ObjModpacksContent", &ObjModpacksContent);

    const QUrl url(u"qrc:Main/main.qml"_qs);
    QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);


    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
