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
          mThumbColor("#FFFFFF"),  // Biały kolor "guzika" (przycisk)
          guzikSize(18) {  // Domyślna średnica guzika
        setCursor(Qt::PointingHandCursor);  // Ustawia kursor w stylu "ręki" przy najechaniu
        resizeWidget();  // Dostosuj wymiary widgetu do rozmiaru guzika
    }

    // Funkcja do ustawiania rozmiaru guzika
    void setGuzikSize(int size) {
        guzikSize = size;
        resizeWidget();  // Przelicz wymiary na podstawie nowego rozmiaru guzika
        update();  // Odśwież wygląd widgetu
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);

        // Włącz gładkie krawędzie
        painter.setRenderHint(QPainter::Antialiasing);

        // Oblicz dynamiczne wymiary toru
        int trackHeight = guzikSize+4;// / 2;  // Wysokość toru to połowa średnicy guzika
        int trackWidth = width() - guzikSize;  // Szerokość toru zależy od szerokości widgetu
        int trackY = (height() - trackHeight) / 2;  // Środek toru w pionie
        QRect trackRect(guzikSize / 4, trackY, trackWidth, trackHeight);

        // Rysowanie tła (toru)
        painter.setBrush(isChecked() ? mCheckedColor : mUncheckedColor);  // Kolor tła zależny od stanu
        painter.setPen(Qt::NoPen);  // Bez obramowania
        painter.drawRoundedRect(trackRect, trackHeight / 2, trackHeight / 2);

        // Oblicz pozycję guzika
        int thumbX = isChecked() ? (width() - guzikSize - guzikSize / 4) : guzikSize / 4;
        int thumbY = (height() - guzikSize) / 2;
        QRect thumbRect(thumbX, thumbY, guzikSize, guzikSize);

        // Rysowanie guzika
        painter.setBrush(mThumbColor);
        painter.drawEllipse(thumbRect);
    }

private:
    void resizeWidget() {
        int widgetHeight = guzikSize + guzikSize / 4;  // Wysokość widgetu zależy od rozmiaru guzika
        int widgetWidth = guzikSize * 3;  // Szerokość widgetu to 3 razy średnica guzika
        setFixedSize(widgetWidth, widgetHeight);  // Ustaw wymiary widgetu
    }

private:
    QColor mCheckedColor;    // Kolor tła, gdy przełącznik jest włączony
    QColor mUncheckedColor;  // Kolor tła, gdy przełącznik jest wyłączony
    QColor mThumbColor;      // Kolor przycisku (guzika)
    int guzikSize;           // Średnica guzika
};
