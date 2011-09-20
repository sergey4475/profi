#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "sqlmodules.h"

namespace Ui {
    class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    WId w_ID;
    int ID_Client;
    QSqlDatabase db;
    void EnableButton(int numButton);
    void SetSumma(double summa);
    void UpdateClients(int IDClient);

private slots:

    void on_butClients_clicked();

    void on_m_sprsotr_triggered();

    void on_m_spruslugi_triggered();

    void on_butMonMaster_clicked();

    void on_butKosmitolog_clicked();

    void on_butUhodzaTelom_clicked();

    void on_butStilist_clicked();

    void on_but_schet_clicked();

    void on_m_spr_triggered();

    void on_add_sklad_triggered();

    void on_oststok_na_sklade_triggered();

    void on_raspredel_triggered();

private:
    Ui::MainForm *ui;
    double summa_uslug;
    int USL_MAN;
    int USL_KOS;
    int USL_UHOD;
    int USL_STIL;
};

#endif // MAINFORM_H
