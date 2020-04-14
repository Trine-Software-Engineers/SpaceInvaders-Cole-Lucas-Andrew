#include "menu.h"
#include "ui_menu.h"
#include <QHBoxLayout>
#include "leaderboard.h"

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

void Menu::on_pushButton_Leaderboard_clicked()
{
    leaderboard board;
//    board.setScore(0);
    board.showLeaderboard();
}
