#ifndef FRMKASSA_H
#define FRMKASSA_H

#include <QWidget>
#include "sqlmodules.h"
#include "params.h"

namespace Ui {
    class frmKassa;
}

class frmKassa : public QWidget
{
    Q_OBJECT

public:
    explicit frmKassa(QWidget *parent = 0);
    ~frmKassa();
    void init();

private slots:
    void on_pushButton_clicked();

private:
    Ui::frmKassa *ui;
};

#endif // FRMKASSA_H
