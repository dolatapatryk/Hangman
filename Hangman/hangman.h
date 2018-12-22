#ifndef HANGMAN_H
#define HANGMAN_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QtGui>
#include<QtWidgets>
#include<QtNetwork>
#include <array>
using namespace std;

namespace Ui {
class Hangman;
}

class Hangman : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hangman(QWidget *parent = nullptr);
    ~Hangman();

protected:
    QTcpSocket * sock;
    QTimer * connTimeoutTimer;
    char GAME_STARTED = '1';
    char READY = '1';
    char GET_FD = '2';
    char GET_RANKING = '3';
    int count = 0;
    void connectButtonHit();
    void socketConnected();
    void readData();
    void readyButtonHit();
    void sendData(char c);
    void getWord(QByteArray dane);
    void getFd(QByteArray dane);
    void getRanking(QByteArray dane);
    void getFdAndRanking(QByteArray dane);
    void getWordAndRanking(QByteArray dane);
    array<int, 2> getMessageLengthAndShift(QByteArray dane);
private:
    Ui::Hangman *ui;
    std::string picspath = "./pics/";
};

#endif // HANGMAN_H
