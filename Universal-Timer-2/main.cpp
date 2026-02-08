#include "UniversalTimer2.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UniversalTimer2 w;
    w.show();
    return a.exec();
}
