#include "core/UniversalTimer2.h"
#include "core/LogManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    defaultMessageHandler = qInstallMessageHandler(customMessageHandler);
    QApplication a(argc, argv);
    UniversalTimer2 w;
    return a.exec();
}
