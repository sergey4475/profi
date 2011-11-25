#include "frmkassa.h"
#include "ui_frmkassa.h"

frmKassa::frmKassa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmKassa)
{
    ui->setupUi(this);
}

frmKassa::~frmKassa()
{
    delete ui;
}

void frmKassa::init(){
    ui->Date->setDateCalendar(QDate::currentDate());
}

void frmKassa::on_pushButton_clicked()
{
    if (ui->vid_zatrat->text().length()!= 0) {
        QSqlDatabase::database().transaction();
        QSqlQuery sql;
        sql.prepare("INSERT INTO KASSA(NUMBER, DATE, type_operacii, summa, vid_zatrat) "
                    "VALUES(:NUMBER, :DATE, :type_operacii, :SUMMA, :vid_zatrat) ");
        sql.bindValue(":NUMBER",0);
        sql.bindValue(":DATE",ui->Date->date().toString("dd.MM.yyyy"));
        sql.bindValue(":TYPE_OPERACII",n_KASSA_RASHOD);
        sql.bindValue(":SUMMA",ui->summa->value());
        sql.bindValue(":vid_zatrat",ui->vid_zatrat->text());
        sql.exec();
        if (sql.lastError().isValid()){
            QSqlDatabase::database().rollback();
            return;
        }
        QSqlDatabase::database().commit();
        this->close();
    }else
        QMessageBox::warning(0,"Внимание!!!","Поле примечание не может быть пустым",QMessageBox::Ok);
}
