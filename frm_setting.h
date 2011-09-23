#ifndef FRM_SETTING_H
#define FRM_SETTING_H

#include <QWidget>

namespace Ui {
    class frm_setting;
}

class frm_setting : public QWidget
{
    Q_OBJECT

public:
    explicit frm_setting(QWidget *parent = 0);
    void InitForm();
    ~frm_setting();

private slots:
    void on_testBUt_clicked();

    void on_pushButton_clicked();

private:
    Ui::frm_setting *ui;
};

#endif // FRM_SETTING_H
