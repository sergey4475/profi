#ifndef FRMSELECT_H
#define FRMSELECT_H

#define USLUGI 1
#define MATERIAL 2
#define OSTATKI_SKALD 3
#define SKALD 4

#include <QtGui/QWidget>
#include "sqlmodules.h"
#include "MyModel.h"
#include "mainform.h"
#include <QtGui/qtableview.h>

namespace Ui {
    class frmSelect;
}

class frmSelect : public QWidget
{
    Q_OBJECT

public:
    explicit frmSelect(QWidget *parent = 0);
    uslSqlTableModel *tabl;
    Ost_model *tabl_;
    uslStandardItemModel *tempModel;
    MainForm *frm;
    QDate DateDoc;
    int type_select, type_uslugi_;
    QTableView *tableParent;
    void init(int type_sel,int type_uslugi, uslStandardItemModel *table,int ID_client, QDate date);
    ~frmSelect();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::frmSelect *ui;
};

#endif // FRMSELECT_H
