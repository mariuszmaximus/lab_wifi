#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QPaintEvent>

namespace WiFi::UI {
class WiFiSwitch : public QCheckBox {
    Q_OBJECT

public:
    explicit WiFiSwitch(QWidget *parent = nullptr)
        : QCheckBox(parent),
          mCheckedColor("#2196F3"),  // Niebieski kolor tła, gdy przełącznik jest włączony
          mUncheckedColor("#BDBDBD"),  // Szary kolor tła, gdy przełącznik jest wyłączony
          mThumbColor("#FFFFFF"),  // Biały kolor "guzika" (przycisk)
          mMargin(4),  // Domyślny margines
          mTextOn("ON"),  // Tekst, gdy przełącznik jest włączony
          mTextOff("OFF") {  // Tekst, gdy przełącznik jest wyłączony
        setCursor(Qt::PointingHandCursor);  // Kursor w stylu "ręki"
    }

    // Funkcja do ustawiania marginesu
    void setMargin(int margin) {
        mMargin = margin;
        update();  // Odśwież wygląd widgetu
    }

    // Funkcja do ustawiania tekstów
    void setTexts(const QString &textOn, const QString &textOff) {
        mTextOn = textOn;
        mTextOff = textOff;
        update();  // Odśwież wygląd widgetu
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);


        // qDebug() << "width:"<<width() <<"   height:"<< height();

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

        // Rysowanie tekstu na środku guzika
        painter.setPen(Qt::black);  // Ustaw kolor tekstu (czarny dla kontrastu)
        painter.setFont(QFont("Arial", 10, QFont::Bold));  // Ustaw font
        QString text = isChecked() ? mTextOn : mTextOff;  // Tekst zależny od stanu
        painter.drawText(thumbRect, Qt::AlignCenter, text);  // Rysuj tekst na środku guzika
    }

    bool hitButton(const QPoint &pos) const override {
        // Zawsze zwracaj true, jeśli kliknięcie mieści się w geometrii widgetu
        return contentsRect().contains(pos);
    }


private:
    QColor mCheckedColor;    // Kolor tła, gdy przełącznik jest włączony
    QColor mUncheckedColor;  // Kolor tła, gdy przełącznik jest wyłączony
    QColor mThumbColor;      // Kolor przycisku (guzika)
    int mMargin;             // Margines wokół guzika
    QString mTextOn;         // Tekst, gdy przełącznik jest włączony
    QString mTextOff;        // Tekst, gdy przełącznik jest wyłączony
};

}; // namespace WiFi::Ui