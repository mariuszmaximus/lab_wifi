#include "examplewifi.h"
#include "./ui_examplewifi.h"
#include "WifiDelegate.h"

#include <QVBoxLayout>
#include "WiFiSwitch.h"

ExampleWifi::ExampleWifi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExampleWifi)
{
    ui->setupUi(this);

    // Inicjalizacja timera
    gKeyTimer.setSingleShot(true);
    connect(&gKeyTimer, &QTimer::timeout, this, [this]() {
        canProcessGKey = true;  // Po upływie 3 sekund klawisz G może znowu działać
    });

    // Tworzenie widgetów
    // checkBox = new QCheckBox("Enable Feature", this);

    wiFiSwitch = new WiFiSwitch( this);
    wiFiSwitch->setFixedHeight(32);
    wiFiSwitch->setFixedWidth(32*2);
    wiFiSwitch->setTexts("G2", "G2");
    connect(wiFiSwitch, &WiFiSwitch::toggled, this, [this](bool value){
        if(value)
            tableView.setModel(&model);
        else
            tableView.setModel(&modelEmpty);
    });
 
    // Tworzenie layoutu
    auto* layout = new QVBoxLayout(this);

    // Górny widget z checkboxem (stała wysokość 66 px)
    auto* topWidget = new QWidget(this);
    auto* topLayout = new QVBoxLayout(topWidget);
    topLayout->addWidget(wiFiSwitch);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);
    topWidget->setFixedHeight(66);

    // Dodawanie widgetów do głównego layoutu
    layout->addWidget(topWidget);  // Górna część
    layout->addWidget(&tableView);  // Dolna część (reszta przestrzeni)

    // Ustawienie głównego layoutu
    setLayout(layout);

    tableView.setParent(this);

    tableView.setModel(&model);

    // Ustawienie delegata
    auto* delegate = new WifiDelegate(this);
    tableView.setItemDelegate(delegate);

    tableView.setWindowTitle("WiFi Networks");

    // Ustaw szerokości kolumn
    tableView.setColumnWidth(0, 250); // SSID
    tableView.setColumnWidth(1, 150); // BSSID
    tableView.setColumnWidth(2, 80);  // Frequency
    tableView.setColumnWidth(3, 80);  // Signal Level
    tableView.setColumnWidth(4, 400); // Flags

    // Ustawienie, aby zaznaczać całe wiersze
    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);

    tableView.installEventFilter(this);

    // Opcjonalne: Ustawienie, aby można było zaznaczać tylko jeden wiersz naraz
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);
   

    QObject::connect(&wifiscanner, &WiFi::WifiScanner::networksUpdated, [this]() {
        model.setNetworks(wifiscanner.getNetworks());
    });

    wifiscanner.startScanning();   
}

ExampleWifi::~ExampleWifi()
{
    delete ui;
}

bool ExampleWifi::eventFilter(QObject* obj, QEvent* event) {
    if (obj == &tableView && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_G) {
            if (canProcessGKey) {
                // Obsłuż klawisz G
                wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
                canProcessGKey = false;  // Zablokuj reakcję na klawisz G
                gKeyTimer.start(3000);   // Ustaw 3-sekundowy timer
            }
            // Zmień stan checkboxa
            // checkBox->setChecked(!checkBox->isChecked());
            // wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
            return true; // Zatrzymanie dalszego przetwarzania zdarzenia
        }
    }
    return QWidget::eventFilter(obj, event); // Domyślne przetwarzanie
}


void ExampleWifi::keyPressEvent(QKeyEvent* event)  {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // Pobierz aktualnie wybrany wiersz
        QModelIndex currentIndex = tableView.currentIndex();
        if (currentIndex.isValid()) {
            // Pobierz dane z kolumny SSID
            QString ssid = tableView.model()->data(currentIndex.siblingAtColumn(0)).toString(); // Kolumna SSID
            qDebug() << "Selected SSID:" << ssid;
        }
    } else if (event->key() == Qt::Key_G) {
        // Zmień stan checkboxa
        //checkBox->setChecked(!checkBox->isChecked());
        // wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
        if (canProcessGKey) {
            // Obsłuż klawisz G
            wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
            canProcessGKey = false;  // Zablokuj reakcję na klawisz G
            gKeyTimer.start(3000);   // Ustaw 3-sekundowy timer
        }
        else
        {
            QWidget::keyPressEvent(event); // Domyślne zachowanie    
        }

    } else {
        QWidget::keyPressEvent(event); // Domyślne zachowanie
    }
}