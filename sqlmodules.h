#ifndef SQLMODULES_H
#define SQLMODULES_H
#include <QtSql>
#include <QFile>
#include <qmessagebox.h>


QSqlg_dataBase ConnectDB(QString g_hostname, QString g_dataBaseName, QString UserName, QString g_password, QString DriverBD,int g_connect_port);
int GetID(QString Gen_Mame);
void CreateDb(QSqlg_dataBase db);
QString GetNameDriver();
#endif // SQLMODULES_H
