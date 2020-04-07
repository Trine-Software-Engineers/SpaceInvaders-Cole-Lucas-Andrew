#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMultimedia/QMediaPlayer>


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



struct alien
{
    int posX = 0;
    int posY = 0;
    int anchorX = 0;
    int anchorY = 0;
    bool alive = true;
};

struct bunker
{
    int posX = 0;
    int posY = 0;
    int health = 10;
    bool alive = true;
};

struct enemyBullet
{
    int bulletX = 0;
    int bulletY = 0;
    bool bulletInPursuit = true;
};

QVector<alien> aliens;
QVector<bunker> bunkers;
QVector<enemyBullet> enemyBullets;

void buildAliens();
void buildBunkers();
int getRand(int min, int max, unsigned int seed);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup time between frames
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(game()));
    timer->start(MSbetweenFrames);
    buildAliens();
    buildBunkers();
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
    //clear vector
    bunkers.clear();

    //fill vector
    for(int z = 0; z < 4; z++)
    {
        bunker newBunker;
        newBunker.posX = 50 + (z*210);
        newBunker.posY = 600;
        bunkers.push_back(newBunker);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //draw player
    painter.setBrush( Qt::green );
    painter.drawRect(playerX, playerY, 50, 50);

    //draw aliens
    for(int x = 0; x < 50; x++)
    {
        if(aliens[x].alive)
        {
            painter.setBrush( Qt::red );
            painter.drawRect(aliens[x].posX,aliens[x].posY, 30, 30);
        }
    }

    //draw bunkers
    for(int z = 0; z < 4; z++)
    {
        if(bunkers[z].alive)
        {
            int colorMultiplier = bunkers[z].health * 25;
            painter.setBrush( QColor(colorMultiplier, colorMultiplier, colorMultiplier, 100) );
            painter.drawRect(bunkers[z].posX, bunkers[z].posY, 75, 75);
        }
    }

    //draw bullet
    if(bulletInPursuit)
    {
        painter.setBrush( Qt::black );
        painter.drawRect(bulletX, bulletY, 10, 25);
    }

    //draw enemy bullet
    for(int j = 0; j < enemyBullets.size(); j++)
    {
        if(enemyBullets[j].bulletInPursuit)
        {
            painter.setBrush( Qt::red );
            painter.drawRect(enemyBullets[j].bulletX, enemyBullets[j].bulletY, 10, 25);
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
    ui->scoreLabel->setText(score);
    ui->livesLabel->setText(lives);
}


void endGame()
{
    MSbetweenFrames = INT_MAX;
    QMessageBox msgBox;
    msgBox.setWindowTitle("Space Invaders");
    msgBox.setText("Game Over!\nFinal Score: " + QString::number(score) + "\nPlay Again?");

    QAbstractButton *yesButton = msgBox.addButton("Yes", QMessageBox::ActionRole);
    QAbstractButton *quitButton = msgBox.addButton("Quit", QMessageBox::ActionRole);
    msgBox.exec();
    if(msgBox.clickedButton() == yesButton)
        {
            score = 0;
            lives = 3;
            buildAliens();
            buildBunkers();
            alienOffsetY = 0;
            roundsBeaten = 0;
            MSbetweenFrames = 16.666666667;
        }
    else if(msgBox.clickedButton() == quitButton)
        {
            QApplication::quit();
        }
}

//runs every frame
void MainWindow::game()
{
    //check if all aliens are dead
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
        bool bulletWithinBunkerX = false;
        bool bulletWithinBunkerY = false;

        for(int x = 0; x < 4; x++)
        {
            if(bunkers[x].alive)
            {
                if((bulletX > (bunkers[x].posX - 10)) && (bulletX < (bunkers[x].posX + 75)))
                {
                    bulletWithinBunkerX = true;
                }
                if((bulletY < (bunkers[x].posY + 75)) && (bulletY > (bunkers[x].posY - 20)))
                {
                    bulletWithinBunkerY = true;
                }

                if(bulletWithinBunkerX && bulletWithinBunkerY)
                {
                    bunkers[x].health -= 1;
                    if(bunkers[x].health < 1) bunkers[x].alive = false;

                    bulletInPursuit = false;
                }

                bulletWithinBunkerX = false;
                bulletWithinBunkerY = false;
            }
        }
    }

    //check for ENEMY bullet impact on bunker
    for(int c = 0; c < enemyBullets.size(); c++)
    {
        if(enemyBullets[c].bulletInPursuit)
        {
            for(int x = 0; x < 4; x++)
            {
                if(bunkers[x].alive)
                {
                    bool bulletWithinBunkerX = false;
                    bool bulletWithinBunkerY = false;

                    int bX = enemyBullets[c].bulletX;
                    int bY = enemyBullets[c].bulletY;

                    if((bX > (bunkers[x].posX - 10)) && (bX < (bunkers[x].posX + 75)))
                    {
                        bulletWithinBunkerX = true;
                    }
                    if((bY < (bunkers[x].posY + 75)) && (bY > (bunkers[x].posY - 20)))
                    {
                        bulletWithinBunkerY = true;
                    }

                    if(bulletWithinBunkerX && bulletWithinBunkerY)
                    {
                        bunkers[x].health -= 1;
                        if(bunkers[x].health < 1) bunkers[x].alive = false;

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
    if(getRand(0, 120, 9) == 1)
    {
        spawnEnemyBullet();
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
            if(bY > (playerY - 20) && bY < (playerY + 50)) bulletInPlayerY = true;

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


    calculateAlienSpeed();

    //update frame
    update();
}

//grab random number between two values. Also takes a seed as input.
int getRand(int min, int max, unsigned int seed){
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch()) + seed;
    std::mt19937 gen(ms);
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}
