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
#include <QSizePolicy>
#include <QMessageBox>

#include "qss_helper.h"
#include "config.h"
#include "path.h"
#include "qsstexteditmanager.h"

#include "textsettingswidget.h"
#include "colordefwidget.h"
#include "QSSTextEdit/qsstextedit.h"
#include "tabwidget.h"
#include "fancytabwidget.h"
#include "utilsicons.h"

#define USE_FANCYTABWIDGET
#ifdef INTERNAL_TEST
#include <QLineEdit>
#endif


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUi();
    initSignalSlots();
    initSettings();

}

MainWindow::~MainWindow()
{
    saveSettings();
    Config::closeInstance();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    qDebug()<< "------------MainWindow---------------";
    qDebug()<< "QKeyEvent Text:"<<e->text();
    qDebug()<< "QKeyEvent Count:"<<e->count();
    qDebug()<< "QKeyEvent isAutoRepeat:"<<e->isAutoRepeat();
    if(e->modifiers().testFlag(Qt::ControlModifier)){qDebug()<<"QKeyEvent modifier:"<<"ControlModifier";}
    if(e->modifiers().testFlag(Qt::NoModifier)){qDebug()<<"QKeyEvent modifier:"<<"NoModifier";}
    if(e->modifiers().testFlag(Qt::ShiftModifier)){qDebug()<<"QKeyEvent modifier:"<<"ShiftModifier";}
    if(e->modifiers().testFlag(Qt::AltModifier)){qDebug()<<"QKeyEvent modifier:"<<"AltModifier";}
    if(e->modifiers().testFlag(Qt::MetaModifier)){qDebug()<<"QKeyEvent modifier:"<<"ControlModifier";}
    if(e->modifiers().testFlag(Qt::KeypadModifier)){qDebug()<<"QKeyEvent modifier:"<<"ControlModifier";}
    if(e->modifiers().testFlag(Qt::GroupSwitchModifier)){qDebug()<<"QKeyEvent modifier:"<<"ControlModifier";}
    if(e->modifiers().testFlag(Qt::KeyboardModifierMask)){qDebug()<<"QKeyEvent modifier:"<<"KeyboardModifierMask";}
    qDebug()<< "QKeyEvent key:"<<e->key();
    qDebug()<< "-------------------------------------";
    return QMainWindow::keyPressEvent(e);
}

