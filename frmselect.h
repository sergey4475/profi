#ifndef FRMSELECT_H
#define FRMSELECT_H

#define n_USLUGI 1
#define n_MATERIAL 2
#define n_OSTATKI_SKALD 3
#define n_SKALD 4

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
    PSqlTableModel *tabl;
    Ost_model *tabl_;
    clSqlQueryModel *selTabl;
    PStandardItemModel *tempModel;
    MainForm *frm;
    QDate DateDoc;
    int type_select, type_uslugi_,Id_Client;
    QTableView *tableParent;
    void init(QDate date);
    ~frmSelect();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_all_ostatki_clicked();

    void on_sel_button_clicked();

private:
    Ui::frmSelect *ui;
    void multeSelect(const QModelIndexList &indexList);
    void Updater();
};

#endif // FRMSELECT_H
