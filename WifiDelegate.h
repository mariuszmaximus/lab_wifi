#include <QStyledItemDelegate>
#include <QPainter>

class WifiDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    WifiDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    // void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
    //     if (index.column() == 0) { // Kolumna SSID
    //         QString ssid = index.data().toString();
    //         if (ssid == "Kadra") {
    //             painter->fillRect(option.rect, QColor(Qt::green)); // Zielone tło
    //         }
    //     }
    //     QStyledItemDelegate::paint(painter, option, index); // Standardowe rysowanie
    // }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {

        if (index.isValid()) {
            const auto model = index.model();
            if (model->data(model->index(index.row(), 0)).toString() == "Kadra") { // Assuming SSID is column 4
                painter->fillRect(option.rect, QColor(Qt::green));
            }
        }
        QStyledItemDelegate::paint(painter, option, index);
    }
};
