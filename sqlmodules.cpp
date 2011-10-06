#include "sqlmodules.h"

QSqlg_dataBase db;
QSqlg_dataBase ConnectDB(QString g_hostname, QString g_dataBaseName, QString UserName, QString g_password, QString DriverBD, int g_connect_port){
   db = QSqlg_dataBase::addg_dataBase(DriverBD);
   db.close();
   db.setg_hostname(g_hostname);
   db.setg_dataBaseName(g_dataBaseName);
   db.setUserName(UserName);
   db.setg_password(g_password);
   db.setg_connect_port(g_connect_port);
   if (! db.open()){
        QMessageBox::critical(0,"Ошибка подключения",db.lastError().text(),QMessageBox::Ok);
        db.close();
   }
   return db;
}
int GetID(QString Gen_Mame){
    QSqlQuery query;
    query.driver();
    query.exec("select gen_id("+Gen_Mame+", 1) as ID FROM rdb$g_dataBase");
    query.next();
    return query.value(0).toInt();
}

// Создание таблиц в базе Данных
void CreateDb(QSqlg_dataBase db)
{
QSqlQuery query;
if (db.g_driverName()== "QSQLITE"){
    db.exec("CREATE TABLE [CLIENTS] ("
            "[ID] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "[FIO] VARCHAR(50)  NOT NULL,"
            "[NOM_TEL] VARCHAR(15)  NULL,"
            "[DATE_R] DATE  NOT NULL,"
            "[INFO] VARCHAR(500)  NULL,"
            "[POL] INTEGER DEFAULT '1' NOT NULL)");
}
if (db.g_driverName()== "QPSQL" || db.g_driverName()== "QPSQL7"){
    // +++++++ Создание таблицы Clients +++++++
//    QFile file(QDir::currentPath()+"/scripts/t_create_PQSQL.sql");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;

//    QTextStream in(&file);

//    db.exec(&in.string());
}

}

QString GetNameDriver(){
    return db.g_driverName();
}

