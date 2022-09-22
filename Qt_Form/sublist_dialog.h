#ifndef SUBLIST_DIALOG_H
#define SUBLIST_DIALOG_H

#include <QDialog>

namespace Ui {
class sublist_dialog;
}

class sublist_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit sublist_dialog(QWidget *parent = nullptr);
    ~sublist_dialog();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::sublist_dialog *ui;
};

#endif // SUBLIST_DIALOG_H
