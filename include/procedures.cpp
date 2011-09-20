#include "procedures.h"

bool EditChetClienta(int ID_cl, int VidOperacii, double summa, QString Date){
if (VidOperacii == 2)
    summa = summa * -1;

QSqlQuery sql;

if (summa != 0){
    sql.prepare("INSERT INTO SCHETA_CLIENTS(DATE,SUMMA,type_operacii,ID_Client) "
                "VALUES(:DATE,:SUMMA,:type_operacii,:ID_Client)");
    sql.bindValue(":DATE",Date);
    sql.bindValue(":SUMMA",summa);
    sql.bindValue(":type_operacii",VidOperacii);
    sql.bindValue(":ID_Client",ID_cl);
    sql.exec();
}
return 1;
}


double GetOstatokNaSchete(int ID_cl, QString Date){
    double summ;
    QSqlQuery sql;
    sql.prepare("SELECT SUM(scheta_clients.summa) AS Summa "
                "FROM scheta_clients "
                "WHERE scheta_clients.ID_Client = :ID_Client");

    sql.bindValue(":ID_Client",ID_cl);
    sql.exec();
    while (sql.next()){
        summ = sql.value(0).toDouble();
    }
    return summ;
}

