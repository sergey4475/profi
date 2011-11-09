#include "repsklad.h"
#include "ui_repsklad.h"

repSklad::repSklad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::repSklad)
{
    ui->setupUi(this);
}

repSklad::~repSklad()
{
    delete ui;
}

void repSklad::init(int type_otch){
    this->setWindowModality(Qt::ApplicationModal);
    otch_ = type_otch;
    QSqlQuery sql;
    QDate date_temp = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    ui->date_start->setDateCalendar(date_temp);
    ui->date_end->setDateCalendar(QDate::currentDate());
    // ����� �����
    ui->otdel_skalda->addItem("���...",0);
    if (otch_ == O_OST_O_SKLAD){
        //************ ��������� ������ ������
        sql.prepare("SELECT ID, Name "
                    "FROM group_o_sklad "
                    "ORDER BY ID");
        sql.exec();
        while (sql.next()){
            ui->otdel_skalda->addItem(sql.value(1).toString(),sql.value(0).toInt());
        }
        ui->lOtdel_Sklada->setText("����� ������:");
    }
    // ������� �������
    if (otch_ == O_OST_SKLAD){
        // ��������� ������ ������ ���������
        sql.prepare("SELECT vidi_zatrat.id, vidi_zatrat.Name "
                    "FROM vidi_zatrat "
                    "ORDER BY vidi_zatrat.id");
        sql.exec();
        while (sql.next()){
            ui->otdel_skalda->addItem(sql.value(1).toString(),sql.value(0).toInt());
        }

        ui->lOtdel_Sklada->setText("������� �������:");
    }

}

void repSklad::on_start_report_clicked()
{
    QString date1 = ui->date_start->date().toString("dd.MM.yyyy");
    QString date2 = ui->date_end->date().toString("dd.MM.yyyy");
    int id_otdel  = ui->otdel_skalda->itemData(ui->otdel_skalda->currentIndex()).toInt();
    QString name_otdel= ui->otdel_skalda->currentText();
    QString sql   = "";
    if (date1 != "" && date2 != ""){
        if (otch_ == O_OST_O_SKLAD) // ���� ����� �����
        if (id_otdel == 0){
            sql = QString("SELECT "
                "   materials.name AS material, "
                "   SUM(o_sklad.count) AS count, "
                "   ed_izm.name AS ed_izm "
                "FROM "
                "   public.o_sklad, "
                "   public.materials, "
                "   public.ed_izm "
                "WHERE "
                "   o_sklad.id_material = materials.id "
                "   AND materials.id_ed_izm = ed_izm.id "
                "   AND o_sklad.date BETWEEN '%1' AND '%2' "
                "GROUP BY "
                "   materials.name, "
                "   ed_izm.name").arg(date1,date2);
        }else{
            sql = QString("SELECT "
                "   materials.name AS material, "
                "   SUM(o_sklad.count) AS count, "
                "   ed_izm.name AS ed_izm "
                "FROM "
                "   public.o_sklad, "
                "   public.materials, "
                "   public.ed_izm "
                "WHERE "
                "   o_sklad.id_material = materials.id "
                "   AND o_sklad.id_group_o_sklad = %3"
                "   AND materials.id_ed_izm = ed_izm.id "
                "   AND o_sklad.date BETWEEN '%1' AND '%2' "
                "GROUP BY "
                "   materials.name, "
                "   ed_izm.name").arg(date1,date2,QString("%1").arg(id_otdel));
        }

        if (otch_ == O_OST_SKLAD) // ���� ����� �����
        if (id_otdel == 0){
            sql = QString("SELECT "
                "   materials.name AS material, "
                "   SUM(sklad.count) AS count, "
                "   ed_izm.name AS ed_izm "
                "FROM "
                "   public.sklad, "
                "   public.materials, "
                "   public.ed_izm "
                "WHERE "
                "   sklad.id_material = materials.id "
                "   AND materials.id_ed_izm = ed_izm.id "
                "   AND sklad.date BETWEEN '%1' AND '%2' "
                "GROUP BY "
                "   materials.name, "
                "   ed_izm.name").arg(date1,date2);
        }else{
            sql = QString("SELECT "
                "   materials.name AS material, "
                "   SUM(sklad.count) AS count, "
                "   ed_izm.name AS ed_izm "
                "FROM "
                "   public.sklad, "
                "   public.materials, "
                "   public.ed_izm "
                "WHERE "
                "   sklad.id_material = materials.id "
                "   AND sklad.id_vid_zatrat = %3"
                "   AND materials.id_ed_izm = ed_izm.id "
                "   AND sklad.date BETWEEN '%1' AND '%2' "
                "GROUP BY "
                "   materials.name, "
                "   ed_izm.name").arg(date1,date2,QString("%1").arg(id_otdel));
        }
    printOstatok(sql,QDate::currentDate(),name_otdel);
    }
}
