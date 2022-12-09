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
#include "theme.h"
#include "qsstexteditmanager.h"

#include "textsettingswidget.h"
#include "colordefwidget.h"
#include "QSSTextEdit/qsstextedit.h"
#include "tabwidget.h"
#include "fancytabwidget.h"
#include "utilsicons.h"
#include "finddialog.h"
#include "findreplacedialog.h"
#include "previewqsswidget.h"
#include "constants.h"

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
    if(m_previewWidget)
    {
        delete m_previewWidget;
        m_previewWidget = nullptr;
    }
    delete ui;

    Config::closeInstance();
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

void MainWindow::closeEvent(QCloseEvent *e)
{
    saveSettings();
}

void MainWindow::initUi()
{
#ifndef CREATE_MODE

#endif
#ifdef USE_FANCYTABWIDGET
    m_fancyTabWidget = new Core::Internal::FancyTabWidget(this);
#else
    //init m_tabWidget
    m_tabWidget = new TabWidget(this);
#endif
    m_colorWidget = new ColorDefWidget(this);
    {
        //添加颜色定义页
        QAction *showColorDefAction = new QAction(this);
        showColorDefAction->setToolTip(tr("Show/Hide color defines widget"));
        showColorDefAction->setCheckable(true);
        showColorDefAction->setChecked(true);//默认显示此页
#ifdef USE_FANCYTABWIDGET
        m_fancyTabWidget->insertTab(0,m_colorWidget,Utils::Icons::MODE_DESIGN.icon(),tr("Colors"),false);
        m_fancyTabWidget->setTabEnabled(0,true);

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
        m_fancyTabWidget->insertTab(1,m_textSettingsWidget,Utils::Icons::SETTINGS_TOOLBAR.icon(),tr("Settings"),false);
        m_fancyTabWidget->setTabEnabled(1,true);

        //添加qsstestwidget
//        QssTestWidget *testWidget = new QssTestWidget(this);
//        m_fancyTabWidget->insertTab(2,testWidget,Utils::Icons::HOME.icon(),tr("Style"),false);
//        m_fancyTabWidget->setTabEnabled(2,true);
#else
        m_tabWidget->addPage(m_textSettingsWidget, showConfigAction);
#endif

    }

    {
        //init m_textEdit
        m_textEdit = new QssTextEdit(this);
        //m_textEdit->setDefKeyword(QssTextEditManager::getInstance()->getCurDefs().keys());
        m_textEdit->initCompleter();
        connect(this, &MainWindow::themeChange, m_textEdit, &QssTextEdit::updateQssKeywordModelAndHighlighter);
        //ui->qssTextEdit->setTextBackgroundColor(QColor("#002b36"));

        QWidgetList list;
        list.append(m_textEdit);

#ifdef USE_FANCYTABWIDGET
    m_fancyTabWidget->addAlwaysShowWidget(list);
    m_fancyTabWidget->setWorkAreaSplitterChildernCollapsible(false);
    m_fancyTabWidget->splitter()->restoreState(Config::getInstance()->value("Layout/SplitterSizes").toByteArray());
    m_fancyTabWidget->setCurrentIndex(0);
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
     layout->addWidget(m_fancyTabWidget,2);
#else
    layout->addWidget(m_tabWidget,2);
#endif
    ui->centralwidget->setLayout(layout);
    {
        //Theme组件
        QString themeName = Config::getInstance()->value("Theme/name", Constants::DEFAULT_THEME_NAME).toString();
        QMenu *themeMenu = new QMenu(this);
        themeMenu->setTitle(tr("Theme"));
        {
            for (auto& name:Config::getInstance()->themes()) {
                QAction *a = new QAction(name);
                a->setCheckable(true);
                m_themeSwitchActions.push_back(a);
                themeMenu->addAction(a);
                if(themeName == a->text()){
                    a->setChecked(true);
                    a->setDisabled(true);
                }
                connect(a, &QAction::triggered, this, &MainWindow::slot_themeSwitchActionsChanged);
            }


        }
        ui->menuBar->addMenu(themeMenu);
        QAction *showPrevieAction = ui->menuBar->addAction(tr("Preview"));
        connect(showPrevieAction, &QAction::triggered, this, &MainWindow::slot_showPreviewActionTriggered);
    }
    themeChanged();
}


void MainWindow::initSignalSlots()
{
    //connect(ui->actionset, &QAction::trigger, this, &MainWindow::on_setAction_triggered);
}

void MainWindow::initSettings()
{
    QString fileName = Config::getInstance()->value("Qss/UserQssFilePath", Path::getInstance()->qssExampleFilePathName()).toString();
    QFont font = getFontFromConfig();
    QFontMetrics metrics(font);
#if QT_VERSION < QT_VERSION_CHECK(5,11,0)
    m_textEdit->setTabStopDistance(4*metrics.width(' '));
#else
    m_textEdit->setTabStopDistance(4*metrics.horizontalAdvance(' '));
#endif

    m_textEdit->setFont(font);
    m_textEdit->setTextFromFile(fileName);
    this->restoreGeometry(Config::getInstance()->value("Layout/MainWindowGeometry").toByteArray());
    this->restoreState(Config::getInstance()->value("Layout/MainWindowState").toByteArray());
}

void MainWindow::saveSettings()
{
    Config::getInstance()->setValue("Font/family", m_textEdit->font().family());
    Config::getInstance()->setValue("Font/size", m_textEdit->font().pointSize());
    Config::getInstance()->setValue("Layout/MainWindowGeometry", saveGeometry());
    Config::getInstance()->setValue("Layout/MainWindowState", saveState());
    if(m_colorWidget)
    {
        m_colorWidget->saveConfig();
    }
    if(m_fancyTabWidget)
    {
        Config::getInstance()->setValue("Layout/SplitterSizes",
                                        m_fancyTabWidget->splitter()->saveState());

    }
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
    QString fileName = Config::getInstance()->value("Qss/UserColorDefineFile", Path::getInstance()->qssDefExampleFilePathName()).toString();
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
    QString qss = m_textEdit->toPlainText();
    QssHelper::replaceDefsWithValues(qss, QssTextEditManager::getInstance()->getCurDefs());
    this->setStyleSheet(qss);
}

void MainWindow::on_actionSaveQssFile_triggered()
{
    QString text = m_textEdit->toPlainText();
    QString fileName = Config::getInstance()->value("Qss/UserQssFilePath", Path::getInstance()->qssExampleFilePathName()).toString();
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

void MainWindow::on_actionSaveQssToFile_triggered()
{
    QString text = m_textEdit->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                 Config::getInstance()->value("Qss/UserQssFile", Path::getInstance()->qssFilePath()).toString(),
                                 tr("Qss File(*.qss *.css *.txt)"));
    Config::getInstance()->setValue("Qss/UserQssFile",fileName);
    QssHelper::writeQStrTofile(text,fileName);
}

void MainWindow::on_actionFind_triggered()
{
    FindDialog *fdialog = new FindDialog(this);
    fdialog->setEditor(m_textEdit);
    fdialog->show();
}

void MainWindow::on_actionFindAndReplace_triggered()
{
    FindReplaceDialog *frdialog = new FindReplaceDialog(this);
    frdialog->setEditor(m_textEdit);
    frdialog->show();
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                 Config::getInstance()->value("Qss/UserQssFile", Path::getInstance()->qssFilePath()).toString(),
                                 tr("Qss File(*.qss *.css *.txt)"));
    if(fileName == "")
    {
        return;
    }
    QString text = m_textEdit->toPlainText();
    QssHelper::replaceDefsWithValues(text,QssTextEditManager::getInstance()->getCurDefs());
    QssHelper::writeQStrTofile(text,fileName);
}

