#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QPaintEvent>

class WiFiSwitch : public QCheckBox {
    Q_OBJECT

public:
    explicit WiFiSwitch(QWidget *parent = nullptr)
        : QCheckBox(parent),
          mCheckedColor("#2196F3"),  // Niebieski kolor tła, gdy przełącznik jest włączony
          mUncheckedColor("#BDBDBD"),  // Szary kolor tła, gdy przełącznik jest wyłączony
          mThumbColor("#FFFFFF") {  // Biały kolor "guzika" (przyciski)
        setCursor(Qt::PointingHandCursor);  // Ustawia kursor w stylu "ręki" przy najechaniu
        setFixedHeight(40);  // Wysokość całego widgetu przełącznika w pikselach
        setFixedWidth(60);   // Szerokość całego widgetu przełącznika w pikselach
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);

        // Rysowanie tła (toru)
        painter.setRenderHint(QPainter::Antialiasing);  // Włącz gładkie krawędzie
        painter.setBrush(isChecked() ? mCheckedColor : mUncheckedColor);  // Kolor tła zależny od stanu
        painter.setPen(Qt::NoPen);  // Bez obramowania
        QRect trackRect(4, 8, 52, 14);  
        /*
         * Prostokąt "toru" przełącznika:
         * - (4, 8): Pozycja lewego górnego rogu prostokąta (x=4, y=8 pikseli)
         * - szerokość: 52 piksele
         * - wysokość: 14 pikseli
         * - odpowiada "ścieżce", po której porusza się guzik
         */
        painter.drawRoundedRect(trackRect, 7, 7);  // Rysowanie zaokrąglonego prostokąta z promieniem łuku 7 px

        // Rysowanie "guzika" (przycisku)
        QRect thumbRect(isChecked() ? 38 : 6, 6, 18, 18);
        /*
         * Prostokąt przycisku:
         * - Pozycja pozioma (x):
         *    * Jeśli przełącznik jest włączony (isChecked): x = 38 (przycisk po prawej stronie)
         *    * Jeśli przełącznik jest wyłączony: x = 6 (przycisk po lewej stronie)
         * - Pozycja pionowa (y): zawsze 6 pikseli od góry
         * - szerokość: 18 pikseli
         * - wysokość: 18 pikseli
         * - odpowiada kształtowi "guzika" poruszającego się po torze
         */
        painter.setBrush(mThumbColor);  // Kolor guzika
        painter.drawEllipse(thumbRect);  // Rysowanie przycisku jako elipsy
    }

private:
    QColor mCheckedColor;    // Kolor tła, gdy przełącznik jest włączony
    QColor mUncheckedColor;  // Kolor tła, gdy przełącznik jest wyłączony
    QColor mThumbColor;      // Kolor przycisku (guzika)
};
