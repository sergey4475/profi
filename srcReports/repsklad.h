#ifndef REPSKLAD_H
#define REPSKLAD_H

#include <QWidget>
#include "print.h"
#include "params.h"
#include "sqlmodules.h"

namespace Ui {
    class repSklad;
}

class repSklad : public QWidget
{
    Q_OBJECT

public:
    explicit repSklad(QWidget *parent = 0);
    ~repSklad();
    void init(int type_otch);

private slots:
    void on_start_report_clicked();

private:
    Ui::repSklad *ui;
    int otch_;
};

#endif // REPSKLAD_H
