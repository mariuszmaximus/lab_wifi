#pragma once 

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QApplication>
#include <QKeyEvent>
#include <QObject>

class KeyboardBlocker : public QObject {
    Q_OBJECT

public:
    explicit KeyboardBlocker(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            // Ignorowanie zdarzeń klawiatury
            return true;  // Zwracamy true, aby zignorować zdarzenie
        }
        // Przekazywanie innych zdarzeń
        return QObject::eventFilter(obj, event);
    }
};
