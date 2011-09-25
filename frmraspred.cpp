#define  PRIHOD 1
#define  RASHOD 2
#include "frmraspred.h"
#include "sqlmodules.h"
#include "frmselect.h"
#include "delegats.h"
#include "ui_frmraspred.h"

frmRaspred::frmRaspred(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmRaspred)
{
    ui->setupUi(this);
}

frmRaspred::~frmRaspred()
{
    delete ui;
}

void frmRaspred::initForm(PStandardItemModel *model, int vid_form){

    QSqlQuery sql;
    this->setWindowFlags(Qt::Tool);
//    this->setWindowModality(Qt::WindowModal);
    QSqlTableModel *model_ = new QSqlTableModel;
    model_->setTable("vidi_zatrat");
    model_->select();
    for (int i=0;i < model_->rowCount();++i){
        QSqlRecord rec = model_->record(i);
        ui->comboVidUslugi->addItem(rec.value("NAME").toString(),rec.value("ID_VID_USLUG").toInt());
    }
    NotEditableDelegate *del = new NotEditableDelegate;
    ui->DateDoc->setDateCalendar(QDate::currentDate());

    if (vid_form == 0){
        tempModel = model;
        sql.prepare("SELECT MAX(number) AS number "
                 "FROM SKLAD");
        sql.exec();
        QSqlRecord record = sql.record();
        sql.next();

        Number = sql.value(record.indexOf("number")).toInt();

        Number++;

        ui->Number->setText(QString::number(Number));

        ui->tableView->setModel(tempModel);
        ui->tableView->setColumnHidden(0,true);
        ui->tableView->setColumnHidden(1,true);
        //ui->tableView->setColumnHidden(2,true);
        ui->tableView->setColumnHidden(4,true);
        ui->tableView->setColumnHidden(5,true);

    }

}

void frmRaspred::on_add_button_clicked()
{
    if (ui->comboVidUslugi->currentIndex() == 0){
        QMessageBox::question(0,"Внимание!!!","Выбеите кабинет!",QMessageBox::Ok);
        return;
    }
    frmSelect *fSelect = new frmSelect();
    qDebug() << QDate::currentDate().toString("dd.MM.yyyy");
    fSelect->init(4,ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toInt(),tempModel,0,QDate::currentDate());
    fSelect->setWindowModality(Qt::ApplicationModal);
    fSelect->show();
}

void frmRaspred::on_ApplyBut_clicked()
{
    int countRow = tempModel->rowCount();
    for (int ind = 0; ind < countRow; ind++){
        QString DATE        = tempModel->itemData(tempModel->index(ind,1)).value(0).toString();
        int ID_MATERIAL     = tempModel->itemData(tempModel->index(ind,0)).value(0).toInt();
        int COUNT           = tempModel->itemData(tempModel->index(ind,3)).value(0).toDouble();
        int vid_zatrat      = ui->comboVidUslugi->itemData(ui->comboVidUslugi->currentIndex()).toInt();
        int NUMBER          = Number;

        // Добавляем приход на склад комнаты
        QSqlQuery sql;
        sql.prepare("INSERT INTO SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,id_VID_ZATRAT,NUMBER) "
                    "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:vid_zatrat,:NUMBER) ");

        sql.bindValue(":DATE",DATE);
        sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
        sql.bindValue(":COUNT",COUNT);
        sql.bindValue(":type_operacii",PRIHOD);
        sql.bindValue(":vid_zatrat",vid_zatrat);
        sql.bindValue(":NUMBER",NUMBER);

        sql.exec();
        qDebug() << sql.lastError();

        //Добавляем расход с общего склада
        sql.prepare("INSERT INTO O_SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,NUMBER) "
                    "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:NUMBER) ");
        sql.bindValue(":DATE",DATE);
        sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
        sql.bindValue(":COUNT",COUNT * (-1));
        sql.bindValue(":type_operacii",RASHOD);
        sql.bindValue(":NUMBER",NUMBER);

        sql.exec();
        qDebug() << sql.lastError();

    }
    if (countRow > 0)
        this->close();

}
