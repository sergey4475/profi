#include "sqlmodules.h"

QSqlDatabase db;
QSqlDatabase ConnectDB(QString HostName, QString DatabaseName, QString UserName, QString Password, QString DriverBD, int Port){
   db = QSqlDatabase::addDatabase(DriverBD);
   db.close();
   db.setHostName(HostName);
   db.setDatabaseName(DatabaseName);
   db.setUserName(UserName);
   db.setPassword(Password);
   db.setPort(Port);
   if (! db.open()){
        QMessageBox::critical(0,"Ошибка подключения",db.lastError().text(),QMessageBox::Ok);
        db.close();
   }
   return db;
}
int GetID(QString Gen_Mame){
    QSqlQuery query;
    query.driver();
    query.exec("select gen_id("+Gen_Mame+", 1) as ID FROM rdb$database");
    query.next();
    return query.value(0).toInt();
}

// Создание таблиц в базе Данных
void CreateDb(QSqlDatabase db)
{
QSqlQuery query;
if (db.driverName()== "QSQLITE"){
    db.exec("CREATE TABLE [CLIENTS] ("
            "[ID] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "[FIO] VARCHAR(50)  NOT NULL,"
            "[NOM_TEL] VARCHAR(15)  NULL,"
            "[DATE_R] DATE  NOT NULL,"
            "[INFO] VARCHAR(500)  NULL,"
            "[POL] INTEGER DEFAULT '1' NOT NULL)");
}
if (db.driverName()== "QPSQL" || db.driverName()== "QPSQL7"){
    // +++++++ Создание таблицы Clients +++++++
//    QFile file(QDir::currentPath()+"/scripts/t_create_PQSQL.sql");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;

//    QTextStream in(&file);

//    db.exec(&in.string());
}

}

QString GetNameDriver(){
    return db.driverName();
}

