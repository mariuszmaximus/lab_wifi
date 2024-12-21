#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include <string>
#include "WiFi/WifiScanner.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    WiFi::WifiScanner scanner;

    scanner.setBackend(WiFi::WifiScanner::WPA_CLI); // Switch to NMCLI if desired

    QObject::connect(&scanner, &WiFi::WifiScanner::networksUpdated, [&scanner]() {
        const auto& networks = scanner.getNetworks();
        qDebug() << "Found networks:";
        for (const auto& network : networks) {
            qDebug() << "BSSID:" << QString::fromStdString(network.bssid)
                     << "Frequency:" << network.frequency
                     << "Signal Level:" << network.signalLevel
                     << "Flags:" << QString::fromStdString(network.flags)
                     << "SSID:" << QString::fromStdString(network.ssid);
        }
    });

    QTimer consoleInputTimer;
    QObject::connect(&consoleInputTimer, &QTimer::timeout, [&]() {
        std::cout << "\nOptions:\n"
                  << "1) Start scanning\n"
                  << "2) Stop scanning\n"
                  << "3) Print networks\n"
                  << "4) Connect\n"
                  << "5) Disconnect\n"
                  << "0) Exit\n"
                  << "Choose an option: ";

        int option;
        std::cin >> option;

        switch (option) {
        case 1:
            scanner.startScanning();
            std::cout << "Started scanning.\n";
            break;
        case 2:
            scanner.stopScanning();
            std::cout << "Stopped scanning.\n";
            break;
        case 3: {
            const auto& networks = scanner.getNetworks();
            std::cout << "Networks found:\n";
            for (const auto& network : networks) {
                std::cout << "BSSID: " << network.bssid
                          << ", Frequency: " << network.frequency
                          << ", Signal Level: " << network.signalLevel
                          << ", Flags: " << network.flags
                          << ", SSID: " << network.ssid << '\n';
            }
            break;
        }
        case 4: {
            std::cout << "Enter SSID to connect: ";
            std::string ssid;
            std::cin >> ssid;
            scanner.connectToNetwork(QString::fromStdString(ssid));
            break;
        }
        case 5:
            scanner.stopScanning();
            std::cout << "Disconnected.\n";
            break;
        case 0:
            app.quit();
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    });

    consoleInputTimer.start(100); // Sprawdzaj konsolę co 100 ms

    return app.exec();
}
