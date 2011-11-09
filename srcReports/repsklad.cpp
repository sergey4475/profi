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
    otch_ = type_otch;
}

void repSklad::on_start_report_clicked()
{
    QString date1 = ui->date_start->date().toString("dd.MM.yyyy");
    QString date2 = ui->date_end->date().toString("dd.MM.yyyy");
    if (date1 != "" && date2 != ""){
    QString sql = QString("SELECT "
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
    printOstatok(sql);
    }
}
