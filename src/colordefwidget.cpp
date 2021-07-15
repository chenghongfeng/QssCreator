#include "colordefwidget.h"
#include "ui_colordefwidget.h"

#include <QStandardItemModel>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>

#include "QSSTextEdit/colordeftablemodel.h"
#include "path.h"
#include "qssmanager.h"

ColorDefWidget::ColorDefWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorDefWidget)
{
    ui->setupUi(this);
    defs = QssManager::getInstance()->getDefs();
    initUi();
}

ColorDefWidget::~ColorDefWidget()
{
    delete ui;
}

void ColorDefWidget::initUi()
{


    ui->colorTableView->setSelectionBehavior(QHeaderView::SelectItems);
    ui->defsTableWidget->setColumnCount(2);
    defListsModel = new QStringListModel();

    ui->defsTableWidget->setVisible(false);
    ui->colorListView->setVisible(false);

//    myModel = new MyModel(this);
//    ui->colorTableView->setModel(myModel);
    ui->colorListView->setVisible(false);;
    ui->defsTableWidget->setVisible(false);
}

void ColorDefWidget::on_applyBtn_clicked()
{
    //getDefs();
    ui->defsTableWidget->clear();
    ui->defsTableWidget->setRowCount(0);
    colorDefs.clear();
    auto iter = defs.constBegin();
    int i = 0;
    while (iter != defs.constEnd())
    {
        ui->defsTableWidget->insertRow(i);
        QTableWidgetItem *keyItem = new QTableWidgetItem(iter.key());
        QTableWidgetItem *valueItem = new QTableWidgetItem(iter.value());
        colorDefs.append(iter.key());
        ui->defsTableWidget->setItem(i,0,keyItem);
        ui->defsTableWidget->setItem(i,1,valueItem);
        iter++;
        i++;
    }
    defListsModel->setStringList(colorDefs);
    ui->colorListView->setModel(defListsModel);

    colorDefModel = new ColorDefTableModel(defs, this);
    QSortFilterProxyModel *proxyModle = new QSortFilterProxyModel(this);
    proxyModle->setSourceModel(colorDefModel);
    proxyModle->sort(1);
    ui->colorTableView->setModel(proxyModle);
    //设置model之后才能在setSectionResizeMode时指定logiclIndex
    //不指定logiclIndex时设置Interactive不会自动拉伸.设置Stretch则不能调整section的width
    //将第一列设置Interactive则可调整第一列的宽度,从而可以调整第一列和第二列的大小,调整整个窗口时第二列自动拉伸,达到想要的效果
    ui->colorTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->colorTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
}

void ColorDefWidget::on_regLineEdit_textChanged(const QString &arg1)
{
    pattern = arg1;
}

void ColorDefWidget::on_openColorDefFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), Path::getInstance()->qssDir(), tr("Color define file(*.qssdef)"));
    //m_strColorDefFile = fileName;
}
