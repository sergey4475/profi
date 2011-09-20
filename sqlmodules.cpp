#include "sqlmodules.h"
QSqlDatabase db;
QSqlDatabase ConnectDB(QString HostName, QString DatabaseName, QString UserName, QString Password, QString DriverBD){
   db = QSqlDatabase::addDatabase(DriverBD);
   db.close();
   db.setHostName(HostName);
   db.setDatabaseName(DatabaseName);
   db.setUserName(UserName);
   db.setPassword(Password);
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
if (db.driverName()== "QIBASE"){
    // +++++++ Создание таблицы Clients +++++++
    db.exec("CREATE TABLE CLIENTS ("
            "ID       INTEGER NOT NULL,"
            "FIO      VARCHAR(50) NOT NULL,"
            "NOM_TEL  VARCHAR(15),"
            "DATE_R   DATE NOT NULL,"
            "INFO     VARCHAR(500),"
            "POL      SMALLINT DEFAULT 1 NOT NULL)");
    db.exec("ALTER TABLE CLIENTS ADD CONSTRAINT PK_CLIENTS PRIMARY KEY (ID)");
    db.exec("CREATE SEQUENCE GEN_CLIENTS_ID");
    db.exec("ALTER SEQUENCE GEN_CLIENTS_ID RESTART WITH 4");
}

}

QString GetNameDriver(){
    return db.driverName();
}

