#include "menu.h"
#include "ui_menu.h"
#include <QHBoxLayout>
#include "leaderboard.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/files/space.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}




Menu::~Menu()
{
    delete ui;
}

void Menu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //draw player left side
    QPixmap playerHighQuality(":/files/player100x100.png");
    painter.drawPixmap(25, 350, playerHighQuality);

    //draw alien left side
    QPixmap alienHighQuality(":/files/alien100x100.png");
    painter.drawPixmap(25, 25, alienHighQuality);


    //draw player right side
    painter.drawPixmap(575, 350, playerHighQuality);

    //draw alien right side
    QPixmap alienHighQualityRed(":/files/alienRed100x100.png");
    painter.drawPixmap(575, 25, alienHighQualityRed);
}

void Menu::on_pushButton_clicked()
{
    this->hide();
    Game.unpause(); //--------Here's the unpause function
    Game.show();
}

void Menu::on_pushButton_Leaderboard_clicked()
{
    leaderboard board;
//    board.setScore(0);
    board.showLeaderboard();
}
