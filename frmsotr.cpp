#include "frmsotr.h"
#include "sqlmodules.h"
#include "ui_frmsotr.h"
#include "MyModel.h"

frmSotr::frmSotr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmSotr)
{
    ui->setupUi(this);
}

frmSotr::~frmSotr()
{
    delete ui;
}

void frmSotr::InitForm(){
    this->setWindowFlags(Qt::Tool);
    ui->comboVidUslugi->clear();
    ui->comboVidUslugi->addItem("Все...");
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("VID_USLUG");
    model->select();
    for (int i=0;i < model->rowCount();++i){
        QSqlRecord rec = model->record(i);
        ui->comboVidUslugi->addItem(rec.value("NAME").toString(),rec.value("ID").toInt());
    }
    QSqlQuery sql;
    sotSqlQueryModel *sqlModel = new sotSqlQueryModel;
    sql.prepare("SELECT Personal.FIO, "
                    "DOLJNOSTI.NAME, "
                    "Personal.Nom_Tel, "
                    "Personal.DEL, "
                    "Personal.POL, "
                    "Personal.ID, "
                    "DOLJNOSTI.ID AS Doljn "
                "FROM Personal, DOLJNOSTI "
                "WHERE Personal.DOLJN = DOLJNOSTI.ID");

    sql.exec();
    sqlModel->setQuery(sql);
    sqlModel->setHeaderData(0,Qt::Horizontal,QObject::tr("Сотрудник"));
    sqlModel->setHeaderData(1,Qt::Horizontal,QObject::tr("Должность"));
    sqlModel->setHeaderData(2,Qt::Horizontal,QObject::tr("тел."));
    sqlModel->setHeaderData(3,Qt::Horizontal,QObject::tr("DEL"));
    sqlModel->setHeaderData(4,Qt::Horizontal,QObject::tr("POL"));
    sqlModel->setHeaderData(5,Qt::Horizontal,QObject::tr("ID"));
    sqlModel->setHeaderData(6,Qt::Horizontal,QObject::tr("Doljn"));
    ui->tableSotr->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableSotr->setModel(sqlModel);
    ui->tableSotr->setColumnHidden(3,true);
    ui->tableSotr->setColumnHidden(4,true);
    ui->tableSotr->setColumnHidden(5,true);
    ui->tableSotr->setColumnHidden(6,true);

    QSqlTableModel *modelTable = new QSqlTableModel;
    modelTable->setTable("DOLJNOSTI");
    modelTable->select();

    for (int i=0;i < modelTable->rowCount(); ++i){
        ui->sotrDoljn->addItem(modelTable->record(i).value("NAME").toString(),modelTable->record(i).value("ID").toInt());
    }

    ui->sotrFIO->setReadOnly(false);
    ui->sotrDoljn->setDisabled(false);
    ui->sotr_nom->setReadOnly(false);
    ui->sotrPol->setDisabled(false);
    ui->sotrAddres->setReadOnly(false);
    ui->sotrInfo->setReadOnly(false);

    ui->sotrFIO->clear();
    ui->sotrDoljn->setCurrentIndex(0);
    ui->sotr_nom->setText("");
    ui->sotrPol->setCurrentIndex(0);
    ui->sotrAddres->setText("");
    ui->sotrInfo->setText("");
    ui->sotrFIO->setFocus();
}

