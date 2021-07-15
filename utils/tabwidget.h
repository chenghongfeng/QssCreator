#ifndef TABWIDGET_H
#define TABWIDGET_H
#include "utils_global.h"
#include <QWidget>
QT_BEGIN_NAMESPACE
class QStackedWidget;
class QToolButton;
class QVBoxLayout;
class QIcon;
class QSplitter;
QT_END_NAMESPACE
//直接使用Tabwidget,将Tabwidget和qssTextEdit放入QSplitter会导致大小变化问题,修改比较麻烦,
//干脆直接加个 addAlwaysShowWidget函数将新加的窗口和stackwidget都放在QSPlitter里面

class UTILS_EXPORT TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    void addPage(QWidget *page, QAction *action);
    void setWorkAreaSplitterChildernCollapsible(bool b);
    virtual void addAlwaysShowWidget(QList<QWidget *> widgets);

protected:
    void initSignelSlots();
    virtual void initUi();
    virtual void updateUi();
    virtual void updatAlwaysShowWidgets();

public slots:
    void slot_toolBtn_clicked(bool checked);

signals:


private:
    QList<QToolButton *> m_buttons;
    QStackedWidget *m_stackedWidget { nullptr };
    QVBoxLayout *m_toolsLayout { nullptr };
    QList<QWidget *> m_addAlwaysShowWidgets;
    QSplitter * m_workAreaSplitter { nullptr };
};

#endif // TABWIDGET_H
