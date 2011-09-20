#ifndef FRM_OKAZANIE_USLUG_H
#define FRM_OKAZANIE_USLUG_H

#include <QFrame>
#include "mainform.h"
#include "MyModel.h"



namespace Ui {
    class frm_okazanie_uslug;
}

class frm_okazanie_uslug : public QFrame
{
    Q_OBJECT

public:
    explicit frm_okazanie_uslug(QWidget *parent = 0);
    ~frm_okazanie_uslug();
    uslStandardItemModel *tempModel;
    void InitForm(int nUslugi,WId w_ID);
    int ID_sotr, ID_client, Number;
    MainForm *frm;

private slots:

    void on_toolButton_clicked();

    void on_add_usluga_clicked();

    void on_Client_editingFinished();

    void on_Sotrudnik_editingFinished();

    void on_but_oplatit_clicked();

    void on_del_usluga_clicked();

    void on_Client_buttonClicked();

    void on_add_prodaja_2_clicked();

    void on_del_prodaja_2_clicked();

private:
    Ui::frm_okazanie_uslug *ui;
    int NumberUslugi;
};

#endif // FRM_OKAZANIE_USLUG_H
