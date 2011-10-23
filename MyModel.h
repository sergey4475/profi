#ifndef MYMODEL_H
#define MYMODEL_H

#include "sqlmodules.h"
#include <QItemDelegate>
#include "mainform.h"
#include <QtGui>

class clSqlQueryModel : public QSqlQueryModel {
  QVariant data(const QModelIndex &index, int role) const;
};

class sotSqlQueryModel : public QSqlQueryModel {
  QVariant data(const QModelIndex &index, int role) const;
};

class PSqlTableModel : public QSqlRelationalTableModel {
    QVariant data(const QModelIndex &index, int role) const;
};

class PStandardItemModel : public QStandardItemModel{
    Q_OBJECT

public:
    PStandardItemModel(QObject *parent = 0);
    virtual ~PStandardItemModel();
    QVariant data(const QModelIndex &index, int role) const;
    double sum_uslugi;
    MainForm *frm;

};

class Ost_model : public QSqlQueryModel{
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // MYMODEL_H
