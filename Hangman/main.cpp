#include "hangman.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hangman w;
    w.show();

    return a.exec();
}
