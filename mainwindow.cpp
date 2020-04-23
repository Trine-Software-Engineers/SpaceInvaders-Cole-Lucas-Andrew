#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leaderboard.h"
#include <QMediaPlayer>
#include <QDir>
#include <QSound>
#include <QMultimedia>



float MSbetweenFrames = 16.666666667; //time between frames in milliseconds (roughly 60fps)

float alienOffsetX = 0;
float alienOffsetMultiplierX = 1;
int alienOffsetY = 0;
int alienOffsetMultiplierY = 1;
float alienSpeed = .5;

int bulletX;
int bulletY;
bool bulletInPursuit = false;

int playerX = 0;
int playerY = 740;

bool moveLeft, moveRight, shoot = false;

int TEMPCOUNT = 0;

bool allAliensDead = false;

int score = 0;
int lives = 3;
int roundsBeaten = 0;

float remainingAliens = 0;

int extraAlienX = 0;
int extraAlienY = 0;
int extraAlienXMultiplier = 2;
bool extraAlienStartLeftside = true;
bool extraAlienAlive = false;

QMediaPlayer *bonusMedia = new QMediaPlayer;
QMediaPlayer *gameMedia = new QMediaPlayer;


struct alien
{
    int posX = 0;
    int posY = 0;
    int anchorX = 0;
    int anchorY = 0;
    bool alive = true;
};

struct sandBag
{
    int posX = 0;
    int posY = 0;
    int health = 4;
    bool alive = true;
};

struct enemyBullet
{
    int bulletX = 0;
    int bulletY = 0;
    bool bulletInPursuit = true;
};

bool startGame = false;

QVector<alien> aliens;
QVector<sandBag> sandBags;
QVector<enemyBullet> enemyBullets;

void buildAliens();
void buildBunkers();
int getRand(int min, int max, unsigned int seed);



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/files/space.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //setup time between frames
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(game()));
    timer->start(MSbetweenFrames);
    buildAliens();
    buildBunkers();

    const QString laserMediaPath = "file://" + QDir::currentPath() + "/laser.mp3";
//    QMediaPlayer *laserMedia = new QMediaPlayer;
    laserMedia->setMedia(QUrl(laserMediaPath));
    laserMedia->setVolume(50);

    const QString bonusMediaPath = "file://" + QDir::currentPath() + "/bonusAlien.mp3";
    bonusMedia->setMedia(QUrl(bonusMediaPath));
    bonusMedia->setVolume(50);

    const QString gameMediaPath = "file://" + QDir::currentPath() + "/gameMusic.mp3";
    gameMedia->setMedia(QUrl(gameMediaPath));
    gameMedia->setVolume(25);
    gameMedia->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void buildAliens()
{
    //clear vector
    aliens.clear();
    alienOffsetX = 0;
    alienOffsetY = 0;

    //fill vector
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 5; y++)
        {
            alien newAlien;
            newAlien.anchorX = (x * 50);
            newAlien.anchorY = (y * 50);
            alienOffsetY = roundsBeaten * 50;// make aliens spawn lower each round you clear
            aliens.push_back(newAlien);
        }
    }
}

