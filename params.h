#ifndef PARAMS_H
#define PARAMS_H

// “ип услуг
#define n_USL_MAN 1
#define n_USL_KOS 2
#define n_USL_UHOD 3
#define n_USL_STIL 4

// тип отображаемых данных в таблице выбора
#define n_USLUGI        1
#define n_MATERIAL      2
#define n_OSTATKI_SKALD 3
#define n_SKALD         4
#define n_CLIENTS       5
#define n_MASTER        6

// ќперации склада
#define n_PRIHOD 1
#define n_RASHOD 2
#define n_SPISAN 3

// —чета
#define N_CH_PRIHOD 1
#define N_CH_RASHOD 2
#define N_CH_SPISAN 3

#include <QtCore>
#include <QSettings>

    static QSettings set("conf.ini",QSettings::IniFormat);
    static QString g_hostname       = set.value("HostName").toString();
    static QString g_dataBase       = set.value("DataBase").toString();
    static QString g_login          = set.value("Login").toString();
    static QString g_password       = set.value("Password").toString();
    static QString g_driverName     = set.value("DriverName").toString();
    static int g_connect_port       = set.value("Port",5432).toInt();
    static bool g_spisanie_so_scheta= set.value("spisanie_so_scheta",false).toBool();

#endif // PARAMS_H
