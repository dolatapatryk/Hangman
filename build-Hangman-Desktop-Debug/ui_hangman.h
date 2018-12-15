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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
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
    QPushButton *readyButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Hangman)
    {
        if (Hangman->objectName().isEmpty())
            Hangman->setObjectName(QString::fromUtf8("Hangman"));
        Hangman->resize(446, 553);
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
        readyButton = new QPushButton(hangmanGroup);
        readyButton->setObjectName(QString::fromUtf8("readyButton"));
        readyButton->setGeometry(QRect(320, 170, 88, 34));
        Hangman->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Hangman);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 446, 30));
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
        readyButton->setText(QApplication::translate("Hangman", "Ready", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Hangman: public Ui_Hangman {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANGMAN_H
