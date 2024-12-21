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

        if (backend == WPA_CLI) {
            std::regex regex("([0-9a-fA-F:]+)\\s+(\\d+)\\s+(-?\\d+)\\s+\\[(.*?)\\]\\s+(.*)");
            std::smatch match;

            // Skip the first two lines (header information)
            std::getline(stream, line);
            std::getline(stream, line);

            while (std::getline(stream, line)) {
                if (std::regex_match(line, match, regex)) {
                    RecWifiNetwork network;
                    network.bssid = match[1];
                    network.frequency = std::stoi(match[2]);
                    network.signalLevel = std::stoi(match[3]);
                    network.flags = match[4];
                    network.ssid = match[5];
                    networks.push_back(network);
                }
            }
        } else if (backend == NMCLI) {
            std::regex regex("([0-9a-fA-F:]+)\\s+(\\d+)\\s+(-?\\d+)\\s+(.*?)\\s+(.*)");
            std::smatch match;

            // Skip the first line (header information)
            std::getline(stream, line);

            while (std::getline(stream, line)) {
                if (std::regex_match(line, match, regex)) {
                    RecWifiNetwork network;
                    network.bssid = match[1];
                    network.frequency = std::stoi(match[2]);
                    network.signalLevel = std::stoi(match[3]);
                    network.flags = match[4];
                    network.ssid = match[5];
                    networks.push_back(network);
                }
            }
        }
    }

    QTimer timer;
    int scanInterval;
    ScannerBackend backend;
    std::vector<RecWifiNetwork> networks;
};

} // namespace WiFi
