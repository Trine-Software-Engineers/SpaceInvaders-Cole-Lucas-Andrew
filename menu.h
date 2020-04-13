#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();
    void UI();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Menu *ui;
    MainWindow Game;
};



#endif // MENU_H
