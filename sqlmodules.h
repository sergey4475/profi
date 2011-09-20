#ifndef SQLMODULES_H
#define SQLMODULES_H
#include <QtSql>
#include <qmessagebox.h>

QSqlDatabase ConnectDB(QString HostName, QString DatabaseName, QString UserName, QString Password, QString DriverBD);
int GetID(QString Gen_Mame);
QString GetNameDriver();
#endif // SQLMODULES_H
