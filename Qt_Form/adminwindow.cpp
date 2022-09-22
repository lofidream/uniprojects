#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QMessageBox>

adminwindow::adminwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminwindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Pannello di amministrazione");
}

adminwindow::~adminwindow()
{
    delete ui;
}

void adminwindow::on_pushButton_logout_clicked()
{
    //Chiudo il pannello di amministrazione
    this->close();
}

void adminwindow::on_pushButton_sublist_clicked()
{
    //Apre la dialog contenente la lista degli utenti registrati
    sublist = new sublist_dialog;
    sublist->show();
}

void adminwindow::on_pushButton_charts_clicked()
{
    //Apre la dialog contenente i grafici delle statistiche
    charts = new charts_dialog;
    charts->show();
}
