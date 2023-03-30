#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include "thirdwindow.h"

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void on_przyciskWyboruProcentow_clicked();

    void on_przyciskObliczenia_clicked();

    void on_przyciskPoprawy_clicked();

private:
    Ui::SecondWindow *ui;
    ThirdWindow *thirdWindow;

    void wpiszWybory();
    void wprowadzWartosci();
    void poprawWartosci();
    void wpiszWartosci();
    void ZMasDoMol();
    void ZMolDoMas();
    int sprawdzPoprawnosc();
    void czysciciel();

    void odliczaniePojedynczych(int);
    void odliczanieZawartych(int);
    void odliczanieDwochRoznych(int);
    void odliczaniePotrojnych(int);

    void Oblicz();

    double rozszerzalnoscTermiczna(); //NOWE!!!!
    double obliczR2O();//NOWE!!!!
    double obliczRO(); //NOWE!!!!

    void zapisPliku();
};

extern std::string nazwaPliku;


#endif // SECONDWINDOW_H
