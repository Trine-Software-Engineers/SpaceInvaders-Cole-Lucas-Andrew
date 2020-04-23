#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QMediaPlayer>

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);
    ~Menu();
    void UI();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Leaderboard_clicked();

private:
    Ui::Menu *ui;
    MainWindow Game;
    QMediaPlayer *player = new QMediaPlayer;
};



#endif // MENU_H
