#ifndef COLORDEFWIDGET_H
#define COLORDEFWIDGET_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QStringListModel;
QT_END_NAMESPACE
class ColorDefTableModel;

namespace Ui {
class ColorDefWidget;
}

class ColorDefWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorDefWidget(QWidget *parent = nullptr);
    ~ColorDefWidget();

    void initUi();

public slots:
    void updateDefs();

private slots:
    void on_applyBtn_clicked();

    void on_regLineEdit_textChanged(const QString &arg1);


private:
    Ui::ColorDefWidget *ui;
    QMap<QString,QString> defs;
    QString pattern;

    QStandardItemModel *defsTableModel {nullptr};

    QStringListModel *defListsModel {nullptr};
    QStringList colorDefs;

    ColorDefTableModel *colorDefModel {nullptr};
};

#endif // COLORDEFWIDGET_H
