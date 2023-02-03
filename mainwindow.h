#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QRect>

#include <iostream>

#include <card.h>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startGame_clicked();

    void on_addCard_clicked();

    void on_stand_clicked();

    void on_bet_textChanged();

private:
    Ui::MainWindow *ui;

    void startGame();

    void endGame();

    void setUIVisibility(bool t);

    void animation(QRect qRect, int x, QList<QLabel*> *list);

    vector<card> deck;
    int probableScorePlayer;
    QList<QLabel*> playerCard;
    QList<QLabel*> dealerCard;

    int cash;

    int bid;

};
#endif // MAINWINDOW_H
