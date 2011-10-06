#define  n_PRIHOD 1
#define  n_RASHOD 2

#include "frm_okazanie_uslug.h"
#include "ui_frm_okazanie_uslug.h"
#include "frmselect.h"
#include "frmclients.h"
#include "params.h"
#include "delegats.h"
#include "procedures.h"
#include "mainform.h"
#include <QTableView>
#include <QCompleter>
#include <QDirModel>

frm_okazanie_uslug::frm_okazanie_uslug(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::frm_okazanie_uslug)
{
    ui->setupUi(this);
}

frm_okazanie_uslug::~frm_okazanie_uslug()
{
    delete ui;
}

void frm_okazanie_uslug::InitForm(int nUslugi, WId w_ID){

    if (w_ID != 0x0){
        QWidget *f = find(w_ID);
        f->deleteLater();
    }

    NumberUslugi = nUslugi;

    QSqlQuery sql;
    sql.prepare("SELECT MAX(number) AS number "
             "FROM CLIENTS_HISTORY");
    sql.exec();
    QSqlRecord record = sql.record();
    sql.next();

    Number = sql.value(record.indexOf("number")).toInt();

    Number++;

    ui->number->setText(QString::number(Number));

    clSqlQueryModel *model = new clSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT FIO, POL, NOM_TEL, DEL FROM Clients ");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,"ФИО");
    model->setHeaderData(2,Qt::Horizontal,"Номер");

    QCompleter *completer = new QCompleter(this);
    completer->setModel(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Client->setCompleter(completer);

    sotSqlQueryModel *smodel = new sotSqlQueryModel;
    query.prepare("SELECT Personal.FIO, "
                    "DOLJNOSTI.NAME, "
                    "Personal.Nom_Tel, "
                    "Personal.DEL, "
                    "Personal.POL, "
                    "Personal.ID, "
                    "DOLJNOSTI.ID AS Doljn "
                "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID "
                "WHERE DOLJNOSTI.VID_USLUGI = :Vid" );
    query.bindValue(":VID",NumberUslugi);
    query.exec();
    smodel->setQuery(query);
    smodel->setHeaderData(0,Qt::Horizontal,"ФИО");

    QCompleter *scompleter = new QCompleter(this);
    scompleter->setModel(smodel);
    scompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Sotrudnik->setCompleter(scompleter);

    sql.exec("SELECT VID_PLATEJA.ID, VID_PLATEJA.NAME "
             "FROM VID_PLATEJA ");

    ui->sposobOplati->clear();
    while (sql.next()){
        ui->sposobOplati->addItem(sql.value(1).toString(),sql.value(0).toInt());
    }
    ui->but_oplatit->setEnabled(true);

}

void frm_okazanie_uslug::on_toolButton_clicked()
{
    frm->EnableButton(NumberUslugi);
    this->~frm_okazanie_uslug();
}

// Добавление услуги
void frm_okazanie_uslug::on_add_usluga_clicked()
{
    NotEditableDelegate *DelegatNotEditCol = new NotEditableDelegate;
    if ((ui->Client->text() != "" && ui->Sotrudnik->text() != "")||(ID_client != 0 && ID_sotr != 0) ){
            QString str = QString("%1").arg(NumberUslugi);
            frmSelect *fSelect = new frmSelect();
            tempModel = new PStandardItemModel;
            tempModel->frm = frm;
            ui->USLUGI->setModel(tempModel);
            ui->USLUGI->setColumnHidden(0,true);
            ui->USLUGI->setColumnHidden(3,true);
            ui->USLUGI->setItemDelegateForColumn(1,DelegatNotEditCol);
            ui->USLUGI->setItemDelegateForColumn(5,DelegatNotEditCol);
            ui->USLUGI->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
            connect(tempModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),tempModel,SLOT(editFinish(QModelIndex)));
            fSelect->frm = frm;
            fSelect->type_select = n_USLUGI;
            fSelect->type_uslugi_= NumberUslugi;
            fSelect->tempModel   = tempModel;
            fSelect->Id_Client   = ID_client;
            fSelect->init(QDate::currentDate());
            ui->USLUGI->setColumnHidden(0,true);
            ui->USLUGI->setColumnHidden(3,true);
            fSelect->show();
    }else{
            QMessageBox::question(0,"Внимание!!!","Не заполнены обязательные поля!!!",QMessageBox::Yes);
    }
}

