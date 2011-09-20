#ifndef FILTEREDIT_H
#define FILTEREDIT_H

#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>

class QSelLineEdit : public QLineEdit
{
Q_OBJECT
  Q_PROPERTY(bool buttonVisible READ buttonIsVisible WRITE setButtonVisible)

public:
    explicit QSelLineEdit(QWidget *parent = 0);
    bool buttonIsVisible();
private:
    QToolButton *button;
    bool visible;
public slots:
    void setButtonVisible(bool);

signals:
    void buttonClicked();

};

#endif // FILTEREDIT_H
