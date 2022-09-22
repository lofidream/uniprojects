#include "charts_dialog.h"
#include "ui_charts_dialog.h"
#include <QtCharts>
#include <QPieSeries>
#include <QChartView>
#include <iostream>

using namespace QtCharts;

charts_dialog::charts_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::charts_dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Statistiche");
    //Richiamo i metodi che generano i grafici
    create_gender_chart();
    create_age_chart();
}

charts_dialog::~charts_dialog()
{
    delete ui;
}

//Questo metodo calcola il numero di utenti maschi e femmine, a partire dai dati contenuti nel file csv, poi realizza il grafico a torta relativo
void charts_dialog::create_gender_chart() {
    //Contatori degli utenti maschi e femmine
    int male = 0;
    int female = 0;
    //Recupero il genere dal file csv ed incremento i contatori
    QFile file("elenco_iscritti.csv");
    //Se il file è vuoto, il grafico viene generato lo stesso, ma ovviamente non ci saranno le fette della torta
    if(file.size() != 0) {
        if(!file.open(QIODevice::ReadOnly))
            std::cerr << "Cannot open file for reading: " << qPrintable(file.errorString()) << std::endl;
        while(!file.atEnd()) {
            QString row = file.readLine();
            QStringList fields = row.split(';');
            if(fields[5] == 'M')
                male++;
            else
                female++;
        }
    }

    QPieSeries *series = new QPieSeries();
    //Aggiungo i dati raccolti alla serie
    series->append("Uomini", male);
    series->append("Donne", female);

    //Personalizzo le slice della torta, andando ad assegnare un colore ed un etichetta che ne indichi la porzione di serie e la percentuale sul totale
    QPieSlice *m = series->slices().at(0);
    m->setBrush(Qt::blue);
    m->setLabel("Uomini " + QString::number(m->percentage()*100) + "%");

    QPieSlice *f = series->slices().at(1);
    f->setBrush(Qt::red);
    f->setLabel("Donne " + QString::number(f->percentage()*100) + "%");

    //Creo il grafico e ne imposto titolo, posizione della legenda e serie
    QChart *chart = new QChart();
    chart->setTitle("Rapporto iscritti per genere");
    chart->legend()->setAlignment(Qt::AlignLeft);
    chart->addSeries(series);

    //La vista del grafico è inserita in un frame presente nella dialog
    QChartView *chartview = new QChartView(chart);
    chartview->setParent(ui->frame_gender);
}

//Questo metodo calcola il numero di utenti per fasce di età, a partire dai dati contenuti nel file csv, poi ne crea il grafico relativo
void charts_dialog::create_age_chart() {
    //Contatori degli utenti divisi nelle fasce di età
    int fascia1 = 0, fascia2 = 0, fascia3 = 0, fascia4 = 0, fascia5 = 0, age;
    //Recupero l'età dal file csv ed incremento i contatori
    QFile file("elenco_iscritti.csv");
    //Se il file è vuoto, il grafico viene generato lo stesso, ma ovviamente non ci saranno le fette della torta
    if(file.size() != 0) {
        if(!file.open(QIODevice::ReadOnly))
            std::cerr << "Cannot open file for reading: " << qPrintable(file.errorString()) << std::endl;
        while(!file.atEnd()) {
            QString row = file.readLine();
            QStringList fields = row.split(';');
            age = fields[6].toInt();
            if(age <= 26)
                fascia1++;
            else if(age > 26 && age <= 35)
                fascia2++;
            else if(age > 35 && age <= 44)
                fascia3++;
            else if(age > 44 && age <= 53)
                fascia4++;
            else
                fascia5++;
        }
    }

    QPieSeries *series = new QPieSeries();
    //Aggiungo i dati raccolti alla serie
    series->append("Fascia 18-26", fascia1);
    series->append("Fascia 27-35", fascia2);
    series->append("Fascia 36-44", fascia3);
    series->append("Fascia 45-53", fascia4);
    series->append("Fascia 54+", fascia5);

    //Personalizzo le slice della torta, andando ad assegnare un colore ed un etichetta che ne indichi la porzione di serie e la percentuale sul totale
    QPieSlice *f1 = series->slices().at(0);
    f1->setBrush(Qt::blue);
    f1->setLabel("Fascia 18-26 " + QString::number(f1->percentage()*100) + "%");

    QPieSlice *f2 = series->slices().at(1);
    f2->setBrush(Qt::red);
    f2->setLabel("Fascia 27-35 " + QString::number(f2->percentage()*100) + "%");

    QPieSlice *f3 = series->slices().at(2);
    f3->setBrush(Qt::green);
    f3->setLabel("Fascia 36-44 " + QString::number(f3->percentage()*100) + "%");

    QPieSlice *f4 = series->slices().at(3);
    f4->setBrush(Qt::cyan);
    f4->setLabel("Fascia 45-53 " + QString::number(f4->percentage()*100) + "%");

    QPieSlice *f5 = series->slices().at(4);
    f5->setBrush(Qt::gray);
    f5->setLabel("Fascia 54+ " + QString::number(f5->percentage()*100) + "%");

    //Creo il grafico e ne imposto titolo, posizione della legenda e serie
    QChart *chart = new QChart();
    chart->setTitle("Rapporto iscritti per fascia di età");
    chart->legend()->setAlignment(Qt::AlignLeft);
    chart->addSeries(series);

    //La vista del grafico è inserita in una frame presente nella dialog
    QChartView *chartview = new QChartView(chart);
    chartview->setParent(ui->frame_age);
}

//Questo metodo chiude la dialog al click del bottone
void charts_dialog::on_pushButton_clicked()
{
    //Chiusura della dialog al click del bottone
    this->close();
}
