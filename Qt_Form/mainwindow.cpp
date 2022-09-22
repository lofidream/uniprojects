#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Form di Login/Iscrizione");
    //Nascondo le label di errore, che si attiveranno in caso i controlli rivelino qualcosa di errato nel form
    ui->label_fields_invalid->setVisible(false);
    ui->label_login_fields_invalid->setVisible(false);
    ui->label_recovery_fields_invalid->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Questo metodo controlla che tutti i campi della sezione d'iscrizione siano stati compilati (richiamato dal click del button d'iscrizione)
bool MainWindow::check_sign_data(const QString &name, const QString &surname, const QString &email_tel, const QString &pass, const char &sex) {
    if(name.isEmpty() || surname.isEmpty() || email_tel.isEmpty() || pass.isEmpty() || (sex != 'F' && sex != 'M'))
        return false;
    return true;
}

//Questo metodo controlla che tutti i campi della sezione di login siano stati compilati (richiamato dal click del button di login)
bool MainWindow::check_login_data(const QString &email_tel, const QString &pass) {
    if(email_tel.isEmpty() || pass.isEmpty())
        return false;
    return true;
}

//Variante del metodo precedente, controlla che il campo email/telefono della sezione di login sia stato compilato (richiamato dal click del tasto di recupero password)
bool MainWindow::check_login_data(const QString &email_tel) {
    if(email_tel.isEmpty())
        return false;
    return true;
}

