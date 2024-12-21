#include "examplewifi.h"
#include "./ui_examplewifi.h"

ExampleWifi::ExampleWifi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExampleWifi)
{
    ui->setupUi(this);

    tableView.setParent(this);

    tableView.setModel(&model);
    tableView.setWindowTitle("WiFi Networks");
    tableView.resize(800, 600);
    tableView.show();

    // Ustaw szerokości kolumn
    tableView.setColumnWidth(0, 250); // SSID
    tableView.setColumnWidth(1, 150); // BSSID
    tableView.setColumnWidth(2, 80);  // Frequency
    tableView.setColumnWidth(3, 80);  // Signal Level
    tableView.setColumnWidth(4, 400); // Flags

    QObject::connect(&wifiscanner, &WiFi::WifiScanner::networksUpdated, [this]() {
        model.setNetworks(wifiscanner.getNetworks());
    });


    // auto  getSelectedSSID = [this](QTableView* tableView) -> QString 
    // {
    //     QModelIndex currentIndex = tableView->currentIndex();
    //     if (!currentIndex.isValid())
    //         return QString();

    //     // Zakładamy, że kolumna SSID to 4
    //     return currentIndex.sibling(currentIndex.row(), 4).data().toString();
    // };
 

    // QObject::connect(&wifiscanner, &WiFi::WifiScanner::networksUpdated, [this, getSelectedSSID ]() {
    //     // Pobierz aktualnie zaznaczone SSID
    //     QString selectedSSID = getSelectedSSID(&tableView);

    //     // Zaktualizuj model tabeli
    //     const auto& networks = wifiscanner.getNetworks();
    //     WifiNetworkModel* model = new WifiNetworkModel(networks);
    //     tableView->setModel(model);

    //     // Przywróć zaznaczenie na podstawie SSID
    //     restoreSelectionBySSID(tableView, selectedSSID);
    // });
    wifiscanner.startScanning();   
}

ExampleWifi::~ExampleWifi()
{
    delete ui;
}
