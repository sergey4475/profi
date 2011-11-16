#include "repkassa.h"
#include "ui_repkassa.h"

repkassa::repkassa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::repkassa)
{
    ui->setupUi(this);
}

repkassa::~repkassa()
{
    delete ui;
}

void repkassa::init(){
    ID_sotr = 0;
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("");
    QDate date_temp = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);

    sotSqlQueryModel *smodel = new sotSqlQueryModel;
    QSqlQuery query;
    ui->date_start->setDateCalendar(date_temp);
    ui->date_end->setDateCalendar(QDate::currentDate());
    query.prepare("SELECT Personal.FIO, "
                  "DOLJNOSTI.NAME, "
                  "Personal.Nom_Tel, "
                  "Personal.DEL, "
                  "Personal.POL, "
                  "Personal.ID, "
                  "DOLJNOSTI.ID AS Doljn "
                  "FROM Personal INNER JOIN DOLJNOSTI ON Personal.DOLJN = DOLJNOSTI.ID ");

    query.exec();
    smodel->setQuery(query);
    smodel->setHeaderData(0,Qt::Horizontal,"ÔÈÎ");

    QCompleter *scompleter = new QCompleter(this);
    scompleter->setModel(smodel);
    scompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Sotrudnik->setCompleter(scompleter);

    ui->Sotrudnik->setVisible(false);
    ui->label_4->setVisible(false);
}


void repkassa::on_start_report_clicked()
{
    QString date1 = ui->date_start->date().toString("dd.MM.yyyy");
    QString date2 = ui->date_end->date().toString("dd.MM.yyyy");


    NCReport *report = new NCReport();
    report->setReportFile(QDir::currentPath()+"/reports/uslugi_po_masteram.xml");
//    report->addParameter("sql",sql);
    report->addParameter("date1",date1);
    report->addParameter("date2",date2);
//    report->addParameter("OtdelSklada",otdel_sklada);
    NCReportOutput *output=0;
    output = new NCReportPreviewOutput();
    output->setAutoDelete( FALSE );
    report->setOutput(output);
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    report->runReport();
    bool error = report->hasError();
    QString err = report->lastErrorMsg();
//    QApplication::restoreOverrideCursor();

    if ( error )
    QMessageBox::information( 0, "Riport error", err );
    else {
    //-----------------------------
    // PRINT PREVIEW
    //-----------------------------
    NCReportPreviewWindow *pv = new NCReportPreviewWindow();
    pv->setReport( report );
    pv->setOutput( (NCReportPreviewOutput*)output );
    pv->setWindowModality(Qt::ApplicationModal );
    pv->setAttribute( Qt::WA_DeleteOnClose );
    pv->show();}
}

void repkassa::on_Sotrudnik_editingFinished()
{
//    QSqlQuery query;
//    query.prepare("SELECT PERSONAL.ID "
//                  "FROM PERSONAL "
//                  "WHERE PERSONAL.FIO = :FIO ");
//    query.bindValue(":FIO",ui->Sotrudnik->text());
//    query.exec();
//    QSqlRecord record = query.record();
//    query.next();

//    ID_sotr = query.value(record.indexOf("ID")).toInt();

}
