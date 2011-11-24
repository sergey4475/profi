#ifndef PARAMS_H
#define PARAMS_H

// Тип услуг
#define n_USL_MAN  1
#define n_USL_KOS  2
#define n_USL_UHOD 3
#define n_USL_STIL 4
#define n_USL_MAG  5

// тип отображаемых данных в таблице выбора
#define n_USLUGI        1
#define n_MATERIAL      2
#define n_OSTATKI_SKALD 3
#define n_SKALD         4
#define n_CLIENTS       5
#define n_MASTER        6
#define n_MAGAZIN       7

// Операции склада
#define n_PRIHOD 1
#define n_RASHOD 2
#define n_SPISAN 3

// Счета
#define N_CH_PRIHOD 1
#define N_CH_RASHOD 2
#define N_CH_SPISAN 3
#define N_CH_SERTIF 4

// Платежи
#define n_PL_OSN    1
#define n_PL_DOLG   2

// Операции по кассе
#define n_KASSA_PRIHOD 1
#define n_KASSA_RASHOD 2

// Тип склада
#define N_O_SKLAD   1
#define N_SKLAD     2

// Типв отчетов
#define O_OST_O_SKLAD    1
#define O_OST_SKLAD      2
#define O_PRIHOD_O_SKLAD 3
#define O_RASHOD_O_SKLAD 4

#include <QtCore>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>

    static QSettings set("Profi","");
    static QString g_hostname       = set.value("Sql/HostName").toString();
    static QString g_dataBase       = set.value("Sql/DataBase").toString();
    static QString g_login          = set.value("Sql/Login").toString();
    static QString g_password       = set.value("Sql/Password").toString();
    static QString g_driverName     = set.value("Sql/DriverName").toString();
    static int g_connect_port       = set.value("Sql/Port",5432).toInt();
    static bool g_spisanie_so_scheta= set.value("Main/spisanie_so_scheta",false).toBool();

#endif // PARAMS_H
