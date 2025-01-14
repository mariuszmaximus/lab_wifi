#include <QStyledItemDelegate>
#include <QPainter>

namespace WiFi::UI {

class WifiDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;
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

}; // namespace WiFi::UI