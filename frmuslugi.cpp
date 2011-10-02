#include "frmuslugi.h"
#include "params.h"
#include "ui_frmuslugi.h"

frmUslugi::frmUslugi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmUslugi)
{
    ui->setupUi(this);
}

frmUslugi::~frmUslugi()
{
    delete ui;
}

void frmUslugi::init()
{
    this->setWindowFlags(Qt::Tool);
    QSqlQuery *model = new QSqlQuery;
    model->exec("SELECT VID_USLUG.NAME AS VID, "
                "       VID_USLUG.ID AS ID_VID, "
                "       GROUP_USLUGI.NAME AS GR, "
                "       GROUP_USLUGI.ID AS ID_GR "
                "FROM VID_USLUG LEFT JOIN GROUP_USLUGI ON "
                "     VID_USLUG.ID = GROUP_USLUGI.VID_USLUGI");

    QString vid_uslugi = "", group ="";
    int id_vid_uslugi, id_group = 0;
    QSqlRecord record = model->record();

    while (model->next()){

        group = model->value(record.indexOf("GR")).toString();
        id_group = model->value(record.indexOf("ID_GR")).toInt();
        vid_uslugi = model->value(record.indexOf("VID")).toString();

        if (id_vid_uslugi != model->value(record.indexOf("ID_VID")).toInt()){
           root = new QTreeWidgetItem(ui->treeWidget);
           root->setText(0,vid_uslugi);
           root->setData(1,Qt::EditRole,model->value(record.indexOf("ID_VID")).toInt());
           root->setIcon(0,QIcon(":/res/icons/21.ico"));
        }

        if (group != ""){
            folder = new QTreeWidgetItem(root);
            folder->setText(0,group);
            folder->setData(1,Qt::EditRole,id_group);
            folder->setIcon(0,QIcon(":/res/icons/24.bmp"));
        }


        id_vid_uslugi = model->value(record.indexOf("ID_VID")).toInt();
    }
    ui->treeWidget->installEventFilter(this);
}

void frmUslugi::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    tabl = new PSqlTableModel;
    if (idGR == "0"){
        tabl->setTable("USLUGI");
        tabl->setFilter("VID_USLUGI="+idVID);
        tabl->select();
    }else{
        tabl->setTable("USLUGI");
        tabl->setFilter("VID_USLUGI="+idVID);
        tabl->setFilter("PARENT="+idGR);
        tabl->select();
    }
    ui->tableUslugi->setModel(tabl);
    ui->tableUslugi->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tabl->setHeaderData(1,Qt::Horizontal,tr("Наименование"));
    tabl->setHeaderData(2,Qt::Horizontal,tr("Цена"));
    ui->tableUslugi->setColumnHidden(0,true);
    ui->tableUslugi->setColumnHidden(3,true);
    ui->tableUslugi->setColumnHidden(4,true);
    ui->tableUslugi->setColumnHidden(5,true);
}

void frmUslugi::on_add_usluga_clicked()
{
    int row = tabl->rowCount();
    tabl->insertRow(row);
    tabl->setData(tabl->index(row,3),idVID.toInt(),Qt::EditRole);
    tabl->setData(tabl->index(row,5),idGR.toInt(),Qt::EditRole);
    tabl->submitAll();

}

void frmUslugi::on_del_usluga_clicked()
{
    if(QMessageBox::question(0,"Вопрос","Вы уверены что желаете удалить данную услугу",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
        int row = ui->tableUslugi->currentIndex().row();
        tabl->setData(tabl->index(row,4),1,Qt::EditRole);
        tabl->submitAll();
    }
}


void frmUslugi::on_add_gr_clicked()
{
    folder = new QTreeWidgetItem(currentItem);
    ui->treeWidget->currentItem()->setExpanded(true);
    folder->setText(0,"group");
    folder->setSelected(true);
    folder->setFlags(folder->flags() | Qt::ItemIsEditable);
    folder->setIcon(0,QIcon(":/res/icons/24.bmp"));
}
void frmUslugi::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    idVID = "0";
    idGR  = "0";

    if (item->parent() == 0x0) {
        if (! item->flags().operator &(Qt::ItemIsEditable)){
            idVID = item->data(1,Qt::EditRole).toString();
            currentItem = item;
        }
    }else{
        if (! item->flags().operator &(Qt::ItemIsEditable)){
            idVID = item->parent()->data(1,Qt::EditRole).toString();
            idGR  = item->data(1,Qt::EditRole).toString();
            currentItem = item->parent();
        }
    }
}


void frmUslugi::updater(QTreeWidgetItem *item, int column)
{
    qDebug() << idVID.toInt();
    if (item->flags().operator &(Qt::ItemIsEditable)){

        if (QMessageBox::question(0,"Внимание!!!","Сохранить изменения?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
            QSqlQuery sql;
            sql.prepare("INSERT INTO GROUP_USLUGI(name,vid_uslugi) VALUES(:Name,:vid_uslugi)");
            sql.bindValue(":Name",item->text(0));
            sql.bindValue(":vid_uslugi",idVID.toInt());
            if (sql.exec())
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }else
            return;
    }

}

bool frmUslugi::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::KeyPress){
        //16777220
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug() << keyEvent->key();
        qDebug() << Qt::Key_Return;
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            updater(ui->treeWidget->currentItem(),ui->treeWidget->currentColumn());
    }

   return QWidget::eventFilter(obj , event);

}
