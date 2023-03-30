#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <string>
#include <cstdlib>
#include <fstream>

std::string nazwaPliku = "Zestawy/";

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);

    wpiszWybory();
    ui->przyciskPoprawy->setEnabled(false);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::on_przyciskWyboruProcentow_clicked()
{
    if(ui->radioMasowe->isChecked()){
        rodzaj_mas = 0;
    }else if(ui->radioMolowe->isChecked()){
        rodzaj_mas = 1;
    }
    wprowadzWartosci();
    wpiszWartosci();
    ui->przyciskPoprawy->setEnabled(true);
}
//Wpisz wybory do tablic
void SecondWindow::wpiszWybory(){
    //listaSurowcow i listaSkladnikow

    //Skladniki
    for(int i = 0 ; i < int((Wyliczone.size())); i++){
    ui->listaSkladnikow->addItem(QString::fromStdString( Wyliczone[i][0])); // dodawanie całej serii elementów do kontrolki QListWidget
    }

    //Surowce
    for(int i = 0 ; i < int((posortowaneSkladniki.size())); i++){
    ui->listaSurowcow->addItem(QString::fromStdString( posortowaneSkladniki[i][1])); // dodawanie całej serii elementów do kontrolki QListWidget
    }


}
//Wprowadzanie wartosci dla zestawu szklarskiego
void SecondWindow::wprowadzWartosci(){
    QMessageBox msgBox;
    msgBox.setText("Wprowadzono błędne wartości! Zawartość musi wynosić 100%");
    double max = 0.0;
    std::string wartosci;
    while(true){
    for(int i = 0 ; i < (int)Wyliczone.size() ; i++){
        wartosci = QInputDialog::getText(this, "Podaj wartosc dla składnika:" , QString::fromStdString(Wyliczone[i][0])).toStdString();
        max += stod(wartosci);
        Wyliczone[i].push_back(wartosci);
        Wyliczone[i].push_back(wartosci);
    }
    if((max > 99.99 && max < 101.00) || rodzaj_mas == 1){
        break;
    }else{
        msgBox.exec();
        for(int i = 0 ; i < (int)Wyliczone.size() ; i++){
            Wyliczone[i].pop_back();
            Wyliczone[i].pop_back();
        }
        max = 0.0;
    }
}
}
//Poprawa wartosci
void SecondWindow::poprawWartosci(){
    for(int i = 0 ; i < (int)Wyliczone.size() ; i++){
        Wyliczone[i].pop_back();
        Wyliczone[i].pop_back();
    }
    wprowadzWartosci();
}


//Wpisz wartosci dla zestawu
void SecondWindow::wpiszWartosci(){
    ui->listaWartosci->clear();
    for(int i = 0 ; i < int((Wyliczone.size())); i++){
    ui->listaWartosci->addItem(QString::fromStdString( Wyliczone[i][0]) + ": " + QString::fromStdString(Wyliczone[i][1])); // dodawanie całej serii elementów do kontrolki QListWidget
    }
}
//Przeliczanie z Mas do Mol
void SecondWindow::ZMasDoMol(){
    double suma_ni = 0;
    double wynik_Ni = 0;
        /*Sprawdzanie wybranych tlenkow z tablica i tworzenie sumy WI*/
        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            for (int kolejny = 0; kolejny < int(Skladniki.size()); kolejny++) {
                if (Wyliczone[rozmiar][0] == Skladniki[kolejny][0]) {
                    suma_ni += (stod(Wyliczone[rozmiar][2]) / stod(Skladniki[kolejny][1]));
                }
            }
        }
        /*Wykonanie obliczen z mas na mol i wpisanie do tablicy tlenkow*/
        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            for (int kolejny = 0; kolejny < int(Skladniki.size()); kolejny++) {
                if (Wyliczone[rozmiar][0] == Skladniki[kolejny][0]) {
                    wynik_Ni = (stod(Wyliczone[rozmiar][2]) / stod(Skladniki[kolejny][1])) * 100;
                    Wyliczone[rozmiar].push_back(std::to_string(ceil((wynik_Ni / suma_ni) * 100) / 100));
                }
            }
        }

}
//Przeliczanie z Mol do Mas
void SecondWindow::ZMolDoMas() {
    double suma_wi = 0;
    double wynik_NiMi = 0;
    /*Sprawdzanie wybranych tlenkow z tablica i tworzenie sumy WI*/
    for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
        for (int kolejny = 0; kolejny < int(Skladniki.size()); kolejny++) {
            if (Wyliczone[rozmiar][0] == Skladniki[kolejny][0]) {
                suma_wi += (stod(Wyliczone[rozmiar][2]) * stod(Skladniki[kolejny][1]));
            }
        }
    }
    /*Wykonanie obliczen z mas na mol i wpisanie do tablicy tlenkow*/
    for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
        for (int kolejny = 0; kolejny < int(Skladniki.size()); kolejny++) {
            if (Wyliczone[rozmiar][0] == Skladniki[kolejny][0]) {
                wynik_NiMi = (stod(Wyliczone[rozmiar][2]) * stod(Skladniki[kolejny][1])) * 100;
                Wyliczone[rozmiar].push_back(std::to_string(ceil((wynik_NiMi / suma_wi) * 100) / 100));
            }
        }
    }


}

