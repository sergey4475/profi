#ifndef PARAMS_H
#define PARAMS_H

#define n_USL_MAN 1
#define n_USL_KOS 2
#define n_USL_UHOD 3
#define n_USL_STIL 4

#define n_USLUGI 1
#define n_MATERIAL 2
#define n_OSTATKI_SKALD 3
#define n_SKALD 4
#define n_PRIHOD 1
#define n_RASHOD 2


#include <QtCore>
#include <QSettings>

    static QSettings set("conf.ini",QSettings::IniFormat);
    static QString g_hostname       = set.value("HostName").toString();
    static QString g_dataBase       = set.value("DataBase").toString();
    static QString g_login          = set.value("Login").toString();
    static QString g_password       = set.value("Password").toString();
    static QString g_driverName     = set.value("DriverName").toString();
    static int g_connect_port       = set.value("Port",5432).toInt();
    static bool g_spisanie_so_scheta= set.value("spisanie_so_scheta",true).toBool();

#endif // PARAMS_H
