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

void frmSelect::Updater(){
    QString mass ="0";
    ///// Выбор услуги ////////////////////
    if (type_select == n_USLUGI) {
        setWindowTitle("Выберите услугу...");
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
        tabl->setTable("USLUGI");
        if (!ui->all_ostatki->isChecked())
            tabl->setFilter("USLUGI.VID_USLUGI="+ QString("%1").arg(type_uslugi_));
        //tabl->setFilter("ID NOT IN ("+mass+")");
        tabl->select();
        tabl->setHeaderData(0,Qt::Horizontal,tr("ID"));
        tabl->setHeaderData(1,Qt::Horizontal,tr("Наименование"));
        tabl->setHeaderData(2,Qt::Horizontal,tr("Цена"));

        ui->tableView->setModel(tabl);
        ui->tableView->setColumnWidth(1,300);
        ui->tableView->setColumnHidden(0,true);
        ui->tableView->setColumnHidden(3,true);
        ui->tableView->setColumnHidden(4,true);
        ui->tableView->setColumnHidden(5,true);
    }
    ////////////////// Выбор материалов /////////////////
    else if (type_select == n_MATERIAL) {
        setWindowTitle("Выберите материал...");
        tabl = new PSqlTableModel;
        tabl->setTable("MATERIALS");
        tabl->setRelation(3,QSqlRelation("ed_izm","id","name"));
        tabl->select();
        tabl->setHeaderData(0,Qt::Horizontal,QObject::tr("Код"));
        tabl->setHeaderData(1,Qt::Horizontal,QObject::tr("Наименование"));
        tabl->setHeaderData(2,Qt::Horizontal,QObject::tr("DEL"));
        tabl->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед. Изм"));
        tabl->sort(0,Qt::AscendingOrder);


        ui->tableView->setModel(tabl);
        ui->tableView->setColumnWidth(0,30);
        ui->tableView->setColumnWidth(1,300);
        ui->tableView->setColumnWidth(3,50);
        ui->tableView->setColumnHidden(2,true);
        ui->tableView->setColumnHidden(0,true);
        ui->tableView->setSortingEnabled(true);
    }
    //**************************** Добавление материала со склада //***********************
    else if (type_select == n_OSTATKI_SKALD || type_select == n_SKALD) {
        setWindowTitle("Выберите материал...");
        if (tempModel->columnCount() == 0) {
            tempModel->insertColumn(0);
            tempModel->insertColumn(1);
            tempModel->insertColumn(2);
            tempModel->insertColumn(3);
            tempModel->setHeaderData(0,Qt::Horizontal,tr("ID"));            //0
            tempModel->setHeaderData(1,Qt::Horizontal,tr("Наименование"));  //1
            tempModel->setHeaderData(2,Qt::Horizontal,tr("Кол-во"));        //2
            tempModel->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед. Изм"));
        }
        QSqlQuery sql;
        if (type_select == n_OSTATKI_SKALD){
            if (ui->all_ostatki->isChecked()){
                sql.prepare("SELECT "
                            "   materials.ID, "
                            "   materials.NAME, "
                            "   SUM(SKLAD.COUNT) AS COUNT, "
                            "   ed_izm.name "
                            "FROM "
                            "   ed_izm, "
                            "   SKLAD INNER JOIN "
                            "       materials ON materials.ID = SKLAD.ID_MATERIAL "
                            "WHERE "
                            "   SKLAD.DATE <= :DATE "
                            "   AND materials.id_ed_izm = ed_izm.id "
                            "GROUP BY "
                            "   materials.NAME, "
                            "   materials.ID, "
                            "   ed_izm.name ");
            }else{
                sql.prepare("SELECT "
                            "   materials.ID, "
                            "   materials.NAME, "
                            "   SUM(SKLAD.COUNT) AS COUNT, "
                            "   ed_izm.name "
                            "FROM "
                            "   ed_izm, "
                            "   SKLAD INNER JOIN "
                            "       materials ON materials.ID = SKLAD.ID_MATERIAL "
                            "       INNER JOIN vidi_zatrat ON vidi_zatrat.id = SKLAD.id_vid_zatrat "
                            "WHERE SKLAD.DATE <= :DATE "
                            "   AND materials.id_ed_izm = ed_izm.id "
                            "   AND vidi_zatrat.id_vid_uslug = :type_uslugi "
                            "GROUP BY "
                            "   materials.NAME, "
                            "   materials.ID, "
                            "   ed_izm.name ");
                sql.bindValue(":type_uslugi",type_uslugi_);
            }
        }else
            if (! ui->all_ostatki->isChecked()){
                sql.prepare("SELECT "
                            "   materials.ID, "
                            "   materials.NAME, "
                            "   SUM(O_SKLAD.COUNT) AS COUNT, "
                            "   ed_izm.name "
                            "FROM O_SKLAD INNER JOIN "
                            "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                            "   INNER JOIN "
                            "       vidi_zatrat ON vidi_zatrat.id_group_o_sklad = O_SKLAD.id_group_o_sklad, "
                            "   ed_izm "
                            "WHERE O_SKLAD.DATE <= :DATE "
                            "   AND vidi_zatrat.id_group_o_sklad = :VidZatrat "
                            "   AND materials.id_ed_izm = ed_izm.id "
                            "GROUP BY "
                            "   materials.NAME, "
                            "   materials.ID,"
                            "   ed_izm.name ");
                sql.bindValue(":VidZatrat",type_uslugi_);
            }else{
                sql.prepare("SELECT "
                            "   materials.ID, "
                            "   materials.NAME, "
                            "   SUM(O_SKLAD.COUNT) AS COUNT, "
                            "   ed_izm.name "
                            "FROM O_SKLAD INNER JOIN "
                            "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                            "   INNER JOIN "
                            "       vidi_zatrat ON vidi_zatrat.id_group_o_sklad = O_SKLAD.id_group_o_sklad, "
                            "   ed_izm "
                            "WHERE O_SKLAD.DATE <= :DATE "
                            "   AND materials.id_ed_izm = ed_izm.id "
                            "GROUP BY "
                            "   materials.NAME, "
                            "   materials.ID,"
                            "   ed_izm.name ");
            }

        sql.bindValue(":DATE",DateDoc.toString("dd.MM.yyyy"));
        sql.exec();
        tabl_ = new Ost_model;
        tabl_->setQuery(sql);
        tabl_->setHeaderData(1,Qt::Horizontal,QObject::tr("Наименование"));
        tabl_->setHeaderData(2,Qt::Horizontal,QObject::tr("Кол-во"));
        tabl_->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед.изм"));
        ui->tableView->setModel(tabl_);
        ui->tableView->setColumnWidth(1,250);
        ui->tableView->setColumnHidden(0,true);
    }
    /////////////// Выбор клиента //////////////////
    if (type_select == n_CLIENTS){
        setWindowTitle("Выберите клиента...");
        tempModel->insertColumn(0);
        tempModel->setHeaderData(0,Qt::Horizontal,tr("ФИО"));  //1

        QSqlQuery sql;
        sql.prepare("select FIO, POL FROM clients");
        sql.exec();

        selTabl = new clSqlQueryModel;
        selTabl->setQuery(sql);
        selTabl->setHeaderData(0,Qt::Horizontal,QObject::tr("ФИО"));
        ui->tableView->setModel(selTabl);
        ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        ui->tableView->hideColumn(1);
    }
    ////////////////// Выбор мастера ////////////////
    if (type_select == n_MASTER){
        setWindowTitle("Выберите мастера...");
        tempModel->insertColumn(0);
        tempModel->setHeaderData(0,Qt::Horizontal,tr("ФИО"));  //1

        QSqlQuery sql;
        if (type_uslugi_ != 5){
            sql.prepare("SELECT Personal.FIO, "
                        " Personal.POL "
                        "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID "
                        "WHERE DOLJNOSTI.VID_USLUGI = :Vid" );
            sql.bindValue(":VID",type_uslugi_);
        }else{
            sql.prepare("SELECT Personal.FIO, "
                        " Personal.POL "
                        "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID ");
        }
        sql.exec();

        selTabl = new clSqlQueryModel;
        selTabl->setQuery(sql);
        selTabl->setHeaderData(0,Qt::Horizontal,QObject::tr("ФИО"));
        ui->tableView->setModel(selTabl);
        ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        ui->tableView->hideColumn(1);
    }

    ////////////////// Выбор товара ////////////////
    if (type_select == n_MAGAZIN){
        setWindowTitle("Выберите товар...");
        if (tempModel->columnCount() == 0) {
            tempModel->insertColumn(0);
            tempModel->insertColumn(1);
            tempModel->insertColumn(2);
            tempModel->insertColumn(3);
            tempModel->insertColumn(4);
            tempModel->setHeaderData(0,Qt::Horizontal,tr("ID"));            //0
            tempModel->setHeaderData(1,Qt::Horizontal,tr("Наименование"));  //1
            tempModel->setHeaderData(2,Qt::Horizontal,tr("Кол-во"));        //2
            tempModel->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед. Изм"));
            tempModel->setHeaderData(4,Qt::Horizontal,QObject::tr("Сумма"));
        }
        QSqlQuery sql;

        if (ui->all_ostatki->isChecked()){
            sql.prepare("SELECT "
                        "   materials.ID, "
                        "   materials.NAME, "
                        "   SUM(SKLAD.COUNT) AS COUNT, "
                        "   ed_izm.name, "
                        "   SUM(0) AS SUMMA "
                        "FROM "
                        "   ed_izm, "
                        "   SKLAD INNER JOIN "
                        "       materials ON materials.ID = SKLAD.ID_MATERIAL "
                        "WHERE "
                        "   SKLAD.DATE <= :DATE "
                        "   AND materials.id_ed_izm = ed_izm.id "
                        "GROUP BY "
                        "   materials.NAME, "
                        "   materials.ID, "
                        "   ed_izm.name ");
        }else{
            sql.prepare("SELECT "
                        "   materials.ID, "
                        "   materials.NAME, "
                        "   SUM(SKLAD.COUNT) AS COUNT, "
                        "   ed_izm.name, "
                        "   SUM(0) AS SUMMA "
                        "FROM "
                        "   ed_izm, "
                        "   SKLAD INNER JOIN "
                        "       materials ON materials.ID = SKLAD.ID_MATERIAL "
                        "       INNER JOIN vidi_zatrat ON vidi_zatrat.id = SKLAD.id_vid_zatrat "
                        "WHERE SKLAD.DATE <= :DATE "
                        "   AND materials.id_ed_izm = ed_izm.id "
                        "   AND vidi_zatrat.id_vid_uslug = :type_uslugi "
                        "GROUP BY "
                        "   materials.NAME, "
                        "   materials.ID, "
                        "   ed_izm.name ");
            sql.bindValue(":type_uslugi",type_uslugi_);

        }
        sql.bindValue(":DATE",DateDoc.toString("dd.MM.yyyy"));
        sql.exec();
        tabl_ = new Ost_model;
        tabl_->setQuery(sql);
        tabl_->setHeaderData(1,Qt::Horizontal,QObject::tr("Наименование"));
        tabl_->setHeaderData(2,Qt::Horizontal,QObject::tr("Кол-во"));
        tabl_->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед.изм"));
        tabl_->setHeaderData(5,Qt::Horizontal,QObject::tr("Сумма"));
        ui->tableView->setModel(tabl_);
        ui->tableView->setColumnWidth(1,250);
        ui->tableView->setColumnHidden(0,true);
    }
}

