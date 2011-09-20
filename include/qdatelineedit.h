#ifndef QDATELINEEDIT_H

#define QDATELINEEDIT_H



#include <QtGui/QWidget>
#include <QDate>
#include <QLineEdit>
#include <QToolButton>



class QDateLineEdit : public QWidget

{
    Q_OBJECT
public:
    QDateLineEdit(QWidget *parent = 0);
    ~QDateLineEdit();
    const QDate date() {return QDate::fromString(qleEdit->text(),"dd.MM.yyyy");}
    void setDateCalendar(QDate date);
private:
    QLineEdit *qleEdit;
    QToolButton *qtbButton;

signals:
    void dateChanged(QDate &Date);
    void editingFinished();
private:
    void createControls();
    void setConnects();
private slots:
    void slotTextChanged(const QString& text);
    void slotCalendar();
    void setDate(const QDate date);
    void slotCheckValidate();
};



#endif

