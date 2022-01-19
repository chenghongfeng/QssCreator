#include "previewqsswidget.h"
#include "ui_previewqsswidget.h"

#include <QTimer>
#include <QPainter>

PreviewQssWidget::PreviewQssWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewQssWidget)
{
    ui->setupUi(this);

    QTimer *progressTimer = new QTimer(this);
    progressTimer->setInterval(500);
    connect(progressTimer, &QTimer::timeout , this, &PreviewQssWidget::updateProgress);
    progressTimer->start();
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setWindowTitle(tr("Preview widget"));

    ui->TabWidget->setTabVisible(0,false);
    ui->TabWidget->setTabVisible(1,true);
    ui->TabWidget->setTabVisible(2,false);
    ui->TabWidget->setTabVisible(3,false);
}

PreviewQssWidget::~PreviewQssWidget()
{
    delete ui;
}

void PreviewQssWidget::updateProgress()
{
    int value = ui->progressBar->value();

    if(value == ui->progressBar->maximum())
        value = 0;
    else
        value++;

    ui->progressBar->setValue(value);
}