void frmSelect::init(QDate date){
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::ApplicationModal);
    DateDoc = date;
    Updater();

    ui->all_ostatki->setVisible(false);
}

void frmSelect::multeSelect(const QModelIndexList &indexList){
    QModelIndex index;

    foreach(index, indexList)
        on_tableView_doubleClicked(index);
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
        tempModel->setData(tempModel->index(row,6),record.value(3).toString(),Qt::EditRole);
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
            tempModel->setData(tempModel->index(row,3),record.value(3).toString(),Qt::EditRole);
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
            tempModel->setData(tempModel->index(row,6),record.value(3).toString(),Qt::EditRole);
        }
    }
    if (type_select == n_CLIENTS){
        tempModel->insertRow(countRow);
        QSqlRecord record = selTabl->record(index.row());
        tempModel->setData(tempModel->index(countRow,0),record.value(0).toString(),Qt::EditRole);
        this->close();
    }

    if (type_select == n_MASTER){
        tempModel->insertRow(countRow);
        QSqlRecord record = selTabl->record(index.row());
        tempModel->setData(tempModel->index(countRow,0),record.value(0).toString(),Qt::EditRole);
        this->close();
    }

    ///////////////////
    if (type_select == n_MAGAZIN){
        QSqlRecord record = tabl_->record(index.row());
        int countRec = record.value("COUNT").toInt();
        QList<QStandardItem*> lst = tempModel->findItems(record.value(0).toString(),Qt::MatchContains,0);

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
        tempModel->setData(tempModel->index(row,3),record.value(3).toString(),Qt::EditRole);
        tempModel->setData(tempModel->index(row,5),record.value(5).toString(),Qt::EditRole);

    }
}

void frmSelect::on_all_ostatki_clicked()
{
    Updater();
}

void frmSelect::on_sel_button_clicked()
{
    //    ui->tableView->selectedIndexes();
    multeSelect(ui->tableView->selectionModel()->selectedRows());
    //    on_tableView_doubleClicked(ui->tableView->currentIndex());
    this->close();
}
