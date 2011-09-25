#include "frmspr.h"
#include "ui_frmspr.h"

frmSpr::frmSpr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmSpr)
{
    ui->setupUi(this);
}

frmSpr::~frmSpr()
{
    delete ui;
}

void frmSpr::init(uslSqlTableModel *Model,QList<QAbstractItemDelegate*> lst){
    this->setWindowFlags(Qt::Tool);
    sprModel = Model;
    ui->sprTable->setModel(Model);
    ui->sprTable->setColumnHidden(2,true);
    ui->sprTable->setColumnWidth(0,40);
    ui->sprTable->setColumnWidth(1,300);

    for (int ind=0; ind < lst.count();ind++)
        ui->sprTable->setItemDelegateForColumn(ind,lst[ind]);
}

void frmSpr::on_add_usluga_clicked()
{
    int row = sprModel->rowCount();
    sprModel->insertRow(row);
    sprModel->submitAll();
}

void frmSpr::on_del_usluga_clicked()
{
        if (QMessageBox::question(0,"Вопрос","Вы уверены что желаете удалить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
            sprModel->setData(sprModel->index(ui->sprTable->currentIndex().row(),2),1);
            sprModel->submit();
        }
}