// Добавление материала
void frm_okazanie_uslug::on_add_material_clicked()
{
    NotEditableDelegate *DelegatNotEditCol = new NotEditableDelegate;
    if ((ui->Client->text() != "" && ui->Sotrudnik->text() != "")||(ID_client != 0 && ID_sotr != 0) ){
            frmSelect *fSelect = new frmSelect();
            tempModel = new PStandardItemModel;
            tempModel->frm = frm;
            ui->Materials->setModel(tempModel);
            ui->Materials->setItemDelegateForColumn(1,DelegatNotEditCol);
            ui->Materials->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
            connect(tempModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),tempModel,SLOT(editFinish(QModelIndex)));
            fSelect->frm = frm;
            fSelect->type_select = n_OSTATKI_SKALD;
            fSelect->type_uslugi_= NumberUslugi;
            fSelect->tempModel   = tempModel;
            fSelect->Id_Client   = ID_client;
            fSelect->init(QDate::currentDate());
            fSelect->show();
            ui->Materials->setColumnHidden(0,true);
    }else{
            QMessageBox::question(0,"Внимание!!!","Не заполнены обязательные поля!!!",QMessageBox::Yes);
    }

}

// Удаление материала из списка
void frm_okazanie_uslug::on_del_material_clicked()
{
    ui->Materials->model()->removeRow(ui->Materials->currentIndex().row());

}

// Удаление услуги из списка
void frm_okazanie_uslug::on_del_usluga_clicked()
{
    ui->USLUGI->model()->removeRow(ui->USLUGI->currentIndex().row());
}

// Окончание выбора или ввода клиента
void frm_okazanie_uslug::on_Client_editingFinished()
{
    ID_client = 0;
    QSqlQuery sql;
    sql.prepare("SELECT Clients.ID "
                "FROM Clients "
                "WHERE Clients.FIO = :FIO ");

    sql.bindValue(":FIO",ui->Client->text());
    sql.exec();

    while (sql.next()){
        ID_client = sql.value(0).toInt();
    }
    if (ID_client != 0)
        frm->UpdateClients(ID_client);
}

// Окончание выбора или ввода сотрудника
void frm_okazanie_uslug::on_Sotrudnik_editingFinished()
{
    QSqlQuery query;
    query.prepare("SELECT PERSONAL.ID "
                  "FROM PERSONAL "
                  "WHERE PERSONAL.FIO = :FIO ");
    query.bindValue(":FIO",ui->Sotrudnik->text());
    query.exec();
    QSqlRecord record = query.record();
    query.next();

    ID_sotr = query.value(record.indexOf("ID")).toInt();
}

// Оплата
void frm_okazanie_uslug::on_but_oplatit_clicked()
{

    QDate date_usl = QDate::currentDate();
    int VidPlateja = ui->sposobOplati->itemData(ui->sposobOplati->currentIndex()).toInt();

    // Оплата услуг
    int countRow = ui->USLUGI->model()->rowCount();
    if (ID_sotr != 0 && ID_client != 0 && countRow !=0 ){

        // Если способ оплаты, счет клиента, то списываем со счета сумму платежа
        if (VidPlateja == 5){

            double sum_uslugi = 0;
            for (int ind = 0;ind < countRow; ind++){
                sum_uslugi += ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,5)).value(0).toDouble();
            }

            if (GetOstatokNaSchete(ID_client,date_usl.toString("dd.MM.yyyy")) < sum_uslugi){
                QMessageBox::question(0,"Внимание!!!!","На счете у клиента не достаточно средств!!! \n "
                                      "Выберите другой способ оплаты или пополните счет клиента",QMessageBox::Ok);
                return;
            }
            bool result = EditChetClienta(ID_client,2,sum_uslugi,date_usl.toString("dd.MM.yyyy"));
        }
        // Если способ оплаты, НЕ счет клиента и установлен параметр списывать со счета при оплате
        qDebug() << g_spisanie_so_scheta;
        if (VidPlateja != 5 && g_spisanie_so_scheta){
            qDebug() << "Вошли в условие!!!!!!!!!!!";
            double sum_uslugi = 0;
            double ostatok = 0;
            for (int ind = 0;ind < countRow; ind++){
                sum_uslugi += ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,5)).value(0).toDouble();
            }
            ostatok = GetOstatokNaSchete(ID_client,date_usl.toString("dd.MM.yyyy"));

            if (ostatok < sum_uslugi){
                sum_uslugi = ostatok;
            }
            bool result = EditChetClienta(ID_client,2,sum_uslugi,date_usl.toString("dd.MM.yyyy"));
        }

        for (int ind = 0; ind < countRow; ind++){
            int IDUsl   = ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,0)).value(0).toInt();
            int count   = ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,4)).value(0).toInt();
            double summa= ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,5)).value(0).toDouble();
            double cena = ui->USLUGI->model()->itemData(ui->USLUGI->model()->index(ind,2)).value(0).toDouble();