void MainWindow::initUi()
{
#ifdef USE_FANCYTABWIDGET
    Core::Internal::FancyTabWidget * fancyTabWidget = new Core::Internal::FancyTabWidget(this);
#else
    //init m_tabWidget
    m_tabWidget = new TabWidget(this);
#endif
    ColorDefWidget *colorWidget = new ColorDefWidget();
    {
        //添加颜色定义页
        QAction *showColorDefAction = new QAction(this);
        showColorDefAction->setToolTip(tr("Show/Hide color defines widget"));
        showColorDefAction->setCheckable(true);
        showColorDefAction->setChecked(true);//默认显示此页
#ifdef USE_FANCYTABWIDGET
        fancyTabWidget->insertTab(0,colorWidget,Utils::Icons::HOME.icon(),tr("Colors"),false);
        fancyTabWidget->setTabEnabled(0,true);
#else
        m_tabWidget->addPage(colorWidget,showColorDefAction);
#endif


        //添加设置页
        QAction *showConfigAction = new QAction(this);
        showConfigAction->setToolTip(tr("Show/Hide color config widget"));
        showConfigAction->setCheckable(true);
       // showConfigAction->setChecked(true);//默认显示此页
        m_textSettingsWidget = new TextSettingsWidget(getFontFromConfig(), this);
        connect(m_textSettingsWidget, &TextSettingsWidget::fontSsttingsChanged,
                this, &MainWindow::slot_fontSettingsChanged);
#ifdef USE_FANCYTABWIDGET
        fancyTabWidget->insertTab(1,m_textSettingsWidget,Utils::Icons::HOME.icon(),tr("Settings"),false);
        fancyTabWidget->setTabEnabled(1,true);
#else
        m_tabWidget->addPage(m_textSettingsWidget, showConfigAction);
#endif

    }

    {
        //init m_textEdit
        m_textEdit = new QssTextEdit(this);
        qssHighlighter = new QssHighlighter(m_textEdit->document());
        m_textEdit->setDefKeyword(QssTextEditManager::getInstance()->getDefs().keys());
        m_textEdit->initCompleter();
        //ui->qssTextEdit->setTextBackgroundColor(QColor("#002b36"));
        QPalette palette(m_textEdit->palette());
        palette.setColor(QPalette::Base, QColor("#002b36"));
        palette.setColor(QPalette::Text, QColor("#839496"));
        m_textEdit->setPalette(palette);
        QWidgetList list;
        list.append(m_textEdit);

#ifdef USE_FANCYTABWIDGET
    fancyTabWidget->addAlwaysShowWidget(list);
    fancyTabWidget->setWorkAreaSplitterChildernCollapsible(false);
#else
        m_tabWidget->addAlwaysShowWidget(list);
        m_tabWidget->setWorkAreaSplitterChildernCollapsible(false);
#endif

    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
#ifdef INTERNAL_TEST
//    QVBoxLayout *testLayout = new QVBoxLayout();
//    QLineEdit *lineEdit = new QLineEdit(this);
//    connect(m_textEdit, &QssTextEdit::completionPrefixChanged,
//            lineEdit, &QLineEdit::setText);
//    testLayout->addWidget(lineEdit);
//    layout->addLayout(testLayout,1);
#endif

#ifdef USE_FANCYTABWIDGET
     layout->addWidget(fancyTabWidget,2);
#else
    layout->addWidget(m_tabWidget,2);
#endif
    ui->centralwidget->setLayout(layout);

}


void MainWindow::initSignalSlots()
{
    //connect(ui->actionset, &QAction::trigger, this, &MainWindow::on_setAction_triggered);
}

void MainWindow::initSettings()
{
    QString fileName = Config::getInstance()->value("Qss/UserQssFilePath", Path::getInstance()->qssFilePath()).toString();
    QFont font = getFontFromConfig();
    QFontMetrics metrics(font);
    m_textEdit->setTabStopDistance(4*metrics.width(' '));
    m_textEdit->setFont(font);
    m_textEdit->setTextFromFile(fileName);
}

void MainWindow::saveSettings()
{
    Config::getInstance()->setValue("Font/family", m_textEdit->font().family());
    Config::getInstance()->setValue("Font/size", m_textEdit->font().pointSize());
    Config::getInstance()->setValue("Font/styleName", m_textEdit->font().styleName());
}

QFont MainWindow::getFontFromConfig()
{
    QString fontFamily = Config::getInstance()->value("Font/family", "宋体").toString();
    int fontSize = Config::getInstance()->value("Font/size", 20).toInt();
    bool fontAntialias = Config::getInstance()->value("Font/Antialias", true).toBool();
    QFont font(fontFamily, fontSize);
    if(fontAntialias)
    {
        font.setStyleStrategy(QFont::PreferAntialias);
    }
    else
    {
        font.setStyleStrategy(QFont::NoAntialias);
    }
    return font;
}


void MainWindow::on_replaceBtn_clicked()
{
    QString resultText = m_textEdit->toPlainText();
    QssHelper::replaceDefsWithValues(resultText,QssTextEditManager::getInstance()->getDefs());
    m_textEdit->setPlainText(resultText);
    //m_textEdit->setText(resultText);
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
    m_textEdit->setTextFromFile(fileName);
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

//void MainWindow::on_actionset_triggered()
//{
//    bool ok;
//    QFont font = QFontDialog::getFont(&ok, m_textEdit->font());
//    if (ok)
//    {
//        m_textEdit->setFont(font);
//    }
//    QFontInfo info(font);
//    info.styleName();
//}

void MainWindow::slot_fontSettingsChanged(const FontSettings &fontSettings)
{
    QFont font(m_textEdit->font());
    font.setFamily(fontSettings.fontFamily);
    font.setPointSize(fontSettings.fontSize);
    if (fontSettings.isAntialias)
    {
        font.setStyleStrategy(QFont::PreferAntialias);
    }
    else
    {
        font.setStyleStrategy(QFont::NoAntialias);
    }
    m_textEdit->setFont(font);
}

void MainWindow::on_actionSetQss_triggered()
{
    QString a = m_textEdit->toPlainText();
    Config::getInstance()->setSkin(a, QssTextEditManager::getInstance()->getDefs());
}

void MainWindow::on_actionSaveQssFile_triggered()
{
    QString text = m_textEdit->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                 Config::getInstance()->value("Qss/UserQssFile", Path::getInstance()->qssFilePath()).toString(),
                                 tr("Qss File(*.qss *.css *.txt)"));
    Config::getInstance()->setValue("Qss/UserQssFile",fileName);
    QssHelper::writeQStrTofile(text,fileName);
}


void MainWindow::on_actionSaveColorDefineFile_triggered()
{
    QssTextEditManager::getInstance()->saveDefsToFile();
}

void MainWindow::on_actionAddNewDef_triggered()
{
    bool ok = QssTextEditManager::getInstance()->addNewDef();
    if(!ok)
    {
        QMessageBox::critical(this, tr("Warning"),tr("Please set a name for the '#set_name' that is different from the other items"));
    }
}

void MainWindow::on_actionshowSourceText_triggered()
{
    QssTextEditManager::getInstance()->setSourceTextVisible(true);
}

void MainWindow::on_actionshowColorDefines_triggered()
{
    QssTextEditManager::getInstance()->setSourceTextVisible(false);
}
