#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QDialog>

//extern int score;

struct LeaderboardRow {
    QString name;
    QString score;
};

namespace Ui {
class leaderboard;
}

class leaderboard : public QDialog
{
    Q_OBJECT

public:
    explicit leaderboard(QWidget *parent = nullptr);
    ~leaderboard();
    void showLeaderboard();
    void LeaderboardUpdate(QVector<LeaderboardRow> &leaderboard);
    void setScore(int newscore);

private:
    Ui::leaderboard *ui;
};



#endif // LEADERBOARD_H
