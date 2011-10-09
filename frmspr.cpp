#include "frmspr.h"
#include "params.h"
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

void frmSpr::init(PSqlTableModel *Model,QList<QAbstractItemDelegate*> lst){
    this->setWindowFlags(Qt::Tool);
    sprModel = Model;
    ui->sprTable->setModel(Model);
    ui->sprTable->setColumnHidden(2,true);
    ui->sprTable->setColumnWidth(0,40);
    ui->sprTable->setColumnWidth(1,300);
    sprModel->submitAll();
    sprModel->setSort(0,Qt::AscendingOrder);
    for (int ind=0; ind < lst.count();ind++)
        ui->sprTable->setItemDelegateForColumn(ind,lst[ind]);

    ui->sprTable->installEventFilter(this);
    QObject::connect(sprModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(editFinish(QModelIndex)));
}

void frmSpr::on_add_usluga_clicked()
{
    int row = sprModel->rowCount();
    sprModel->insertRow(row);
    sprModel->submitAll();
    ui->sprTable->edit(sprModel->index(row,1));
    ui->sprTable->scrollToBottom();

}

void frmSpr::on_del_usluga_clicked()
{
        if (QMessageBox::question(0,"Вопрос","Вы уверены что желаете удалить?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
            sprModel->setData(sprModel->index(ui->sprTable->currentIndex().row(),2),1);
            sprModel->submit();
        }
}

bool frmSpr::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // Обработка нажатия enter или return
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
            if(ui->sprTable->model() != 0)
                updater(ui->sprTable->currentIndex(),ui->sprTable->model()->rowCount(),obj);
        }
        // Обработка нажатия Insert
        if (keyEvent->key() == Qt::Key_Insert)
            if(ui->sprTable->model() != 0)
                on_add_usluga_clicked();
    }
}

void frmSpr::updater(QModelIndex item, int count_row, QObject *obj){
    if (obj->objectName() == "sprTable"){
        int col = item.column();
        int row = item.row();

        if (col == 1){
            col= 3;
        }else if (col == 3){
            col = 1;
            row++;
        }else
            col = 1;

        if (row >= count_row || row < 0)
            row = 0;
        item = item.model()->index(row,col);
        ui->sprTable->setCurrentIndex(item);
    }

}

void frmSpr::editFinish(QModelIndex index){
    ui->sprTable->setCurrentIndex(index);
    sprModel->submit();
}
