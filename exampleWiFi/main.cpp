#include "WiFiConfig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WiFiConfig w;
#ifdef WIN32
    w.resize(1024, 768);
    w.show();
#else    
    w.showFullScreen();
#endif
    return a.exec();
}
