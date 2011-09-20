#include "qdatelineedit.h"
#include <QHBoxLayout>
#include <QCalendarWidget>


QDateLineEdit::QDateLineEdit(QWidget *parent) :
    QWidget(parent)
{
    createControls();
    setConnects();
}

QDateLineEdit::~QDateLineEdit(){

}

void QDateLineEdit::createControls(){
    qleEdit=new QLineEdit(this);
    qleEdit->setInputMask("00.00.0000");

    qtbButton=new QToolButton(this);
    qtbButton->setIcon(QIcon(":/res/icons/calendar.png"));

    this->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    QHBoxLayout* qhblLayout=new QHBoxLayout();

    qhblLayout->addWidget(qleEdit);
    qhblLayout->addWidget(qtbButton);
    qhblLayout->setContentsMargins(0,0,0,0);
    qhblLayout->setSpacing(0);

    this->setLayout(qhblLayout);
}

void QDateLineEdit::setConnects(){
    connect(qleEdit,SIGNAL(editingFinished()),this,SLOT(slotCheckValidate()));
    connect(qleEdit,SIGNAL(textChanged(const QString&)),this,SLOT(slotTextChanged(const QString&)));
    connect(qtbButton,SIGNAL(clicked()),this,SLOT(slotCalendar()));
}

void QDateLineEdit::slotTextChanged(const QString& text)
{
        QStringList qslTmp=text.split(".");
        if (qslTmp.at(0).toInt()>31 || qslTmp.at(0).toInt()>QDate(qslTmp.at(2).toInt(),qslTmp.at(1).toInt(),01) .daysInMonth())
        {
                qleEdit->setCursorPosition(0);
                qleEdit->setSelection(0,2);
        }
        if (qslTmp.at(1).toInt()>12)
        {
                qleEdit->setCursorPosition(3);
                qleEdit->setSelection(3,2);
        }
        QDate date=QDate::fromString(qleEdit->text(),"dd.MM.yyyy");
        if (date.isValid())
                emit dateChanged(date);
}

void QDateLineEdit::slotCalendar()
{
        QCalendarWidget* qcwCalendar=new QCalendarWidget(this);
        qcwCalendar->setWindowFlags(Qt::Popup);
        qcwCalendar->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(qcwCalendar, SIGNAL(clicked(QDate)), this, SLOT(setDate(QDate)));
        QObject::connect(qcwCalendar, SIGNAL(clicked(QDate)), qcwCalendar, SLOT(close()));

        QPoint pos = (this->layoutDirection() == Qt::RightToLeft) ? this->rect().bottomRight() : this->rect().bottomLeft();
        pos = this->mapToGlobal(pos);
        qcwCalendar->move(pos);
        qcwCalendar->setSelectedDate(QDate().fromString(qleEdit->text(),"dd.MM.yyyy"));
        qcwCalendar->show();
}

void QDateLineEdit::setDate(QDate date)
{
        if (date.isValid())
                qleEdit->setText(date.toString("dd.MM.yyyy"));
}

void QDateLineEdit::setDateCalendar(QDate date){
    if (date.isValid())
            qleEdit->setText(date.toString("dd.MM.yyyy"));
}

void QDateLineEdit::slotCheckValidate()
{
        QDate date=QDate::fromString(qleEdit->text(),"dd.MM.yyyy");
        if (!date.isValid())
        {
                if (qleEdit->text()!="..")
                {
                        qleEdit->setFocus();
                        qleEdit->setCursorPosition(0);
                }
                else
                {
                        emit editingFinished();
                }
        }
        else
                emit editingFinished();
}
