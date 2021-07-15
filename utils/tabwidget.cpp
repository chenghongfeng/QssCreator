#include "tabwidget.h"

#include <QStackedWidget>
#include <QToolButton>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QVariant>
#include <QSplitter>

#define STACK_INDEX "stackedWidgetIndex"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    initUi();
}

void TabWidget::initUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    {
        QVBoxLayout *barLayout = new QVBoxLayout();
        m_toolsLayout = new QVBoxLayout();
        m_toolsLayout->setSizeConstraint(QLayout::SetMinimumSize);
        m_toolsLayout->setSpacing(0);
        barLayout->addLayout(m_toolsLayout);
        QSpacerItem *vSpacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        barLayout->addSpacerItem(vSpacerItem);
        mainLayout->addLayout(barLayout);
    }
    m_workArea->addWidget(m_stackedWidget);
    mainLayout->addWidget(m_workArea);
    this->setLayout(mainLayout);
}

void TabWidget::updateUi()
{
    QToolButton *toolBtn = nullptr;

    for (auto btn : m_buttons) {
        if(btn->isChecked())
        {
            toolBtn = btn;
            break;

        }
    }
    if (toolBtn)
    {
        m_stackedWidget->setVisible(true);
        m_stackedWidget->setCurrentIndex(toolBtn->property(STACK_INDEX).toInt());
    }
    else
    {
        m_stackedWidget->setVisible(false);
        this->update();
    }

}

void TabWidget::addAlwaysShowWidget(QList<QWidget *> widgets)
{
    m_addAlwaysShowWidgets.append(widgets);
    for (auto w : widgets) {
        m_workArea->addWidget(w);
    }
    updatAlwaysShowWidgets();
}

void TabWidget::updatAlwaysShowWidgets()
{

}

void TabWidget::addPage(QWidget *page, QAction *action)
{
    QToolButton *b = new QToolButton();
    b->setCheckable(true);
    //b->setDefaultAction(action);
    m_buttons.append(b);
    m_toolsLayout->addWidget(b);
    int index = m_stackedWidget->addWidget(page);
    b->setProperty(STACK_INDEX, QVariant(index));
    connect(b, QOverload<bool>::of(&QToolButton::clicked), this, &TabWidget::slot_toolBtn_clicked);

    updateUi();
}

void TabWidget::slot_toolBtn_clicked(bool checked)
{
    QToolButton *senderBtn = (QToolButton *)sender();
    for (auto btn : m_buttons) {
        if(btn != senderBtn)
            btn->setChecked(false);
    }
    updateUi();
}

