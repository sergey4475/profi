#ifndef DELEGATS_H
#define DELEGATS_H

#include <QItemDelegate>
#include "sqlmodules.h"
#include <QtGui>

class MyDelegate : public QItemDelegate {
    Q_OBJECT
public:
    MyDelegate(bool calpopup = true,QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor,
        const QModelIndex &index) const;
        void setModelData(QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index) const;

    void updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

    virtual ~MyDelegate();
private:
bool m_calpopup;
};

class NotEditableDelegate : public QItemDelegate{
    Q_OBJECT
public:
    NotEditableDelegate(QObject *parent = 0);
    virtual ~NotEditableDelegate();
    virtual QWidget* createEditor (QWidget *parent,const QStyleOptionViewItem&,const QModelIndex& ) const;
};

class BoxDelegate : public QItemDelegate{
    Q_OBJECT
public:
    BoxDelegate(QList< QPair<QString,QString> > &aValues,QObject *parent = 0);
    virtual ~BoxDelegate();
    QList< QPair<QString,QString> > Value;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;

};


#endif // DELEGATS_H
