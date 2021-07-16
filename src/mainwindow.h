#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFontComboBox>

#include "QssTextEdit/qsshighlighter.h"

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QStandardItemModel;
class QStringListModel;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class ColorDefTableModel;
class ConfigDialog;
class TabWidget;
class QssTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    void initUi();
    void initSignalSlots();
    void initSettings();
    void saveSettings();

private slots:
    void on_replaceBtn_clicked();

    void on_saveTextBtn_clicked();

    void on_actionOpenQssFile_triggered();

    void on_actionOpenDefineFile_triggered();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;
    QssTextEdit *m_textEdit { nullptr };
    TabWidget *m_tabWidget { nullptr };
    QssHighlighter *qssHighlighter { nullptr };

    ConfigDialog *m_configDialog {nullptr};
};
#endif // MAINWINDOW_H
