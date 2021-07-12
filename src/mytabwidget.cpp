#include "mytabwidget.h"
#include "ui_mytabwidget.h"

MytabWidget::MytabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MytabWidget)
{
    ui->setupUi(this);
}

MytabWidget::~MytabWidget()
{
    delete ui;
}