/* Obliczenia */

void SecondWindow::odliczaniePojedynczych(int miejsce_tlenku){
    dane.clear();
        for (int rozmiar = 0; rozmiar < int(posortowaneSkladniki.size()); rozmiar++) {
            if (Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][3] && posortowaneSkladniki[rozmiar][6] == "-") { //Sprawdz czy to pojedynczy tlenek i czy tam jest
                dane.push_back(posortowaneSkladniki[rozmiar][0]);
                dane.push_back(posortowaneSkladniki[rozmiar][1]);

                double procent_mas = stod(Wyliczone[miejsce_tlenku][1]);
                double mnoznik_mas = stod(posortowaneSkladniki[rozmiar][4 + rodzaj_mas]);

                dane.push_back(std::to_string(ceil(procent_mas * mnoznik_mas * 100.0)/100.0));
                wartosciKoncowe.push_back(dane);

                Wyliczone[miejsce_tlenku][1] = "0";
            }
        }
}
void SecondWindow::odliczanieZawartych(int miejsce){
    dane.clear();
        for (int rozmiar = 0; rozmiar < int(posortowaneSkladniki.size()); rozmiar++) {
            if (Wyliczone[miejsce][0] == posortowaneSkladniki[rozmiar][6] && posortowaneSkladniki[rozmiar][9] == "-") {  //Szukanie drugirgo tlenku [miejsce]
                for (int i = 0; i < int(Wyliczone.size()); i++) {
                    if (Wyliczone[i][0] == posortowaneSkladniki[rozmiar][3]) { //Szukanie pierwszego tlenku [i]
                        std::string check = posortowaneSkladniki[rozmiar][1];
                        if (check[0] != '`') {
                            dane.push_back(posortowaneSkladniki[rozmiar][0]);
                            dane.push_back(posortowaneSkladniki[rozmiar][1]); //Dopisujemy składnik

                            double chk_drugi_mnoznik = (stod(Wyliczone[miejsce][1]) * stod(posortowaneSkladniki[rozmiar][7 + rodzaj_mas]));
                            double chk_pierwszy_mnoznik = (stod(Wyliczone[i][1]) * stod(posortowaneSkladniki[rozmiar][4 + rodzaj_mas]));
                            double mas_wag_pierwszego = stod(posortowaneSkladniki[rozmiar][4 + rodzaj_mas]);
                            double mas_wag_drugiego = stod(posortowaneSkladniki[rozmiar][7 + rodzaj_mas]);
                            double procent_pierwszego = stod(Wyliczone[i][1]);
                            double procent_drugiego = stod(Wyliczone[miejsce][1]);


                            if (chk_drugi_mnoznik / mas_wag_pierwszego > procent_pierwszego) { //Jeżeli pierwszy tlenek jest poza skalą to wybieramy jego
                                dane.push_back(std::to_string(chk_pierwszy_mnoznik));
                                Wyliczone[i][1] = "0";
                                Wyliczone[miejsce][1] = std::to_string(ceil((procent_drugiego - (chk_pierwszy_mnoznik / mas_wag_drugiego)) * 100.0) / 100.0);
                                wartosciKoncowe.push_back(dane);
                            }
                            else { //Jeżeli drugi tlenek jest mniejszy lub równy wartością
                                dane.push_back(std::to_string(chk_drugi_mnoznik));
                                Wyliczone[miejsce][1] = "0";
                                Wyliczone[i][1] = std::to_string(ceil((procent_pierwszego - (chk_drugi_mnoznik / mas_wag_pierwszego)) * 100.0) / 100.0);
                                wartosciKoncowe.push_back(dane);
                            }
                        }
                    }
                }
            }
        }
}
void SecondWindow::odliczanieDwochRoznych(int miejsce_tlenku){
    dane.clear();
        for (int rozmiar = 0; rozmiar < int(posortowaneSkladniki.size()); rozmiar++) {
            if ((Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][3] || Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][6]) && posortowaneSkladniki[rozmiar][9] == "-") {  //Szukanie drugirgo tlenku [miejsce]
                std::string check = posortowaneSkladniki[rozmiar][1];
                if(check[0]=='`') { //Surowce z oznaczeniem przed wzorem chemicznym "`" mogą brać udział w dalszym sprawdzaniu
                dane.push_back(posortowaneSkladniki[rozmiar][0]);
                dane.push_back(posortowaneSkladniki[rozmiar][1]); //Dopisujemy składnik

                    if (Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][3]) {
                        double procent_mas = stod(Wyliczone[miejsce_tlenku][1]);
                        double mnoznik_mas = stod(posortowaneSkladniki[rozmiar][4 + rodzaj_mas]);

                        dane.push_back(std::to_string(ceil(procent_mas * mnoznik_mas * 100.0) / 100.0));
                        wartosciKoncowe.push_back(dane);

                        Wyliczone[miejsce_tlenku][1] = "0";
                    }
                    else if (Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][6]) {
                        double procent_mas = stod(Wyliczone[miejsce_tlenku][1]);
                        double mnoznik_mas = stod(posortowaneSkladniki[rozmiar][7 + rodzaj_mas]);

                        dane.push_back(std::to_string(ceil(procent_mas * mnoznik_mas * 100.0) / 100.0));
                        wartosciKoncowe.push_back(dane);

                        /************Jak znajdzie F ****************/
                        if (posortowaneSkladniki[rozmiar][12] != "-") {

                            double mnoznik_mas_F = stod(posortowaneSkladniki[rozmiar][13 + rodzaj_mas]);

                                dane.clear();
                                dane.push_back(posortowaneSkladniki[rozmiar][12]);
                                dane.push_back("0");
                                dane.push_back(std::to_string(ceil((procent_mas * mnoznik_mas) / mnoznik_mas_F * 100.0) / 100.0));
                                Wyliczone.push_back(dane);
                        }
                        Wyliczone[miejsce_tlenku][1] = "0";
                    }
                    check = "";
                }
            }
        }
}
void SecondWindow::odliczaniePotrojnych(int miejsce_tlenku){
    dane.clear();
        for (int rozmiar = 0; rozmiar < int(posortowaneSkladniki.size()); rozmiar++) { //Dla pierwszego
            if (Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][3] && posortowaneSkladniki[rozmiar][6] != "-" && posortowaneSkladniki[rozmiar][9] != "-") { //Sprawdz czy to pojedynczy tlenek i czy tam jest
                dane.push_back(posortowaneSkladniki[rozmiar][0]);
                dane.push_back(posortowaneSkladniki[rozmiar][1]);

                double procent_mas = stod(Wyliczone[miejsce_tlenku][1]);
                double mnoznik_mas = stod(posortowaneSkladniki[rozmiar][4 + rodzaj_mas]);

                dane.push_back(std::to_string(ceil(procent_mas * mnoznik_mas * 100.0) / 100.0));
                wartosciKoncowe.push_back(dane);

                Wyliczone[miejsce_tlenku][1] = "0";
            }
            else if (posortowaneSkladniki[rozmiar][3] != "-" && posortowaneSkladniki[rozmiar][6] != "-" && Wyliczone[miejsce_tlenku][0] == posortowaneSkladniki[rozmiar][9]) {
                for (int i = 0; i < int(Wyliczone.size()); i++) {
                    if (Wyliczone[i][0] == posortowaneSkladniki[rozmiar][6]) {

                        dane.push_back(posortowaneSkladniki[rozmiar][0]);
                        dane.push_back(posortowaneSkladniki[rozmiar][1]); //Dopisujemy składnik

                        double chk_drugi_mnoznik = (stod(Wyliczone[miejsce_tlenku][1]) * stod(posortowaneSkladniki[rozmiar][10 + rodzaj_mas]));
                        double chk_pierwszy_mnoznik = (stod(Wyliczone[i][1]) * stod(posortowaneSkladniki[rozmiar][7 + rodzaj_mas]));
                        double mas_wag_pierwszego = stod(posortowaneSkladniki[rozmiar][7 + rodzaj_mas]);
                        double mas_wag_drugiego = stod(posortowaneSkladniki[rozmiar][10 + rodzaj_mas]);
                        double procent_pierwszego = stod(Wyliczone[i][1]);
                        double procent_drugiego = stod(Wyliczone[miejsce_tlenku][1]);
                        double max_war_f = 0;


                        if (chk_drugi_mnoznik / mas_wag_pierwszego > procent_pierwszego) { //Jeżeli pierwszy tlenek jest poza skalą to wybieramy jego
                            dane.push_back(std::to_string(chk_pierwszy_mnoznik));
                            max_war_f = chk_pierwszy_mnoznik;
                            Wyliczone[i][1] = "0";
                            Wyliczone[miejsce_tlenku][1] = std::to_string(ceil((procent_drugiego - (chk_pierwszy_mnoznik / mas_wag_drugiego)) * 100.0) / 100.0);
                            wartosciKoncowe.push_back(dane);

                        }
                        else { //Jeżeli drugi tlenek jest mniejszy lub równy wartością
                            dane.push_back(std::to_string(chk_drugi_mnoznik));
                            max_war_f = chk_drugi_mnoznik;
                            Wyliczone[miejsce_tlenku][1] = "0";
                            Wyliczone[i][1] = std::to_string(ceil((procent_pierwszego - (chk_drugi_mnoznik / mas_wag_pierwszego)) * 100.0) / 100.0);
                            wartosciKoncowe.push_back(dane);

                        }

                    /*Dodawanie F do tablicy w potrójnych*/
                        if (posortowaneSkladniki[rozmiar][12] != "-") {

                            double mnoznik_mas_F = stod(posortowaneSkladniki[rozmiar][13 + rodzaj_mas]);


                            dane.clear();
                            dane.push_back(posortowaneSkladniki[rozmiar][12]);
                            dane.push_back("0");
                            dane.push_back(std::to_string(ceil((max_war_f / mnoznik_mas_F) * 100.0) / 100.0));
                            Wyliczone.push_back(dane);
                        }

                    }
                }

            }
        }
}

