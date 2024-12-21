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
          mMargin(4) {  // Domyślny margines
        setCursor(Qt::PointingHandCursor);  // Kursor w stylu "ręki"
        setFixedHeight(40);  // Domyślna wysokość widgetu
        resizeWidget();  // Przelicz szerokość widgetu na podstawie wysokości i marginesu
    }

    // Funkcja do ustawiania marginesu
    void setMargin(int margin) {
        mMargin = margin;
        resizeWidget();  // Przelicz wymiary na podstawie nowego marginesu
        update();  // Odśwież wygląd widgetu
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);

        // Włącz gładkie krawędzie
        painter.setRenderHint(QPainter::Antialiasing);

        // Rozmiar guzika obliczany na podstawie wysokości i marginesu
        int thumbSize = height() - 2 * mMargin;

        // Oblicz wymiary toru (tła)
        int trackHeight = height();
        int trackWidth = width();
        QRect trackRect(0, 0, trackWidth, trackHeight);

        // Rysowanie tła (toru)
        painter.setBrush(isChecked() ? mCheckedColor : mUncheckedColor);  // Kolor zależny od stanu
        painter.setPen(Qt::NoPen);  // Bez obramowania
        painter.drawRoundedRect(trackRect, trackHeight / 2, trackHeight / 2);

        // Oblicz pozycję guzika
        int thumbX = isChecked() ? (width() - thumbSize - mMargin) : mMargin;
        int thumbY = mMargin;
        QRect thumbRect(thumbX, thumbY, thumbSize, thumbSize);

        // Rysowanie guzika
        painter.setBrush(mThumbColor);
        painter.drawEllipse(thumbRect);
    }
    bool hitButton(const QPoint &pos) const override {
        // Zawsze zwracaj true, jeśli kliknięcie mieści się w geometrii widgetu
        return contentsRect().contains(pos);
    }

private:
    void resizeWidget() {
        // Szerokość widgetu obliczana na podstawie wysokości i marginesu
        int widgetWidth = height() * 3;  // Szerokość to 3x wysokość widgetu
        setFixedWidth(widgetWidth);  // Ustaw szerokość widgetu
    }

private:
    QColor mCheckedColor;    // Kolor tła, gdy przełącznik jest włączony
    QColor mUncheckedColor;  // Kolor tła, gdy przełącznik jest wyłączony
    QColor mThumbColor;      // Kolor przycisku (guzika)
    int mMargin;             // Margines wokół guzika
};