//Questo metodo controlla il formato del campo email/telefono, attraverso l'uso di espressioni regolari (richiamato dal click dei button di login/iscrizione/recupero password)
bool MainWindow::check_email_tel(const QString &email_tel) {
    QRegularExpression rx("\\b[A-Z0-9._]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption); //accetta email con suffisso (dopo il .) che abbia tra 2 e 4 lettere (quindi anche .com, ecc...)
    QRegularExpression rx2("\\b[0-9]{10}\\b"); //Una sequenza di 10 cifre esatte rappresenta il numero di telefono

    if((rx.match(email_tel).hasMatch()) || (rx2.match(email_tel).hasMatch()))
        return true;
    return false;
}

/*
Questo metodo controlla se la mail/telefono inserito in fase d'iscrizione sia già stato usato (richiamato dal click del button di iscrizione).
Se il file è vuoto, allora non vi è alcuna registrazione precedente con quell'email/telefono
*/
bool MainWindow::is_already_signed(const QString &email_tel) {
    QFile file("elenco_iscritti.csv");
    if(file.size() == 0)
        return false;
    if(!file.open(QIODevice::ReadOnly))
        std::cerr << "Cannot open file for reading: " << qPrintable(file.errorString()) << std::endl;
    while(!file.atEnd()) {
        QString row = file.readLine();
        QStringList fields = row.split(';'); //il punto e virgola è il carattere separatore del file csv
        if(fields[2] == email_tel) //il campo mail/tel è inserito al terzo posto nella riga del file csv
            return true;
    }
    return false;
}

/*
Questo metodo controlla l'esistenza di un utente con mail/telefono e password inseriti nel form di login (richiamato dal click del button di login).
Se il file è vuoto, allora non esiste alcun account con quella combinazione di email/telefono e password
*/
bool MainWindow::is_account_verified(const QString &email_tel, const QString &pass) {
    QFile file("elenco_iscritti.csv");
    if(file.size() == 0)
        return false;
    if(!file.open(QIODevice::ReadOnly))
        std::cerr << "Cannot open file for reading: " << qPrintable(file.errorString()) << std::endl;
    while(!file.atEnd()) {
        QString row = file.readLine();
        QStringList fields = row.split(';'); //il punto e virgola è il carattere separatore del file csv
        if(fields[2] == email_tel && fields[3] == pass) //i campi mail/tel e password sono al terzo e quarto posto nella riga del file csv
            return true;
    }
    return false;
}

//Questo metodo calcola l'età attraverso la differenza tra la data di nascita inserita e la data corrente (richiamato dal click del button di iscrizione)
int MainWindow::calculate_age(const QDate &date) {
    QDate currentDate = QDate::currentDate(); //Recupero la data di oggi
    QDate bday = QDate(currentDate.year(), date.month(), date.day()); //Costruisco la data del compleanno della persona nell'anno corrente
    int age;
    //se il compleanno nell'anno corrente è già passato, allora faccio la differenza tra l'anno corrente e quello di nascita
    if(currentDate >= bday)
        age = currentDate.year() - date.year();
    //altrimenti, faccio la stessa differenza e sottraggo l'anno non ancora compiuto
    else
        age = currentDate.year() - date.year() - 1;

    return age;
}

//Metodo che gestisce tutti i controlli e le azioni da effettuare quando l'utente preme il tasto di iscrizione
void MainWindow::on_pushButton_sign_clicked()
{
    //Nascondo eventuali label attive
    ui->label_recovery_fields_invalid->setVisible(false);
    ui->label_login_fields_invalid->setVisible(false);
    //Recupero i dati dal form d'iscrizione e li inserisco in una struct
    iscritto i;
    i.nome = ui->lineEdit_sign_name->text();
    i.cognome = ui->lineEdit_sign_surname->text();
    i.email_tel = ui->lineEdit_sign_mail_tel->text();
    i.password = ui->lineEdit_sign_pass->text();
    i.birth = ui->dateEdit_birth->date();
    if(ui->radioButton_female->isChecked())
        i.sesso = 'F';
    else {
        if(ui->radioButton_male->isChecked())
            i.sesso = 'M';
    }

    //Calcolo l'età per riutilizzarla nel grafico a torta e per verificare che l'utente è maggiorenne (per comodità, nel file csv verrà aggiunta anche l'età dell'utente)
    int age = MainWindow::calculate_age(i.birth);

    bool all_fields = MainWindow::check_sign_data(i.nome, i.cognome, i.email_tel, i.password, i.sesso);
    bool date_valid = (i.birth <= QDate::currentDate());
    bool good_format = MainWindow::check_email_tel(i.email_tel);

    //Controllo che tutti i campi siano stati compilati ed abbiano informazioni coerenti (data non futura, email/telefono in formato corretto, no email riservata all'admin)
    //Nota: il controllo del formato/coerenza è solo sul campo email/tel e data, gli altri campi non sono soggetti a controlli diversi dall'essere compilati
    if((!all_fields) || (!date_valid) || (!good_format) || i.email_tel == "admin@pas.com") {
        ui->label_fields_invalid->setStyleSheet("QLabel { color : red; }");
        ui->label_fields_invalid->setVisible(true);
        return;
    }
    //Se tutti i dati sono coerenti, procedo con ulteriori controlli richiesti
    else {
        bool already_signed = MainWindow::is_already_signed(i.email_tel);
        bool adult = (age >= 18);
        ui->label_fields_invalid->setVisible(false);
        //Controllo se l'utente è già iscritto e se è maggiorenne
        if(already_signed || !(adult)) {
            //Crea una dialog modale che avverte dell'iscrizione fallita
            QMessageBox::warning(this, "Iscrizione rifiutata", "Attenzione: assicurarsi di essere maggiorenne e/o di inserire una mail che non sia già stata registrata.");
            return;
        }
        //Se l'utente non è già iscritto ed è maggiorenne, inserisce i suoi dati dopo il contenuto del file .csv ed apre una dialog modale di avvenuta iscrizione
        else {
            QFile file("elenco_iscritti.csv");
            if(!file.open(QIODevice::Append))
                std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;

            QTextStream out(&file);
            out << i.nome << ";"
                << i.cognome << ";"
                << i.email_tel << ";"
                << i.password << ";"
                << i.birth.toString() << ";"
                << i.sesso << ";"
                << age << ";"
                <<Qt::endl;

            QMessageBox::information(this, "Iscrizione accettata", "Benvenuto/a sul portale, " + i.nome + " " + i.cognome + "!");
            this->close(); //Dopo che l'utente preme il bottone di OK della dialog modale, il form di iscrizione si chiude
                           //in modo da "simulare" il passaggio all'area privata del portale
        }
    }
}

//Metodo che gestisce tutti i controlli e le azioni da effettuare quando l'utente preme il tasto di login
void MainWindow::on_pushButton_login_clicked()
{
    //Nascondo eventuali label attive
    ui->label_fields_invalid->setVisible(false);
    ui->label_recovery_fields_invalid->setVisible(false);
    //Recupero i dati dal form di login
    iscritto tmp;
    tmp.email_tel = ui->lineEdit_login_mail_tel->text();
    tmp.password = ui->lineEdit_login_pass->text();

    bool good_format = MainWindow::check_email_tel(tmp.email_tel);
    bool all_fields = MainWindow::check_login_data(tmp.email_tel, tmp.password);

    //Controllo che tutti i campi siani inseriti e nel giusto formato, in caso contrario compare una label con avviso
    if(!(all_fields) || !(good_format)) {
        ui->label_login_fields_invalid->setStyleSheet("QLabel { color : red; }");
        ui->label_login_fields_invalid->setVisible(true);
        return;
    }
    else {
        //Se ho inserito le credenziali da admin, apre la finestra dedicata all'amministrazione e chiude il form
        if(tmp.email_tel == "admin@pas.com" && tmp.password == "admin") {
            admin = new adminwindow();
            admin->show();
            this->close();
        }
        else {
            bool account_verified = MainWindow::is_account_verified(tmp.email_tel, tmp.password);
            //Controllo che esista un utente già iscritto con quelle credenziali
            if(!account_verified) {
                QMessageBox::warning(this, "Accesso fallito", "Email/telefono e/o password errati.");
                return;
            }
            else {
                QMessageBox::information(this, "Accesso riuscito", "Bentornato/a sul portale!");
                this->close(); //Dopo che l'utente preme il bottone di OK della dialog modale, il form di login si chiude
                               //in modo da "simulare" il passaggio all'area privata del portale
            }
        }
    }
}

//Metodo che gestisce tutti i controlli e le azioni da effettuare quando l'utente preme il tasto di recupero password
void MainWindow::on_pushButton_recovery_pass_clicked()
{
    //Nascondo eventuali label attive
    ui->label_fields_invalid->setVisible(false);
    ui->label_login_fields_invalid->setVisible(false);
    //Recupero l'email/telefono dal form di login
    iscritto tmp;
    tmp.email_tel = ui->lineEdit_login_mail_tel->text();

    bool good_format = MainWindow::check_email_tel(tmp.email_tel);
    bool all_fields = MainWindow::check_login_data(tmp.email_tel);
    
    //Controllo che il campo email/telefono non sia vuoto ed abbia un formato corretto e non sia riservata
    if(!(all_fields) || !(good_format) || tmp.email_tel == "admin@pas.com") {
        ui->label_recovery_fields_invalid->setStyleSheet("QLabel { color : red; }");
        ui->label_recovery_fields_invalid->setVisible(true);
    }
    else {
        bool already_signed = MainWindow::is_already_signed(tmp.email_tel);
        //Controllo che ci sia già un'iscrizione con l'email/telefono presente nel campo del form
        if(!already_signed) {
            QMessageBox::warning(this, "Recupero password fallito", "Email/telefono non presente: assicurati di aver inserito l'email/telefono che hai fornito in fase di iscrizione.");
            return;
        }
        else {
            QMessageBox::information(this, "Recupero password avviato", "Email con procedura di ripristino inviata. Controlla la tua casella di posta.");
            this->close(); //Anche in questo caso il form viene chiuso dopo l'OK della dialog modale
        }
    }
}
