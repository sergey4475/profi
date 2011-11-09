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
#include "srcReports/repsklad.h"
MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    db = ConnectDB(g_hostname,g_dataBase,g_login,g_password,g_driverName,g_connect_port);
//    CreateDb(db);
    UpdateClients(0);

}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_butClients_clicked()
{
    frmClients *frame = new frmClients;
    frame->initForm(w_ID,0);
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
    ui->butMagazin->setEnabled(true);
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
    ui->butMagazin->setEnabled(true);
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
    ui->butMagazin->setEnabled(true);
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
    ui->butMagazin->setEnabled(true);
}
//*************** Услуги Магазин
void MainForm::on_butMagazin_clicked()
{
    UpdateClients(0);
    frm_okazanie_uslug *frm_uslugi = new frm_okazanie_uslug;
    frm_uslugi->InitForm(n_USL_MAG,w_ID);
    frm_uslugi->setParent(ui->frame);
    frm_uslugi->frm = this;
    frm_uslugi->show();
    w_ID = frm_uslugi->winId();
    ui->butClients->setEnabled(true);
    ui->butMonMaster->setEnabled(true);
    ui->butKosmitolog->setEnabled(true);
    ui->butUhodzaTelom->setEnabled(true);
    ui->butStilist->setEnabled(true);
    ui->butMagazin->setEnabled(false);

}

//*************** Обновление информации по клиенту
void MainForm::UpdateClients(int IDClient){

    ID_Client = IDClient;
    if (IDClient == 0)
        ui->but_schet->setDisabled(true);
    else
        ui->but_schet->setEnabled(true);

    ui->but_vnesti_dolg->setVisible(false);
    ui->dolg_client->setVisible(false);
    ui->l_dolg->setVisible(false);
    ui->dolg_client->setText("");

    ui->na_schetu->setText("0");
    ui->summa_uslug->setText("0");
    ui->count_client->setText("");
    QSqlQuery sql;
    sql.prepare("SELECT SUM(scheta_clients.summa) AS Summa "
                "FROM scheta_clients "
                "WHERE scheta_clients.ID_Client = :ID_Client");

    sql.bindValue(":ID_Client",IDClient);
    sql.exec();
    while (sql.next()){
        ui->na_schetu->setText(sql.value(0).toString());
    }

    sql.prepare("SELECT COUNT(DISTINCT number) "
                "FROM clients_history "
                "WHERE clients_history.ID_Client = :ID_Client");
    sql.bindValue(":ID_Client",IDClient);
    sql.exec();
    while (sql.next()){
        ui->count_client->setText(sql.value(0).toString());
    }

    sql.prepare("SELECT SUM(oplati_clients.summa_k_opl - oplati_clients.summa_opl) AS dolg  "
                "FROM oplati_clients "
                "WHERE oplati_clients.ID_Client = :ID_Client");
    sql.bindValue(":ID_Client",IDClient);
    sql.exec();
    sql.next();
    double summa_dolg = sql.value(0).toDouble();

    if (summa_dolg != 0){
        ui->but_vnesti_dolg->setVisible(true);
        ui->dolg_client->setVisible(true);
        ui->l_dolg->setVisible(true);
        ui->dolg_client->setText(QString("%1").arg(summa_dolg));
    }


    ui->summa_uslug->setText("");
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
    model->insertColumns(0,7);
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Дата");
    model->setHeaderData(2,Qt::Horizontal,"Материал");
    model->setHeaderData(3,Qt::Horizontal,"Кол-во");
    model->setHeaderData(4,Qt::Horizontal,"type_operacii");
    model->setHeaderData(5,Qt::Horizontal,"NUMBER");
    model->setHeaderData(6,Qt::Horizontal,"Ед.изм");

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
    model->setHeaderData(3,Qt::Horizontal,"Ед.изм");

    frm->initForm(model,f_ostatki,d_oskald);
    frm->show();

}

// - Форма распределение материала на складе
void MainForm::on_raspredel_triggered()
{
    frmDocument *frm = new frmDocument;

    PStandardItemModel *model = new PStandardItemModel;
    model->insertColumns(0,8);
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Дата");
    model->setHeaderData(2,Qt::Horizontal,"Материал");
    model->setHeaderData(3,Qt::Horizontal,"Кол-во");
    model->setHeaderData(4,Qt::Horizontal,"type_operacii");
    model->setHeaderData(5,Qt::Horizontal,"vid_zatrat");
    model->setHeaderData(6,Qt::Horizontal,"Ед.изм");

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

void MainForm::on_otc_ostatok_triggered()
{
    repSklad *frmRepSklad = new repSklad;
    frmRepSklad->show();
}

void MainForm::on_but_vnesti_dolg_clicked()
{
    QSqlQuery sql;
    sql.prepare("SELECT MAX(OPLATI_CLIENTS) AS number "
                "FROM OPLATI_CLIENTS");
    sql.exec();
    QSqlRecord record = sql.record();
    sql.next();

    int Number = sql.value(record.indexOf("number")).toInt();

    Number++;


    double summ = ui->dolg_client->text().toDouble();
    double summa_Oplati = QInputDialog::getDouble(this,"Введите сумму ","Сумма:",summ,0,99999999,2,0,0);

    sql.prepare("INSERT INTO OPLATI_CLIENTS(NUMBER,DATE,TYPE_OPERACII,ID_CLIENT,SUMMA_K_OPL,SUMMA_OPL) "
                "VALUES(:NUMBER,:DATE,:TYPE_OPERACII,:ID_CLIENT,:SUMMA_K_OPL,:SUMMA_OPL) ");
    sql.bindValue(":NUMBER",Number);
    sql.bindValue(":DATE",QDate::currentDate().toString("dd.MM.yyyy"));
    sql.bindValue(":TYPE_OPERACII",n_PL_DOLG);
    sql.bindValue(":ID_CLIENT",ID_Client);
    sql.bindValue(":SUMMA_K_OPL",0);
    sql.bindValue(":SUMMA_OPL",summa_Oplati);

    sql.exec();

    UpdateClients(ID_Client);
}
