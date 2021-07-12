#include "tabwidget.h"

#include <QStackedWidget>
#include <QToolButton>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QVariant>

#define STACK_INDEX "stackedWidgetIndex"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    initUi();
}

void TabWidget::initUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    {
        QVBoxLayout *barLayout = new QVBoxLayout();
        m_toolsLayout = new QVBoxLayout();
        barLayout->addLayout(m_toolsLayout);
        //QSpacerItem *vSpacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        barLayout->addSpacing(20);
        mainLayout->addLayout(barLayout);
    }
    mainLayout->addWidget(m_stackedWidget);
    this->setLayout(mainLayout);
}

void TabWidget::addPage(QWidget *page, QAction *action)
{
    QToolButton *b = new QToolButton();
    b->setDefaultAction(action);
    m_buttons.append(b);
    m_toolsLayout->addWidget(b);
    int index = m_stackedWidget->addWidget(page);
    b->setProperty(STACK_INDEX, QVariant(index));
}

