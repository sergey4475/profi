#include "repsklad.h"
#include "ui_repsklad.h"

repSklad::repSklad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::repSklad)
{
    ui->setupUi(this);
}

repSklad::~repSklad()
{
    delete ui;
}
