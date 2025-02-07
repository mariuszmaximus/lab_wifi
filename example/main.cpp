#include "WiFi/UI/WiFiConfigDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WiFi::UI::WiFiConfigDialog w;
#ifdef WIN32
    w.resize(1024, 768);
    w.show();
#else    
    w.showFullScreen();
#endif
    return a.exec();
}
