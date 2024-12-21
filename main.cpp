#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include <string>
#include "WifiScanner.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>
#include <iostream>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include "WifiScanner.h"
#include "ConsoleHandler.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    WiFi::WifiScanner scanner;
    scanner.setBackend(WiFi::WifiScanner::NMCLI);

    QObject::connect(&scanner, &WiFi::WifiScanner::networksUpdated, [&scanner]() {
        qDebug() << "Networks updated.";
    });

    ConsoleHandler consoleHandler(&scanner);
    app.installEventFilter(&consoleHandler);

    return app.exec();
}
