#pragma once 

#include <QObject>
#include "WifiScanner.h"

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


class ConsoleHandler : public QObject {
    Q_OBJECT

public:
    explicit ConsoleHandler(WiFi::WifiScanner* scanner, QObject* parent = nullptr)
        : QObject(parent), scanner(scanner) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        qDebug() << "eventFilter "<< obj;
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            switch (keyEvent->key()) {
            case Qt::Key_1:
                scanner->startScanning();
                qDebug() << "Started scanning.";
                break;
            case Qt::Key_2:
                scanner->stopScanning();
                qDebug() << "Stopped scanning.";
                break;
            case Qt::Key_3: {
                const auto& networks = scanner->getNetworks();
                qDebug() << "Networks found:";
                for (const auto& network : networks) {
                    qDebug() << "BSSID:" << QString::fromStdString(network.bssid)
                             << "Frequency:" << network.frequency
                             << "Signal Level:" << network.signalLevel
                             << "Flags:" << QString::fromStdString(network.flags)
                             << "SSID:" << QString::fromStdString(network.ssid);
                }
                break;
            }
            case Qt::Key_4:
                qDebug() << "Enter SSID to connect: ";
                // Logic for connection (implement as needed)
                break;
            case Qt::Key_5:
                scanner->stopScanning();
                qDebug() << "Disconnected.";
                break;
            case Qt::Key_0:
                QCoreApplication::quit();
                break;
            default:
                qDebug() << "Invalid key.";
            }
            return true;
        }
        return QObject::eventFilter(obj, event);
    }

private:
    WiFi::WifiScanner* scanner;
};
