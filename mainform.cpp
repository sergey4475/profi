#include "mainform.h"
#include "sqlmodules.h"
#include <QDebug>
#include "frmclients.h"
#include "params.h"
#include "frmuslugi.h"
#include "frm_okazanie_uslug.h"
#include "frmsotr.h"
#include "frmspr.h"
#include "frmschetclienta.h"
#include "frmdocument.h"
#include "frm_setting.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    QSettings settings("conf.ini",QSettings::IniFormat);
    hostname    = settings.value("HostName").toString();
    DataBase    = settings.value("DataBase").toString();
    login       = settings.value("Login").toString();
    Password    = settings.value("Password").toString();
    DriverName  = settings.value("DriverName").toString();
    Port        = settings.value("Port",5432).toInt();

    db = ConnectDB(hostname,DataBase,login,Password,DriverName,Port);
//    CreateDb(db);

}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_butClients_clicked()
{
    frmClients *frame = new frmClients;
    frame->initForm(w_ID);
    frame->setParent(ui->frame);
    frame->frm = this;
    frame->show();
    w_ID = frame->winId();
    ui->butClients->setEnabled(false);
    ui->butMonMaster->setEnabled(true);
    ui->butKosmitolog->setEnabled(true);
    ui->butUhodzaTelom->setEnabled(true);
    ui->butStilist->setEnabled(true);
}

void MainForm::on_m_sprsotr_triggered()
{
    frmSotr *fSotr = new frmSotr;
    fSotr->InitForm();
    fSotr->show();
}

void MainForm::on_m_spruslugi_triggered()
{
    frmUslugi *frmUsl = new frmUslugi;
    frmUsl->init();
    frmUsl->show();
}

void MainForm::SetSumma(double summa){
    ui->summa_uslug->setText(QVariant(summa).toString());
}

void MainForm::EnableButton(int numButton){
    switch(numButton) {
    case 0:
        ui->butClients->setEnabled(true);
        break;
    case 1:
        ui->butMonMaster->setEnabled(true);
        break;
    case 2:
        ui->butKosmitolog->setEnabled(true);
        break;
    case 3:
        ui->butUhodzaTelom->setEnabled(true);
        break;
    case 4:
        ui->butStilist->setEnabled(true);
        break;
    case 5:
        ui->butMagazin->setEnabled(true);
        break;
    };
}

//*************** Услуги Маникюрного мастера
void MainForm::on_butMonMaster_clicked()
{
    UpdateClients(0);
    frm_okazanie_uslug *frm_uslugi = new frm_okazanie_uslug;
    frm_uslugi->InitForm(n_USL_MAN,w_ID);
    frm_uslugi->setParent(ui->frame);
    frm_uslugi->frm = this;
    frm_uslugi->show();
    w_ID = frm_uslugi->winId();
    ui->butClients->setEnabled(true);
    ui->butMonMaster->setEnabled(false);
    ui->butKosmitolog->setEnabled(true);
    ui->butUhodzaTelom->setEnabled(true);
    ui->butStilist->setEnabled(true);
}
//*************** Услуги Космитолога
void MainForm::on_butKosmitolog_clicked()
{
    UpdateClients(0);
    frm_okazanie_uslug *frm_uslugi = new frm_okazanie_uslug;
    frm_uslugi->InitForm(n_USL_KOS,w_ID);
    frm_uslugi->setParent(ui->frame);
    frm_uslugi->frm = this;
    frm_uslugi->show();
    w_ID = frm_uslugi->winId();
    ui->butClients->setEnabled(true);
    ui->butMonMaster->setEnabled(true);
    ui->butKosmitolog->setEnabled(false);
    ui->butUhodzaTelom->setEnabled(true);
    ui->butStilist->setEnabled(true);
}
//*************** Услуги По уходу за телом
void MainForm::on_butUhodzaTelom_clicked()
{
    UpdateClients(0);
    frm_okazanie_uslug *frm_uslugi = new frm_okazanie_uslug;
    frm_uslugi->InitForm(n_USL_UHOD,w_ID);
    frm_uslugi->setParent(ui->frame);
    frm_uslugi->frm = this;
    frm_uslugi->show();
    w_ID = frm_uslugi->winId();
    ui->butClients->setEnabled(true);
    ui->butMonMaster->setEnabled(true);
    ui->butKosmitolog->setEnabled(true);
    ui->butUhodzaTelom->setEnabled(false);
    ui->butStilist->setEnabled(true);
}
//*************** Услуги Стилиста
void MainForm::on_butStilist_clicked()
{
    UpdateClients(0);
    frm_okazanie_uslug *frm_uslugi = new frm_okazanie_uslug;
    frm_uslugi->InitForm(n_USL_STIL,w_ID);
    frm_uslugi->setParent(ui->frame);
    frm_uslugi->frm = this;
    frm_uslugi->show();
    w_ID = frm_uslugi->winId();
    ui->butClients->setEnabled(true);
    ui->butMonMaster->setEnabled(true);
    ui->butKosmitolog->setEnabled(true);
    ui->butUhodzaTelom->setEnabled(true);
    ui->butStilist->setEnabled(false);
}
//*************** Обновление информации по клиенту
void MainForm::UpdateClients(int IDClient){
    ID_Client = IDClient;
    if (IDClient == 0)
        ui->but_schet->setDisabled(true);
    else
        ui->but_schet->setEnabled(true);

    ui->na_schetu->setText("0");
    QSqlQuery sql;
    sql.prepare("SELECT SUM(scheta_clients.summa) AS Summa "
                "FROM scheta_clients "
                "WHERE scheta_clients.ID_Client = :ID_Client");

    sql.bindValue(":ID_Client",IDClient);
    sql.exec();
    while (sql.next()){
        ui->na_schetu->setText(sql.value(0).toString());
    }
}

