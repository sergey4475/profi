#ifndef FRMSPR_H
#define FRMSPR_H

#include <QtGui/QWidget>
#include "MyModel.h"
#include "include/delegats.h"

namespace Ui {
    class frmSpr;
}

class frmSpr : public QWidget
{
    Q_OBJECT

public:
    explicit frmSpr(QWidget *parent = 0);
    void init(uslSqlTableModel *Model);
    uslSqlTableModel *sprModel;
    ~frmSpr();

private slots:
    void on_add_usluga_clicked();

    void on_del_usluga_clicked();

private:
    Ui::frmSpr *ui;
};

#endif // FRMSPR_H
