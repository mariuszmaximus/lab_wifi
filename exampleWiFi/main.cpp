#include "examplewifi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExampleWifi w;
#ifdef WIN32
    w.resize(1024, 768);
#else    
    w.showFullScreen();
#endif
    return a.exec();
}
