#include "mainwindow.h"
#include "menu.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Menu menu;
    menu.show();
    //MainWindow w;
    //w.show();
    return a.exec();
}

