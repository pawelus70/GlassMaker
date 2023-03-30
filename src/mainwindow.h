#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secondwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_przyciskKolejnegoEtapu_clicked();

    void on_przyciskWyboruSkladnikow_clicked();

    void on_przyciskWyboruSurowcow_clicked();

private:
    Ui::MainWindow *ui;
    SecondWindow *secondWindow;

    void naSurowce();
    void naSkladniki();
    void uzupelnijDane();
    void zatwierdzDane();
    void posortujSurowce();

};

//Miejsca na zmienne
/*****************************************/
extern std::fstream plik;

//Magazyny na dane
extern std::vector< std::vector<std::string> > Surowce;
extern std::vector< std::vector<std::string> > Skladniki;
extern std::vector< std::vector<std::string> > Wyliczone;
extern std::vector< std::vector<std::string> > wartosciKoncowe;
extern std::vector< std::vector<std::string> > wybrSur;
extern std::vector< std::vector<std::string> > posortowaneSkladniki;
extern std::vector<std::string> dane;

//Pomocnicze
extern std::string nazwa;
extern int rodzaj_mas;


/*****************************************/

#endif // MAINWINDOW_H
