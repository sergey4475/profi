#include "qclearlineedit.h"

QClearLineEdit::QClearLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    button=new QToolButton(this);
    button->setCursor(Qt::ArrowCursor);
    button->setGeometry(0,0,15,15);
    button->setText("...");
    button->setFocusPolicy(Qt::NoFocus);
    button->setIcon(QIcon::fromTheme("edit-clear"));
    button->show();
    button->setStyleSheet("border: none;");
    connect(button,SIGNAL(clicked()),this,SIGNAL(buttonClicked()));

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(button,0,Qt::AlignRight);
    layout->setSpacing(0);
    layout->setMargin(0);
}

void QClearLineEdit::setButtonVisible(bool v){
    visible = v;
    button->setVisible(v);
}

bool QClearLineEdit::buttonIsVisible(){
    return visible;
}
