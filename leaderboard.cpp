#include "leaderboard.h"
#include "ui_leaderboard.h"

#include <QInputDialog>
#include <QFile>
#include <QDebug>
#include <QTableView>

int lscore;
void leaderboard::setScore(int newscore) {
    lscore = newscore;
}

leaderboard::leaderboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::leaderboard)
{
    ui->setupUi(this);
    setWindowTitle("Leaderboard");
    ui->tableWidget->setColumnCount(3);
    QStringList columnTitles;
    columnTitles << "Name" << "Score";
    ui->tableWidget->setHorizontalHeaderLabels(columnTitles);
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->resizeSection(0,20);
    header->resizeSection(1,80);
    header->resizeSection(2,80);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);

    QVector<LeaderboardRow> leaderboard; //create leaderboard vector
    LeaderboardUpdate(leaderboard);
    //generate random test leaderboard data
    /*for(int i = 0; i<5; i++) {
        LeaderboardRow row;
        row.name = QString::number(qrand() % 3);
        row.score = i;
        leaderboard.append(row);
    }*/

    //load data onto leaderboard table
    int rownum;
    for (int i = 0; i < 6; i++) {
        if(i==0){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            rownum = ui->tableWidget->rowCount() - (1);
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem("Name"));
            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem("Score"));
        } else {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            qDebug() << "1 done";
            rownum = ui->tableWidget->rowCount() - (1);
            qDebug() << "2 done";
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(i));
            qDebug() << "3 done";
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(leaderboard.at(i-1).name));
            qDebug() << "4 done";
            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem((leaderboard.at(i-1).score)));
            qDebug() << "5 done";
        }
    }
}

leaderboard::~leaderboard()
{
    delete ui;
}

void leaderboard::showLeaderboard() {
    leaderboard board;
    board.exec();
}

void leaderboard::LeaderboardUpdate(QVector<LeaderboardRow> &leaderboard)
{
    QFile file("leaderboard.txt");
    file.close();
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
//        return 1;
    }

    //    QStringList nameList;
    //    int scoreList[10];
    //QVector<LeaderboardRow> leaderboard;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        LeaderboardRow row;
        if (line.split(',').first() == "\r\n") {
                    qDebug() << "fej";
            break;
        } else {
//            qDebug()<<"here";
            row.name = line.split(',').first();
            row.score = line.split(',').at(1);
            leaderboard.append(row);
        }

//        nameList.append(line.split(',').first());
//        scoreList.append( line.split(',').at(1).toInt()
    }
    file.close();

//    qDebug() << QString::number(lscore);

    QString topScore = 0;
    for (int i = 10; i < leaderboard.size(); i--) // ++i
    {
        if (leaderboard.at(i).score.toInt() > topScore.toInt())
        {
            topScore = leaderboard.at(i).score;
            break;
        }
    }


//    qDebug() << "first";//QString::number(leaderboard.at(0).score) <<;
//    qDebug() << QString::number(lscore);

//    qDebug() << nameList;
    if (lscore > topScore.toInt())
    {
        bool ok;
        QString name = QInputDialog::getText(0, "New High Score!",
                                                 "Player Name:", QLineEdit::Normal,
                                                 "", &ok);
        for (int i = 0; i < leaderboard.size(); i++) // ++i
        {
            if (leaderboard.at(i).score.toInt() < lscore)
            {
                LeaderboardRow newRow;
                newRow.name = name;
                newRow.score = QString::number(lscore) + "\n";
                leaderboard.insert(i, newRow);
                leaderboard.pop_back();
                break;
            }
        }
    }


//    qDebug() << "second";//QString::number(leaderboard.at(0).score) << ;
//    qDebug() << QString::number(lscore);
//    QFile outfile("leaderboard.csv");
    QTextStream out(&file);

    if (file.open(QIODevice::ReadWrite))
//        return;
    {
        for (int i = 0; i < leaderboard.size(); ++i)
        {
            out << leaderboard.at(i).name << "," << leaderboard.at(i).score;
            qDebug() << "file row written";
        }
    }
    out.flush();
    file.close();


    //showLeaderboard();

//    if (ok && !text.isEmpty())
//        textLabel->setText(text);
}
