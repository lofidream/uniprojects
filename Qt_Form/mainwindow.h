#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <vector>
#include <QDialog>
#include "adminwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//Struttura che immagazzina i dati dal form
struct iscritto {
    QString nome;
    QString cognome;
    QString email_tel;
    QString password;
    QDate birth;
    char sesso;

    //costruttori
    iscritto() {}
    iscritto(const QString &n, const QString &c,
             const QString &et, const QString &p,
             const QDate &d, const char &s) : nome(n), cognome(c), email_tel(et), password(p), birth(d), sesso(s) {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_sign_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_recovery_pass_clicked();

private:
    Ui::MainWindow *ui;
    adminwindow *admin; //Oggetto adminwindow per richiamare il pannello di amministrazione

    bool check_sign_data(const QString &name, const QString &surname, const QString &email_tel, const QString &pass, const char &sex);
    bool check_login_data(const QString &email_tel, const QString &pass);
    bool check_login_data(const QString &emai_tel);
    bool check_email_tel(const QString &email_tel);
    bool is_already_signed(const QString &email_tel);
    bool is_account_verified(const QString &email_tel, const QString &pass);
    int calculate_age(const QDate &date);
};
#endif // MAINWINDOW_H
