#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QSlider>
#include <QDebug>

class WiFiSwitch3state : public QSlider {
    Q_OBJECT

public:
    explicit WiFiSwitch3state(QWidget *parent = nullptr)
        : QSlider(parent),
          mCheckedColor("#2196F3"),  // Niebieski kolor tła, gdy przełącznik jest włączony
          mUncheckedColor("#BDBDBD"),  // Szary kolor tła, gdy przełącznik jest wyłączony
          mThumbColor("#FFFFFF"),  // Biały kolor "guzika" (przycisk)
          mMargin(4),  // Domyślny margines
          mTextOn("ON"),  // Tekst, gdy przełącznik jest włączony
          mTextOff("OFF")   // Tekst, gdy przełącznik jest wyłączony
    {
        setMinimum(-1);
        setMaximum(1);
        //setCursor(Qt::PointingHandCursor);  // Kursor w stylu "ręki"
        // setFocusPolicy(Qt::StrongFocus); // Umożliw fokus
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

    // void setvalue(int value) {
    //     if (m_value != value) { // Sprawdzamy, czy wartość faktycznie się zmienia
    //         m_value = value;
    //         update(); // Wywołujemy przerysowanie widgetu
    //     }
    // }
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
        // painter.setBrush(isChecked() ? mCheckedColor : mUncheckedColor);  // Kolor zależny od stanu
        if(value() <0)
            painter.setBrush(mCheckedColor);  // Kolor zależny od stanu
        else if(value() > 0)
            painter.setBrush(mCheckedColor);  // Kolor zależny od stanu
        else
            painter.setBrush(mUncheckedColor);  // Kolor zależny od stanu



        painter.setPen(Qt::NoPen);  // Bez obramowania
        painter.drawRoundedRect(trackRect, trackHeight / 2, trackHeight / 2);

        // Oblicz pozycję guzika
        //int thumbX = isChecked() ? (width() - thumbSize - mMargin) : mMargin;
        int thumbX = mMargin;
        if(value() <0)
            thumbX = mMargin;
        else if(value() > 0)
            thumbX = (width() - thumbSize - mMargin);
        else
            thumbX = ((width() - thumbSize - mMargin) + mMargin)/2;



        int thumbY = mMargin;
        QRect thumbRect(thumbX, thumbY, thumbSize, thumbSize);

        // Rysowanie guzika
        painter.setBrush(mThumbColor);
        painter.drawEllipse(thumbRect);

        // Rysowanie tekstu na środku guzika
        painter.setPen(Qt::black);  // Ustaw kolor tekstu (czarny dla kontrastu)
        painter.setFont(QFont("Arial", 10, QFont::Bold));  // Ustaw font
        // QString text = isChecked() ? mTextOn : mTextOff;  // Tekst zależny od stanu
        QString text = mTextOn;
        painter.drawText(thumbRect, Qt::AlignCenter, text);  // Rysuj tekst na środku guzika
    }

    // bool hitButton(const QPoint &pos) const override {
    //     // Zawsze zwracaj true, jeśli kliknięcie mieści się w geometrii widgetu
    //     return contentsRect().contains(pos);
    // }


    // void keyPressEvent(QKeyEvent *event) override {
    //     qDebug() << "keyPressEvent " << event;

    //     if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Enter) {
    //         toggle();
    //     } else {
    //         QAbstractButton::keyPressEvent(event);
    //     }
    // }

private:
    QColor mCheckedColor;    // Kolor tła, gdy przełącznik jest włączony
    QColor mUncheckedColor;  // Kolor tła, gdy przełącznik jest wyłączony
    QColor mThumbColor;      // Kolor przycisku (guzika)
    int mMargin;             // Margines wokół guzika
    QString mTextOn;         // Tekst, gdy przełącznik jest włączony
    QString mTextOff;        // Tekst, gdy przełącznik jest wyłączony

    // int m_value{0};
};
