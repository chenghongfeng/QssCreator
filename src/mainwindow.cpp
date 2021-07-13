#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QStackedWidget>
#include <QDebug>

#include "qss_helper.h"
#include "QssTextEdit/colordeftablemodel.h"
#include "config.h"
#include "path.h"

#include "configdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->defsTableWidget->setColumnCount(2);
    defListsModel = new QStringListModel();

    ui->defsTableWidget->setVisible(false);
    ui->colorListView->setVisible(false);

//    myModel = new MyModel(this);
//    ui->colorTableView->setModel(myModel);
    ui->colorListView->setVisible(false);;
    ui->defsTableWidget->setVisible(false);
    qssHighlighter = new QssHighlighter(ui->qssTextEdit->document());
    //ui->qssTextEdit->setTextBackgroundColor(QColor("#002b36"));
    QPalette palette(ui->qssTextEdit->palette());
    palette.setColor(QPalette::Base, QColor("#002b36"));
    palette.setColor(QPalette::Text, QColor("#839496"));
    ui->qssTextEdit->setPalette(palette);
    initSignalSlots();
    initSettings();
    m_configDialog = new ConfigDialog(this);

}

MainWindow::~MainWindow()
{
    saveSettings();
    Config::closeInstance();
    delete ui;
}

void MainWindow::initUi()
{
    QAction *a = new QAction("1",this);
    QTextEdit *e = new QTextEdit();
    QLabel *lbael = new QLabel("hgusdfahgsdffdsa\ndsfga");
    ui->widget->addPage(e, a);
    ui->widget->addPage(lbael, a);
//    //设置tab在左侧,此时tabbar的文字垂直显示;
//    ui->tabWidget->setTabPosition(QTabWidget::TabPosition::West);
//    //设置toolbutton的widget为label,再将tabText隐藏,这样就可以得到水平文字
//    QLabel *label1 = new QLabel();
//    label1->setText("tab 1");
//    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, label1);
//    ui->tabWidget->tabBar()->setTabText(0, "");
//    auto * tab_pane = ui->tabWidget->findChild<QStackedWidget *>();
//    //tab_pane->hide();
//    tab_pane->setFixedWidth(1);
//    auto widgets = ui->tabWidget->findChildren<QWidget *>(QString(),Qt::FindDirectChildrenOnly);
//    foreach (auto w, widgets) {
//        qDebug()<<w->objectName();
//        //w->hide();
//    }
//    this->update();
//    ui->tabWidget->hide();

//    QHBoxLayout *a = new QHBoxLayout(this);
//    a->addWidget(ui->tabWidget->tabBar());
//    ui->widget->setLayout(a);


}
void MainWindow::getDefs()
{
    if (m_strColorDefFile.isEmpty())
    {
        return;
    }
    QFile file(m_strColorDefFile);
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        defs = QssHelper::getColorDefineFromQStr(defsText,pattern);
    }
}



void MainWindow::initSignalSlots()
{
    //connect(ui->actionset, &QAction::trigger, this, &MainWindow::on_setAction_triggered);
}

void MainWindow::initSettings()
{
    QString fontFamily = Config::getInstance()->value("Font/family", "宋体").toString();
    int fontSize = Config::getInstance()->value("Font/size", 20).toInt();
    QFont font(fontFamily, fontSize);
    ui->qssTextEdit->setFont(font);
}

void MainWindow::saveSettings()
{
    Config::getInstance()->setValue("Font/family", ui->qssTextEdit->font().family());
    Config::getInstance()->setValue("Font/size", ui->qssTextEdit->font().pointSize());
}

void MainWindow::on_actionset_triggered()
{
    initUi();
    //m_configDialog->setVisible(!m_configDialog->isVisible());
//    bool ok;
//    QFont font = QFontDialog::getFont(&ok, ui->qssTextEdit->font(), this, tr("set editor font"));
//    if (ok)
//    {
//        ui->qssTextEdit->setFont(font);
//    }
}


void MainWindow::on_openColorDefFileBtn_clicked()
{
    //QString fileName = "F:/MyGitProject/qssHelper/qssFile/color.def";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), Path::getInstance()->qssDir(), tr("Color define file(*.qssdef)"));
    m_strColorDefFile = fileName;

}

void MainWindow::on_openQssFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), Path::getInstance()->qssDir(), tr("Skin file(*.qss)"));
    m_strQssFile = fileName;
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            ui->qssTextEdit->setText(str);
        }
    }
}

void MainWindow::on_applyBtn_clicked()
{
    getDefs();
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

}

void MainWindow::on_regLineEdit_textChanged(const QString &arg1)
{
    pattern = arg1;
}


void MainWindow::on_replaceBtn_clicked()
{
    QString resultText = ui->qssTextEdit->toPlainText();
    QssHelper::replaceDefsWithValues(resultText,defs);
    ui->qssTextEdit->setText(resultText);
}

void MainWindow::on_saveTextBtn_clicked()
{
    //QString fileName = QFileDialog::getSaveFileName(this, "打开文件", QString("%1/%2.css").arg(qApp->applicationDirPath()).arg(dirName), "皮肤文件(*.css)");
    QString fileName = QFileDialog::getSaveFileName(this, "打开文件", "F:/MyGitProject/qssHelper/qssFile/default.qss", "皮肤文件(*.qss)");
    QssHelper::writeQStrTofile(ui->qssTextEdit->toPlainText(),fileName);
}