/* !*******! */

//Główna funkcja obliczająca
void SecondWindow::Oblicz(){
    for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            odliczanieDwochRoznych(rozmiar);
        }

        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            odliczanieZawartych(rozmiar);
        }

        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            odliczaniePotrojnych(rozmiar);
        }

        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            odliczaniePojedynczych(rozmiar);
        }

}

void SecondWindow::on_przyciskObliczenia_clicked()
{
    Oblicz();
    if (rodzaj_mas == 0) {
        ZMasDoMol();
        }
        else {
        ZMolDoMas();
    }
    if(sprawdzPoprawnosc()){
    zapisPliku();
    hide();
    thirdWindow = new ThirdWindow(this);
    thirdWindow->show();
    this->parentWidget()->show();
    }else{
        czysciciel();
        hide();
        this->parentWidget()->show();
    }

}

void SecondWindow::zapisPliku(){
    nazwaPliku += QInputDialog::getText(this, "Podaj nazwę pliku:" , "Wpisz nazwe:").toStdString();
    nazwaPliku += ".txt";

    std::fstream plik(nazwaPliku, std::ios::out);
    if(plik.good()){
        plik << "|Nazwa:|\t|Wzor:|\t|Ilosc:|\n";
        for (int rozmiar = 0; rozmiar < int(wartosciKoncowe.size()); rozmiar++) {
                plik << wartosciKoncowe[rozmiar][0] << ": " << wartosciKoncowe[rozmiar][1] << " - " << wartosciKoncowe[rozmiar][2] << "\n";
            }
        if(rodzaj_mas == 0){
        plik << "\n\n\tMas[%]\tMol[%]\n";
        }else{
        plik << "\n\n\tMol[%]\tMas[%]\n";
        }
        for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            plik<<Wyliczone[rozmiar][0]<<"\t"<<Wyliczone[rozmiar][2]<<"\t"<<Wyliczone[rozmiar][3]<<"\n";
            }
        plik << "\n\nRozszerzalnosc termiczna: " << rozszerzalnoscTermiczna() << " [1/st.C*10^7]";
    }
}

