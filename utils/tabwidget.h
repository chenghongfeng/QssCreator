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

//一个左侧是按钮,右侧是stackwidget,如果使用addAlwaysShowWidget,将和stackWidget加在一个QSplitter内
class UTILS_EXPORT TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    void addPage(QWidget *page, QAction *action);

protected:
    virtual void initUi();
    virtual void updateUi();
    virtual void addAlwaysShowWidget(QList<QWidget *> widgets);
    virtual void updatAlwaysShowWidgets();

public slots:
    void slot_toolBtn_clicked(bool checked);

signals:


private:
    QList<QToolButton *> m_buttons;
    QStackedWidget *m_stackedWidget { nullptr };
    QVBoxLayout *m_toolsLayout { nullptr };
    QList<QWidget *> m_addAlwaysShowWidgets;
    QSplitter * m_workArea { nullptr };
};

#endif // TABWIDGET_H
