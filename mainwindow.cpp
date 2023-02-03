#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <fstream>
#include <vector>

#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include <QFile>
#include <QLabel>
#include <QStyle>
#include <QSize>
#include <QPropertyAnimation>

#include "card.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cash = 500;
    startGame();
}

void MainWindow::startGame(){
    //        Diamonds (Бубы)
    //        Hearts (Черви)
    //        Clubs (Крест)
    //        Spades (Пики)
    setUIVisibility(false);
    deck.clear();
    probableScorePlayer = 0;
    QFile file(":/res/deck.txt");
    if(file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!file.atEnd()) {
            QString str = file.readLine();
            QStringList lst = str.split(";");
            deck.push_back(card(lst[0].toInt(), lst[1].toInt(), lst[2].toStdString()));
        }
    }
    file.close();

    for (int i = 0; i < playerCard.size(); ++i) {
        playerCard[i]->hide();
        playerCard[i]->close();
    }
    for (int i = 0; i < dealerCard.size(); ++i) {
        dealerCard[i]->hide();
        dealerCard[i]->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUIVisibility(bool t){
    ui->startGame->setVisible(!t);
    ui->Score_1->setVisible(t);
    ui->Score_2->setVisible(t);
    ui->dealerFirstCard->setVisible(false);
    ui->playerFirstCard->setVisible(false);
    ui->addCard->setVisible(t);
    ui->addCard->setIcon(QIcon("://res/button/hit.png"));
    ui->dScore->setVisible(t);
    ui->pScore->setVisible(t);
    ui->stand->setVisible(t);
    ui->stand->setIcon(QIcon("://res/button/pass.png"));
    ui->deckCard->setVisible(t);
    ui->playerCash->setText(QString::number(cash));
    ui->bet->setEnabled(!t);
}

void MainWindow::animation(QRect qRect, int x, QList<QLabel*> *list){
    QLabel *c = new QLabel("c", this);
    QString cc = ":/res/img/v1/" + QString::fromStdString(deck[x].getPathToFile()) + ".png";
    c->setIndent(100);
    c->resize(93, 132);
    c->setVisible(true);
    c->setPixmap(QPixmap(cc).scaled(93, 132, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    c->show();
    list->append(c);
    QPropertyAnimation *animation = new QPropertyAnimation(c, "geometry");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->setStartValue(ui->deckCard->geometry());
    animation->setEndValue(qRect);
    animation->start(QAbstractAnimation::KeepWhenStopped);
}

void MainWindow::on_startGame_clicked()
{
    bid = ui->bet->toPlainText().toInt();
    cash -= bid;
    setUIVisibility(true);

    int x = rand() % deck.size();

    ui->dScore->setText(QString::number(deck[x].getValue()));

    animation(ui->dealerFirstCard->geometry(), x, &dealerCard);

    deck.erase(deck.begin() + x);

    x = rand() % deck.size();
    if (deck[x].getValue() == 11) probableScorePlayer = 1;

    animation(ui->playerFirstCard->geometry(), x, &playerCard);

    ui->pScore->setText(QString::number(deck[x].getValue()));

    deck.erase(deck.begin() + x);

    x = rand() % deck.size();

    QRect dCard(QPoint(ui->playerFirstCard->geometry().x() + 50, ui->playerFirstCard->geometry().y()), QSize(93, 132));

    animation(dCard, x, &playerCard);

    switch (probableScorePlayer) {
    case 0:
        if (deck[x].getValue() == 11){
            probableScorePlayer = 1 + ui->pScore->text().toInt();
        }
        break;
    case 1:
        if (deck[x].getValue() == 11){
            probableScorePlayer = 2;
        } else probableScorePlayer += deck[x].getValue();
        break;
    }


    if (probableScorePlayer == 2){
        ui->pScore->setText(QString::number(probableScorePlayer));
    } else {
        int score = deck[x].getValue() + ui->pScore->text().toInt();
        ui->pScore->setText(QString::number(score));
    }

    if (ui->pScore->text().toInt() == 21){
        bid *= 2.5;
        cash += bid;
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
    }

    deck.erase(deck.begin() + x);
}


void MainWindow::on_addCard_clicked()
{
    ui->bet->setEnabled(false);

    ui->playerCash->setText(QString::number(cash));

    int x = rand() % deck.size();

    if (probableScorePlayer != 0 && probableScorePlayer != 2){

        if (deck[x].getValue() > (21 - ui->pScore->text().toInt()))
            ui->pScore->setText(QString::number(probableScorePlayer));

    }

    QRect dCard;
    dCard.setX(playerCard[playerCard.size() - 1]->geometry().x() + 50);
    dCard.setY(playerCard[playerCard.size() - 1]->geometry().y());
    dCard.setSize(QSize(93, 132));

    animation(dCard, x, &playerCard);

    int score = deck[x].getValue() + ui->pScore->text().toInt();
    ui->pScore->setText(QString::number(score));


    deck.erase(deck.begin() + x);

    if (ui->pScore->text().toInt() > 21){
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Lose");
        msgBox.exec();
        if (cash == 0){
            msgBox.setText("You have no money. Restart the game");
            msgBox.exec();
            cash = 500;
        }
        startGame();
    }
    if (ui->pScore->text().toInt() == 21){
        bid *= 2;
        cash += bid;
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
    }
}

void MainWindow::on_stand_clicked()
{
    ui->bet->setEnabled(false);
    bool t = true;
    int x = rand() % deck.size();

    QRect dCard(QPoint(ui->dealerFirstCard->geometry().x() + 50, ui->dealerFirstCard->geometry().y()), QSize(93, 132));

    animation(dCard, x, &dealerCard);

    int score = deck[x].getValue() + ui->dScore->text().toInt();
    ui->dScore->setText(QString::number(score));

    if (ui->dScore->text().toInt() > 16) {
        t = false;
    }

    deck.erase(deck.begin() + x);
    while (t) {
        x = rand() % deck.size();


        dCard.setX(dCard.x() + 50);
        dCard.setY(dCard.y());
        dCard.setSize(QSize(93, 132));

        animation(dCard, x, &dealerCard);

        int score = deck[x].getValue() + ui->dScore->text().toInt();
        ui->dScore->setText(QString::number(score));

        deck.erase(deck.begin() + x);

        if (score > 16) {
            t = false;
        }
    }
    endGame();
}

void MainWindow::endGame(){
    deck.clear();
    if (ui->dScore->text().toInt() > 21){
        bid *= 2;
        cash += bid;
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
    } else {
        if (ui->dScore->text().toInt() > ui->pScore->text().toInt()){
            QMessageBox msgBox;
            msgBox.setText("Game finish. You Lose");
            msgBox.exec();
            if (cash == 0){
                msgBox.setText("You have no money. Restart the game");
                msgBox.exec();
                cash = 500;
            }
            startGame();
        } else if (ui->dScore->text().toInt() == ui->pScore->text().toInt()){
            cash += bid;
            QMessageBox msgBox;
            msgBox.setText("Game finish. Draw");
            msgBox.exec();
            startGame();
        } else {
            bid *= 2;
            cash += bid;
            QMessageBox msgBox;
            msgBox.setText("Game finish. You Win");
            msgBox.exec();
            startGame();
        }
    }
}


void MainWindow::on_bet_textChanged()
{
    if (ui->bet->toPlainText().toInt() > cash){
        ui->bet->setPlainText("");
        QMessageBox msgBox;
        msgBox.setText("Make a bet less your cash");
        msgBox.exec();
    }
}

