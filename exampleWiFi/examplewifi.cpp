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



    QObject::connect(&wifiscanner, &WiFi::WifiScanner::networksUpdated, [this]() {
        model.setNetworks(wifiscanner.getNetworks());
    });

    wifiscanner.startScanning();        
}

ExampleWifi::~ExampleWifi()
{
    delete ui;
}
