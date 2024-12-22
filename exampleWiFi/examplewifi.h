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

class QCheckBox;
class WiFiSwitch;

class ExampleWifi : public QWidget
{
    Q_OBJECT

public:
    ExampleWifi(QWidget *parent = nullptr);
    ~ExampleWifi();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override ;
private:
    Ui::ExampleWifi *ui;

    WiFi::WifiScanner wifiscanner;
    WifiNetworkModel model;
    WifiNetworkModel modelEmpty;
    QTableView tableView;    
    WiFiSwitch *wiFiSwitch;

    bool canProcessGKey{true};
    QTimer gKeyTimer;
};
#endif // EXAMPLEWIFI_H
