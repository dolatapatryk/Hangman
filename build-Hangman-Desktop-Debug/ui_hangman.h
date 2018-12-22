/********************************************************************************
** Form generated from reading UI file 'hangman.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANGMAN_H
#define UI_HANGMAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Hangman
{
public:
    QWidget *centralWidget;
    QGroupBox *connectGroup;
    QLineEdit *hostLineEdit;
    QSpinBox *portSpinBox;
    QPushButton *connectButton;
    QGroupBox *hangmanGroup;
    QGroupBox *lettersGroup;
    QPushButton *aButton;
    QPushButton *bButton;
    QPushButton *cButton;
    QPushButton *dButton;
    QPushButton *eButton;
    QPushButton *fButton;
    QPushButton *gButton;
    QPushButton *hButton;
    QPushButton *iButton;
    QPushButton *jButton;
    QPushButton *kButton;
    QPushButton *nButton;
    QPushButton *oButton;
    QPushButton *pButton;
    QPushButton *qButton;
    QPushButton *rButton;
    QPushButton *lButton;
    QPushButton *mButton;
    QPushButton *sButton;
    QPushButton *uButton;
    QPushButton *tButton;
    QPushButton *vButton;
    QPushButton *wButton;
    QPushButton *xButton;
    QPushButton *yButton;
    QPushButton *zButton;
    QPushButton *readyButton;
    QListView *rankingList;
    QLabel *rankingLabel;
    QLabel *wordLabel;
    QTextEdit *wordTextEdit;
    QPushButton *testButton;
    QLabel *picLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Hangman)
    {
        if (Hangman->objectName().isEmpty())
            Hangman->setObjectName(QString::fromUtf8("Hangman"));
        Hangman->resize(769, 553);
        centralWidget = new QWidget(Hangman);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        connectGroup = new QGroupBox(centralWidget);
        connectGroup->setObjectName(QString::fromUtf8("connectGroup"));
        connectGroup->setGeometry(QRect(10, 0, 421, 51));
        hostLineEdit = new QLineEdit(connectGroup);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));
        hostLineEdit->setGeometry(QRect(10, 10, 201, 32));
        portSpinBox = new QSpinBox(connectGroup);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        portSpinBox->setGeometry(QRect(230, 10, 71, 32));
        portSpinBox->setMinimum(1);
        portSpinBox->setMaximum(65535);
        portSpinBox->setValue(1234);
        connectButton = new QPushButton(connectGroup);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(320, 10, 88, 34));
        hangmanGroup = new QGroupBox(centralWidget);
        hangmanGroup->setObjectName(QString::fromUtf8("hangmanGroup"));
        hangmanGroup->setEnabled(false);
        hangmanGroup->setGeometry(QRect(10, 70, 421, 421));
        lettersGroup = new QGroupBox(hangmanGroup);
        lettersGroup->setObjectName(QString::fromUtf8("lettersGroup"));
        lettersGroup->setEnabled(false);
        lettersGroup->setGeometry(QRect(20, 260, 231, 151));
        lettersGroup->setAutoFillBackground(false);
        lettersGroup->setFlat(false);
        aButton = new QPushButton(lettersGroup);
        aButton->setObjectName(QString::fromUtf8("aButton"));
        aButton->setGeometry(QRect(10, 30, 21, 26));
        bButton = new QPushButton(lettersGroup);
        bButton->setObjectName(QString::fromUtf8("bButton"));
        bButton->setGeometry(QRect(40, 30, 21, 26));
        cButton = new QPushButton(lettersGroup);
        cButton->setObjectName(QString::fromUtf8("cButton"));
        cButton->setGeometry(QRect(70, 30, 21, 26));
        dButton = new QPushButton(lettersGroup);
        dButton->setObjectName(QString::fromUtf8("dButton"));
        dButton->setGeometry(QRect(100, 30, 21, 26));
        eButton = new QPushButton(lettersGroup);
        eButton->setObjectName(QString::fromUtf8("eButton"));
        eButton->setGeometry(QRect(130, 30, 21, 26));
        fButton = new QPushButton(lettersGroup);
        fButton->setObjectName(QString::fromUtf8("fButton"));
        fButton->setGeometry(QRect(160, 30, 21, 26));
        gButton = new QPushButton(lettersGroup);
        gButton->setObjectName(QString::fromUtf8("gButton"));
        gButton->setGeometry(QRect(190, 30, 21, 26));
        hButton = new QPushButton(lettersGroup);
        hButton->setObjectName(QString::fromUtf8("hButton"));
        hButton->setGeometry(QRect(10, 60, 21, 26));
        iButton = new QPushButton(lettersGroup);
        iButton->setObjectName(QString::fromUtf8("iButton"));
        iButton->setGeometry(QRect(40, 60, 21, 26));
        jButton = new QPushButton(lettersGroup);
        jButton->setObjectName(QString::fromUtf8("jButton"));
        jButton->setGeometry(QRect(70, 60, 21, 26));
        kButton = new QPushButton(lettersGroup);
        kButton->setObjectName(QString::fromUtf8("kButton"));
        kButton->setGeometry(QRect(100, 60, 21, 26));
        nButton = new QPushButton(lettersGroup);
        nButton->setObjectName(QString::fromUtf8("nButton"));
        nButton->setGeometry(QRect(190, 60, 21, 26));
        oButton = new QPushButton(lettersGroup);
        oButton->setObjectName(QString::fromUtf8("oButton"));
        oButton->setGeometry(QRect(10, 90, 21, 26));
        pButton = new QPushButton(lettersGroup);
        pButton->setObjectName(QString::fromUtf8("pButton"));
        pButton->setGeometry(QRect(40, 90, 21, 26));
        qButton = new QPushButton(lettersGroup);
        qButton->setObjectName(QString::fromUtf8("qButton"));
        qButton->setGeometry(QRect(70, 90, 21, 26));
        rButton = new QPushButton(lettersGroup);
        rButton->setObjectName(QString::fromUtf8("rButton"));
        rButton->setGeometry(QRect(100, 90, 21, 26));
        lButton = new QPushButton(lettersGroup);
        lButton->setObjectName(QString::fromUtf8("lButton"));
        lButton->setGeometry(QRect(130, 60, 21, 26));
        mButton = new QPushButton(lettersGroup);
        mButton->setObjectName(QString::fromUtf8("mButton"));
        mButton->setGeometry(QRect(160, 60, 21, 26));
        sButton = new QPushButton(lettersGroup);
        sButton->setObjectName(QString::fromUtf8("sButton"));
        sButton->setGeometry(QRect(130, 90, 21, 26));
        uButton = new QPushButton(lettersGroup);
        uButton->setObjectName(QString::fromUtf8("uButton"));
        uButton->setGeometry(QRect(190, 90, 21, 26));
        tButton = new QPushButton(lettersGroup);
        tButton->setObjectName(QString::fromUtf8("tButton"));
        tButton->setGeometry(QRect(160, 90, 21, 26));
        vButton = new QPushButton(lettersGroup);
        vButton->setObjectName(QString::fromUtf8("vButton"));
        vButton->setGeometry(QRect(10, 120, 21, 26));
        wButton = new QPushButton(lettersGroup);
        wButton->setObjectName(QString::fromUtf8("wButton"));
        wButton->setGeometry(QRect(40, 120, 21, 26));
        xButton = new QPushButton(lettersGroup);
        xButton->setObjectName(QString::fromUtf8("xButton"));
        xButton->setGeometry(QRect(70, 120, 21, 26));
        yButton = new QPushButton(lettersGroup);
        yButton->setObjectName(QString::fromUtf8("yButton"));
        yButton->setGeometry(QRect(100, 120, 21, 26));
        zButton = new QPushButton(lettersGroup);
        zButton->setObjectName(QString::fromUtf8("zButton"));
        zButton->setGeometry(QRect(130, 120, 21, 26));
        readyButton = new QPushButton(hangmanGroup);
        readyButton->setObjectName(QString::fromUtf8("readyButton"));
        readyButton->setGeometry(QRect(20, 230, 88, 34));
        rankingList = new QListView(hangmanGroup);
        rankingList->setObjectName(QString::fromUtf8("rankingList"));
        rankingList->setGeometry(QRect(260, 220, 141, 192));
        rankingLabel = new QLabel(hangmanGroup);
        rankingLabel->setObjectName(QString::fromUtf8("rankingLabel"));
        rankingLabel->setGeometry(QRect(260, 200, 58, 18));
        wordLabel = new QLabel(hangmanGroup);
        wordLabel->setObjectName(QString::fromUtf8("wordLabel"));
        wordLabel->setGeometry(QRect(30, 30, 58, 18));
        wordTextEdit = new QTextEdit(hangmanGroup);
        wordTextEdit->setObjectName(QString::fromUtf8("wordTextEdit"));
        wordTextEdit->setGeometry(QRect(10, 50, 281, 31));
        testButton = new QPushButton(centralWidget);
        testButton->setObjectName(QString::fromUtf8("testButton"));
        testButton->setGeometry(QRect(310, 50, 80, 26));
        picLabel = new QLabel(centralWidget);
        picLabel->setObjectName(QString::fromUtf8("picLabel"));
        picLabel->setEnabled(true);
        picLabel->setGeometry(QRect(510, 100, 191, 251));
        picLabel->setPixmap(QPixmap(QString::fromUtf8("pics/1.png")));
        Hangman->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Hangman);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 769, 23));
        Hangman->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Hangman);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Hangman->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Hangman);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Hangman->setStatusBar(statusBar);

        retranslateUi(Hangman);

        QMetaObject::connectSlotsByName(Hangman);
    } // setupUi

    void retranslateUi(QMainWindow *Hangman)
    {
        Hangman->setWindowTitle(QApplication::translate("Hangman", "Hangman", nullptr));
        connectGroup->setTitle(QString());
        hostLineEdit->setText(QApplication::translate("Hangman", "localhost", nullptr));
        connectButton->setText(QApplication::translate("Hangman", "connect", nullptr));
        hangmanGroup->setTitle(QString());
        lettersGroup->setTitle(QString());
        aButton->setText(QApplication::translate("Hangman", "A", nullptr));
        bButton->setText(QApplication::translate("Hangman", "B", nullptr));
        cButton->setText(QApplication::translate("Hangman", "C", nullptr));
        dButton->setText(QApplication::translate("Hangman", "D", nullptr));
        eButton->setText(QApplication::translate("Hangman", "E", nullptr));
        fButton->setText(QApplication::translate("Hangman", "F", nullptr));
        gButton->setText(QApplication::translate("Hangman", "G", nullptr));
        hButton->setText(QApplication::translate("Hangman", "H", nullptr));
        iButton->setText(QApplication::translate("Hangman", "I", nullptr));
        jButton->setText(QApplication::translate("Hangman", "J", nullptr));
        kButton->setText(QApplication::translate("Hangman", "K", nullptr));
        nButton->setText(QApplication::translate("Hangman", "N", nullptr));
        oButton->setText(QApplication::translate("Hangman", "O", nullptr));
        pButton->setText(QApplication::translate("Hangman", "P", nullptr));
        qButton->setText(QApplication::translate("Hangman", "Q", nullptr));
        rButton->setText(QApplication::translate("Hangman", "R", nullptr));
        lButton->setText(QApplication::translate("Hangman", "L", nullptr));
        mButton->setText(QApplication::translate("Hangman", "M", nullptr));
        sButton->setText(QApplication::translate("Hangman", "S", nullptr));
        uButton->setText(QApplication::translate("Hangman", "U", nullptr));
        tButton->setText(QApplication::translate("Hangman", "T", nullptr));
        vButton->setText(QApplication::translate("Hangman", "V", nullptr));
        wButton->setText(QApplication::translate("Hangman", "W", nullptr));
        xButton->setText(QApplication::translate("Hangman", "X", nullptr));
        yButton->setText(QApplication::translate("Hangman", "Y", nullptr));
        zButton->setText(QApplication::translate("Hangman", "Z", nullptr));
        readyButton->setText(QApplication::translate("Hangman", "Ready", nullptr));
        rankingLabel->setText(QApplication::translate("Hangman", "Ranking", nullptr));
        wordLabel->setText(QApplication::translate("Hangman", "WORD", nullptr));
        testButton->setText(QApplication::translate("Hangman", "test", nullptr));
        picLabel->setText(QApplication::translate("Hangman", "xddd", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Hangman: public Ui_Hangman {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANGMAN_H
