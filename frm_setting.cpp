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
    ui->NameDataBase->setText(g_dataBase);
    ui->loginDB->setText(g_login);
    ui->passwordDB->setText(g_password);
    ui->Port->setText(QString("%1").arg(g_connect_port));
    ui->DriversDB->setCurrentIndex(ui->DriversDB->findText(g_driverName));
    ui->spisanie_so_cheta->setChecked(g_spisanie_so_scheta);
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
    settings.setValue("Port",ui->Port->text().toInt());
    settings.setValue("DriverName",ui->DriversDB->currentText());
    settings.setValue("spisanie_so_scheta",ui->spisanie_so_cheta->isChecked());

    static QSettings set("conf.ini",QSettings::IniFormat);
    g_hostname          = ui->hostname->text();
    g_dataBase          = ui->NameDataBase->text();
    g_login             = ui->loginDB->text();
    g_password          = ui->passwordDB->text();
    g_driverName        = ui->DriversDB->currentText();
    g_connect_port      = ui->Port->text().toInt();
    g_spisanie_so_scheta= ui->spisanie_so_cheta->isChecked();

}
