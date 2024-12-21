#pragma once 

#include <QProcess>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <string>
#include <sstream>
#include <regex>


namespace WiFi {

struct RecWifiNetwork {
    std::string bssid;
    int frequency;
    int signalLevel;
    std::string flags;
    std::string ssid;
};

class WifiScanner : public QObject {
    Q_OBJECT

public:
    enum ScannerBackend {
        WPA_CLI,
        NMCLI
    };

    WifiScanner(QObject* parent = nullptr, ScannerBackend backend = WPA_CLI)
        : QObject(parent), scanInterval(5000), backend(backend) 
    { 
        // Default interval is 5 seconds
        connect(&timer, &QTimer::timeout, this, &WifiScanner::scanNetworks);
        qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaaaa WifiScanner:: WifiScanner";
    }

    void setBackend(ScannerBackend newBackend) {
        backend = newBackend;
    }

    void startScanning() {
        qDebug() << " aaaaaaaaaaaaaaaaaaaaaaaaa startScanning " << scanInterval;
        timer.start(scanInterval);
        scanNetworks();     
    }

    void stopScanning() {
        timer.stop();
    }

    const std::vector<RecWifiNetwork>& getNetworks() const {
        return networks;
    }

signals:
    void networksUpdated();

private slots:
    void scanNetworks() {
        qDebug() << " aaaaaaaaaaaaaaaaa scanNetworks";
        QString output;

        if (backend == WPA_CLI) {
            QProcess process;
            process.start("wpa_cli", {"scan_results"});
            process.waitForFinished();
            output = process.readAllStandardOutput();
            // qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            // qDebug() << output;
            // qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        } else if (backend == NMCLI) {
            QProcess process;
            process.start("nmcli", {"-f", "BSSID,FREQ,SIGNAL,SECURITY,SSID", "dev", "wifi"});
            process.waitForFinished();
            output = process.readAllStandardOutput();
        }

        parseScanResults(output.toStdString());
        emit networksUpdated();
    }

public:

    void connectToNetwork(const QString& ssid) {
        if (backend != WPA_CLI) {
            qWarning() << "Connection feature is only supported with WPA_CLI backend.";
            return;
        }

        QProcess process;
        process.start("wpa_cli", {"list_networks"});
        process.waitForFinished();
        QString output = process.readAllStandardOutput();

        QString networkId;
        QTextStream stream(&output);
        QString line;
        while (stream.readLineInto(&line)) {
            if (line.contains(ssid)) {
                QStringList fields = line.split('\t');
                if (fields.size() > 0) {
                    networkId = fields[0]; // Extract network ID
                    break;
                }
            }
        }

        if (networkId.isEmpty()) {
            qWarning() << "Network with SSID" << ssid << "not found.";
            return;
        }

        process.start("wpa_cli", {"select_network", networkId});
        process.waitForFinished();
        QString result = process.readAllStandardOutput();
        if (result.contains("OK")) {
            qDebug() << "Successfully connected to" << ssid;
        } else {
            qWarning() << "Failed to connect to" << ssid;
        }
    }


private:
    void parseScanResults(const std::string& results) {
        networks.clear();
        std::istringstream stream(results);
        std::string line;

        int lineNumber = 0;

        while (std::getline(stream, line)) {
            lineNumber++;

            qDebug() << " LUNE = " << line;

            // Pomijamy nagłówki (pierwsze dwie linie dla WPA_CLI)
            if (lineNumber <= 2 && backend == WPA_CLI) {
                continue;
            }

            // Podziel wiersz na pola według białych znaków
            std::istringstream lineStream(line);
            std::vector<std::string> fields;
            std::string field;

            while (lineStream >> field) {
                fields.push_back(field);
            }

            // Sprawdzenie, czy wiersz ma wystarczającą liczbę pól
            if (fields.size() >= 5) {
                RecWifiNetwork network;
                network.bssid = fields[0];
                network.frequency = std::stoi(fields[1]);
                network.signalLevel = std::stoi(fields[2]);
                network.flags = fields[3];
                network.ssid = fields[4]; // Reszta linii jako SSID (jeśli zawiera spacje, trzeba odpowiednio złożyć)

                networks.push_back(network);
            } else {
                qDebug() << "Wiersz pominięty (za mało pól):" << QString::fromStdString(line) << " line:" << lineNumber << " " << line ;
            }
        }
    }

    QTimer timer;
    int scanInterval;
    ScannerBackend backend;
    std::vector<RecWifiNetwork> networks;
};

} // namespace WiFi
