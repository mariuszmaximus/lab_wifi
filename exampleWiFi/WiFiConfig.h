#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QTableView>

#include "WiFi/WifiScanner.h"
#include "WiFi/MVC/WifiNetworkModel.h"
#include "WiFi/UI/WiFiSlider.h"


class WiFiConfig : public QWidget
{
    Q_OBJECT

public:
    WiFiConfig(QWidget *parent = nullptr);
    ~WiFiConfig();

protected:
    // void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override ;
protected:
    void showEvent(QShowEvent* event) override;

private:
    QTableView tableView;    

    WiFi::WifiScanner wifiscanner;
    WiFi::MVC::WifiNetworkModel model;
    WiFi::MVC::WifiNetworkModel modelEmpty;
    WiFi::UI::WiFiSlider *wifiSlider;
};
