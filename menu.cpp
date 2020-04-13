#include "menu.h"
#include "ui_menu.h"
#include <QHBoxLayout>

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);


}



Menu::~Menu()
{
    delete ui;
}


void Menu::on_pushButton_clicked()
{
    this->hide();
    Game.show();
}