#ifndef FRMUSLUGI_H
#define FRMUSLUGI_H

#include <QtGui>
#include "sqlmodules.h"
#include "MyModel.h"

namespace Ui {
    class frmUslugi;
}

class frmUslugi : public QWidget
{
    Q_OBJECT

public:
    explicit frmUslugi(QWidget *parent = 0);
    ~frmUslugi();
    void init();
    PSqlTableModel *tabl;
    QString idVID, idGR;
private slots:

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_add_usluga_clicked();

    void on_del_usluga_clicked();

private:
    Ui::frmUslugi *ui;
};

#endif // FRMUSLUGI_H
