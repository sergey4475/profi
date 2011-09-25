#include "frmdocument.h"
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

    // --- Документ по складу ---
    if (type_doc == d_oskald){
        if (vid_form == f_document){ // -- Тип формы документ
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
        if (vid_form == f_ostatki){ // -- Тип формы остатки
            ui->Number->setDisabled(true);
            ui->groupBox->setDisabled(true);
            ui->ApplyBut->setDisabled(true);
            GetOstaok();
        }
    }

    ui->tableView->setColumnWidth(2,280);

    ui->tableView->setItemDelegateForColumn(0,del);
    ui->tableView->setItemDelegateForColumn(1,del);
    ui->tableView->setItemDelegateForColumn(2,del);

}