void buildBunkers()
{
    //clear bunkers
    sandBags.clear();

    //fill bunkers
    int sandBagsOffsetX = 0;
    sandBag newSandBag;
    newSandBag.alive = true;
    newSandBag.health = 4;

    for(int w = 0; w < 4; w++)
    {
        //create top 8 sandbags
        sandBagsOffsetX = w*200;
        for(int z = 0; z < 2; z++)
        {
            newSandBag.posX = 50 + sandBagsOffsetX;
            newSandBag.posY = 600 + (z * 25);
            sandBags.push_back(newSandBag);

            newSandBag.posX = 75 + sandBagsOffsetX;
            newSandBag.posY = 600 + (z * 25);
            sandBags.push_back(newSandBag);

            newSandBag.posX = 100 + sandBagsOffsetX;
            newSandBag.posY = 600 + (z * 25);
            sandBags.push_back(newSandBag);

            newSandBag.posX = 125 + sandBagsOffsetX;
            newSandBag.posY = 600 + (z * 25);
            sandBags.push_back(newSandBag);
        }
        //create bottom two sandbags
        newSandBag.posX = 50 + sandBagsOffsetX;
        newSandBag.posY = 650;
        sandBags.push_back(newSandBag);

        newSandBag.posX = 125 + sandBagsOffsetX;
        newSandBag.posY = 650;
        sandBags.push_back(newSandBag);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //draw player
    QPixmap playerShip(":/files/player50x50.png");
    painter.drawPixmap(playerX, playerY, playerShip);

    //draw extra alien at top
    if(extraAlienAlive)
    {
        QPixmap alien(":/files/alien50x50.png");
        painter.drawPixmap(extraAlienX, extraAlienY, alien);
    }

    //draw aliens
    for(int x = 0; x < 50; x++)
    {
        if(aliens[x].alive)
        {
            QPixmap alien(":/files/alien30x30.png");
            painter.drawPixmap(aliens[x].posX,aliens[x].posY, alien);
        }
    }

    //draw bunkers
    for(int z = 0; z < 40; z++)
    {
        if(sandBags[z].alive)
        {
            //get health of each sandbag, then assign proper sprite to each health value
            QString filePath = "";
            switch(sandBags[z].health) {
                case 4:
                    filePath = ":/files/bunker4.png";
                    break;

                case 3:
                    filePath = ":/files/bunker3.png";
                    break;

                case 2:
                    filePath = ":/files/bunker2.png";
                    break;

                case 1:
                    filePath = ":/files/bunker1.png";
                    break;

                case 0:
                    filePath = ":/files/bunker0.png";
                    break;
            }
            QPixmap bunker(filePath);
            painter.drawPixmap(sandBags[z].posX, sandBags[z].posY, bunker);
        }
    }

    //draw bullet
    if(bulletInPursuit)
    {
        QPixmap bullet(":/files/bullet10x25.png");
        painter.drawPixmap(bulletX, bulletY, bullet);
    }

    //draw enemy bullet
    for(int j = 0; j < enemyBullets.size(); j++)
    {
        if(enemyBullets[j].bulletInPursuit)
        {
            QPixmap enemyBullet(":/files/enemyBullet10x25.png");
            painter.drawPixmap(enemyBullets[j].bulletX, enemyBullets[j].bulletY, enemyBullet);
        }
    }
}

//gets keyboard inputs
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int currentInput = e->key();

    switch(currentInput) {
        case 16777234  :
            moveLeft = true;
            break;

        case 16777236  :
            moveRight = true;
            break;

        case 32        :
            shoot = true;
            laserMedia->play();
            break;
    }
}

void spawnEnemyBullet()
{

    //pick a random enemy to shoot from
    bool enemyNotChosen = true;
    while(enemyNotChosen && !allAliensDead)
    {
        int pick = getRand(0,49,123);//pick random enemy ship
        if(aliens[pick].alive) //if ship is alive, shoot enemy bullet from that ship
        {
            enemyBullet newEnemyBullet;
            newEnemyBullet.bulletX = aliens[pick].posX + 10;
            newEnemyBullet.bulletY = aliens[pick].posY + 15;
            enemyBullets.push_back(newEnemyBullet);
            enemyNotChosen = false;
        }
    }
}

void spawnExtraAlien()
{
    extraAlienAlive = true;

    //start leftside, move to the right
    if(extraAlienStartLeftside)
    {
        extraAlienX = -100;
        extraAlienXMultiplier = 2;

    }
    else //start rightside, move to left
    {
        extraAlienX = 900;
        extraAlienXMultiplier = -2;
    }

    //randomly choose what side extra alien spawns at
    if(getRand(0,1,999) == 1) extraAlienStartLeftside = !extraAlienStartLeftside;

    bonusMedia->play();
}

void calculateAlienSpeed()
{
    if(remainingAliens > 40)
    {
        alienSpeed = .5;
    }
    else if(remainingAliens > 30)
    {
        alienSpeed = .75;
    }
    else if(remainingAliens > 20)
    {
        alienSpeed = 1;
    }
    else if(remainingAliens > 10)
    {
        alienSpeed = 1.25;
    }
    else if(remainingAliens > 0)
    {
        alienSpeed = 1.5;
    }
}

//get keyboard release
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    int currentRelease = e->key();

    switch(currentRelease) {
        case 16777234  :
            moveLeft = false;
            break;

        case 16777236  :
            moveRight = false;
            break;

        case 32        :
            shoot = false;
            break;
    }
}

void MainWindow::updatelabel(QString score, QString lives)
{
    ui->scoreLabel->setText("<font color='white'>" + score + "</font>");
    ui->livesLabel->setText("<font color='white'>" + lives + "</font>");
}


