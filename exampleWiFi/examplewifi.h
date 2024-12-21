#ifndef EXAMPLEWIFI_H
#define EXAMPLEWIFI_H

#include <QWidget>
#include <QKeyEvent>
#include <QTableView>
#include "WifiScanner.h"
#include "WifiNetworkModel.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class ExampleWifi;
}
QT_END_NAMESPACE

class ExampleWifi : public QWidget
{
    Q_OBJECT

public:
    ExampleWifi(QWidget *parent = nullptr);
    ~ExampleWifi();


protected:
    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
        case Qt::Key_A:
            qDebug() << "Key A pressed";
            // Obsługa klawisza A

            wifiscanner.startScanning();
            break;
        case Qt::Key_S:
            qDebug() << "Key S pressed";
            // Obsługa klawisza S
            {
                const auto& networks = wifiscanner.getNetworks();
                qDebug() << "Networks found:";
                for (const auto& network : networks) {
                    qDebug() << "BSSID:" << QString::fromStdString(network.bssid)
                             << "Frequency:" << network.frequency
                             << "Signal Level:" << network.signalLevel
                             << "Flags:" << QString::fromStdString(network.flags)
                             << "SSID:" << QString::fromStdString(network.ssid);
                }
            } 
            break;
        case Qt::Key_D:
            qDebug() << "Key D pressed";
            // Obsługa klawisza D
            break;
        case Qt::Key_F:
            qDebug() << "Key F pressed";
            // Obsługa klawisza F
            break;
        default:
            QWidget::keyPressEvent(event); // Wywołanie domyślnej obsługi
        }
    }
private:
    Ui::ExampleWifi *ui;

    WiFi::WifiScanner wifiscanner;
    WifiNetworkModel model;
    QTableView tableView;
};
#endif // EXAMPLEWIFI_H