//            QList<QStandardItem*> lst = mUslugi->findItems(QString(IDUsl),Qt::MatchContains,0);

//            if (lst.count()==0){
                QSqlQuery sql;
                sql.prepare("INSERT INTO CLIENTS_HISTORY(NUMBER,DATE_USLUGI,ID_CLIENT,ID_SOTRUDNIK,ID_USLUGA,count,cena,SUMMA,oplacheno,vid_oplati) "
                            "VALUES(:NUMBER,:DATE_USLUGI,:ID_CLIENT,:ID_SOTRUDNIK,:ID_USLUGA,:count,:cena,:SUMMA,:oplacheno,:vid_oplati) ");

                sql.bindValue(":NUMBER",Number);
                sql.bindValue(":DATE_USLUGI",date_usl.toString("dd.MM.yyyy"));
                sql.bindValue(":ID_CLIENT",ID_client);
                sql.bindValue(":ID_SOTRUDNIK",ID_sotr);
                sql.bindValue(":ID_USLUGA",IDUsl);
                sql.bindValue(":count",count);
                sql.bindValue(":cena",cena);
                sql.bindValue(":SUMMA",summa);
                sql.bindValue(":oplacheno",true);
                sql.bindValue(":vid_oplati",VidPlateja);

                sql.exec();
                qDebug() << sql.lastError();

                ui->USLUGI->setDisabled(true);
                ui->prodaja->setDisabled(true);
                ui->but_oplatit->setDisabled(true);
  //          }
        }
        // Списание материалов
        countRow = 0;
        if (ui->Materials->model() != NULL)
            countRow = ui->Materials->model()->rowCount();

        for (int ind = 0; ind < countRow; ind++){
            int ID_MATERIAL   = ui->Materials->model()->itemData(ui->Materials->model()->index(ind,0)).value(0).toInt();
            int COUNT   = ui->Materials->model()->itemData(ui->Materials->model()->index(ind,2)).value(0).toInt();
//            double summa= ui->Materials->model()->itemData(ui->USLUGI->model()->index(ind,5)).value(0).toDouble();
//            double cena = ui->Materials->model()->itemData(ui->USLUGI->model()->index(ind,2)).value(0).toDouble();

            QSqlQuery sql;
            sql.prepare("INSERT INTO SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,id_VID_ZATRAT,NUMBER) "
                        "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:vid_zatrat,:NUMBER) ");

            sql.bindValue(":DATE",date_usl.toString("dd.MM.yyyy"));
            sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
            sql.bindValue(":COUNT",COUNT * (-1));
            sql.bindValue(":type_operacii",n_PRIHOD);
            sql.bindValue(":vid_zatrat",NumberUslugi);
            sql.bindValue(":NUMBER",Number);
            sql.exec();
            qDebug() << sql.lastError();
            if (ID_client != 0)
                frm->UpdateClients(ID_client);
        }
    }
    else{
        QMessageBox::question(0,"Внимание!!!","Не заполнены обязательные поля!!!",QMessageBox::Ok);
    }

}

void frm_okazanie_uslug::on_Client_buttonClicked()
{
    frmClients *frm_client = new frmClients;
    frm_client->initForm(0);
    frm_client->show();
}
