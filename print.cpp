#include "print.h"
#include <QDate>

void printOstatok(QString sql,QDate date, QString otdel_sklada){
    NCReport *report = new NCReport();
    report->setReportFile(QDir::currentPath()+"/reports/ostatki_o_skald.xml");
    report->addParameter("sql",sql);
    report->addParameter("Date",date);
    report->addParameter("OtdelSklada",otdel_sklada);
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
