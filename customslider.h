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

class CustomSlider : public QSlider {
public:
    explicit CustomSlider(QWidget *parent = nullptr)
        : QSlider(Qt::Horizontal, parent),
            text("G2"),
            textColor(Qt::black)
    {

    }
    void setText(const QString &newText) {
        text = newText;
        update(); // Request a repaint
    }

    void setTextColor(const QColor &color) {
        textColor = color;
        update();
    }
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

private:
    QString text;         // Text displayed on the handle
    QColor textColor;    // Color of the displayed text
};
