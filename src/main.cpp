#include "UniversalTimer2.h"
#include "Logger.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    defaultMessageHandler = qInstallMessageHandler(customMessageHandler);
    QApplication a(argc, argv);
    UniversalTimer2 w;
    w.show();
    return a.exec();
}
