#include "frmdocument.h"
#include "params.h"
#include "ui_frmdocument.h"

frmDocument::frmDocument(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDocument)
{
    ui->setupUi(this);
}

frmDocument::~frmDocument()
{
    delete ui;
}

void frmDocument::GetOstaok(){
    QSqlQuery sql;

    sql.prepare("SELECT materials.NAME, "
                "   SUM(O_SKLAD.COUNT) AS COUNT "
                "FROM O_SKLAD INNER JOIN "
                "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                "WHERE O_SKLAD.DATE <= :DATE "
                "GROUP BY materials.NAME");
    sql.bindValue(":DATE",ui->DateDoc->date().toString("dd.MM.yyyy"));
    Ost_model *model_ = new Ost_model;
    sql.exec();
    model_->setQuery(sql);
    model_->setHeaderData(0,Qt::Horizontal,"Материал");
    model_->setHeaderData(1,Qt::Horizontal,"Количество");

    ui->tableView->setModel(model_);
}

void frmDocument::initForm(PStandardItemModel *model, int vid_form, int type_doc){
    QSqlQuery sql;
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::ApplicationModal);
    NotEditableDelegate *del = new NotEditableDelegate;
    ui->DateDoc->setDateCalendar(QDate::currentDate());
    QSqlRecord record;

    type_doc_ = type_doc;
    // --- Документ по складу ---
    if (type_doc_ == d_oskald){
        if (vid_form == f_document){ // -- Тип формы документ
            tempModel = model;
            sql.prepare("SELECT MAX(number) AS number "
                        "FROM O_SKLAD");
            sql.exec();
            record = sql.record();
            sql.next();

            Number = sql.value(record.indexOf("number")).toInt();

            Number++;

            sql.prepare("SELECT ID, Name "
                        "FROM group_o_sklad ");
            sql.exec();
            qDebug() << sql.lastError();
            record = sql.record();
            while (sql.next()){
                ui->Group->addItem(sql.value(1).toString(),sql.value(0).toInt());
            }


            ui->Number->setText(QString::number(Number));

            //connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),model,SLOT(editFinish(QModelIndex)));
            ui->tableView->setModel(tempModel);
            ui->tableView->setColumnHidden(0,true);
            ui->tableView->setColumnHidden(1,true);
            ui->tableView->setColumnHidden(4,true);
            ui->tableView->setColumnHidden(5,true);

        }
        if (vid_form == f_ostatki){ // -- Тип формы остатки
            ui->Number->setDisabled(true);
            ui->groupBox->setDisabled(true);
            ui->ApplyBut->setDisabled(true);
            GetOstaok();
        }
    }
    // --- Документ по распределению материала ---
    if (type_doc_ == d_raspred){
        if (vid_form == 0){
            tempModel = model;
            sql.prepare("SELECT MAX(number) AS number "
                     "FROM SKLAD");
            sql.exec();
            record = sql.record();
            sql.next();

            Number = sql.value(record.indexOf("number")).toInt();

            Number++;

            sql.prepare("SELECT vidi_zatrat.ID, vidi_zatrat.Name "
                        "FROM vidi_zatrat");
            sql.exec();
            record = sql.record();
            while (sql.next()){
                ui->Group->addItem(sql.value(1).toString(),sql.value(0).toInt());
            }


            ui->Number->setText(QString::number(Number));

            ui->tableView->setModel(tempModel);
            ui->tableView->setColumnHidden(0,true);
            ui->tableView->setColumnHidden(1,true);
            //ui->tableView->setColumnHidden(2,true);
            ui->tableView->setColumnHidden(4,true);
            ui->tableView->setColumnHidden(5,true);
        }
    }

    ui->tableView->setColumnWidth(2,280);

    ui->tableView->setItemDelegateForColumn(0,del);
    ui->tableView->setItemDelegateForColumn(1,del);
    ui->tableView->setItemDelegateForColumn(2,del);

}

