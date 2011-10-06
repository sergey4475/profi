#include "QtSql"
#include "frm_setting.h"
#include "params.h"
#include "QMessageBox"
#include "ui_frm_setting.h"

frm_setting::frm_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frm_setting)
{
    ui->setupUi(this);
}

frm_setting::~frm_setting()
{
    delete ui;
}

void frm_setting::InitForm(){
    QSqlDatabase db;
    int count = db.drivers().count();

    for (int i=0;i<count;++i){
        ui->DriversDB->addItem(db.drivers().value(i));
    }
    db.~QSqlDatabase();

    ui->hostname->setText(g_hostname);
}

void frm_setting::on_testBUt_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(ui->DriversDB->currentText());
    db.close();
    db.setHostName(ui->hostname->text());
    db.setDatabaseName(ui->NameDataBase->text());
    db.setUserName(ui->loginDB->text());
    db.setPassword(ui->passwordDB->text());
    if (! db.open()){
         QMessageBox::critical(0,"Ошибка подключения",db.lastError().text(),QMessageBox::Ok);
         db.close();
    }else{
        QMessageBox::warning(0,"Подключени прошло успешно","Подключение успешно!!!",QMessageBox::Ok);
        db.close();
    }

}

void frm_setting::on_pushButton_clicked()
{
    QSettings settings("conf.ini",QSettings::IniFormat);
    settings.setValue("g_hostname",ui->hostname->text());
    settings.setValue("g_dataBase",ui->NameDataBase->text());
    settings.setValue("g_login",ui->loginDB->text());
    settings.setValue("g_password",ui->passwordDB->text());
    settings.setValue("g_connect_port",ui->Port->text().toInt());
    settings.setValue("g_driverName",ui->DriversDB->currentText());
    settings.setValue("spisanie_so_cheta",ui->spisanie_so_cheta->checkState());
}
