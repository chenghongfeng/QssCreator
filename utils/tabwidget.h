#ifndef TABWIDGET_H
#define TABWIDGET_H
#include "utils_global.h"
#include <QWidget>
QT_BEGIN_NAMESPACE
class QStackedWidget;
class QToolButton;
class QVBoxLayout;
class QIcon;
QT_END_NAMESPACE

class UTILS_EXPORT TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    void initUi();
    void addPage(QWidget *page, QAction *action);

signals:

private:
    QList<QToolButton *> m_buttons;
    QStackedWidget *m_stackedWidget{nullptr};
    QVBoxLayout *m_toolsLayout{nullptr};

};

#endif // TABWIDGET_H
