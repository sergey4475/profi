#include "frmschetclienta.h"
#include "params.h"
#include "ui_frmschetclienta.h"

frmSchetClienta::frmSchetClienta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmSchetClienta)
{
    ui->setupUi(this);
}

frmSchetClienta::~frmSchetClienta()
{
    delete ui;
}

void frmSchetClienta::initForm(int IDClient){

    ID_client = IDClient;
    this->setWindowFlags(Qt::Tool);
    ui->naschete->setText("0");
    QSqlQuery sql;

    double ost = GetOstatokNaSchete(IDClient,QDate::currentDate().toString("dd.MM.yyyy"));
    ui->naschete->setText(QString("%1").arg(ost));

    sql.prepare("SELECT Clients.FIO "
                "FROM Clients "
                "WHERE Clients.ID = :IDClient");
    sql.bindValue(":IDClient",IDClient);
    sql.exec();

    while (sql.next()){
        ui->l_client->setText(sql.value(0).toString());
    }
}

void frmSchetClienta::on_add_summa_clicked()
{
    QDate date_ = QDate::currentDate();
    EditChetClienta(ID_client,N_CH_PRIHOD,ui->summa->value(),date_.toString("dd.MM.yyyy"));
    initForm(ID_client);
}

void frmSchetClienta::on_del_summa_clicked()
{
    QDate date_ = QDate::currentDate();
    EditChetClienta(ID_client,N_CH_SPISAN,ui->summa->value(),date_.toString("dd.MM.yyyy"));
    initForm(ID_client);
}
