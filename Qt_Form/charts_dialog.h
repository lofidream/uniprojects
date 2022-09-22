#ifndef CHARTS_DIALOG_H
#define CHARTS_DIALOG_H

#include <QDialog>

namespace Ui {
class charts_dialog;
}

class charts_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit charts_dialog(QWidget *parent = nullptr);
    ~charts_dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::charts_dialog *ui;
    void create_gender_chart();
    void create_age_chart();
    int calculate_age(const QDate &date);
};

#endif // CHARTS_DIALOG_H
