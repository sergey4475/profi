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

private:
    Ui::repkassa *ui;
};

#endif // REPKASSA_H