int SecondWindow::sprawdzPoprawnosc(){
    for (int rozmiar = 0; rozmiar < int(Wyliczone.size()); rozmiar++) {
            if (stod(Wyliczone[rozmiar][1]) != 0) {
                QMessageBox msgBox;
                msgBox.setText("Wybierz inny surowiec lub popraw dane dla: " + QString::fromStdString(Wyliczone[rozmiar][0]) + " - Pozostalo jeszcze - " + QString::fromStdString(Wyliczone[rozmiar][1]));
                msgBox.exec();
                return 0;
            }
        }
        return 1;
}

void SecondWindow::czysciciel(){

        for (int i = 0; i < int(Wyliczone.size()); i++) {
                Wyliczone[i].clear();
        }
        Wyliczone.clear();
        for (int i = 0; i < int(posortowaneSkladniki.size()); i++) {
            posortowaneSkladniki[i].clear();
        }
        posortowaneSkladniki.clear();
        for (int i = 0; i < int(wybrSur.size()); i++) {
            wybrSur[i].clear();
        }
        wybrSur.clear();
        for (int i = 0; i < int(wartosciKoncowe.size()); i++) {
            wartosciKoncowe[i].clear();
        }
        wartosciKoncowe.clear();

        dane.clear();
    }


void SecondWindow::on_przyciskPoprawy_clicked()
{
poprawWartosci();
wpiszWartosci();
}

