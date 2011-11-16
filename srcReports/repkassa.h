#ifndef REPKASSA_H
#define REPKASSA_H

#include <QWidget>
#include "sqlmodules.h"
#include "MyModel.h"
#include "print.h"

namespace Ui {
    class repkassa;
}

class repkassa : public QWidget
{
    Q_OBJECT

public:
    explicit repkassa(QWidget *parent = 0);
    ~repkassa();
    void init();

private slots:
    void on_start_report_clicked();

    void on_Sotrudnik_editingFinished();

private:
    Ui::repkassa *ui;
    int ID_sotr;
};

#endif // REPKASSA_H
