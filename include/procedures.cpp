#include "procedures.h"

bool EditChetClienta(int ID_cl, int VidOperacii, double summa, QString Date){
if (VidOperacii == N_CH_RASHOD || VidOperacii == N_CH_SPISAN)
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

double GetOstatokNaSklade(int ID_MATERIAL, int vid_zatrat, QString Date, int type_sklad){
    double ostatok;
    QSqlQuery sql;
    if (type_sklad == N_O_SKLAD){
        sql.prepare("SELECT SUM(O_SKLAD.count) AS count "
                    "FROM O_SKLAD "
                    "WHERE O_SKLAD.id_material = :id_material "
                    "AND O_SKLAD.id_group_o_sklad = :id_vid_zatrat "
                    "AND O_SKLAD.Date <= :Date");
    }

    if (type_sklad == N_SKLAD){
        sql.prepare("SELECT SUM(SKLAD.count) AS count "
                    "FROM SKLAD "
                    "WHERE SKLAD.id_material = :id_material "
                    "AND SKLAD.id_vid_zatrat = :id_vid_zatrat "
                    "AND SKLAD.Date <= :Date");
    }


    sql.bindValue(":id_material",ID_MATERIAL);
    sql.bindValue(":id_vid_zatrat",vid_zatrat);
    sql.bindValue(":Date",Date);
    sql.exec();
    sql.next();
    ostatok = sql.value(0).toDouble();

    return ostatok;
}

