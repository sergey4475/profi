#include "frmo_sklad.h"
#include "ui_frmo_sklad.h"

frmO_SKLAD::frmO_SKLAD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmO_SKLAD)
{
    ui->setupUi(this);
}

frmO_SKLAD::~frmO_SKLAD()
{
    delete ui;
}

void frmO_SKLAD::initForm(uslStandardItemModel *model,int vid_form){

    QSqlQuery sql;
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::ApplicationModal);
    NotEditableDelegate *del = new NotEditableDelegate;
    ui->qDateLineEdit->setDateCalendar(QDate::currentDate());

    if (vid_form == 0){
        tempModel = model;
        sql.prepare("SELECT MAX(number) AS number "
                 "FROM O_SKLAD");
        sql.exec();
        QSqlRecord record = sql.record();
        sql.next();

        Number = sql.value(record.indexOf("number")).toInt();

        Number++;

        ui->Number->setText(QString::number(Number));

        //connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),model,SLOT(editFinish(QModelIndex)));
        ui->tableView->setModel(tempModel);
        ui->tableView->setColumnHidden(0,true);
        ui->tableView->setColumnHidden(1,true);
        ui->tableView->setColumnHidden(4,true);
        ui->tableView->setColumnHidden(5,true);

    }
    if (vid_form == 1){
        ui->Number->setDisabled(true);
        ui->groupBox->setDisabled(true);
        GetOstaok();
    }

    ui->tableView->setColumnWidth(2,280);

    ui->tableView->setItemDelegateForColumn(0,del);
    ui->tableView->setItemDelegateForColumn(1,del);
    ui->tableView->setItemDelegateForColumn(2,del);
}

void frmO_SKLAD::on_toolButton_4_clicked()
{
    frmSelect *fSelect = new frmSelect();
    fSelect->init(2,0,tempModel,0,QDate::currentDate());
    fSelect->setWindowModality(Qt::ApplicationModal);
    fSelect->show();

}

void frmO_SKLAD::on_del_button_clicked()
{
    if (tempModel->rowCount() > 0)
        tempModel->removeRow(ui->tableView->currentIndex().row());
}

void frmO_SKLAD::GetOstaok(){
    QSqlQuery sql;

    sql.prepare("SELECT materials.NAME, "
                "   SUM(O_SKLAD.COUNT) AS COUNT "
                "FROM O_SKLAD INNER JOIN "
                "   materials ON materials.ID = O_SKLAD.ID_MATERIAL "
                "WHERE O_SKLAD.DATE <= :DATE "
                "GROUP BY materials.NAME");
    sql.bindValue(":DATE",ui->qDateLineEdit->date().toString("dd.MM.yyyy"));

    Ost_model *model_ = new Ost_model;
    sql.exec();
    qDebug() << ui->qDateLineEdit->date().toString();
    model_->setQuery(sql);
    model_->setHeaderData(0,Qt::Horizontal,"Материал");
    model_->setHeaderData(1,Qt::Horizontal,"Количество");

    ui->tableView->setModel(model_);


}

void frmO_SKLAD::on_ApplyBut_clicked()
{
    int countRow = tempModel->rowCount();
    for (int ind = 0; ind < countRow; ind++){
        QString DATE        = tempModel->itemData(tempModel->index(ind,1)).value(0).toString();
        int ID_MATERIAL     = tempModel->itemData(tempModel->index(ind,0)).value(0).toInt();
        int COUNT           = tempModel->itemData(tempModel->index(ind,3)).value(0).toDouble();
        int type_operacii   = tempModel->itemData(tempModel->index(ind,4)).value(0).toInt();
        int NUMBER          = Number;


        QSqlQuery sql;
        sql.prepare("INSERT INTO O_SKLAD(DATE,ID_MATERIAL,COUNT,type_operacii,NUMBER) "
                    "VALUES(:DATE,:ID_MATERIAL,:COUNT,:type_operacii,:NUMBER) ");

        sql.bindValue(":DATE",DATE);
        sql.bindValue(":ID_MATERIAL",ID_MATERIAL);
        sql.bindValue(":COUNT",COUNT);
        sql.bindValue(":type_operacii",type_operacii);
        sql.bindValue(":NUMBER",NUMBER);

        sql.exec();
    }
    if (countRow > 0)
        this->close();

}

void frmO_SKLAD::on_qDateLineEdit_dateChanged(QDate &Date)
{
    GetOstaok();
}
