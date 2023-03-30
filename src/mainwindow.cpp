#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <QMessageBox>

using namespace std;

/*Ściąganie globalnych zmiennych*/
fstream plik;

//Magazyny na dane
vector< vector<string> > Surowce;
vector< vector<string> > Skladniki;
vector< vector<string> > Wyliczone;
vector< vector<string> > wartosciKoncowe;
vector< vector<string> > posortowaneSkladniki;
vector< vector<string> > wybrSur;
vector<string> dane;

//Pomocnicze
string nazwa;
int rodzaj_mas = 0;
/*********************************************/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    naSurowce();
    naSkladniki();
    uzupelnijDane();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Kolejny etap po wyborze (secondmain.cpp)
void MainWindow::on_przyciskKolejnegoEtapu_clicked()
{
    zatwierdzDane();
    hide();
    secondWindow = new SecondWindow(this);
    secondWindow->show();
}
//Przypisywanie Surowcow z pliku
void MainWindow::naSurowce(){
    dane.clear();
    QMessageBox msgBox;
    msgBox.setText("Brak pliku Surowce.txt");
    plik.open("Surowce.txt", ios::in);
        if (plik.good() == false)
        {
            msgBox.exec();
            exit(0);
        }
        while (true) {
            if (!plik.eof()) {
                for (int i = 0; i < 15; i++) {
                    plik >> nazwa;
                    dane.push_back(nazwa);
                }
                Surowce.push_back(dane);
                dane.clear();
            }
            else {
                nazwa = "";
                plik.close();
                break;
            }
        }
}
//Przypisywanie Skladnikow z pliku
void MainWindow::naSkladniki(){
    dane.clear();
    QMessageBox msgBox;
    msgBox.setText("Brak pliku Skladniki.txt");
        plik.open("Tlenki.txt", ios::in);
        if (plik.good() == false)
        {
            msgBox.exec();
            exit(0);
        }
        while (true) {
            if (!plik.eof()) {
                for (int i = 0; i < 3; i++) {
                    plik >> nazwa;
                    dane.push_back(nazwa);
                }
                Skladniki.push_back(dane);
                dane.clear();
            }
            else {
                break;
            }
        }
        plik.close();
}
//Uzupelnianie list danymi
void MainWindow::uzupelnijDane(){
    //Dodawanie Surowcow do listy
    for(int i = 0 ; i < int((Surowce.size())); i++){
    ui->listaSurowcow->addItem(QString::fromStdString(Surowce[i][1])); // dodawanie całej serii elementów do kontrolki QListWidget
    }
    ui->listaSurowcow->setSelectionMode(QAbstractItemView::SingleSelection); // ustawienie trybu zaznaczania na możliwość zaznaczania pojedynczej pozycji


    QListWidgetItem* item = 0;
        for(int i = 0; i < ui->listaSurowcow->count(); ++i){
            item = ui->listaSurowcow->item(i);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
    //Dodawanie Skladnikow do listy
        for(int i = 0 ; i < int((Skladniki.size())); i++){
        ui->listaSkladnikow->addItem(QString::fromStdString( Skladniki[i][0])); // dodawanie całej serii elementów do kontrolki QListWidget
        }
        ui->listaSkladnikow->setSelectionMode(QAbstractItemView::SingleSelection); // ustawienie trybu zaznaczania na możliwość zaznaczania pojedynczej pozycji

        QListWidgetItem* itemm = 0;
            for(int i = 0; i < ui->listaSkladnikow->count(); ++i){
                itemm = ui->listaSkladnikow->item(i);
                itemm->setFlags(itemm->flags() | Qt::ItemIsUserCheckable);
                itemm->setCheckState(Qt::Unchecked);
            }
}
//Przypisanie wybranych skladnikow
void MainWindow::on_przyciskWyboruSkladnikow_clicked()
{
    ui->listaWybranychSkladnikow->clear();

    QListWidgetItem* item = 0;
        for(int i = 0; i < ui->listaSkladnikow->count(); ++i){
            item = ui->listaSkladnikow->item(i);
            if(item->checkState() == Qt::Checked){
                ui->listaWybranychSkladnikow->addItem(item->text());
            }
        }
}
//Przypisanie wybranych surowcow
void MainWindow::on_przyciskWyboruSurowcow_clicked()
{
    ui->listaWybranychSurowcow->clear();

    QListWidgetItem* item = 0;
        for(int i = 0; i < ui->listaSurowcow->count(); ++i){
            item = ui->listaSurowcow->item(i);
            if(item->checkState() == Qt::Checked){
                ui->listaWybranychSurowcow->addItem(item->text());
            }
        }
}
//Przypisanie wyborów do obliczen
void MainWindow::zatwierdzDane(){
    dane.clear();
    string uchwyt;
    //Zatwierdzanie i uzupełnianie tablicy wybranymi skladnikami
    QListWidgetItem* item = 0;
        for(int i = 0; i < ui->listaSkladnikow->count(); ++i){
            item = ui->listaSkladnikow->item(i);
            if(item->checkState() == Qt::Checked){
                dane.push_back((item->text()).toStdString());
                Wyliczone.push_back(dane);
                dane.clear();
            }
        }


        dane.clear();

    //Zatwierdzanie i uzupełnianie tablicy wybranymi surowcami
        QListWidgetItem* itemm = 0;
            for(int i = 0; i < ui->listaSurowcow->count(); ++i){
                itemm = ui->listaSurowcow->item(i);
                if(itemm->checkState() == Qt::Checked){
                    //dane.push_back((ui->listaWybranychSkladnikow->item(i)->text()).toStdString());
                    uchwyt = (itemm->text()).toStdString(); //Wstaw nazwe w uchwyt
                    for(int j = 0 ; j < (int)Surowce.size() ; j++){  //Sprawdz w Surowcach gdzie się znajduje
                        if(uchwyt == Surowce[j][1]){  //Jeżeli znaleziony to wpisz wszystkie dane do tablicy posortowaneSkladniki
                           wybrSur.push_back(Surowce[j]);

                        }
                    }
                }
            }

posortujSurowce();
}
//Sortowanie surowcow
void MainWindow::posortujSurowce(){
    for (int i = 0; i < int(wybrSur.size()); i++) {
            if ((wybrSur[i][4] != "-") && (wybrSur[i][7] != "-") && (wybrSur[i][10] != "-")) {
                posortowaneSkladniki.push_back(wybrSur[i]);
            }
        }
            for (int i = 0; i < int(wybrSur.size()); i++) {
                if ((wybrSur[i][4] != "-") && (wybrSur[i][7] != "-") && (wybrSur[i][10] == "-")) {
                    posortowaneSkladniki.push_back(wybrSur[i]);
                }
            }
            for (int i = 0; i < int(wybrSur.size()); i++) {
                if ((wybrSur[i][4] != "-") && (wybrSur[i][7] == "-") && (wybrSur[i][10] == "-")) {
                    posortowaneSkladniki.push_back(wybrSur[i]);
                }
            }
}
