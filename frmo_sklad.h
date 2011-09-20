#ifndef FRMO_SKLAD_H
#define FRMO_SKLAD_H

#include <QtGui/QWidget>
#include "MyModel.h"
#include "include/delegats.h"
#include "frmselect.h"

namespace Ui {
    class frmO_SKLAD;
}

class frmO_SKLAD : public QWidget
{
    Q_OBJECT

public:
    explicit frmO_SKLAD(QWidget *parent = 0);
    void initForm(uslStandardItemModel *model,int vid_form);
    uslStandardItemModel *tempModel;
    int Number;
    ~frmO_SKLAD();

private:
    void GetOstaok();

private slots:
    void on_toolButton_4_clicked();

    void on_del_button_clicked();

    void on_ApplyBut_clicked();

    void on_qDateLineEdit_dateChanged(QDate &Date);

private:
    Ui::frmO_SKLAD *ui;
};

#endif // FRMO_SKLAD_H
