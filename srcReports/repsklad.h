#ifndef REPSKLAD_H
#define REPSKLAD_H

#include <QWidget>

namespace Ui {
    class repSklad;
}

class repSklad : public QWidget
{
    Q_OBJECT

public:
    explicit repSklad(QWidget *parent = 0);
    ~repSklad();

private:
    Ui::repSklad *ui;
};

#endif // REPSKLAD_H
