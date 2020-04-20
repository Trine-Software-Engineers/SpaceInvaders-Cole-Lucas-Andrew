#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QDebug>
#include <QKeyEvent>
#include <random>
#include "QDateTime"
#include <QApplication>
#include <QMessageBox>
#include <QVector>
#include <QPushButton>
#include <QSize>
#include <QInputDialog>
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QColor>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *event);
public slots:
    void game();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void updatelabel(QString score, QString lives);

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};
#endif // MAINWINDOW_H
