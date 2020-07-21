#include "QILT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QILT w;
    w.show();
    return a.exec();
}
