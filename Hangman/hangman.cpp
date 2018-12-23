#include "hangman.h"
#include "ui_hangman.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

Hangman::Hangman(QWidget *parent) : QMainWindow(parent), ui(new Ui::Hangman) {
    ui->setupUi(this);
    addButonsToArray();
    connect(ui->connectButton, &QPushButton::clicked, this, &Hangman::connectButtonHit);
    connect(ui->hostLineEdit, &QLineEdit::returnPressed, ui->connectButton, &QPushButton::click);
    connect(ui->readyButton, &QPushButton::clicked, this, &Hangman::readyButtonHit);
    connect(ui->aButton, &QPushButton::clicked, this, &Hangman::aButtonHit);
    connect(ui->bButton, &QPushButton::clicked, this, &Hangman::bButtonHit);
    connect(ui->cButton, &QPushButton::clicked, this, &Hangman::cButtonHit);
    connect(ui->dButton, &QPushButton::clicked, this, &Hangman::dButtonHit);
    connect(ui->eButton, &QPushButton::clicked, this, &Hangman::eButtonHit);
    connect(ui->fButton, &QPushButton::clicked, this, &Hangman::fButtonHit);
    connect(ui->gButton, &QPushButton::clicked, this, &Hangman::gButtonHit);
    connect(ui->hButton, &QPushButton::clicked, this, &Hangman::hButtonHit);
    connect(ui->iButton, &QPushButton::clicked, this, &Hangman::iButtonHit);
    connect(ui->jButton, &QPushButton::clicked, this, &Hangman::jButtonHit);
    connect(ui->kButton, &QPushButton::clicked, this, &Hangman::kButtonHit);
    connect(ui->lButton, &QPushButton::clicked, this, &Hangman::lButtonHit);
    connect(ui->mButton, &QPushButton::clicked, this, &Hangman::mButtonHit);
    connect(ui->nButton, &QPushButton::clicked, this, &Hangman::nButtonHit);
    connect(ui->oButton, &QPushButton::clicked, this, &Hangman::oButtonHit);
    connect(ui->pButton, &QPushButton::clicked, this, &Hangman::pButtonHit);
    connect(ui->qButton, &QPushButton::clicked, this, &Hangman::qButtonHit);
    connect(ui->rButton, &QPushButton::clicked, this, &Hangman::rButtonHit);
    connect(ui->sButton, &QPushButton::clicked, this, &Hangman::sButtonHit);
    connect(ui->tButton, &QPushButton::clicked, this, &Hangman::tButtonHit);
    connect(ui->uButton, &QPushButton::clicked, this, &Hangman::uButtonHit);
    connect(ui->vButton, &QPushButton::clicked, this, &Hangman::vButtonHit);
    connect(ui->wButton, &QPushButton::clicked, this, &Hangman::wButtonHit);
    connect(ui->xButton, &QPushButton::clicked, this, &Hangman::xButtonHit);
    connect(ui->yButton, &QPushButton::clicked, this, &Hangman::yButtonHit);
    connect(ui->zButton, &QPushButton::clicked, this, &Hangman::zButtonHit);
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
    ui->rankingTextEdit->setEnabled(false);
}

