#ifndef FRMSOTR_H
#define FRMSOTR_H
#define add_rec 1
#define edit_rec 2

#include <QWidget>
#include "sqlmodules.h"

namespace Ui {
    class frmSotr;
}

class frmSotr : public QWidget
{
    Q_OBJECT

public:
    explicit frmSotr(QWidget *parent = 0);
    ~frmSotr();
    void InitForm();
    int IDSotr;
    int flag_record;
private slots:

    void on_comboVidUslugi_activated(int index);

    void on_tableSotr_clicked(const QModelIndex &index);

    void on_add_sotr_clicked();

    void on_edit_sotr_clicked();

    void on_ApplyBut_clicked();

    void on_del_sotr_clicked();

private:
    Ui::frmSotr *ui;
};

#endif // FRMSOTR_H
