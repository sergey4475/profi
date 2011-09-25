#ifndef FRMDOCUMENT_H
#define FRMDOCUMENT_H

#define f_document  0
#define f_ostatki   1
#define d_oskald    1
#define d_rasored   2

#include <QWidget>
#include "MyModel.h"
#include "include/delegats.h"
#include "frmselect.h"

namespace Ui {
    class frmDocument;
}

class frmDocument : public QWidget
{
    Q_OBJECT

public:
    explicit frmDocument(QWidget *parent = 0);
    void initForm(PStandardItemModel *model,int vid_form, int type_doc);
    PStandardItemModel *tempModel;
    int Number;

    ~frmDocument();

private:
    void GetOstaok();

private:
    Ui::frmDocument *ui;
};

#endif // FRMDOCUMENT_H
