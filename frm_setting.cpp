#include "QtSql"
#include "QSettings"
#include "frm_setting.h"
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
    QSettings settings("conf.ini",QSettings::IniFormat);
    ui->hostname->setText(settings.value("HostName").toString());
    ui->NameDataBase->setText(settings.value("DataBase").toString());
    ui->loginDB->setText(settings.value("Login").toString());
    ui->passwordDB->setText(settings.value("Password").toString());

    QSqlDatabase db;
    int count = db.drivers().count();

    for (int i=0;i<count;++i){
        ui->DriversDB->addItem(db.drivers().value(i));
    }
    db.~QSqlDatabase();

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
    settings.setValue("HostName",ui->hostname->text());
    settings.setValue("DataBase",ui->NameDataBase->text());
    settings.setValue("Login",ui->loginDB->text());
    settings.setValue("Password",ui->passwordDB->text());

}
