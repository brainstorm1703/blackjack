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

#include "card.h"

using namespace std;
vector<card> deck;
int probableScorePlayer;
int probableScoreDealer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startGame();
}

void MainWindow::startGame(){
    //        Diamonds (Бубы)
    //        Hearts (Черви)
    //        Clubs (Крест)
    //        Spades (Пики)
    deck.clear();
    probableScorePlayer = 0;
    probableScoreDealer = 0;
    QFile file(":/res/deck.txt");
    if(file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!file.atEnd()) {
            QString str = file.readLine();
            QStringList lst = str.split(";");
            deck.push_back(card(lst[0].toInt(), lst[1].toInt(), lst[2].toStdString()));
        }
    }
    file.close();
    setUIVisibility(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUIVisibility(bool t){
    ui->addCard->setVisible(t);
    ui->dealerScore->setVisible(t);
    ui->playerScore->setVisible(t);
    ui->dScore->setVisible(t);
    ui->pScore->setVisible(t);
    ui->stand->setVisible(t);
}

void MainWindow::on_startGame_clicked()
{
    ui->startGame->setVisible(false);
    setUIVisibility(true);

    int x = rand() % deck.size();
    if (deck[x].getValue() == 11) probableScoreDealer = 1;

    ui->dealerScore->setText(QString::fromStdString(deck[x].getPathToFile()));
    ui->dScore->setText(QString::number(deck[x].getValue()));

    deck.erase(deck.begin() + x);

    x = rand() % deck.size();
    if (deck[x].getValue() == 11) probableScorePlayer = 1;

    ui->playerScore->setText(QString::fromStdString(deck[x].getPathToFile()));
    ui->pScore->setText(QString::number(deck[x].getValue()));

    x = rand() % deck.size();

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

    ui->playerScore->setText(ui->playerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
    int score = deck[x].getValue() + ui->pScore->text().toInt();
    ui->pScore->setText(QString::number(score));

    if (score == 21){
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
        on_startGame_clicked();
    }

    deck.erase(deck.begin() + x);
}


void MainWindow::on_addCard_clicked()
{

    int x = rand() % deck.size();

    if (probableScorePlayer != 0){
        if (deck[x].getValue() > (21 - ui->pScore->text().toInt()))
            ui->pScore->setText(QString::number(probableScorePlayer));
    }

    ui->playerScore->setText(ui->playerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
    int score = deck[x].getValue() + ui->pScore->text().toInt();
    ui->pScore->setText(QString::number(score));


    deck.erase(deck.begin() + x);

    if (score > 21){
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Lose");
        msgBox.exec();
        startGame();
        on_startGame_clicked();
    }
    if (score == 21){
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
        on_startGame_clicked();
    }
}

void MainWindow::on_stand_clicked()
{
    bool t = true;
    int x = rand() % deck.size();
    switch (probableScoreDealer) {
    case 0:
        if (deck[x].getValue() == 11){
            probableScoreDealer = 1 + ui->dScore->text().toInt();
        }
        break;
    case 1:
        if (deck[x].getValue() == 11){
            probableScoreDealer = 2;
        } else probableScoreDealer += deck[x].getValue();
        break;
    }

    switch (probableScoreDealer) {
    case 0:
        ui->dealerScore->setText(ui->dealerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
        ui->dScore->setText(QString::number(deck[x].getValue() + ui->dScore->text().toInt()));
        break;
    case 1:
        if (deck[x].getValue() > (21 - ui->dScore->text().toInt())){
            ui->dealerScore->setText(ui->dealerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
            ui->dScore->setText(QString::number(probableScoreDealer + deck[x].getValue()));
        }
        break;
    case 2:
        ui->dealerScore->setText(ui->dealerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
        ui->dScore->setText(QString::number(probableScoreDealer));
        break;
    }

    if (ui->dScore->text().toInt() > 16) {
        t = false;
    }

    deck.erase(deck.begin() + x);
    while (t) {
        x = rand() % deck.size();

        if (probableScoreDealer != 0){
            if (deck[x].getValue() > (21 - ui->dScore->text().toInt()))
                ui->dScore->setText(QString::number(probableScoreDealer));
        }

        ui->dealerScore->setText(ui->dealerScore->text() + " + " + QString::fromStdString(deck[x].getPathToFile()));
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
        QMessageBox msgBox;
        msgBox.setText("Game finish. You Win");
        msgBox.exec();
        startGame();
        on_startGame_clicked();
    } else {
        if (ui->dScore->text().toInt() > ui->pScore->text().toInt()){
            QMessageBox msgBox;
            msgBox.setText("Game finish. You Lose");
            msgBox.exec();
            startGame();
            on_startGame_clicked();
        } else {
            QMessageBox msgBox;
            msgBox.setText("Game finish. You Win");
            msgBox.exec();
            startGame();
            on_startGame_clicked();
        }
    }
}
