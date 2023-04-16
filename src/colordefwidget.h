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
    void setSourceTextEditVisible(bool isShow);
    void updateDefs();
    void addNewDef();
    void saveConfig();

private slots:

    void slot_colorDefTableView_customContextMenuRequested(const QPoint &pos);


private:
    Ui::ColorDefWidget *ui;

    QStandardItemModel *defsTableModel {nullptr};

    QStringList colorDefs;

    ColorDefTableModel *colorDefModel {nullptr};
};

#endif // COLORDEFWIDGET_H