void frmSotr::on_comboVidUslugi_activated(int index)
{
    QSqlQuery sql;
    sotSqlQueryModel *sqlModel = new sotSqlQueryModel;
    if (ui->comboVidUslugi->currentIndex() > 0){
        sql.prepare("SELECT Personal.FIO, "
                        "DOLJNOSTI.NAME, "
                        "Personal.Nom_Tel, "
                        "Personal.DEL, "
                        "Personal.POL, "
                        "Personal.ID, "
                        "DOLJNOSTI.ID AS Doljn "
                    "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID "
                    "WHERE DOLJNOSTI.VID_USLUGI = :Vid" );
        sql.bindValue(":Vid",ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toInt());
    }else{
        sql.prepare("SELECT Personal.FIO, "
                        "DOLJNOSTI.NAME, "
                        "Personal.Nom_Tel, "
                        "Personal.DEL, "
                        "Personal.POL, "
                        "Personal.ID, "
                        "DOLJNOSTI.ID AS Doljn "
                    "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID");
    }

    sql.exec();
    sqlModel->setQuery(sql);
    sqlModel->setHeaderData(0,Qt::Horizontal,QObject::tr("Сотрудник"));
    sqlModel->setHeaderData(1,Qt::Horizontal,QObject::tr("Должность"));
    sqlModel->setHeaderData(2,Qt::Horizontal,QObject::tr("тел."));
    sqlModel->setHeaderData(3,Qt::Horizontal,QObject::tr("DEL"));
    sqlModel->setHeaderData(4,Qt::Horizontal,QObject::tr("POL"));
    sqlModel->setHeaderData(5,Qt::Horizontal,QObject::tr("ID"));
    sqlModel->setHeaderData(6,Qt::Horizontal,QObject::tr("Doljn"));
    ui->tableSotr->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableSotr->setModel(sqlModel);
    ui->tableSotr->setColumnHidden(3,true);
    ui->tableSotr->setColumnHidden(4,true);
    ui->tableSotr->setColumnHidden(5,true);
    ui->tableSotr->setColumnHidden(6,true);


    QSqlTableModel *modelTable = new QSqlTableModel;
    modelTable->setTable("DOLJNOSTI");
    if (ui->comboVidUslugi->currentIndex() > 0)
        modelTable->setFilter("VID_USLUGI="+ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toString());
    modelTable->select();
    ui->sotrDoljn->clear();
    for (int i=0;i < modelTable->rowCount(); ++i){
        ui->sotrDoljn->addItem(modelTable->record(i).value("NAME").toString(),modelTable->record(i).value("ID").toInt());
    }

    ui->sotrFIO->setReadOnly(false);
    ui->sotrDoljn->setDisabled(false);
    ui->sotr_nom->setReadOnly(false);
    ui->sotrPol->setDisabled(false);
    ui->sotrAddres->setReadOnly(false);
    ui->sotrInfo->setReadOnly(false);

    ui->sotrFIO->clear();
    ui->sotrDoljn->setCurrentIndex(0);
    ui->sotr_nom->setText("");
    ui->sotrPol->setCurrentIndex(0);
    ui->sotrAddres->setText("");
    ui->sotrInfo->setText("");
    ui->sotrFIO->setFocus();


}

void frmSotr::on_tableSotr_clicked(const QModelIndex &index)
{
    ui->sotrFIO->setReadOnly(true);
    ui->sotrDoljn->setDisabled(true);
    ui->sotr_nom->setReadOnly(true);
    ui->sotrPol->setDisabled(true);
    ui->sotrAddres->setReadOnly(true);
    ui->sotrInfo->setReadOnly(true);

    QModelIndex ID = ui->tableSotr->model()->index(index.row(),5);
    IDSotr = ID.data().toInt();
    QSqlQuery query;
    query.prepare("SELECT "
                    "Personal.FIO, "      //0
                    "DOLJNOSTI.ID, "      //1
                    "Personal.Nom_Tel, "  //2
                    "Personal.POL, "      //3
                    "Personal.Addres, "   //4
                    "Personal.Info, "     //5
                    "Personal.ID "        //6
                  "FROM Personal, DOLJNOSTI "
                  "WHERE "
                    "Personal.DOLJN = DOLJNOSTI.ID "
                    "AND Personal.ID = :ID "
              );
    query.bindValue(":ID",IDSotr);
    query.exec();
    while (query.next()){
        ui->sotrFIO->setText(query.value(0).toString());
        ui->sotrDoljn->setCurrentIndex(ui->sotrDoljn->findData(query.value(1).toInt()));
        ui->sotr_nom->setText(query.value(2).toString());
        ui->sotrPol->setCurrentIndex(query.value(3).toInt());
        ui->sotrAddres->setText(query.value(4).toString());
        ui->sotrInfo->setText(query.value(5).toString());
    }

}