double SecondWindow::rozszerzalnoscTermiczna(){
    double suma_rozszerzalnosci = 0.0;
        double psi = 0.0;
        double rmon = 0.0;



        for (int i = 0; i < (int)Wyliczone.size(); i++) {
            if (Wyliczone[i][0] == "SiO2") {
                for (int j = 0; j < (int)Skladniki.size(); j++) {
                    if (Wyliczone[i][0] == Skladniki[j][0]) {
                        if ((stod(Wyliczone[i][3])) > 67) {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3]) / 100)) * (38 - 1.0 * (((stod(Wyliczone[i][3])) / 100) - 67));
                        }
                        else {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (stod(Skladniki[j][2]));
                        }
                    }
                }
            }
            else if (Wyliczone[i][0] == "B2O3") { //CHECK
                for (int j = 0; j < (int)Skladniki.size(); j++) {
                    if (Wyliczone[i][0] == Skladniki[j][0]) {
                        //Obliczanie "w"
                        for (int k = 0; k < (int)Wyliczone.size(); k++) {//Sprawdzanie Al2O3
                            if (Wyliczone[k][0] == "Al2O3") {
                                psi -= (stod(Wyliczone[k][3])) / 100;
                            }
                        }
                        psi = (psi + obliczR2O() + obliczRO()) / (stod(Wyliczone[i][3]) / 100); //Obliczanie psi

                        if (psi > 4) {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (stod(Skladniki[j][2])); //Jeżeli psi > 4 to -50
                        }
                        else {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (12.5 * (4 - psi) - 50);   //Jeżeli nie to wzor i mnożenie m * a
                        }
                    }
                }
            }
            else if (Wyliczone[i][0] == "PbO") { //CHECK
                for (int j = 0; (int)Wyliczone.size(); j++) {    //Obliczanie RmOn
                    if (Wyliczone[j][0] == "P2O5" || Wyliczone[j][0] == "Al2O3" || Wyliczone[j][0] == "MgO" || Wyliczone[j][0] == "Cr2O3" || Wyliczone[j][0] == "B2O3" || Wyliczone[j][0] == "CaO" || Wyliczone[j][0] == "ZnO" || Wyliczone[j][0] == "Bao" || Wyliczone[j][0] == "SrO" || Wyliczone[j][0] == "Bi2O3" || Wyliczone[j][0] == "CdO" || Wyliczone[j][0] == "CuO" || Wyliczone[j][0] == "Nd2O3" || Wyliczone[j][0] == "V2O5" || Wyliczone[j][0] == "Sb2O3" || Wyliczone[j][0] == "Ga2O3") {
                        rmon += (stod(Wyliczone[j][3])) / 100;
                    }
                }

                for (int k = 0; k < (int)Skladniki.size(); k++) {
                    if (Wyliczone[i][0] == Skladniki[k][0]) {
                for (int j = 0; (int)Wyliczone.size(); j++) {
                    if (Wyliczone[j][0] == "SiO2") {
                        if ((obliczR2O() > 0 && obliczR2O() < 3 && (stod(Wyliczone[j][3]) / 100) > 0) || ((rmon / obliczR2O()) > (1 / 3))) {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (stod(Skladniki[k][2])); // Czyli to 130
                        }
                        else {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (130 + 5*(obliczR2O() - 3));
                        }
                    }
                }
                    }
                }
            }
            else if (Wyliczone[i][0] == "TiO") { //TODO
                for (int k = 0; k < (int)Skladniki.size(); k++) {
                    if (Wyliczone[i][0] == Skladniki[k][0]) {
                for (int j = 0; j < (int)Wyliczone.size(); j++) {
                    if (Wyliczone[j][0] == "SiO2") {
                        if (80 > stod(Wyliczone[j][3]) && stod(Wyliczone[j][3]) > 50 && obliczR2O() <= 0.15) {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (30 - 1.5*((stod(Wyliczone[j][3]) / 100) - 50));
                        }
                        else {
                            suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (stod(Skladniki[k][2]));
                        }
                    }
                }
                    }
                }
            }
            else {
                for (int j = 0; j < (int)Skladniki.size(); j++) {
                    if (Wyliczone[i][0] == Skladniki[j][0]) {
                        suma_rozszerzalnosci += ((stod(Wyliczone[i][3])) / 100) * (stod(Skladniki[j][2]));
                    }
                }
            }
        }
        return suma_rozszerzalnosci;
}

