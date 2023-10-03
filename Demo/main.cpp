#include "VisualStation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisualStation w;
    w.show();
    return a.exec();
}
