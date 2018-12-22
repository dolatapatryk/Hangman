#include "hangman.h"
#include "ui_hangman.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

Hangman::Hangman(QWidget *parent) : QMainWindow(parent), ui(new Ui::Hangman) {
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &Hangman::connectButtonHit);
    connect(ui->hostLineEdit, &QLineEdit::returnPressed, ui->connectButton, &QPushButton::click);
    connect(ui->readyButton, &QPushButton::clicked, this, &Hangman::readyButtonHit);
}

Hangman::~Hangman() {
    delete ui;
}

void Hangman::connectButtonHit() {
    ui->connectGroup->setEnabled(false);

    auto host = ui->hostLineEdit->text();
    int port = ui->portSpinBox->text().toInt();

    sock = new QTcpSocket(this);
    sock->connectToHost(host, port);

    connect(sock, &QTcpSocket::connected, this, &Hangman::socketConnected);
    connect(sock, &QTcpSocket::readyRead, this, &Hangman::readData);

    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);
    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        connTimeoutTimer->deleteLater();
        ui->connectGroup->setEnabled(true);
        QMessageBox::critical(this, "Error", "Connect timed out");
     });
     connTimeoutTimer->start(3000);
}

void Hangman::socketConnected() {
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    ui->hangmanGroup->setEnabled(true);
    ui->wordTextEdit->setEnabled(false);
    ui->rankingList->setEnabled(false);
}

void Hangman::readData() {
    QByteArray dane = sock->read(512);
    if(dane[0] == GAME_STARTED) {
        getWord(dane);
    } else if (dane[0] == GET_FD) {
        getFd(dane);
    }
}

void Hangman::readyButtonHit() {
    sendData(READY);
}

void Hangman::sendData(char c) {
    QString str = "";
    str += c;
    auto data = (str).toUtf8();

    sock->write(data);
}

void Hangman::getWord(QByteArray dane) {
    ui->lettersGroup->setEnabled(true);
    int length = dane[1];
    QByteArray word;
    for(int i = 0; i < length; i++) {
        word[i] = dane[i+2];
    }
    ui->wordTextEdit->append(QString::fromUtf8(word).trimmed());
    ui->wordTextEdit->setAlignment(Qt::AlignCenter);
    QFont font("Noto Sans", 16);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    ui->wordTextEdit->setFont(font);
}

void Hangman::getFd(QByteArray dane) {
    int length = dane[1];
    QByteArray fd;
    for(int i = 0; i < length; i++) {
        fd[i] = dane[i+2];
    }
    QString playerName = "GRACZ: " + QString::fromUtf8(fd).trimmed();
    ui->playerLabel->setText(playerName);
}
