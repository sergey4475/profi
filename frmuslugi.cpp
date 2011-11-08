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

void frmUslugi::update_tree(){
    ui->treeWidget->clear();
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
    ui->treeWidget->setSortingEnabled(true);
}

void frmUslugi::init()
{
    this->setWindowFlags(Qt::Tool);
    this->setWindowModality(Qt::ApplicationModal);
    update_tree();
    ui->treeWidget->installEventFilter(this);
    ui->tableUslugi->installEventFilter(this);
    ui->tableUslugi->setSortingEnabled(true);
//    connect(ui->tableUslugi->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(on_DataChanged()));
}

void frmUslugi::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    tabl = new PSqlTableModel;
    if (idGR == "0"){
        qDebug() << "1";
        tabl->setTable("USLUGI");
        tabl->setFilter("VID_USLUGI="+idVID);
        tabl->select();
    }else{
        qDebug() << "2";
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
    tabl->setSort(0,Qt::AscendingOrder);
    bEdit = false;
    QObject::connect(tabl,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(editFinish(QModelIndex)));
}

void frmUslugi::editFinish(QModelIndex index){
    ui->tableUslugi->setFocus();
    ui->tableUslugi->setCurrentIndex(index);
    tabl->submitAll();
    bEdit = false;
}

void frmUslugi::on_add_usluga_clicked()
{
    int row = tabl->rowCount();
    tabl->insertRow(row);
    tabl->setData(tabl->index(row,3),idVID.toInt(),Qt::EditRole);
    tabl->setData(tabl->index(row,5),idGR.toInt(),Qt::EditRole);
    tabl->submitAll();
    ui->tableUslugi->edit(tabl->index(row,1));
    ui->tableUslugi->scrollToBottom();
    bEdit = true;
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
    folder->setData(1,Qt::EditRole,0);
    folder->setSelected(true);
    folder->setFlags(folder->flags() | Qt::ItemIsEditable);
    folder->setIcon(0,QIcon(":/res/icons/24.bmp"));
    ui->treeWidget->editItem(folder,0);
}

void frmUslugi::on_edit_gr_clicked()
{
    ui->treeWidget->currentItem()->setFlags(ui->treeWidget->currentItem()->flags() | Qt::ItemIsEditable);
    ui->treeWidget->currentItem()->setSelected(true);
    ui->treeWidget->editItem(ui->treeWidget->currentItem(),0);
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

void frmUslugi::updater(QModelIndex item, int count_row, QObject *obj){
    if (obj->objectName() == "tableUslugi"){
        if (bEdit == false){
            int col = item.column();
            int row = item.row();

            if (col == 1){
                col++;
            }else if (col == 2){
                col--;
                row++;
            }else
                col = 1;

            if (row >= count_row || row < 0)
                row = 0;
            item = item.model()->index(row,col);
            ui->tableUslugi->setCurrentIndex(item);
        }else{
            ui->tableUslugi->setFocus();
            ui->tableUslugi->setCurrentIndex(item);
            tabl->submit();
            bEdit == false;
        }
    }
    if (obj->objectName() == "tableUslugi")
        tabl->submit();

}

void frmUslugi::updater(QTreeWidgetItem *item, int count_row,QObject *obj)
{
    if (obj->objectName() == "treeWidget") {
        if (item->flags().operator &(Qt::ItemIsEditable)){

            if (QMessageBox::question(0,"Внимание!!!","Сохранить изменения?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
                QSqlQuery sql;
                int id_vid = item->parent()->data(1,Qt::EditRole).toInt();
                int id_gr = item->data(1,Qt::EditRole).toInt();

                if (item->data(1,Qt::EditRole)==0){
                    sql.prepare("INSERT INTO GROUP_USLUGI(name,vid_uslugi) VALUES(:Name,:vid_uslugi)");
                }

                if (item->data(1,Qt::EditRole)!=0){
                    sql.prepare("UPDATE GROUP_USLUGI SET name=:Name "
                                "WHERE GROUP_USLUGI.vid_uslugi = :vid_uslugi AND GROUP_USLUGI.ID = :ID ");
                    sql.bindValue(":ID",id_gr);
                }

                sql.bindValue(":Name",item->text(0));
                sql.bindValue(":vid_uslugi",id_vid);

                if (sql.exec()){
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                    update_tree();
                }
            }else
                return;
        }
    }
    if (obj->objectName() == "tableUslugi")
        tabl->submit();
}

bool frmUslugi::eventFilter(QObject *obj, QEvent *event){

    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // Обработка нажатия enter или return
        if (obj->objectName() == "treeWidget") {
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
                updater(ui->treeWidget->currentItem(),ui->treeWidget->currentColumn(),obj);

            if (keyEvent->key() == Qt::Key_Insert)
                on_add_gr_clicked();
        }
        if (obj->objectName() == "tableUslugi") {
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){

                if(ui->tableUslugi->model() != 0)
                    updater(ui->tableUslugi->currentIndex(),ui->tableUslugi->model()->rowCount(),obj);
            }

            if (keyEvent->key() == Qt::Key_Insert)
                if(ui->tableUslugi->model() != 0)
                    on_add_usluga_clicked();
        }
    }

   return QWidget::eventFilter(obj , event);

}
