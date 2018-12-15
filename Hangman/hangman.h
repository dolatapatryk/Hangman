#ifndef HANGMAN_H
#define HANGMAN_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

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
    void connectButtonHit();
    void socketConnected();
    void readData();
    void readyButtonHit();
    void sendData(char c);

private:
    Ui::Hangman *ui;
};

#endif // HANGMAN_H