void frmDocument::on_add_button_clicked()
{
    // --- Документ по складу ---
    if (type_doc_ == d_oskald) {
        if (ui->Group->currentIndex() == 0){
            QMessageBox::question(0,"Внимание!!!","Выберите отдел склада!",QMessageBox::Ok);
            return;
        }

        frmSelect *fSelect = new frmSelect();
        fSelect->type_select = n_MATERIAL;
        fSelect->type_uslugi_= 0;
        fSelect->tempModel   = tempModel;
        fSelect->Id_Client   = 0;
        fSelect->init(QDate::currentDate());
        fSelect->setWindowModality(Qt::ApplicationModal);
        fSelect->show();
    }

    // --- Документ по распределению материала ---
    if (type_doc_ == d_raspred){
        if (ui->Group->currentIndex() == 0){
            QMessageBox::question(0,"Внимание!!!","Выберите кабинет!",QMessageBox::Ok);
            return;
        }
        frmSelect *fSelect = new frmSelect();
        fSelect->type_select = n_SKALD;
        fSelect->type_uslugi_= ui->Group->itemData(ui->Group->currentIndex()).toInt();
        fSelect->tempModel   = tempModel;
        fSelect->Id_Client   = 0;
        fSelect->init(QDate::currentDate());

        fSelect->setWindowModality(Qt::ApplicationModal);
        fSelect->show();
    }

}

void frmDocument::on_del_button_clicked()
{
    if (tempModel->rowCount() > 0)
        tempModel->removeRow(ui->tableView->currentIndex().row());
}

void frmDocument::on_ApplyBut_clicked()
{
    int countRow = tempModel->rowCount();
    for (int ind = 0; ind < countRow; ind++){
        QString DATE        = tempModel->itemData(tempModel->index(ind,1)).value(0).toString();
        int ID_MATERIAL     = tempModel->itemData(tempModel->index(ind,0)).value(0).toInt();
        int COUNT           = tempModel->itemData(tempModel->index(ind,3)).value(0).toDouble();
        int type_operacii   = tempModel->itemData(tempModel->index(ind,4)).value(0).toInt();
        int vid_zatrat      = ui->Group->itemData(ui->Group->currentIndex()).toInt();
        int NUMBER          = Number;
        QSqlQuery sql;
        // --- Операции по документу склад ---
        if (type_doc_ == d_oskald){

            sql.prepare("INSERT INTO O_SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,NUMBER,id_group_o_sklad) "
                        "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:NUMBER,:id_group_o_sklad) ");
            sql.bindValue(":DATE",DATE);
            sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
            sql.bindValue(":COUNT",COUNT);
            sql.bindValue(":type_operacii",type_operacii);
            sql.bindValue(":NUMBER",NUMBER);
            sql.bindValue(":vid_zatrat",vid_zatrat);
            sql.exec();

        }
        // --- Операции по документу распределение материалов ---
        if (type_doc_ == d_raspred){
            sql.prepare("INSERT INTO SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,id_VID_ZATRAT,NUMBER) "
                        "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:vid_zatrat,:NUMBER) ");

            sql.bindValue(":DATE",DATE);
            sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
            sql.bindValue(":COUNT",COUNT);
            sql.bindValue(":type_operacii",n_PRIHOD);
            sql.bindValue(":vid_zatrat",vid_zatrat);
            sql.bindValue(":NUMBER",NUMBER);
            sql.exec();

            sql.prepare("SELECT vidi_zatrat.id_group_o_sklad "
                        "FROM vidi_zatrat "
                        "WHERE vidi_zatrat.id_vid_uslug = :vid_zatrat");
            sql.bindValue(":vid_zatrat",vid_zatrat);
            sql.exec();
            sql.next();
            int id_group_o_sklad = sql.record().value("id_group_o_sklad").toInt();

            //Добавляем расход с общего склада
            sql.prepare("INSERT INTO O_SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,NUMBER,id_group_o_sklad) "
                        "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:NUMBER,:id_group_o_sklad)");
            sql.bindValue(":DATE",DATE);
            sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
            sql.bindValue(":COUNT",COUNT * (-1));
            sql.bindValue(":type_operacii",n_RASHOD);
            sql.bindValue(":NUMBER",NUMBER);
            sql.bindValue(":id_group_o_sklad",id_group_o_sklad);
            sql.exec();
        }
    }
    if (countRow > 0)
        this->close();
}

void frmDocument::on_DateDoc_dateChanged(QDate &Date)
{
    GetOstaok();
}
