#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "QssTextEdit/qsshighlighter.h"

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QStandardItemModel;
class QStringListModel;
class ColorDefTableModel;
class ConfigDialog;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void on_actionset_triggered();

    void on_openQssFileBtn_clicked();


    void on_replaceBtn_clicked();

    void on_saveTextBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;


    QssHighlighter *qssHighlighter { nullptr };

    ConfigDialog *m_configDialog {nullptr};
};
#endif // MAINWINDOW_H
