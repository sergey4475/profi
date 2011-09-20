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

class uslSqlTableModel : public QSqlTableModel {
    QVariant data(const QModelIndex &index, int role) const;
    //bool setData(const QModelIndex &index,const QVariant &value, int role);
};

class uslStandardItemModel : public QStandardItemModel{
    Q_OBJECT

public:
    uslStandardItemModel(QObject *parent = 0);
    virtual ~uslStandardItemModel();
    QVariant data(const QModelIndex &index, int role) const;
    double sum_uslugi;
    MainForm *frm;

public slots:
    void editFinish(QModelIndex index);

};

class Ost_model : public QSqlQueryModel{
    QVariant data(const QModelIndex &index, int role) const;
};


#endif // MYMODEL_H
