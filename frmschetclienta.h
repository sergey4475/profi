#ifndef FRMSCHETCLIENTA_H
#define FRMSCHETCLIENTA_H

#include <QtGui/QWidget>
#include "mainform.h"
#include "procedures.h"
#include "sqlmodules.h"


namespace Ui {
    class frmSchetClienta;
}

class frmSchetClienta : public QWidget
{
    Q_OBJECT

public:
    explicit frmSchetClienta(QWidget *parent = 0);
    void initForm(int IDClient);
    MainForm *pfrm;
    int ID_client;
    ~frmSchetClienta();

private slots:
    void on_add_summa_clicked();
    void on_del_summa_clicked();

private:
    Ui::frmSchetClienta *ui;
};

#endif // FRMSCHETCLIENTA_H
