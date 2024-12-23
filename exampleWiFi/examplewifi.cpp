#include "examplewifi.h"
#include "./ui_examplewifi.h"
#include "WifiDelegate.h"

#include <QVBoxLayout>
#include <QShowEvent>
#include <QLabel>
#include <QPushButton>
#include "WiFiSwitch.h"

ExampleWifi::ExampleWifi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExampleWifi)
{
    ui->setupUi(this);

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


    wiFiSwitch->setEnabled(false);    

    // Tworzenie kontenerów dla WiFiSwitch
    QWidget* container1 = new QWidget(this);
    QVBoxLayout* containerLayout1 = new QVBoxLayout(container1);
    containerLayout1->addWidget(wiFiSwitch, 0, Qt::AlignCenter);
    containerLayout1->setContentsMargins(0, 0, 0, 0); // Usuń marginesy
    containerLayout1->setSpacing(0); // Usuń przestrzeń wewnętrzną

    // 
    auto *btnConnect = new WiFiSwitch( this);
    btnConnect->setFixedHeight(32);
    btnConnect->setFixedWidth(32);
    btnConnect->setTexts("OK", "OK");
    // Tworzenie kontenerów dla btnConnect
    QWidget* container_btnConnect = new QWidget(this);
    QVBoxLayout* layout_btnConnect  = new QVBoxLayout(container_btnConnect);
    layout_btnConnect->addWidget(btnConnect, 0, Qt::AlignCenter);
    layout_btnConnect->setContentsMargins(0, 0, 0, 0); // Usuń marginesy
    layout_btnConnect->setSpacing(0); // Usuń przestrzeń wewnętrzną
    //
    auto *btnDisConnect = new WiFiSwitch( this);
    btnDisConnect->setFixedHeight(32);
    btnDisConnect->setFixedWidth(32);
    btnDisConnect->setTexts("❄", "❄");
    // Tworzenie kontenerów dla btnConnect
    QWidget* container_btnDisConnect = new QWidget(this);
    QVBoxLayout* layout_btnDisConnect  = new QVBoxLayout(container_btnDisConnect);
    layout_btnDisConnect->addWidget(btnDisConnect, 0, Qt::AlignCenter);
    layout_btnDisConnect->setContentsMargins(0, 0, 0, 0); // Usuń marginesy
    layout_btnDisConnect->setSpacing(0); // Usuń przestrzeń wewnętrzną
    // 
    auto *btnForget = new WiFiSwitch( this);
    btnForget->setFixedHeight(32);
    btnForget->setFixedWidth(32);
    btnForget->setTexts("P", "P");
    // Tworzenie kontenerów dla btnConnect
    QWidget* container_btnForget= new QWidget(this);
    QVBoxLayout* layout_btnForget  = new QVBoxLayout(container_btnForget);
    layout_btnForget->addWidget(btnForget, 0, Qt::AlignCenter);
    layout_btnForget->setContentsMargins(0, 0, 0, 0); // Usuń marginesy
    layout_btnForget->setSpacing(0); // Usuń przestrzeń wewnętrzną


    // {
    // Tworzenie głównego widżetu
    QWidget *window = new QWidget(this);

    // Tworzenie etykiet
    QLabel* label1 = new QLabel("ON / OFF WiFi");
    QLabel* label2 = new QLabel("Connect");
    QLabel* label3 = new QLabel("Disconnect");
    QLabel* label4 = new QLabel("Forget network");

    // Wyśrodkowanie tekstu w QLabel
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);
    label4->setAlignment(Qt::AlignCenter);    

    // Tworzenie przycisków
    // QPushButton* button1 = new QPushButton("Przycisk 1");
    // QPushButton* button2 = new QPushButton("Connect");
    // QPushButton* button3 = new QPushButton("Disconnect");
    // QPushButton* button4 = new QPushButton("Forget network");

    // Tworzenie layoutu siatki
    QGridLayout* layoutGrid = new QGridLayout;

    // Dodawanie etykiet do pierwszego wiersza
    layoutGrid->addWidget(label1, 0, 0); // Wiersz 0, kolumna 0
    layoutGrid->addWidget(label2, 0, 1); // Wiersz 0, kolumna 1
    layoutGrid->addWidget(label3, 0, 2); // Wiersz 0, kolumna 2
    layoutGrid->addWidget(label4, 0, 3); // Wiersz 0, kolumna 3

    // Dodawanie przycisków do drugiego wiersza
    layoutGrid->addWidget(container1,              1, 0); // Wiersz 1, kolumna 0
    layoutGrid->addWidget(container_btnConnect,    1, 1); // Wiersz 1, kolumna 1
    layoutGrid->addWidget(container_btnDisConnect, 1, 2); // Wiersz 1, kolumna 2
    layoutGrid->addWidget(container_btnForget,     1, 3); // Wiersz 1, kolumna 3

    // Ustawienie layoutu na głównym widżecie
    window->setLayout(layoutGrid);



    // Inicjalizacja timera
    gKeyTimer.setSingleShot(true);
    connect(&gKeyTimer, &QTimer::timeout, this, [this]() {
        canProcessGKey = true;  // Po upływie 3 sekund klawisz G może znowu działać
    });

 
    // Tworzenie layoutu
    auto* layout = new QVBoxLayout(this);

    // Dodawanie widgetów do głównego layoutu
    layout->addWidget(window);  // Górna część
    // layout->addWidget(topWidget);  // Górna część
    layout->addWidget(&tableView);  // Dolna część (reszta przestrzeni)

    // Ustawienie głównego layoutu
    setLayout(layout);

    tableView.setParent(this);

    tableView.setModel(&model);

    // Ustawienie delegata
    auto* delegate = new WifiDelegate(this);
    tableView.setItemDelegate(delegate);

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
   
    // // Ustawianie fokusu na pierwszym elemencie
    // tableView.setFocus();
    // tableView.selectRow(0); // Zaznacza pierwszy wiersz
    // tableView.scrollTo(model.index(0, 0)); // Przewija do pierwszego elementu (opcjonalnie)

    // tableView.show();

    // Ustawianie fokusu na tabelę
    tableView.setFocus(Qt::OtherFocusReason);

    // Ustawienie aktywnego indeksu
    QModelIndex firstIndex = model.index(0, 0);
    if (firstIndex.isValid()) {
        tableView.setCurrentIndex(firstIndex); // Ustawia pierwszy indeks jako aktywny
        tableView.scrollTo(firstIndex);       // Przewija do pierwszego elementu
    }

    QObject::connect(&wifiscanner, &WiFi::WifiScanner::networksUpdated, [this]() {
        model.setNetworks(wifiscanner.getNetworks());
    });

    wifiscanner.startScanning();   

     // Pobierz selektor z QTableView
    QItemSelectionModel* selectionModel = tableView.selectionModel();

    // Podłącz sygnał currentRowChanged do własnej metody
    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, 
    [this](const QModelIndex &current, const QModelIndex &previous) {
        Q_UNUSED(previous);

        if (current.isValid()) {
            int row = current.row();

            // Odczytaj dane z modelu
            const WiFi::RecWifiNetwork& network = model.getNetwork(row);
            qDebug() << "Zmieniono wiersz na:" << row;
            qDebug() << "SSID:" << QString::fromStdString(network.ssid);
            qDebug() << "Signal Level:" << network.signalLevel;
        }
    });    
}


void ExampleWifi::showEvent(QShowEvent* event) {
    QWidget::showEvent(event); // Wywołanie bazowej implementacji

    // Kod wykonywany podczas pokazywania widżetu
    tableView.setFocus(Qt::OtherFocusReason);

    // Ustawienie aktywnego indeksu
    QModelIndex firstIndex = model.index(0, 0);
    if (firstIndex.isValid()) {
        tableView.setCurrentIndex(firstIndex);
        tableView.scrollTo(firstIndex);
    }
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