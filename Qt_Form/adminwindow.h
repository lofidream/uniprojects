#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "sublist_dialog.h"
#include "charts_dialog.h"

namespace Ui {
class adminwindow;
}

class adminwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminwindow(QWidget *parent = nullptr);
    ~adminwindow();

private slots:
    void on_pushButton_logout_clicked();

    void on_pushButton_sublist_clicked();

    void on_pushButton_charts_clicked();

private:
    Ui::adminwindow *ui;
    sublist_dialog *sublist; //Oggetto sublist_dialog per richiamare la dialog della lista di utenti
    charts_dialog *charts; //Oggetto charts_dialog per richiamare la dialog dei grafici a torta
};

#endif // ADMINWINDOW_H
