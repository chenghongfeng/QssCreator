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
    QAction *a = new QAction("1",this);
    QTextEdit *e = new QTextEdit();
    QLabel *lbael = new QLabel("hgusdfahgsdffdsa\ndsfga");
    ColorDefWidget *widget = new ColorDefWidget();
//    ui->widget->addPage(e, a);
//    ui->widget->addPage(lbael, a);
//    ui->widget->addPage(widget,a);


    {
        //init m_tabWidget
        m_tabWidget = new TabWidget(this);
        m_tabWidget->addPage(e, a);
        m_tabWidget->addPage(lbael, a);
        m_tabWidget->addPage(widget,a);
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
    }

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(m_tabWidget);
    splitter->addWidget(m_textEdit);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(splitter);
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
    QFont font(fontFamily, fontSize);
    m_textEdit->setFont(font);
}

void MainWindow::saveSettings()
{
    Config::getInstance()->setValue("Font/family", m_textEdit->font().family());
    Config::getInstance()->setValue("Font/size", m_textEdit->font().pointSize());
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


void MainWindow::on_openQssFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), Path::getInstance()->qssDir(), tr("Skin file(*.qss)"));
    m_strQssFile = fileName;
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            m_textEdit->setText(str);
        }
    }
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
