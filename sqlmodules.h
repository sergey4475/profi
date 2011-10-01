#ifndef SQLMODULES_H
#define SQLMODULES_H
#include <QtSql>
#include <QFile>
#include <qmessagebox.h>


QSqlDatabase ConnectDB(QString HostName, QString DatabaseName, QString UserName, QString Password, QString DriverBD);
int GetID(QString Gen_Mame);
void CreateDb(QSqlDatabase db);
QString GetNameDriver();
#endif // SQLMODULES_H