void MainWindow::slot_themeSwitchActionsChanged()
{
    QAction *senderAction = (QAction *)sender();
    if(!senderAction) return;
    if(!senderAction->isChecked()) return;
    for (auto a : m_themeSwitchActions) {
        if(a == senderAction){
            Config::getInstance()->setValue("Theme/name",a->text());
            a->setDisabled(true);
            themeChanged();
            emit themeChange();
        }
        else if(a->isChecked()){
            a->setDisabled(false);
            a->setChecked(false);
        }
    }

}

void MainWindow::slot_showPreviewActionTriggered()
{
    if(!m_previewWidget)
    {
        m_previewWidget = new PreviewQssWidget();
    }
    QString qss = m_textEdit->toPlainText();
    QssHelper::replaceDefsWithValues(qss, QssTextEditManager::getInstance()->getCurDefs());
    m_previewWidget->setStyleSheet(qss);
    m_previewWidget->show();
}

void MainWindow::themeChanged()
{

    QString qss;
    //None Theme dont load qss
    if(Config::getInstance()->themeName() != Constants::NONE_THEME_NAME)
    {
        QString themeFileName = Config::getInstance()->themeFilePathName();
        QFile themeFile(themeFileName);
        QMap<QString,QString> defs;
        if(themeFile.open(QFile::ReadOnly))
        {
            QString defsText = themeFile.readAll();
            defs = QssHelper::getColorDefineFromQStr(defsText);
            themeFile.close();
        }
        QString qssFileName = Path::getInstance()->qssFilePath();
        QFile qssFile(qssFileName);

        if(qssFile.open(QFile::ReadOnly))
        {
            qss = qssFile.readAll();
            qssFile.close();
        }
        QssHelper::replaceDefsWithValues(qss, defs);
    }
    Utils::Theme::getInstance()->readColors();
    this->setStyleSheet(qss);
    this->repaint();
    m_fancyTabWidget->repaintAll();
}




void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("about Qss Creator"),
                       tr("Qss Creator is a tool that supports editing and previewing qss and color variables"));
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}