//****************** Обработка формы счета клиента
void MainForm::on_but_schet_clicked()
{
    frmSchetClienta *frmSchet = new frmSchetClienta;
    frmSchet->initForm(ID_Client);
    frmSchet->show();
}

// - Справочник Материалов
void MainForm::on_m_spr_triggered()
{
    QList<QAbstractItemDelegate*> lst;

    PSqlTableModel *table = new PSqlTableModel;
    table->setTable("MATERIALS");
    table->setRelation(3,QSqlRelation("ed_izm","id","name"));
    table->select();
    table->setHeaderData(0,Qt::Horizontal,QObject::tr("Код"));
    table->setHeaderData(1,Qt::Horizontal,QObject::tr("Наименование"));
    table->setHeaderData(2,Qt::Horizontal,QObject::tr("DEL"));
    table->setHeaderData(3,Qt::Horizontal,QObject::tr("Ед. Изм"));

    lst.insert(0,new NotEditableDelegate);
    lst.insert(1,0);
    lst.insert(2,0);

    QSqlQuery sql;
    sql.exec("SELECT ed_izm.ID, ed_izm.name FROM ed_izm");
    QList< QPair<QString,QString> > aValues;
    while (sql.next()){
        aValues.append(qMakePair(sql.record().value(1).toString(),sql.record().value(0).toString()));
    }

    BoxDelegate *box = new BoxDelegate(aValues);
    lst.insert(3,box);

    frmSpr *fSpr = new frmSpr;
    fSpr->init(table,lst);
    fSpr->setWindowTitle("Справочник Материалов");
    fSpr->show();
}

// - Добавление материала на склад
void MainForm::on_add_sklad_triggered()
{
    frmDocument *frm = new frmDocument;
    PStandardItemModel *model = new PStandardItemModel;
    model->insertColumns(0,6);
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Дата");
    model->setHeaderData(2,Qt::Horizontal,"Материал");
    model->setHeaderData(3,Qt::Horizontal,"Количество");
    model->setHeaderData(4,Qt::Horizontal,"type_operacii");
    model->setHeaderData(5,Qt::Horizontal,"NUMBER");

    frm->initForm(model,f_document,d_oskald);
    frm->show();
}

// - Форма получения остатков на складе
void MainForm::on_oststok_na_sklade_triggered()
{
    frmDocument *frm = new frmDocument;
    PStandardItemModel *model = new PStandardItemModel;
    model->insertColumns(0,2);
    model->setHeaderData(0,Qt::Horizontal,"Материал");
    model->setHeaderData(1,Qt::Horizontal,"Количество");

    frm->initForm(model,f_ostatki,d_oskald);
    frm->show();

}

// - Форма распределение материала на складе
void MainForm::on_raspredel_triggered()
{
    frmDocument *frm = new frmDocument;

    PStandardItemModel *model = new PStandardItemModel;
    model->insertColumns(0,6);
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Дата");
    model->setHeaderData(2,Qt::Horizontal,"Материал");
    model->setHeaderData(3,Qt::Horizontal,"Количество");
    model->setHeaderData(4,Qt::Horizontal,"type_operacii");
    model->setHeaderData(5,Qt::Horizontal,"vid_zatrat");

    frm->initForm(model,f_document,d_raspred);
    frm->show();
}

void MainForm::on_settings_triggered()
{
    frm_setting *frm = new frm_setting;
    frm->InitForm();
    frm->setWindowFlags(Qt::Tool);
    frm->setWindowModality(Qt::WindowModal);
    frm->show();
}
