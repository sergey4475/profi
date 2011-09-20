#define USLUGI 1
#define MATERIAL 2
#define OSTATKI_SKALD 3
#define SKALD 4


#include "frmselect.h"
#include "ui_frmselect.h"
#include <QList>

frmSelect::frmSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmSelect)
{
    ui->setupUi(this);
}

frmSelect::~frmSelect()
{
    delete ui;
}

void frmSelect::init(int type_sel, int type_uslugi, uslStandardItemModel *table,int ID_client, QDate date){
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::WindowModal);
    QString mass ="0";
    DateDoc = date;
    type_select = type_sel;
    type_uslugi_= type_uslugi;

    /////
    if (type_select == USLUGI) {
        QSqlQuery query;
        query.prepare("SELECT CLIENTS_HISTORY.ID_USLUGA "
                      "FROM CLIENTS_HISTORY "
                      "WHERE CLIENTS_HISTORY.ID_CLIENT = :ID_CLIENT AND CLIENTS_HISTORY.DATE_USLUGI = :DATE_USLUGI");
        query.bindValue(":ID_CLIENT",ID_client);
        query.bindValue(":DATE_USLUGI",date.toString("dd.MM.yyyy"));
        query.exec();

        while (query.next()) {
            mass = mass + ",";
            mass = mass + query.value(0).toString();
        }

        tempModel = table;

        if (tempModel->columnCount() == 0) {
            tempModel->insertColumn(0);
            tempModel->insertColumn(1);
            tempModel->insertColumn(2);
            tempModel->insertColumn(3);
            tempModel->insertColumn(4);
            tempModel->insertColumn(5);
            tempModel->setHeaderData(0,Qt::Horizontal,tr("ID"));            //0
            tempModel->setHeaderData(1,Qt::Horizontal,tr("Наименование"));  //1
            tempModel->setHeaderData(2,Qt::Horizontal,tr("Цена"));          //2
            tempModel->setHeaderData(3,Qt::Horizontal,tr("DEL"));           //3
            tempModel->setHeaderData(4,Qt::Horizontal,tr("Кол-во"));        //4
            tempModel->setHeaderData(5,Qt::Horizontal,tr("Сумма"));         //5
        }

        tabl = new uslSqlTableModel;
        tabl->setTable("USLUGI");
        tabl->setFilter("VID_USLUGI="+ QString("%1").arg(type_uslugi));
        tabl->setFilter("ID NOT IN ("+mass+")");
        tabl->select();
        tabl->setHeaderData(0,Qt::Horizontal,tr("ID"));
        tabl->setHeaderData(1,Qt::Horizontal,tr("Наименование"));
        tabl->setHeaderData(3,Qt::Horizontal,tr("Цена"));

        ui->tableView->setModel(tabl);
        ui->tableView->setColumnWidth(1,300);
        ui->tableView->setColumnHidden(0,true);
        ui->tableView->setColumnHidden(3,true);
        ui->tableView->setColumnHidden(4,true);
        ui->tableView->setColumnHidden(5,true);
    }
    //////////////////
    else if (type_select == MATERIAL) {
        tempModel = table;

        tabl = new uslSqlTableModel;
        tabl->setTable("MATERIALS");

        tabl->select();
        tabl->setHeaderData(0,Qt::Horizontal,tr("Код"));
        tabl->setHeaderData(1,Qt::Horizontal,tr("Наименование"));

        ui->tableView->setModel(tabl);
        ui->tableView->setColumnWidth(1,300);
        ui->tableView->setColumnHidden(2,true);
    }
    //**************************** Добавление материала со склада //***********************
    else if (type_select == OSTATKI_SKALD || type_select == SKALD) {
    tempModel = table;

    if (tempModel->columnCount() == 0) {
        tempModel->insertColumn(0);
        tempModel->insertColumn(1);
        tempModel->insertColumn(2);
        tempModel->setHeaderData(0,Qt::Horizontal,tr("ID"));            //0
        tempModel->setHeaderData(1,Qt::Horizontal,tr("Наименование"));  //1
        tempModel->setHeaderData(2,Qt::Horizontal,tr("Кол-во"));        //2
    }
    QSqlQuery sql;
    if (type_select == OSTATKI_SKALD)
        sql.prepare("SELECT "
                    "   materials.ID, "
                    "   materials.NAME, "
                    "   SUM(SKLAD.COUNT) AS COUNT "
                    "FROM SKLAD INNER JOIN "
                    "   materials ON materials.ID = SKLAD.ID_MATERIAL "
                    "WHERE SKLAD.DATE <= :DATE "
                    "   AND SKLAD.id_Vid_Zatrat = :VidZatrat "
                    "GROUP BY materials.NAME");
    else
        sql.prepare("SELECT "
                    "   materials.ID, "
                    "   materials.NAME, "
                    "   SUM(O_SKLAD.COUNT) AS COUNT "
                    "FROM O_SKLAD INNER JOIN "
                    "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                    "WHERE O_SKLAD.DATE <= :DATE "
                    "GROUP BY materials.NAME");

    sql.bindValue(":DATE",date);
    sql.exec();
    tabl_ = new Ost_model;
    tabl_->setQuery(sql);
    tabl_->setHeaderData(1,Qt::Horizontal,QObject::tr("Наименование"));
    tabl_->setHeaderData(2,Qt::Horizontal,QObject::tr("Кол-во"));
    ui->tableView->setModel(tabl_);
    ui->tableView->setColumnWidth(1,300);
    ui->tableView->setColumnHidden(0,true);
    }
}