double SecondWindow::obliczR2O(){
    double sumaR2O = 0.0;
        for (int i = 0; i < (int)Wyliczone.size(); i++) {
            if (Wyliczone[i][0] == "Na2O" || Wyliczone[i][0] == "K2O") {
                sumaR2O += (stod(Wyliczone[i][3])) / 100;
            }
            else if (Wyliczone[i][0] == "Li2O") {
                sumaR2O += (0.3 * (stod(Wyliczone[i][3]))) / 100;
            }
        }
        return sumaR2O;
}
double SecondWindow::obliczRO(){
    double sumaRO = 0.0;
        for (int i = 0; i < (int)Wyliczone.size(); i++) {
            if (Wyliczone[i][0] == "BaO") {
                sumaRO += (stod(Wyliczone[i][3])) / 100;
            }
            else if (Wyliczone[i][0] == "CaO" || Wyliczone[i][0] == "SrO" || Wyliczone[i][0] == "CdO" || Wyliczone[i][0] == "PbO") {
                sumaRO += (0.7 * (stod(Wyliczone[i][3]))) / 100;
            }
            else if (Wyliczone[i][0] == "MgO" || Wyliczone[i][0] == "ZnO") {
                sumaRO += (0.3 * (stod(Wyliczone[i][3]))) / 100;
            }
        }
        return sumaRO;
}



