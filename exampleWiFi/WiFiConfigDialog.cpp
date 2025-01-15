#include "WiFiConfigDialog.h"


#include <QVBoxLayout>
#include <QShowEvent>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QTimer>
#include <QStackedWidget>



// #include <qabstractspinbox.h>
// #include <qnamespace.h>
// #include <qslider.h>

#include "WiFi/UI/WiFiSwitch.h"
#include "WiFi/UI/KeyboardBlocker.h"
#include "WiFi/UI/WifiDelegate.h"


WiFiConfigDialog::WiFiConfigDialog(QWidget *parent)
    : QWidget(parent)
{
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

    int COLUMN_COUNT{4};
    // Opcjonalne proporcje szerokości
    for (int i = 0; i < COLUMN_COUNT; ++i) {
        layoutTopGrid->setColumnStretch(i, 1); // Wszystkie kolumny są rozciągane proporcjonalnie
    }

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
 


    wifiSlider  = new WiFi::UI::WiFiSlider(this);
    wifiSlider->setMinimumWidth(96);


    // 
    auto *btnConnect = new WiFi::UI::WiFiSwitch( this);
    btnConnect->setFixedHeight(32);
    btnConnect->setFixedWidth(32);
    btnConnect->setTexts("OK", "OK");
    // Tworzenie kontenerów dla btnConnect
    //
    auto *btnDisConnect = new WiFi::UI::WiFiSwitch( this);
    btnDisConnect->setFixedHeight(32);
    btnDisConnect->setFixedWidth(32);
    btnDisConnect->setTexts("❄", "❄");
    // Tworzenie kontenerów dla btnConnect
    // 
    auto *btnForget = new WiFi::UI::WiFiSwitch( this);
    btnForget->setFixedHeight(32);
    btnForget->setFixedWidth(32);
    btnForget->setTexts("P", "P");
    // Tworzenie kontenerów dla btnConnect


    containerLayout0->addWidget(wifiSlider, 0, Qt::AlignCenter);
    containerLayout1->addWidget(btnConnect, 0, Qt::AlignCenter);
    containerLayout2->addWidget(btnDisConnect, 0, Qt::AlignCenter);
    containerLayout3->addWidget(btnForget, 0, Qt::AlignCenter);

    // Tworzenie etykiet
    QLabel* label0 = new QLabel("AP  / OFF / WiFi");
    QLabel* label1 = new QLabel("Connect");
    QLabel* label2 = new QLabel("Disconnect");
    QLabel* label3 = new QLabel("Forget network");


    // Wyśrodkowanie tekstu w QLabel
    label0->setAlignment(Qt::AlignCenter);
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);


    // Tworzenie layoutu siatki

    // Dodawanie etykiet do pierwszego wiersza
    layoutTopGrid->addWidget(label0,           0, 0); 
    layoutTopGrid->addWidget(label1,           0, 1); 
    layoutTopGrid->addWidget(label2,           0, 2); // layoutTopGrid->addWidget(horizontalSlider, 0, 2); 
    layoutTopGrid->addWidget(label3,           0, 3); 

    // Dodawanie przycisków do drugiego wiersza
    layoutTopGrid->addWidget(container0, 1, 0);
    layoutTopGrid->addWidget(container1, 1, 1); 
    layoutTopGrid->addWidget(container2, 1, 2); 
    layoutTopGrid->addWidget(container3, 1, 3); 


    // Ustawienie layoutu na głównym widżecie
    widgetTop->setLayout(layoutTopGrid);


    auto *stackedWidget = new QStackedWidget;
 
    // Tworzenie layoutu
    auto* layout = new QVBoxLayout(this);

    // Dodawanie widgetów do głównego layoutu
    layout->addWidget(widgetTop);  // Górna część
    layout->addWidget(stackedWidget);  // Dolna część (reszta przestrzeni)

    // Ustawienie głównego layoutu
    setLayout(layout);


    tableView.setModel(&model);

    // Ustawienie delegata
    auto* delegate = new WiFi::UI::WifiDelegate(this);
    tableView.setItemDelegate(delegate);

    // Ustaw szerokości kolumn
    tableView.setColumnWidth(0, 250); // SSID
    tableView.setColumnWidth(1, 150); // BSSID
    tableView.setColumnWidth(2, 80);  // Frequency
    tableView.setColumnWidth(3, 80);  // Signal Level
    tableView.setColumnWidth(4, 400); // Flags

    // Ustawienie, aby zaznaczać całe wiersze
    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);


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


    tableView.installEventFilter(this);
    wifiSlider->installEventFilter(this);


    auto *keyboardBlocker = new WiFi::UI::KeyboardBlocker(this);



    auto  blockKeyboardInput = [this, keyboardBlocker](bool block) {
        // Globalne zablokowanie klawiatury w aplikacji
        if (block) {
            QApplication::instance()->installEventFilter(keyboardBlocker);
        } else {
            QApplication::instance()->removeEventFilter(keyboardBlocker);
        }
    };

    auto plaseWait = [this, blockKeyboardInput](int ms){
        QLabel *waitLabel;  // Etykieta z napisem "Proszę czekać"

        // Utworzenie widgetu z napisem "Proszę czekać"
        waitLabel = new QLabel(this);
        waitLabel->setText("Proszę czekać");
        waitLabel->setAlignment(Qt::AlignCenter);
        waitLabel->setStyleSheet("QLabel { font-size: 32px; color: white; background-color: rgba(0, 0, 0, 180); }");

        // Umieszczenie napisu na całym oknie (przykrywając całe okno)
        waitLabel->setGeometry(0, 0, width(), height());

        waitLabel->show();
        waitLabel->raise();

        QTimer::singleShot(ms, this,[waitLabel, blockKeyboardInput]()
        {
            waitLabel->hide();
            blockKeyboardInput(false);
        }); 

        blockKeyboardInput(true);
    };

    auto *tabAP = new QLabel();
    tabAP->setText("Acces Point mode");
    tabAP->setAlignment(Qt::AlignCenter);
    tabAP->setStyleSheet("QLabel { font-size: 24px; }");
    
    
    
    auto *tabOFF = new QLabel();
    tabOFF->setText("Comunication disabled");
    tabOFF->setAlignment(Qt::AlignCenter);
    tabOFF->setStyleSheet("QLabel { font-size: 24px;  }");


    QWidget *tabWIFI = &tableView;//new QWidget();


    stackedWidget->addWidget(tabAP);


    stackedWidget->addWidget(tabOFF);
    stackedWidget->addWidget(tabWIFI);


    auto setPageEvent = [this,  plaseWait, stackedWidget, tabAP, tabWIFI, tabOFF, 
        label1,label2,label3, container1, container2, container3 ](int index) {

        // ustawiam zakladke 
        if(index == 0)
        {
            // tableView.setFocus(Qt::OtherFocusReason);
            stackedWidget->setCurrentWidget(tabOFF);
        }
        else if(index == -1) 
        {
            stackedWidget->setCurrentWidget(tabAP);
        }
        else if (index == 1)
        {
            stackedWidget->setCurrentWidget(tabWIFI);
            tableView.setFocus(Qt::OtherFocusReason);
        }

        // ustawiam komponent
        label1->setVisible(index == 1);
        label2->setVisible(index == 1);
        label3->setVisible(index == 1);
        container1->setVisible(index == 1);
        container2->setVisible(index == 1);
        container3->setVisible(index == 1);
    };

    wifiSlider->setValue(1);
    setPageEvent(wifiSlider->value());

    connect(wifiSlider, &WiFi::UI::WiFiSlider::valueChanged, this, [this, setPageEvent, plaseWait](int value) {
        qDebug() << "CustomSlider value changed:" << value;
        setPageEvent(value);
        if(value == 0)
            plaseWait(500);
        else
            plaseWait(1500);
    });
}

void WiFiConfigDialog::showEvent(QShowEvent* event) {
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

WiFiConfigDialog::~WiFiConfigDialog()
{
}

bool WiFiConfigDialog::eventFilter(QObject* obj, QEvent* event) {
    if (obj == &tableView && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // G2 
        if (keyEvent->key() == Qt::Key_A) {
            wifiSlider->setFocus();
            return true; // Zatrzymanie dalszego przetwarzania zdarzenia
        }

        // G1
        if (keyEvent->key() == Qt::Key_G) {
            wifiSlider->setFocus();
            return true; // Zatrzymanie dalszego przetwarzania zdarzenia
        }

        if (keyEvent->key() == Qt::Key_Q) {
            wifiSlider->setValue(-1);
        }
        if (keyEvent->key() == Qt::Key_W) {
            wifiSlider->setValue(0);
        }
        if (keyEvent->key() == Qt::Key_E) {
            wifiSlider->setValue(1);
        }

    }

    if (obj == wifiSlider && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            tableView.setFocus(Qt::OtherFocusReason);
            return true; // Zatrzymanie dalszego przetwarzania zdarzenia
        }
    }        

    return QWidget::eventFilter(obj, event); // Domyślne przetwarzanie
}
