#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>

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

private:
    Ui::MainWindow *ui;

    void startGame();

    void endGame();

    void setUIVisibility(bool t);

};
#endif // MAINWINDOW_H
