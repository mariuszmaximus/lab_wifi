#include "examplewifi.h"
#include "./ui_examplewifi.h"
#include "WifiDelegate.h"

#include <QVBoxLayout>
#include <QShowEvent>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>


#include <qabstractspinbox.h>
#include <qslider.h>
#include "WiFiSwitch.h"
#include "WiFiSwitch3state.h"

ExampleWifi::ExampleWifi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExampleWifi)
{
    ui->setupUi(this);

    // 
    //  widgetTop
    //    layoutTopGrid   
    //       container0  
    //          containerLayout0
    //            ... 
    //       container1  
    //          containerLayout1 

    // Tworzenie głównego widżetu
    QWidget *widgetTop = new QWidget(this);
    QGridLayout* layoutTopGrid = new QGridLayout;

 
    // kolumna 0 
    QWidget* container0 = new QWidget(this);
    QVBoxLayout* containerLayout0 = new QVBoxLayout(container0);
    containerLayout0->setContentsMargins(0, 0, 0, 0); // Usuń marginesy
    containerLayout0->setSpacing(0); // Usuń przestrzeń wewnętrzną
    // kolumna 1
    QWidget* container1 = new QWidget(this);
    QVBoxLayout* containerLayout1  = new QVBoxLayout(container1);
    containerLayout1->setContentsMargins(0, 0, 0, 0); 
    containerLayout1->setSpacing(0); 
    // kolumna 2 
    QWidget* container2 = new QWidget(this);
    QVBoxLayout* containerLayout2  = new QVBoxLayout(container2);
    containerLayout2->setContentsMargins(0, 0, 0, 0); 
    containerLayout2->setSpacing(0); 
    // kolumna 3 
    QWidget* container3= new QWidget(this);
    QVBoxLayout* containerLayout3  = new QVBoxLayout(container3);
    containerLayout3->setContentsMargins(0, 0, 0, 0); 
    containerLayout3->setSpacing(0); 
    // kolumna 4 
    QWidget* container4= new QWidget(this);
    QVBoxLayout* containerLayout4  = new QVBoxLayout(container4);
    containerLayout4->setContentsMargins(0, 0, 0, 0); 
    containerLayout4->setSpacing(0); 
 


    wiFiSwitch = new WiFiSwitch3state( this);
    wiFiSwitch->setFixedHeight(32);
    wiFiSwitch->setFixedWidth(32*3);
    wiFiSwitch->setTexts("G2", "G2");
    // connect(wiFiSwitch, &WiFiSwitch::toggled, this, [this](bool value){
    //     if(value)
    //         tableView.setModel(&model);
    //     else
    //         tableView.setModel(&modelEmpty);
    // });
    // wiFiSwitch->setEnabled(false);    

    // 
    auto *btnConnect = new WiFiSwitch( this);
    btnConnect->setFixedHeight(32);
    btnConnect->setFixedWidth(32);
    btnConnect->setTexts("OK", "OK");
    // Tworzenie kontenerów dla btnConnect
    //
    auto *btnDisConnect = new WiFiSwitch( this);
    btnDisConnect->setFixedHeight(32);
    btnDisConnect->setFixedWidth(32);
    btnDisConnect->setTexts("❄", "❄");
    // Tworzenie kontenerów dla btnConnect
    // 
    auto *btnForget = new WiFiSwitch( this);
    btnForget->setFixedHeight(32);
    btnForget->setFixedWidth(32);
    btnForget->setTexts("P", "P");
    // Tworzenie kontenerów dla btnConnect


    containerLayout0->addWidget(wiFiSwitch, 0, Qt::AlignCenter);
    containerLayout1->addWidget(btnConnect, 0, Qt::AlignCenter);
    containerLayout2->addWidget(btnDisConnect, 0, Qt::AlignCenter);
    containerLayout3->addWidget(btnForget, 0, Qt::AlignCenter);


    // {

    // Tworzenie etykiet
    //QLabel* label1 = new QLabel("ON / OFF WiFi");
    QLabel* label1 = new QLabel("AP  / OFF / WiFi");
    QLabel* label2 = new QLabel("Connect");
    QLabel* label3 = new QLabel("Disconnect");
    QLabel* label4 = new QLabel("Forget network");

    horizontalSlider = new QSlider(Qt::Horizontal);
    horizontalSlider->setMinimum(-1);
    horizontalSlider->setMaximum(1);
    horizontalSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // Ustawienie stylu suwaka z okrągłym uchwytem
    horizontalSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: 1px solid #999999;
            height: 8px;
            background: #bcbcbc;
            margin: 2px 0;
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: #ff5733;
            border: 1px solid #5c5c5c;
            width: 18px;
            height: 18px;
            border-radius: 9px;
            margin: -5px 0; /* Wyrównanie uchwytu */
        }
        QSlider::handle:horizontal:hover {
            background: #ff784e;
        }
        QSlider::sub-page:horizontal {
            background: #ff8c42;
            border-radius: 4px;
        }
    )");




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

    // Dodawanie etykiet do pierwszego wiersza
    layoutTopGrid->addWidget(label1,           0, 0); 
    layoutTopGrid->addWidget(label2,           0, 1); 
    layoutTopGrid->addWidget(horizontalSlider, 0, 2); 
    layoutTopGrid->addWidget(label4,           0, 3); 

    // Dodawanie przycisków do drugiego wiersza
    layoutTopGrid->addWidget(container0, 1, 0);
    layoutTopGrid->addWidget(container1, 1, 1); 
    layoutTopGrid->addWidget(container2, 1, 2); 
    layoutTopGrid->addWidget(container3, 1, 3); 
    layoutTopGrid->addWidget(container4, 1, 4); 



    // Ustawienie layoutu na głównym widżecie
    widgetTop->setLayout(layoutTopGrid);



    // Inicjalizacja timera
    gKeyTimer.setSingleShot(true);
    connect(&gKeyTimer, &QTimer::timeout, this, [this]() {
        canProcessGKey = true;  // Po upływie 3 sekund klawisz G może znowu działać
    });

 
    // Tworzenie layoutu
    auto* layout = new QVBoxLayout(this);

    // Dodawanie widgetów do głównego layoutu
    layout->addWidget(widgetTop);  // Górna część
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
    // tableView.setFocus(Qt::OtherFocusReason);

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
                // wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
                canProcessGKey = false;  // Zablokuj reakcję na klawisz G
                gKeyTimer.start(3000);   // Ustaw 3-sekundowy timer
            }
            // Zmień stan checkboxa
            // checkBox->setChecked(!checkBox->isChecked());
            // wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
            return true; // Zatrzymanie dalszego przetwarzania zdarzenia
        }
        if (keyEvent->key() == Qt::Key_Q) {
            wiFiSwitch->setValue(-1);
            // wiFiSwitch->setvalue(-1);
            
            // wiFiSwitch->setFocus();
            // horizontalSlider->setFocus();
        }
        if (keyEvent->key() == Qt::Key_W) {
            wiFiSwitch->setValue(0);
        }
        if (keyEvent->key() == Qt::Key_E) {
            wiFiSwitch->setValue(1);
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
            // wiFiSwitch->setChecked(!wiFiSwitch->isChecked());
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