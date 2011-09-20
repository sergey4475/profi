#ifndef FRMRASPRED_H
#define FRMRASPRED_H

#include <QWidget>
#include "MyModel.h"

namespace Ui {
    class frmRaspred;
}

class frmRaspred : public QWidget
{
    Q_OBJECT

public:
    explicit frmRaspred(QWidget *parent = 0);
    void initForm(uslStandardItemModel *model,int vid_form);
    uslStandardItemModel *tempModel;
    int Number;

    ~frmRaspred();

private slots:
    void on_add_button_clicked();

    void on_ApplyBut_clicked();

private:
    Ui::frmRaspred *ui;
};

#endif // FRMRASPRED_H
