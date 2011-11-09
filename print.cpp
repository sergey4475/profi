#include "print.h"
#include <QDate>

void printOstatok(QString sql){
    NCReport *report = new NCReport();
    report->setReportFile(QDir::currentPath()+"/reports/ostatki_o_skald.xml");
    report->addParameter("sql",sql);
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
