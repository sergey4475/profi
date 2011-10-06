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
    QSqlg_dataBase db;
    int count = db.drivers().count();

    for (int i=0;i<count;++i){
        ui->DriversDB->addItem(db.drivers().value(i));
    }
    db.~QSqlg_dataBase();

//    QSettings settings("conf.ini",QSettings::IniFormat);
//    ui->g_hostname->setText(settings.value("g_hostname").toString());
//    ui->Nameg_dataBase->setText(settings.value("g_dataBase").toString());
//    ui->g_loginDB->setText(settings.value("g_login").toString());
//    ui->g_passwordDB->setText(settings.value("g_password").toString());
//    ui->g_connect_port->setText(settings.value("g_connect_port").toString());
//    ui->DriversDB->setCurrentIndex(ui->DriversDB->findText(settings.value("g_driverName").toString()));
//    ui->spisanie_so_cheta->setChecked(settings.value("spisanie_so_cheta",true).toBool());
    ui->g_hostname->setText(g_hostname);
}

void frm_setting::on_testBUt_clicked()
{
    QSqlg_dataBase db = QSqlg_dataBase::addg_dataBase(ui->DriversDB->currentText());
    db.close();
    db.setg_hostname(ui->g_hostname->text());
    db.setg_dataBaseName(ui->Nameg_dataBase->text());
    db.setUserName(ui->g_loginDB->text());
    db.setg_password(ui->g_passwordDB->text());
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
    settings.setValue("g_hostname",ui->g_hostname->text());
    settings.setValue("g_dataBase",ui->Nameg_dataBase->text());
    settings.setValue("g_login",ui->g_loginDB->text());
    settings.setValue("g_password",ui->g_passwordDB->text());
    settings.setValue("g_connect_port",ui->g_connect_port->text().toInt());
    settings.setValue("g_driverName",ui->DriversDB->currentText());
    settings.setValue("spisanie_so_cheta",ui->spisanie_so_cheta->checkState());
}
