#include "thirdwindow.h"
#include "ui_thirdwindow.h"
#include "secondwindow.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

ThirdWindow::ThirdWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);

    pokazPlik();
    czysciciel();
}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}

void ThirdWindow::pokazPlik(){
    QFile plik(QString::fromStdString(nazwaPliku));
    if(!plik.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"Brak pliku",plik.errorString());

    QTextStream in(&plik);

    ui->textView->setText(in.readAll());
    nazwaPliku="";
}
void ThirdWindow::czysciciel(){

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