void frmSelect::on_tableView_doubleClicked(const QModelIndex &index)
{
    int count = 1;
    int row = 0;
    int countRow = tempModel->rowCount();
    bool flag = true;
    ///////////
    if (type_select == 1){
        QSqlRecord record = tabl->record(index.row());
        double summ  = 0.00;

        if (record.value("DEL").toInt()==1){
            flag = false;
            if(QMessageBox::question(0,"Внимание","Элемент помечен на удаление! Вы уверены, что хотите его добавить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
                flag = true;
            }
        };

    QList<QStandardItem*> lst = tempModel->findItems(record.value("ID").toString(),Qt::MatchContains,0);

    if (flag == true){
        if (lst.count()==0){
            tempModel->insertRow(countRow);
            row = countRow;
            count = 1;
        }else{
            row = lst[0]->index().row();
            count = tempModel->itemData(tempModel->index(lst[0]->index().row(),4)).value(0).toInt();
            count++;
        }

        tempModel->setData(tempModel->index(row,0),record.value("ID").toInt(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,1),record.value("Name").toString(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,2),record.value("Cena").toFloat(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,3),record.value("DEL").toInt(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,4),count,Qt::EditRole);
        summ = record.value("Cena").toFloat() * record.value(4).toInt();
        tempModel->setData(tempModel->index(row,5),summ,Qt::EditRole);
    }
    }
    /////////////
    if (type_select == 2){
        QSqlRecord record = tabl->record(index.row());
        if (record.value("DEL").toInt()==1){
            flag = false;
            if(QMessageBox::question(0,"Внимание","Элемент помечен на удаление! Вы уверены, что хотите его добавить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
                flag = true;
            }
        };


        QList<QStandardItem*> lst = tempModel->findItems(record.value("ID").toString(),Qt::MatchContains,0);
        if (flag == true){
            if (lst.count()==0){
                tempModel->insertRow(countRow);
                row = countRow;
                count = 1;
            }else{
                row = lst[0]->index().row();
                count = tempModel->itemData(tempModel->index(lst[0]->index().row(),3)).value(0).toInt();
                count++;
            }
        }
        tempModel->setData(tempModel->index(row,0),record.value(0).toInt(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,1),DateDoc.toString("dd.MM.yyyy"),Qt::EditRole);
        tempModel->setData(tempModel->index(row,2),record.value(1).toString(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,3),count,Qt::EditRole);
        tempModel->setData(tempModel->index(row,4),1,Qt::EditRole);
    }
    ///////////////////
    if (type_select == 3 || type_select == 4){
        QSqlRecord record = tabl_->record(index.row());

        QList<QStandardItem*> lst = tempModel->findItems(record.value(0).toString(),Qt::MatchContains,0);
        if (flag == true){
            if (lst.count()==0){
                tempModel->insertRow(countRow);
                row = countRow;
                count = 1;
            }else{
                row = lst[0]->index().row();
                count = tempModel->itemData(tempModel->index(lst[0]->index().row(),3)).value(0).toInt();
                count++;
            }
        }
        tempModel->setData(tempModel->index(row,0),record.value(0).toString(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,1),DateDoc.toString("dd.MM.yyyy"),Qt::EditRole);
        tempModel->setData(tempModel->index(row,2),record.value(1).toString(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,3),count,Qt::EditRole);
        tempModel->setData(tempModel->index(row,4),1,Qt::EditRole);
        tempModel->setData(tempModel->index(row,5),type_uslugi_,Qt::EditRole);
    }
}
