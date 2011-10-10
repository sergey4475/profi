#ifndef FRMSPR_H
#define FRMSPR_H

#include <QtGui/QWidget>
#include "MyModel.h"
#include "QMenu"
#include "QAction"
#include "include/delegats.h"

namespace Ui {
    class frmSpr;
}

class frmSpr : public QWidget
{
    Q_OBJECT

public:
    explicit frmSpr(QWidget *parent = 0);
    void init(PSqlTableModel *Model,QList<QAbstractItemDelegate*> lst);
    PSqlTableModel *sprModel;
    ~frmSpr();

private slots:
    void on_add_usluga_clicked();

    void on_del_usluga_clicked();

    void editFinish(QModelIndex index);

private:
    Ui::frmSpr *ui;
    QMenu *Menu;
    bool eventFilter(QObject * obj, QEvent * event);
    void updater(QModelIndex item, int count_row,QObject *obj);

};

#endif // FRMSPR_H
