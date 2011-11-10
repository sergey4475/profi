#include "frmclients.h"
#include "params.h"
#include "ui_frmclients.h"
#include <QMessageBox>
#include "MyModel.h"

frmClients::frmClients(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::frmClients)
{
    ui->setupUi(this);
    flag_record = 0;
}

frmClients::~frmClients()
{
    delete ui;
}

void frmClients::initForm(WId w_ID,int fl){

    if (fl==1){
        ui->closeFrame->setHidden(true);
        selClients = true;
    }
    else{
        ui->closeFrame->setVisible(true);
        selClients = false;
    }

    if (w_ID != 0x0){
        QWidget *frm = find(w_ID);
        frm->deleteLater();
    }
    clSqlQueryModel *model = new clSqlQueryModel();
    model->setQuery("select FIO, nom_tel, DATE_R, ID, DEL, POL FROM clients");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ФИО"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Тел."));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Дата рожд."));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("DEL"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("POL"));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(5,true);
}

void frmClients::on_tableView_clicked(const QModelIndex &index)
{
    ui->FIOEdit->setReadOnly(true);
    ui->Nom_Edit->setReadOnly(true);
    ui->dateEdit->setDisabled(true);
    ui->InfoEdit->setReadOnly(true);
    ui->Pol->setDisabled(true);

    QModelIndex ID = ui->tableView->model()->index(index.row(),3);
    IDClients = ID.data().toInt();

    frm->UpdateClients(IDClients);
    QSqlQuery query;
    query.prepare("SELECT FIO, nom_tel, Date_R, info, pol  FROM Clients WHERE Clients.ID = :ID");
    query.bindValue(":ID",IDClients);
    query.exec();
    while (query.next()){
        ui->FIOEdit->setText(query.value(0).toString());
        ui->Nom_Edit->setText(query.value(1).toString());
        ui->dateEdit->setDateCalendar(QDate::fromString(query.value(2).toString(),"dd.MM.yyyy"));
        ui->InfoEdit->setText(query.value(3).toString());
        ui->Pol->setCurrentIndex(query.value(4).toInt());
   }
    query.prepare("SELECT "
                  " Clients_history.DATE_USLUGI, "
                  " USLUGI.NAME, "
                  " SUM(Clients_history.SUMMA) AS SUMMA, "
                  " Clients_history.NUMBER "
                  "FROM CLIENTS_HISTORY INNER JOIN USLUGI ON CLIENTS_HISTORY.ID_USLUGA = USLUGI.ID "
                  "WHERE Clients_history.ID_CLIENT = :ID "
                  "GROUP BY"
                  " Clients_history.DATE_USLUGI, "
                  " USLUGI.NAME, "
                  " Clients_history.NUMBER "
                  "ORDER BY "
                  " Clients_history.NUMBER");
    query.bindValue(":ID",IDClients);
    query.exec();
    qDebug() << query.lastError();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Дата"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Услуга"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Сумма"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Ном. док."));
    ui->tClient_history->setModel(model);
    ui->tClient_history->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

//============== Кнопка редактирования записи ===========================
void frmClients::on_edit_button_clicked()
{
    ui->FIOEdit->setReadOnly(false);
    ui->Nom_Edit->setReadOnly(false);
    ui->dateEdit->setDisabled(false);
    ui->InfoEdit->setReadOnly(false);
    ui->Pol->setDisabled(false);
    ui->FIOEdit->setFocus();
    flag_record = edit_rec;
}

//============== Кнопка добавление нового клиента =======================
void frmClients::on_toolButton_4_clicked()
{
    ui->FIOEdit->clear();
    ui->Nom_Edit->clear();
    ui->dateEdit->setDateCalendar(QDate::fromString("01.01.0001","yyyy.MM.dd"));
    ui->InfoEdit->clear();
    ui->Pol->setCurrentIndex(0);

    ui->FIOEdit->setReadOnly(false);
    ui->Nom_Edit->setReadOnly(false);
    ui->dateEdit->setDisabled(false);
    ui->InfoEdit->setReadOnly(false);
    ui->Pol->setDisabled(false);
    ui->FIOEdit->setFocus();
    flag_record = add_rec;
}
//============= Кнопка применить =======================
void frmClients::on_ApplyBut_clicked()
{
    if (ui->FIOEdit->text() != "" && ui->Nom_Edit->text() != "" && ui->dateEdit->date() != QDate::fromString("01.01.0001","yyyy.MM.dd"))
    {
        QString g_driverName;
        QSqlQuery query;
        g_driverName = GetNameDriver();
        if (flag_record == add_rec){
            if (g_driverName == "QIBASE")
            {
                IDClients = GetID("GEN_CLIENTS_ID");
                query.prepare("INSERT INTO Clients(ID,FIO,Nom_tel,date_R,info, pol) VALUES(:ID,:FIO,:Nom_tel,:Date_R,:info,:pol)");
            }
            else
            {
                query.prepare("INSERT INTO Clients(FIO,Nom_tel,date_R,info, pol) VALUES(:FIO,:Nom_tel,:Date_R,:info,:pol)");
            }
        }else if (flag_record==edit_rec){
            query.prepare("UPDATE Clients SET FIO=:FIO,Nom_tel=:Nom_tel,Date_R=:Date_R,info=:info,pol=:pol WHERE Id=:ID");
//            query.bindValue(":ID",IDClients);
        }

        if (flag_record != 0)
        {
            query.bindValue(":FIO",ui->FIOEdit->text());
            query.bindValue(":Nom_Tel",ui->Nom_Edit->text());
            query.bindValue(":Date_R",ui->dateEdit->date().toString("dd.MM.yyyy"));
            query.bindValue(":info",ui->InfoEdit->toPlainText());
            query.bindValue(":pol",ui->Pol->currentIndex());
            if (flag_record==edit_rec)
                query.bindValue(":ID",IDClients);

            query.exec();
            qDebug() << query.lastError();

            frmClients::on_toolButton_4_clicked();
            frmClients::initForm(0x0,0);
        }
    }
    flag_record = 0;
}

void frmClients::on_tableView_activated(const QModelIndex &index)
{
    frmClients::on_tableView_clicked(index);
}

void frmClients::on_del_button_clicked()
{
    if (QMessageBox::question(0,"Вопрос","Вы уверены что желаете удалить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("UPDATE Clients SET Del=1 WHERE Clients.id = :ID");
        query.bindValue("ID",IDClients);
        query.exec();
        frmClients::on_toolButton_4_clicked();
        frmClients::initForm(0x0,0);
    };
}

void frmClients::on_closeFrame_clicked()
{
    frm->EnableButton(0);
    this->~frmClients();
}
