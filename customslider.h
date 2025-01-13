#pragma once

#include <QApplication>
#include <QSlider>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QApplication>
#include <QSlider>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QDebug>
// #include <qglobal.h>

class CustomSlider : public QSlider {
public:
    explicit CustomSlider(QWidget *parent = nullptr)
        : QSlider(Qt::Horizontal, parent),
            text("G2"),
            textColor(Qt::black)
    {
        setMinimum(-1);
        setMaximum(1);

        // QSlider 
        //   height: 
        //   border:   
        //    -- wyliczan z height i border
        //      - min-height = height - border*2
        //      - radius = height/2   
        //  QSlider::groove:horizontal
        //     

        styleStr = R"(
            QSlider {
                min-height: 40px; /* jak jest border to musi byc min-height = height - border*2 np. aby bylo 44 to  (44 -4 -4) = 36  */
                max-height: 40px;
                background: #BDBDBD;
                border: 5px solid #BDBDBD; /* Ustawia kolor obramowania na czerwony */
                border-radius: 25px;
            }

            QSlider::groove:horizontal {
                height: 10px;
                background: #FFBDBD;
                margin: 0 10px; /* 0-gora/dół 20px-lewo/prawo */
                border-radius: 5px;

            }

            QSlider::handle:horizontal {
                background: #FFFFFF;
                border: 5px solid #FFFFFF;
                width: 30px; /* 40px-5px*2 = 30px */
                height: 1px;
                margin: -15 -10px;
                border-radius: 20px; /* Okrągły uchwyt */
            }
        )" ;
        setStyleSheet(styleStr.c_str());
    }
    void setText(const QString &newText) {
        text = newText;
        update(); // Request a repaint
    }

    void setTextColor(const QColor &color) {
        textColor = color;
        update();
    }

    std::string styleStr;
protected:
    void paintEvent(QPaintEvent *event) override {
        QSlider::paintEvent(event); // Rysuje standardowy suwak

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setFont(QFont("Arial", 10, QFont::Bold));

        // Inicjalizacja opcji stylu suwaka
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        // Pobranie prostokąta uchwytu suwaka
        QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        // Wyznaczenie środkowej pozycji uchwytu
        QPoint handleCenter = handleRect.center();

        // Tekst do narysowania
        QFontMetrics fontMetrics(painter.font());
        QRect textRect = fontMetrics.boundingRect(text);

        // Wyznaczenie pozycji tekstu, aby był wyśrodkowany na uchwycie
        // int textX = handleCenter.x() - textRect.width() / 2;
        // int textY = handleCenter.y() + textRect.height() / 2;
        int textWidth = fontMetrics.horizontalAdvance(text);
        int textX = handleCenter.x() - textWidth / 2;
        int textY = handleCenter.y() + (textRect.height() / 2) - fontMetrics.descent(); ;

        // Rysowanie tekstu "G2" na uchwycie
        painter.setPen(textColor);
        painter.drawText(textX, textY, text);
    }
    void sliderChange(SliderChange change) override 
    {
        qDebug() << "sliderChange "<< change;
        QSlider::sliderChange(change);
        updateStyle();
    }


    void focusInEvent(QFocusEvent* event) override {
        QSlider::focusInEvent(event);  // Wywołanie bazowego zachowania
        qDebug() << "Focus gained";
        updateStyle();
    }

    void focusOutEvent(QFocusEvent* event) override {
        QSlider::focusOutEvent(event);  // Wywołanie bazowego zachowania
        qDebug() << "Focus lost";
        updateStyle();
    }
private:
    QString text;         // Text displayed on the handle
    QColor textColor;    // Color of the displayed text

    void updateStyle() {

        auto text = styleStr;

        qDebug() << "value() = " << value();

        if(value() != 0)
        {
            std::string from = "BDBDBD";
            std::string to = "2196F3";

            size_t pos = text.find(from);
            while (pos != std::string::npos) {
                text.replace(pos, from.length(), to);
                pos = text.find(from, pos + to.length());
            }
        }

        if(hasFocus())
        {
            std::string from = "FFFFFF";
            std::string to = "00FF00";

            size_t pos = text.find(from);
            while (pos != std::string::npos) {
                text.replace(pos, from.length(), to);
                pos = text.find(from, pos + to.length());
            }
        }


        setStyleSheet(text.c_str());
    }
};
