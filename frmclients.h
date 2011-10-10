#ifndef FRMCLIENTS_H
#define FRMCLIENTS_H
#define add_rec 1
#define edit_rec 2

#include <QFrame>
#include "sqlmodules.h"
#include "mainform.h"

namespace Ui {
    class frmClients;
}

class frmClients : public QFrame
{
    Q_OBJECT

public:
    explicit frmClients(QWidget *parent = 0);
    ~frmClients();
    void initForm(WId w_ID,int fl);

    int IDClients;
    int flag_record;
    MainForm *frm;

private slots:

    void on_tableView_clicked(const QModelIndex &index);

    void on_toolButton_4_clicked();

    void on_ApplyBut_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_edit_button_clicked();

    void on_del_button_clicked();

    void on_closeFrame_clicked();

private:
    Ui::frmClients *ui;
};

#endif // FRMCLIENTS_H
