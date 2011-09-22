#include "MyModel.h"
#include "include/delegats.h"

QVariant clSqlQueryModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::BackgroundColorRole) {
        if(index.row() %2){
            return QVariant(QColor(200,220,255));
        }else{
            return QVariant(QColor(255,255,255));
        }
    }
    if(role == Qt::DecorationRole){
      int pol = record(index.row()).value("POL").toInt();
      int Del = record(index.row()).value("DEL").toInt();
      QImage image;
      if (index.column() == 0) {
          if (Del == 0){
          if (pol == 0)
              image.load(":/res/icons/13.ico");
          else
              image.load(":/res/icons/12.ico");
          }else{
              if (pol == 0)
                  image.load(":/res/icons/18.ico");
              else
                  image.load(":/res/icons/19.ico");
          }
        return image;
      }
    }

  return QSqlQueryModel::data(index, role);
}

// ******************* Оформление табличного поля *************************
QVariant sotSqlQueryModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::BackgroundColorRole) {
        if(index.row() %2){
            return QVariant(QColor(200,220,255));
        }else{
            return QVariant(QColor(255,255,255));
        }
    }
  if(role == Qt::DecorationRole){
    int pol = record(index.row()).value("POL").toInt();
    int Del = record(index.row()).value("DEL").toInt();
    QImage image;
    if (index.column() == 0) {
        if (Del == 0){
        if (pol == 0)
            image.load(":/res/icons/13.ico");
        else
            image.load(":/res/icons/12.ico");
        }else{
            if (pol == 0)
                image.load(":/res/icons/18.ico");
            else
                image.load(":/res/icons/19.ico");
        }

      }
      return image;
  }

  return QSqlQueryModel::data(index, role);
}

QVariant uslSqlTableModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DecorationRole){
        int Del = record(index.row()).value("DEL").toInt();
      QImage image;
      if (index.column() == 1) {
          if (Del == 0)
              image.load(":/res/icons/14a.bmp");
          else
              image.load(":/res/icons/14b.bmp");

      }
        return image;
    }

    if(role == Qt::BackgroundColorRole) {
        if(index.row() %2){
            return QVariant(QColor(200,220,255));
        }else{
            return QVariant(QColor(255,255,255));
        }
    }
    return QSqlTableModel::data(index, role);
}

void uslStandardItemModel::editFinish(QModelIndex index){

    double cena = itemData(this->index(index.row(),2)).value(0).toDouble();
    int count   = itemData(this->index(index.row(),4)).value(0).toInt();

    double summa = cena * count;
    this->setData(this->index(index.row(),5),summa);

    sum_uslugi = 0;
    for (int ind = 0;ind < this->rowCount(); ind++){
        sum_uslugi += this->itemData(this->index(ind,5)).value(0).toDouble();
        frm->SetSumma(sum_uslugi);
    }
}

uslStandardItemModel::uslStandardItemModel(QObject *parent)
    : QStandardItemModel(parent)
{
sum_uslugi = 0;
}

uslStandardItemModel::~uslStandardItemModel(){

}

QVariant uslStandardItemModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::BackgroundColorRole) {
        if(index.row() %2){
            return QVariant(QColor(200,220,255));
        }else{
            return QVariant(QColor(255,255,255));
        }
    }
    return QStandardItemModel::data(index, role);
}

QVariant Ost_model::data(const QModelIndex &index, int role) const {
    if(role == Qt::TextColorRole) {
        int count = record(index.row()).value("COUNT").toInt();
        if(count <= 0){
            return QVariant(QColor(255,0,0));
        }else{
            return QVariant(QColor(0,0,0));
        }
    }
    return QSqlQueryModel::data(index, role);

}

MyDelegate::MyDelegate(bool calpopup, QObject *parent)
    : QItemDelegate(parent),
      m_calpopup(calpopup)
{

}

MyDelegate::~MyDelegate(){

}

QWidget *MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setCalendarPopup(m_calpopup);
    editor->installEventFilter(const_cast<MyDelegate*>(this));
    return editor;
}

void MyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    QDate value = index.model()->data(index, Qt::EditRole).toDate();
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->setDate(value);
}

void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->interpretText();
    QDate value = de->date();
    model->setData(index, value);
}

void MyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    editor->setGeometry(option.rect);
}

NotEditableDelegate::NotEditableDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

NotEditableDelegate::~NotEditableDelegate(){

}

QWidget *NotEditableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    return 0;
}

BoxDelegate::BoxDelegate(QList< QPair<QString,QString> > &aValues, QObject *parent)
    :QItemDelegate(parent){
    Value = aValues;
}

BoxDelegate::~BoxDelegate(){
}

QWidget *BoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{

    QComboBox *editor = new QComboBox(parent);

    QList< QPair<QString,QString> >::const_iterator it=Value.begin();
    while (it != Value.end())
    {
        const QPair <QString,QString> aPair = *it;
        editor->addItem(aPair.first,aPair.second);
        ++it;
    }
    return editor;
}
void BoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    QComboBox *ComboBox = static_cast<QComboBox*>(editor);
    int value = index.model()->data(index, Qt::DisplayRole).toInt();
    ComboBox->setCurrentIndex(ComboBox->findData(value));
    }

void BoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    QComboBox *ComboBox = static_cast<QComboBox*>(editor);
    int value = ComboBox->itemData(ComboBox->currentIndex()).toInt();
    model->setData(index, value, Qt::EditRole);

}

void BoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    editor->setGeometry(option.rect);
}