void frmSotr::on_add_sotr_clicked()
{
    ui->sotrFIO->setReadOnly(false);
    ui->sotrDoljn->setDisabled(false);
    ui->sotr_nom->setReadOnly(false);
    ui->sotrPol->setDisabled(false);
    ui->sotrAddres->setReadOnly(false);
    ui->sotrInfo->setReadOnly(false);

    ui->sotrFIO->clear();
    ui->sotrDoljn->setCurrentIndex(0);
    ui->sotr_nom->setText("");
    ui->sotrPol->setCurrentIndex(0);
    ui->sotrAddres->setText("");
    ui->sotrInfo->setText("");
    ui->sotrFIO->setFocus();
    flag_record = add_rec;
}

void frmSotr::on_edit_sotr_clicked()
{
    ui->sotrFIO->setReadOnly(false);
    ui->sotrDoljn->setDisabled(false);
    ui->sotr_nom->setReadOnly(false);
    ui->sotrPol->setDisabled(false);
    ui->sotrAddres->setReadOnly(false);
    ui->sotrInfo->setReadOnly(false);
    ui->sotrFIO->setFocus();

    flag_record = edit_rec;

}

void frmSotr::on_ApplyBut_clicked()
{
    QString DriverName;
    QSqlQuery query;
    DriverName = GetNameDriver();
    if (flag_record == add_rec){
        if (DriverName != "QSQLITE")
        {
            IDSotr = GetID("GEN_CLIENTS_ID");
            query.prepare("INSERT INTO PERSONAL(ID,FIO,Nom_tel,DOLJN,info, pol, ADDRES) VALUES(:ID,:FIO,:Nom_tel,:DOLJN,:info,:Pol,:ADDRES)");
        }
        else if (DriverName == "QSQLITE")
        {
            query.prepare("INSERT INTO PERSONAL(FIO,Nom_tel,DOLJN,info, pol, ADDRES) VALUES(:FIO,:Nom_tel,:DOLJN,:info,:Pol,:ADDRES)");
        }
    }else if (flag_record==edit_rec){
        query.prepare("UPDATE PERSONAL SET FIO=:FIO,DOLJN=:DOLJN,ADDRES=:ADDRES, NOM_TEL=:Nom_tel,INFO=:info, POL=:Pol WHERE Id=:ID");
    }

    if (flag_record != 0)
    {
        query.bindValue(":FIO",ui->sotrFIO->text());
        query.bindValue(":DOLJN",ui->sotrDoljn->itemData(ui->sotrDoljn->currentIndex()).toInt());
        query.bindValue(":ADDRES",ui->sotrAddres->text());
        query.bindValue(":Nom_tel",ui->sotr_nom->text());
        query.bindValue(":info",ui->sotrInfo->toPlainText());
        query.bindValue(":Pol",ui->sotrPol->currentIndex());

        if ((flag_record == edit_rec) || (flag_record == add_rec && DriverName != "QSQLITE"))
                query.bindValue(":ID",IDSotr);

        query.exec();

        frmSotr::on_comboVidUslugi_activated(ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toInt());
    }
flag_record = 0;
}

void frmSotr::on_del_sotr_clicked()
{
    if (QMessageBox::question(0,"Вопрос","Вы уверены что желаете удалить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("UPDATE PERSONAL SET Del=1 WHERE PERSONAL.id = :ID");
        query.bindValue("ID",IDSotr);
        query.exec();
        frmSotr::on_comboVidUslugi_activated(ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toInt());
    };

}
