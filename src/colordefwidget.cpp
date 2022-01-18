#include "colordefwidget.h"
#include "ui_colordefwidget.h"

#include <QStandardItemModel>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "QSSTextEdit/colordeftablemodel.h"
#include "path.h"
#include "qsstexteditmanager.h"
#include "QSSTextEdit/colordeftableview.h"
#include "QSSTextEdit/qssproxymodel.h"
#include "config.h"
#include "constants.h"

const QString ConfigEnableSort = "Custom/EnableSort";

ColorDefWidget::ColorDefWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorDefWidget)
{
    ui->setupUi(this);
    //defs = QssTextEditManager::getInstance()->getDefs();
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
    ColorDefTableProxyModel *proxyModle = new ColorDefTableProxyModel(this);
    proxyModle->setSourceModel(colorDefModel);
    ui->colorTableView->setModel(proxyModle);
    ui->colorTableView->setSortingEnabled(Config::getInstance()->value(ConfigEnableSort, false).toBool());
    //设置model之后才能在setSectionResizeMode时指定logiclIndex
    //不指定logiclIndex时设置Interactive不会自动拉伸.设置Stretch则不能调整section的width
    //将第一列设置Interactive则可调整第一列的宽度,从而可以调整第一列和第二列的大小,调整整个窗口时第二列自动拉伸,达到想要的效果
    ui->colorTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->colorTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    //ui->colorTableView->verticalHeader()->hide();
    connect(ui->colorTableView, &ColorDefTableView::customContextMenuRequested,
            this, &ColorDefWidget::slot_colorDefTableView_customContextMenuRequested);

}

void ColorDefWidget::setSourceTextEditVisible(bool isVisible)
{
    ui->textEdit->setVisible(isVisible);
    ui->colorTableView->setVisible(!isVisible);
}

void ColorDefWidget::updateDefs()
{
    //defs = QssTextEditManager::getInstance()->getDefs();
    colorDefModel->resetDefInfos(QssTextEditManager::getInstance()->getDefInfos());
    ui->textEdit->setText(QssTextEditManager::getInstance()->getCurDefsText());
}

void ColorDefWidget::addNewDef()
{
    bool ok = QssTextEditManager::getInstance()->addNewDef();
    if(!ok)
    {
        QMessageBox::critical(this, tr("Warning"),tr("Please set a name for the '%1' that is different from the other items").arg(Constants::NEW_DEFINE_NAME));
    }
}

void ColorDefWidget::saveConfig()
{
    Config::getInstance()->setValue(ConfigEnableSort,ui->colorTableView->isSortingEnabled());
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

void ColorDefWidget::slot_colorDefTableView_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(ui->colorTableView);
    {
        QAction *addNewDef = menu->addAction(tr("Add new"));
        connect(addNewDef, &QAction::triggered,[this]{
            this->addNewDef();
        });
        QAction *enableSortAction = new QAction(menu);
        menu->addAction(enableSortAction);
        QString enableSortActionText = ui->colorTableView->isSortingEnabled()?tr("Disable Sort"):tr("Enable Sort");
        enableSortAction->setText(enableSortActionText);
        connect(enableSortAction, &QAction::triggered,[this]{
            //先取消排序结果
            ui->colorTableView->model()->sort(-1);

            ui->colorTableView->setSortingEnabled(!ui->colorTableView->isSortingEnabled());

        });
    }
    QModelIndex index = ui->colorTableView->indexAt(pos);
    if(index.isValid())
    {
        QString key = colorDefModel->data(index, ColorDefTableModel::KeyRole).toString();
        QAction *removeCurDef = menu->addAction(tr("Remove"));
        connect(removeCurDef, &QAction::triggered,[key]{
            QssTextEditManager::getInstance()->removeDef(key);
        });
    }

    menu->exec(QCursor::pos());
    menu->deleteLater();

}
