#include "frmselect.h"
#include "params.h"
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

void frmSelect::init(QDate date){
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::WindowModal);
    QString mass ="0";
    DateDoc = date;
    /////
    if (type_select == n_USLUGI) {
        QSqlQuery query;
        query.prepare("SELECT CLIENTS_HISTORY.ID_USLUGA "
                      "FROM CLIENTS_HISTORY "
                      "WHERE CLIENTS_HISTORY.ID_CLIENT = :ID_CLIENT AND CLIENTS_HISTORY.DATE_USLUGI = :DATE_USLUGI");
        query.bindValue(":ID_CLIENT",Id_Client);
        query.bindValue(":DATE_USLUGI",DateDoc.toString("dd.MM.yyyy"));
        query.exec();

        while (query.next()) {
            mass = mass + ",";
            mass = mass + query.value(0).toString();
        }

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

        tabl = new PSqlTableModel;
        tabl->setTable("n_USLUGI");
        tabl->setFilter("VID_USLUGI="+ QString("%1").arg(type_uslugi_));
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
    else if (type_select == n_MATERIAL) {
        tabl = new PSqlTableModel;
        tabl->setTable("MATERIALS");

        tabl->select();
        tabl->setHeaderData(0,Qt::Horizontal,tr("Код"));
        tabl->setHeaderData(1,Qt::Horizontal,tr("Наименование"));

        ui->tableView->setModel(tabl);
        ui->tableView->setColumnWidth(1,300);
        ui->tableView->setColumnHidden(2,true);
    }
    //**************************** Добавление материала со склада //***********************
    else if (type_select == n_OSTATKI_SKALD || type_select == n_SKALD) {

    if (tempModel->columnCount() == 0) {
        tempModel->insertColumn(0);
        tempModel->insertColumn(1);
        tempModel->insertColumn(2);
        tempModel->setHeaderData(0,Qt::Horizontal,tr("ID"));            //0
        tempModel->setHeaderData(1,Qt::Horizontal,tr("Наименование"));  //1
        tempModel->setHeaderData(2,Qt::Horizontal,tr("Кол-во"));        //2
    }
    QSqlQuery sql;
    if (type_select == n_OSTATKI_SKALD){
        sql.prepare("SELECT "
                    "   materials.ID, "
                    "   materials.NAME, "
                    "   SUM(SKLAD.COUNT) AS COUNT "
                    "FROM SKLAD INNER JOIN "
                    "   materials ON materials.ID = SKLAD.ID_MATERIAL "
                    "WHERE SKLAD.DATE <= :DATE "
                    "   AND SKLAD.id_Vid_Zatrat = :VidZatrat "
                    "GROUP BY "
                    "   materials.NAME, "
                    "   materials.ID");
        sql.bindValue(":VidZatrat",type_uslugi_);
    }
    else
        sql.prepare("SELECT "
                        "materials.ID, "
                        "materials.NAME, "
                        "SUM(O_SKLAD.COUNT) AS COUNT "
                    "FROM O_SKLAD INNER JOIN "
                    "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                    "   INNER JOIN "
                    "       vidi_zatrat ON vidi_zatrat.id_group_o_sklad = O_SKLAD.id_group_o_sklad "
                    "WHERE O_SKLAD.DATE <= :DATE "
                    "AND vidi_zatrat.ID = :VidZatrat "
                    "GROUP BY "
                    "   materials.NAME, "
                    "   materials.ID");

    qDebug() << "Дата Документа: " + DateDoc.toString("dd.MM.yyyy");
    sql.bindValue(":DATE",DateDoc.toString("dd.MM.yyyy"));
    sql.bindValue(":VidZatrat",type_uslugi_);
    sql.exec();
    qDebug() << sql.lastError();
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
    if (type_select == n_USLUGI){
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
    if (type_select == n_MATERIAL){
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
    if (type_select == n_OSTATKI_SKALD || type_select == n_SKALD){
        QSqlRecord record = tabl_->record(index.row());
        int countRec = record.value("COUNT").toInt();
        QList<QStandardItem*> lst = tempModel->findItems(record.value(0).toString(),Qt::MatchContains,0);
        if (type_select == n_OSTATKI_SKALD){

            flag = true;
            if(countRec <= 0){

                flag = false;
                QMessageBox::question(0,"Внимание","Данного материала нет на складе!",QMessageBox::Yes,QMessageBox::No);
            }
            if (flag == true){
                if (lst.count()==0){
                    tempModel->insertRow(countRow);
                    row = countRow;
                    count = 1;
                }else{
                    row = lst[0]->index().row();
                    count = tempModel->itemData(tempModel->index(lst[0]->index().row(),2)).value(0).toInt();
                    count++;
                }
                countRec = countRec - 1;

                tabl_->setData(tabl_->index(index.row(),2),countRec);
                ui->tableView->setModel(tabl_);
            }

            tempModel->setData(tempModel->index(row,0),record.value(0).toString(),Qt::EditRole);
            tempModel->setData(tempModel->index(row,1),record.value(1).toString(),Qt::EditRole);
            tempModel->setData(tempModel->index(row,2),count,Qt::EditRole);
        }else{
            flag = true;
            if(record.value("COUNT").toInt()<=0){
                flag = false;
                QMessageBox::question(0,"Внимание","Данного материала нет на складе!",QMessageBox::Yes,QMessageBox::No);
            }
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
                countRec = countRec - 1;
                tabl_->setData(tabl_->index(index.row(),2),countRec);
                ui->tableView->setModel(tabl_);
            }
            tempModel->setData(tempModel->index(row,0),record.value(0).toString(),Qt::EditRole);
            tempModel->setData(tempModel->index(row,1),DateDoc.toString("dd.MM.yyyy"),Qt::EditRole);
            tempModel->setData(tempModel->index(row,2),record.value(1).toString(),Qt::EditRole);
            tempModel->setData(tempModel->index(row,3),count,Qt::EditRole);
            tempModel->setData(tempModel->index(row,4),1,Qt::EditRole);
            tempModel->setData(tempModel->index(row,5),type_uslugi_,Qt::EditRole);
        }
    }
}
