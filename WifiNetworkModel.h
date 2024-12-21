#include <QAbstractTableModel>
#include <vector>
#include <QString>
#include "WifiScanner.h" // Zakładamy, że tu jest definicja RecWifiNetwork

class WifiNetworkModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit WifiNetworkModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}

    void setNetworks(const std::vector<WiFi::RecWifiNetwork>& networks) {
        beginResetModel();
        this->networks = networks;
        endResetModel();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
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

    // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
    //     if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    //         switch (section) {
    //         case 0: return "SSID";
    //         case 1: return "BSSID";
    //         case 2: return "Frequency";
    //         case 3: return "Signal Level";
    //         case 4: return "Flags";
    //         default: return QVariant();
    //         }
    //     }
    //     return QVariant();
    // }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return QVariant();

        static QStringList headers = {"SSID", "BSSID", "Frequency", "Signal Level", "Flags"};
        return headers.at(section);
    }

private:
    std::vector<WiFi::RecWifiNetwork> networks;
};