void Hangman::readData() {
    QByteArray dane = sock->read(512);
    qDebug()<<dane;
    if(dane[0] == GAME_STARTED) {
        getWordAndRanking(dane, started);
    } else if (dane[0] == GET_FD) {
        getFdAndRanking(dane);
    } else if (dane[0] == GET_RANKING) {
        getRanking(dane);
    } else if (dane[0] == GAME_ENDED) {
        gameEndedAndRanking(dane);
    } else if (dane[0] >= 'A' && dane[0] <= 'Z') {
        disableButton(dane);
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

void Hangman::sendData(QString s) {
    auto data = (s).toUtf8();

    sock->write(data);
}

void Hangman::getWord(QByteArray dane) {
    array<int, 2> lengthAndShift = getMessageLengthAndShift(dane);
    int length = lengthAndShift[0];
    int shift = lengthAndShift[1];
    ui->lettersGroup->setEnabled(true);
    QByteArray word;
    for(int i = 0; i < length; i++) {
        word[i] = dane[i+2+shift];
    }
    ui->wordTextEdit->append(QString::fromUtf8(word).trimmed());
    ui->wordTextEdit->setAlignment(Qt::AlignCenter);
    QFont font("Noto Sans", 16);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    ui->wordTextEdit->setFont(font);
    ui->readyButton->setEnabled(false);
}

void Hangman::getFd(QByteArray dane) {
    array<int, 2> lengthAndShift = getMessageLengthAndShift(dane);
    int length = lengthAndShift[0];
    int shift = lengthAndShift[1];

    QByteArray fd;
    for(int i = 0; i < length; i++) {
        fd[i] = dane[i+2 + shift];
    }
    QString playerName = "PLAYER: " + QString::fromUtf8(fd).trimmed();
    ui->playerLabel->setText(playerName);
}

void Hangman::getRanking(QByteArray dane) {
    array<int, 2> lengthAndShift = getMessageLengthAndShift(dane);
    int length = lengthAndShift[0];
    int shift = lengthAndShift[1];

    QByteArray ranking;
    for(int i = 0; i< length; i++) {
        ranking[i] = dane[i+2+shift];
    }
    ui->rankingTextEdit->clear();
    ui->rankingTextEdit->append(QString::fromUtf8(ranking));
}

void Hangman::getFdAndRanking(QByteArray dane) {
    array<int, 2> lengthAndShift = getMessageLengthAndShift(dane);
    int length = lengthAndShift[0];
    int shift = lengthAndShift[1];

    QByteArray fd;
    for(int i = 0; i < length; i++) {
        fd[i] = dane[i+2 + shift];
    }
    QString playerName = "PLAYER: " + QString::fromUtf8(fd).trimmed();
    ui->playerLabel->setText(playerName);
    int lengthFd = length + 2 + shift;
    QByteArray ranking;
    for(int i = 0; i < dane.length(); i++) {
        ranking[i] = dane[i + lengthFd];
    }
    getRanking(ranking);
}

void Hangman::getWordAndRanking(QByteArray dane, bool isStarted) {
    if(!isStarted) {
        started = true;
        enableButtons();
        ui->wordTextEdit->clear();
    }
    array<int, 2> lengthAndShift = getMessageLengthAndShift(dane);
    int length = lengthAndShift[0];
    int shift = lengthAndShift[1];
    ui->lettersGroup->setEnabled(true);
    QByteArray word;
    for(int i = 0; i < length; i++) {
        word[i] = dane[i+2+shift];
    }
    ui->wordTextEdit->append(QString::fromUtf8(word).trimmed());
    ui->wordTextEdit->setAlignment(Qt::AlignCenter);
    QFont font("Noto Sans", 16);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    ui->wordTextEdit->setFont(font);
    ui->readyButton->setEnabled(false);
    int lengthWord = length +2 +shift;
    QByteArray ranking;
    for(int i = 0; i < dane.length(); i++) {
        ranking[i] = dane[i + lengthWord];
    }
    getRanking(ranking);
    ui->readyButton->setEnabled(false);
}


void Hangman::gameEndedAndRanking(QByteArray dane) {
    ui->readyButton->setEnabled(true);
    ui->wordTextEdit->clear();
    int win = dane[1] - '0';
    if(win == 1)
        ui->wordTextEdit->setText("WIN");
    else
        ui->wordTextEdit->setText("LOSS");

    QByteArray ranking;
    for(int i = 0; i < dane.length(); i++) {
        ranking[i] = dane[i + 2];
    }
    getRanking(ranking);
    ui->lettersGroup->setEnabled(false);
    started = false;
}

array<int,2> Hangman::getMessageLengthAndShift(QByteArray dane) {
    array<int, 2> lengthAndShift;
    int i = 1;
    int j = 0;
    string lengthString;
    while(dane[i] != ';') {
        lengthString[j] = dane[i];
        i++;
        j++;
    }
    lengthAndShift[0] = stoi(lengthString);
    lengthAndShift[1] = j;
    return lengthAndShift;
}

void Hangman::disableButton(QByteArray dane) {
    if(started) {
        int i = 0;
        for(char x = 'A'; x <= 'Z'; x++) {
            if(x == dane[0]) {
                letterButtons[i]->setEnabled(false);
                qDebug()<<"poszedl disable buttona";
                break;
            }
            i++;
        }
        QByteArray wordAndRanking;
        for(i = 0; i < dane.length(); i++) {
            wordAndRanking[i] = dane[i+1];
        }
        getWordAndRanking(wordAndRanking, started);
    }
}

void Hangman::enableButtons() {
    for(int i = 0; i < 26; i++) {
        letterButtons[i]->setEnabled(true);
    }
}

void Hangman::aButtonHit() {
    QString message = "A";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::bButtonHit() {
    QString message = "B";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::cButtonHit() {
    QString message = "C";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::dButtonHit() {
    QString message = "D";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::eButtonHit() {
    QString message = "E";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::fButtonHit() {
    QString message = "F";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::gButtonHit() {
    QString message = "G";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::hButtonHit() {
    QString message = "H";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::iButtonHit() {
    QString message = "I";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::jButtonHit() {
    QString message = "J";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::kButtonHit() {
    QString message = "K";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::lButtonHit() {
    QString message = "L";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::mButtonHit() {
    QString message = "M";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::nButtonHit() {
    QString message = "N";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::oButtonHit() {
    QString message = "O";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::pButtonHit() {
    QString message = "P";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::qButtonHit() {
    QString message = "Q";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::rButtonHit() {
    QString message = "R";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::sButtonHit() {
    QString message = "S";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::tButtonHit() {
    QString message = "T";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::uButtonHit() {
    QString message = "U";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::vButtonHit() {
    QString message = "V";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::wButtonHit() {
    QString message = "W";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::xButtonHit() {
    QString message = "X";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::yButtonHit() {
    QString message = "Y";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::zButtonHit() {
    QString message = "Z";
    message = message + QString::number(QDateTime::currentMSecsSinceEpoch()) + ";";
    sendData(message);
}

void Hangman::addButonsToArray() {
    letterButtons[0] = ui->aButton;
    letterButtons[1] = ui->bButton;
    letterButtons[2] = ui->cButton;
    letterButtons[3] = ui->dButton;
    letterButtons[4] = ui->eButton;
    letterButtons[5] = ui->fButton;
    letterButtons[6] = ui->gButton;
    letterButtons[7] = ui->hButton;
    letterButtons[8] = ui->iButton;
    letterButtons[9] = ui->jButton;
    letterButtons[10] = ui->kButton;
    letterButtons[11] = ui->lButton;
    letterButtons[12] = ui->mButton;
    letterButtons[13] = ui->nButton;
    letterButtons[14] = ui->oButton;
    letterButtons[15] = ui->pButton;
    letterButtons[16] = ui->qButton;
    letterButtons[17] = ui->rButton;
    letterButtons[18] = ui->sButton;
    letterButtons[19] = ui->tButton;
    letterButtons[20] = ui->uButton;
    letterButtons[21] = ui->vButton;
    letterButtons[22] = ui->wButton;
    letterButtons[23] = ui->xButton;
    letterButtons[24] = ui->yButton;
    letterButtons[25] = ui->zButton;
}
