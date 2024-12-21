#include "examplewifi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExampleWifi w;
    w.show();
    return a.exec();
}
