#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H

#include <QDialog>

namespace Ui {
class ThirdWindow;
}

class ThirdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = nullptr);
    ~ThirdWindow();

private:
    Ui::ThirdWindow *ui;


    void pokazPlik();
    void czysciciel();
};

#endif // THIRDWINDOW_H
