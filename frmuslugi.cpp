#include "frmuslugi.h"
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
                        "VID_USLUG.ID AS ID_VID, "
                         "GROUP_USLUGI.NAME AS GR "
                    "FROM VID_USLUG LEFT JOIN GROUP_USLUGI ON "
                    " VID_USLUG.ID = GROUP_USLUGI.VID_USLUGI");

    QTreeWidgetItem *root, *folder;
    QString vid_uslugi = "", group ="";
    QSqlRecord record = model->record();

    while (model->next()){
        group = model->value(record.indexOf("GR")).toString();
        if (vid_uslugi != model->value(record.indexOf("VID")).toString()){
           root = new QTreeWidgetItem(ui->treeWidget);
           root->setText(0,model->value(record.indexOf("VID")).toString());
           root->setIcon(0,QIcon(":/res/icons/21.ico"));
        }

        if (group != ""){
            folder = new QTreeWidgetItem(root);
            folder->setText(0,group);
            folder->setIcon(0,QIcon(":/res/icons/24.bmp"));
        }

        vid_uslugi = model->value(record.indexOf("VID")).toString();
    }
}

void frmUslugi::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    tabl = new uslSqlTableModel;
    if (item->parent() == 0x0) {
        QSqlQuery sql;
        sql.prepare("SELECT VID_USLUG.ID FROM VID_USLUG "
                    "WHERE VID_USLUG.NAME = :VID ");
        sql.bindValue(":VID",item->text(0));
        sql.exec();
        QSqlRecord record = sql.record();
        sql.next();

        idVID = sql.value(record.indexOf("ID")).toString();

        tabl->setTable("USLUGI");
        tabl->setFilter("VID_USLUGI="+idVID);
        tabl->select();
    }else {
        QSqlQuery sql;
        sql.prepare("SELECT VID_USLUG.ID AS VID, GROUP_USLUGI.ID AS GR "
                    "FROM VID_USLUG INNER JOIN GROUP_USLUGI "
                        "ON VID_USLUG.ID = GROUP_USLUGI.VID_USLUGI "
                    "WHERE VID_USLUG.NAME = :VID AND GROUP_USLUGI.NAME = :GR ");
        sql.bindValue(":VID",item->parent()->text(0));
        sql.bindValue(":GR",item->text(0));
        sql.exec();
        QSqlRecord record = sql.record();
        sql.next();

        idVID = sql.value(record.indexOf("VID")).toString();
        idGR  = sql.value(record.indexOf("GR")).toString();

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
