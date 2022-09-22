#include "sublist_dialog.h"
#include "ui_sublist_dialog.h"
#include <QFile>
#include <iostream>

sublist_dialog::sublist_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sublist_dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Elenco iscritti");
    //Recupera i dati degli iscritti dal file csv e li accoda al contenuto del textEdit
    //Nota: ho scelto di non inserire la password degli utenti all'interno dei dati mostrati
    QFile file("elenco_iscritti.csv");
    //Se il file non è vuoto copia la lista degli utenti nel textEdit, altrimenti la textEdit rimane vuota
    if(file.size() != 0){
        if(!file.open(QIODevice::ReadOnly))
            std::cerr << "Cannot open file for reading: " << qPrintable(file.errorString()) << std::endl;
        while(!file.atEnd()) {
            QString row = file.readLine();
            QStringList fields = row.split(';');
            ui->textEdit_sublist->append("Nome e cognome: " + fields[0] + " " + fields[1] +"\n"
                                        + "Mail/tel: " + fields[2]+ "\n"
                                        + "Data di nascita: " + fields[4] + "\n"
                                        + "Genere: " + fields[5] + "\n\n");
        }
    }
    //La textEdit è impostata su readonly, non è possibile modificarne il contenuto
    ui->textEdit_sublist->setReadOnly(true);
}

sublist_dialog::~sublist_dialog()
{
    delete ui;
}

void sublist_dialog::on_pushButton_close_clicked()
{
    //Chiusura della dialog in seguito al click del bottone
    this->close();
}