void endGame()
{
    gameMedia->pause();
    MSbetweenFrames = INT_MAX;
    QMessageBox msgBox;
    msgBox.setWindowTitle("Space Invaders");
    msgBox.setText("Game Over!\nFinal Score: " + QString::number(score) + "\nPlay Again?");

    leaderboard board;
    board.setScore(score);
    board.showLeaderboard();

    QAbstractButton *yesButton = msgBox.addButton("Yes", QMessageBox::ActionRole);
    QAbstractButton *quitButton = msgBox.addButton("Quit", QMessageBox::ActionRole);
    msgBox.exec();
    if(msgBox.clickedButton() == yesButton) //play again
        {
            score = 0;
            lives = 3;
            buildAliens();
            buildBunkers();
            alienOffsetY = 0;
            roundsBeaten = 0;
            MSbetweenFrames = 16.666666667;
            extraAlienAlive = false;
        }
    else if(msgBox.clickedButton() == quitButton)//quit
        {
            QApplication::quit();
        }
}

//runs every frame
void MainWindow::game()
{
    if(startGame == true){//check if all aliens are dead
    bool atLeastOneAlienAlive = false;
    for(int x = 0; x < 50; x++)
    {
        if(aliens[x].alive)
        {
            atLeastOneAlienAlive = true;
        }
    }
    if(atLeastOneAlienAlive) allAliensDead = false;
    else allAliensDead = true;

    //get number of aliens killed (use this to speed up remaining aliens
    remainingAliens = 0;
    for(int x = 0; x < 50; x++)
    {
        if(aliens[x].alive) remainingAliens += 1;
    }


    //check if aliens reached border of window
    if(alienOffsetX > 320) //aliens reach right border of window
    {
        alienOffsetMultiplierX = -alienSpeed;
        alienOffsetX = 320;
        alienOffsetY += 30;
    }
    else if(alienOffsetX < 0) //aliens reach left border of window
    {
        alienOffsetMultiplierX = alienSpeed;
        alienOffsetX = 0;
        alienOffsetY += 30;
    }
    else //check for speed update
    {
        if(alienOffsetMultiplierX > 0) alienOffsetMultiplierX = alienSpeed;
        else alienOffsetMultiplierX = -alienSpeed;
    }
    alienOffsetX += alienOffsetMultiplierX;


    //move aliens
    for(int x = 0; x < 50; x++)
    {
        aliens[x].posX = aliens[x].anchorX + alienOffsetX;
        aliens[x].posY = aliens[x].anchorY + alienOffsetY;
    }


    //get player input
    if(moveLeft && moveRight)
    {
        //do nothing
    }
    else if(moveLeft)
    {
        playerX -= 5;
    }
    else if(moveRight)
    {
        playerX += 5;
    }

    //make sure player is in window
    if(playerX > 750) playerX = 750;
    else if(playerX < 0) playerX = 0;

    //bullet velocity
    bulletY -= 15;

    //if bullet misses everything, allow another bullet
    if(bulletY < 0)
    {
        bulletInPursuit = false;
    }

    //fire bullet, if bullet is not in air AND spacebar is pushed
    if(!bulletInPursuit && shoot)
    {
        bulletX = playerX + 20;
        bulletY = 715;
        bulletInPursuit = true;
    }

    //check for bullet impact on bunker
    if(bulletInPursuit)
    {
        for(int x = 0; x < 40; x++)
        {
            if(sandBags[x].alive)
            {
                bool bulletWithinSandBagX = false;
                bool bulletWithinSandBagY = false;

                if((bulletX > (sandBags[x].posX - 10)) && (bulletX < (sandBags[x].posX + 25)))
                {
                    bulletWithinSandBagX = true;
                }
                if((bulletY < (sandBags[x].posY + 25)) && (bulletY > (sandBags[x].posY - 20)))
                {
                    bulletWithinSandBagY = true;
                }

                if(bulletWithinSandBagX && bulletWithinSandBagY)
                {
                    sandBags[x].health -= 1;
                    if(sandBags[x].health < 1) sandBags[x].alive = false;

                    bulletInPursuit = false;
                }


            }
        }
    }

    //check for ENEMY bullet impact on bunker
    for(int c = 0; c < enemyBullets.size(); c++)
    {
        if(enemyBullets[c].bulletInPursuit)
        {
            for(int x = 0; x < 40; x++)
            {
                if(sandBags[x].alive)
                {
                    bool bulletWithinSandBagX = false;
                    bool bulletWithinSandBagY = false;

                    int bX = enemyBullets[c].bulletX;
                    int bY = enemyBullets[c].bulletY;

                    if((bX > (sandBags[x].posX - 10)) && (bX < (sandBags[x].posX + 25)))
                    {
                        bulletWithinSandBagX = true;
                    }
                    if((bY < (sandBags[x].posY + 25)) && (bY > (sandBags[x].posY - 25)))
                    {
                        bulletWithinSandBagY = true;
                    }

                    if(bulletWithinSandBagX && bulletWithinSandBagY)
                    {
                        sandBags[x].health -= 1;
                        if(sandBags[x].health < 1) sandBags[x].alive = false;

                        enemyBullets[c].bulletInPursuit = false;
                    }
                }
            }
        }
    }



    //enemy bullet velocity
    for(int c = 0; c < enemyBullets.size(); c++)
    {
        if(enemyBullets[c].bulletInPursuit)
        {
            enemyBullets[c].bulletY += 10;
        }
    }

    //(1 out of 120) chance to spawn enemy bullet
    if(getRand(0, 120, 945634) == 1)
    {
        spawnEnemyBullet();
    }

    //(1 out of 1200) chance to spawn extra enemy, as long as extra enemy isn't already on screen
    if(getRand(0,1200, 135435) == 1 && !extraAlienAlive)
    {
        spawnExtraAlien();
    }

    //check if enemy shot player
    for(int c = 0; c < enemyBullets.size(); c++)
    {
        if(enemyBullets[c].bulletInPursuit)
        {
            bool bulletInPlayerX = false;
            bool bulletInPlayerY = false;
            int bX = enemyBullets[c].bulletX;
            int bY = enemyBullets[c].bulletY;

            if(bX > (playerX - 10) && bX < (playerX + 50)) bulletInPlayerX = true;
            if(bY > playerY && bY < (playerY + 50)) bulletInPlayerY = true;

            if(bulletInPlayerX && bulletInPlayerY)
            {
                enemyBullets[c].bulletInPursuit = false;
                lives -= 1;
                if(lives < 1) endGame();
            }
        }
    }

    //check if player shot enemy
    if(bulletInPursuit)
    {
        for(int x = 0; x < 50; x++)
        {
            if(aliens[x].alive)
            {
                bool bulletInEnemyX = false;
                bool bulletInEnemyY = false;
                int pX = aliens[x].posX;
                int pY = aliens[x].posY;

                if(bulletX > (pX - 10) && bulletX < (pX + 30)) bulletInEnemyX = true;
                if((bulletY > pY) && bulletY < (pY + 30)) bulletInEnemyY = true;
                if(bulletInEnemyX && bulletInEnemyY)
                {
                    bulletInPursuit = false;
                    aliens[x].alive = false;
                    score += 20;
                }
            }
        }
    }

    //check if player shot extra alien
    if(bulletInPursuit)
    {
        if(extraAlienAlive)
        {
            bool bulletInEnemyX = false;
            bool bulletInEnemyY = false;
            int pX = extraAlienX;
            int pY = extraAlienY;
            if(bulletX > (pX - 10) && bulletX < (pX + 50)) bulletInEnemyX = true;
            if((bulletY > pY) && bulletY < (pY + 30)) bulletInEnemyY = true;
            if(bulletInEnemyX && bulletInEnemyY)
            {
                bulletInPursuit = false;
                extraAlienAlive = false;
                score += 150;
            }
        }
    }

    if(extraAlienAlive)
    {
        //move extra alien accross screen
        extraAlienX += extraAlienXMultiplier;

        //if extra alien survives crossing the screen, kill extra alien
        if(extraAlienX > 1000 || extraAlienX < -200)
        {
            extraAlienAlive = false;
        }

        bonusMedia->play();
    }

    //player kills all aliens on screen
    if(allAliensDead)
    {
        score += 1000;
        lives += 1;
        roundsBeaten += 1;
        buildAliens();
        buildBunkers();
        enemyBullets.clear();
        allAliensDead = false;
    }

    //check if aliens reached player
    {
        if(alienOffsetY >= 500)
        {
            endGame();
        }
    }

    //update labels
    QString scoreTemp = "Score: " + QString::number(score);
    QString livesTemp = "Lives: " + QString::number(lives);
    updatelabel(scoreTemp, livesTemp);

    //applies speedup if enough aliens are dead
    calculateAlienSpeed();

    //update frame
    update();
    }
}

//grab random number between two values. Also takes a seed as input.
int getRand(int min, int max, unsigned int seed){
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch()) + seed;
    std::mt19937 gen(ms);
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}

//--------Here's the unpause function
void MainWindow::unpause() {
    startGame = true;
}
