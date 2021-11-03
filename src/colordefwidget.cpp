#include "colordefwidget.h"
#include "ui_colordefwidget.h"

#include <QStandardItemModel>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>

#include "QSSTextEdit/colordeftablemodel.h"
#include "path.h"
#include "qsstexteditmanager.h"

ColorDefWidget::ColorDefWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorDefWidget)
{
    ui->setupUi(this);
    defs = QssTextEditManager::getInstance()->getDefs();
    initUi();

    connect(QssTextEditManager::getInstance(), &QssTextEditManager::defsUpdated, this, &ColorDefWidget::updateDefs);
    connect(QssTextEditManager::getInstance(), &QssTextEditManager::sourceTextEditVisibleChange, this, &ColorDefWidget::setSourceTextEditVisible);
}

ColorDefWidget::~ColorDefWidget()
{
    delete ui;
}

void ColorDefWidget::initUi()
{

    ui->textEdit->setVisible(false);
    //目前设置成无法编辑的状态
    ui->textEdit->setEnabled(false);
    ui->colorTableView->setSelectionBehavior(QHeaderView::SelectItems);


#ifdef Show_ListView
    defListsModel = new QStringListModel();
#else
    ui->colorListView->setVisible(false);
#endif

    colorDefModel = new ColorDefTableModel(QssTextEditManager::getInstance()->getDefInfos(), this);
    ui->textEdit->setText(QssTextEditManager::getInstance()->getCurDefsText());
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

void ColorDefWidget::setSourceTextEditVisible(bool isVisible)
{
    ui->textEdit->setVisible(isVisible);
    ui->colorTableView->setVisible(!isVisible);
}

void ColorDefWidget::updateDefs()
{
    defs = QssTextEditManager::getInstance()->getDefs();
    colorDefModel->resetDefInfos(QssTextEditManager::getInstance()->getDefInfos());
    ui->textEdit->setText(QssTextEditManager::getInstance()->getCurDefsText());
    //colorDefModel->resetDefMap(defs);
}

void ColorDefWidget::addNewDef()
{
    QssTextEditManager::getInstance()->addNewDef();
}


void ColorDefWidget::on_applyBtn_clicked()
{
#ifdef Show_ListView
        colorDefs.clear();
        auto iter = defs.constBegin();
        while (iter != defs.constEnd())
        {
            colorDefs.append(iter.key());
            iter++;
        }
        defListsModel->setStringList(colorDefs);
        ui->colorListView->setModel(defListsModel);
#endif


}

void ColorDefWidget::on_regLineEdit_textChanged(const QString &arg1)
{
    pattern = arg1;
}
