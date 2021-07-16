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
#include <QSplitter>
#include <QHBoxLayout>

#include "qss_helper.h"
#include "config.h"
#include "path.h"
#include "qssmanager.h"

#include "configdialog.h"
#include "colordefwidget.h"
#include "QSSTextEdit/qsstextedit.h"
#include "tabwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUi();
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
    //init m_tabWidget
    m_tabWidget = new TabWidget(this);
    QTextEdit *e = new QTextEdit();
    QLabel *lbael = new QLabel("hgusdfahgsdffdsa\ndsfga");
    ColorDefWidget *colorWidget = new ColorDefWidget();
    QAction *a = new QAction("1",this);
    {
        QAction *showColorDefAction = new QAction(this);
        showColorDefAction->setToolTip(tr("Show/Hide color defines widget"));
        showColorDefAction->setCheckable(true);
        showColorDefAction->setChecked(true);
        m_tabWidget->addPage(colorWidget,showColorDefAction);
        m_tabWidget->addPage(e, a);
        m_tabWidget->addPage(lbael, a);
    }

    {
        //init m_textEdit
        m_textEdit = new QssTextEdit(this);
        qssHighlighter = new QssHighlighter(m_textEdit->document());
        //ui->qssTextEdit->setTextBackgroundColor(QColor("#002b36"));
        QPalette palette(m_textEdit->palette());
        palette.setColor(QPalette::Base, QColor("#002b36"));
        palette.setColor(QPalette::Text, QColor("#839496"));
        m_textEdit->setPalette(palette);
        QWidgetList list;
        list.append(m_textEdit);

        m_tabWidget->addAlwaysShowWidget(list);
        m_tabWidget->setWorkAreaSplitterChildernCollapsible(false);
    }

    //QSplitter *splitter = new QSplitter(this);
    //splitter->addWidget(m_tabWidget);
    //splitter->addWidget(m_textEdit);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_tabWidget);
    ui->centralwidget->setLayout(layout);

}


void MainWindow::initSignalSlots()
{
    //connect(ui->actionset, &QAction::trigger, this, &MainWindow::on_setAction_triggered);
}

void MainWindow::initSettings()
{
    QString fontFamily = Config::getInstance()->value("Font/family", "宋体").toString();
    int fontSize = Config::getInstance()->value("Font/size", 20).toInt();
    QString fileName = Config::getInstance()->value("Qss/UserQssFilePath", Path::getInstance()->qssFilePath()).toString();
    QFont font(fontFamily, fontSize);

    m_textEdit->setFont(font);
    m_textEdit->setFile(fileName);
}

void MainWindow::saveSettings()
{
    Config::getInstance()->setValue("Font/family", m_textEdit->font().family());
    Config::getInstance()->setValue("Font/size", m_textEdit->font().pointSize());
}


void MainWindow::on_replaceBtn_clicked()
{
    QString resultText = m_textEdit->toPlainText();
    QssHelper::replaceDefsWithValues(resultText,QssManager::getInstance()->getDefs());
    m_textEdit->setText(resultText);
}

void MainWindow::on_saveTextBtn_clicked()
{
    //QString fileName = QFileDialog::getSaveFileName(this, "打开文件", QString("%1/%2.css").arg(qApp->applicationDirPath()).arg(dirName), "皮肤文件(*.css)");
    QString fileName = QFileDialog::getSaveFileName(this, "打开文件", "F:/MyGitProject/qssHelper/qssFile/default.qss", "皮肤文件(*.qss)");
    QssHelper::writeQStrTofile(m_textEdit->toPlainText(),fileName);
}

void MainWindow::on_actionOpenQssFile_triggered()
{
    QString fileName = Config::getInstance()->value("Qss/UserQssFile", Path::getInstance()->qssFilePath()).toString();
    fileName = QFileDialog::getOpenFileName(this, tr("Open qss file"), Path::cdUp(fileName), tr("Skin file(*.qss)"));
    if (fileName == QString())
        return;
    Config::getInstance()->setValue("Qss/UserQssFilePath",fileName);
    //m_strQssFile = fileName;
    m_textEdit->setFile(fileName);
}

void MainWindow::on_actionOpenDefineFile_triggered()
{
    QString fileName = Config::getInstance()->value("Qss/UserColorDefineFile", Path::getInstance()->colorDefFilePath()).toString();
    fileName = QFileDialog::getOpenFileName(this, tr("Open color define file"), Path::cdUp(fileName), tr("Color define file(*.qssdef)"));
    if (fileName == QString())
        return;
    Config::getInstance()->setValue("Qss/UserColorDefineFile",fileName);
    //m_strQssFile = fileName;
}
