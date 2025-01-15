#pragma once 

#include <QAbstractTableModel>
#include <vector>
#include <QString>
#include "WiFi/WifiScanner.h" // Zakładamy, że tu jest definicja RecWifiNetwork

namespace WiFi::MVC {

class WifiNetworkModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit WifiNetworkModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}

    void setNetworks(const std::vector<WiFi::RecWifiNetwork>& newNetworks) {
        // this->networks = networks;
        // Usuwanie wierszy, które już nie istnieją
        for (int row = networks.size() - 1; row >= 0; --row) {
            auto it = std::find_if(newNetworks.begin(), newNetworks.end(),
                [&](const WiFi::RecWifiNetwork& net) { return net.bssid == networks[row].bssid; });
            if (it == newNetworks.end()) {
                beginRemoveRows(QModelIndex(), row, row);
                networks.erase(networks.begin() + row);
                endRemoveRows();
            }
        }

        // Dodawanie nowych wierszy
        for (const auto& newNet : newNetworks) {
            auto it = std::find_if(networks.begin(), networks.end(),
                [&](const WiFi::RecWifiNetwork& net) { return net.bssid == newNet.bssid; });
            if (it == networks.end()) {
                beginInsertRows(QModelIndex(), networks.size(), networks.size());
                networks.push_back(newNet);
                endInsertRows();
            }
        }

        // Aktualizacja istniejących wierszy
        for (int row = 0; row < static_cast<int>(networks.size()); ++row) {
            const auto& newNet = newNetworks[row];
            if (networks[row].signalLevel != newNet.signalLevel || networks[row].ssid != newNet.ssid) {
                networks[row] = newNet;
                emit dataChanged(index(row, 0), index(row, columnCount() - 1));
            }
        }
    }


    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);

        if(networks.size()==0)  qDebug() << "RRRRRRRRRRRRRRRRRRRRRRRRRRRR rowcount="<<networks.size();
        return static_cast<int>(networks.size());
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return 5; // BSSID, Frequency, Signal Level, Flags, SSID
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const auto& network = networks.at(index.row());
        switch (index.column()) {
        case 0: return QString::fromStdString(network.ssid);
        case 1: return QString::fromStdString(network.bssid);
        case 2: return network.frequency;
        case 3: return network.signalLevel;
        case 4: return QString::fromStdString(network.flags);
        default: return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return QVariant();

        static QStringList headers = {"SSID", "BSSID", "Frequency", "Signal Level", "Flags"};
        return headers.at(section);
    }


    const WiFi::RecWifiNetwork& getNetwork(int row) const {
        return networks.at(row);
    }    

private:
    std::vector<WiFi::RecWifiNetwork> networks;
};

}; // namespace WiFi::MVC