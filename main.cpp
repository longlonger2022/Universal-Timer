#include "ExamCountdown_v1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExamCountdown_v1 w;
    w.show();
    return a.exec();
}
