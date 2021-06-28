#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "qsshighlighter.h"

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
    void getDefs();
    void initSignalSlots();
    void initSettings();

private slots:
    void on_actionset_triggered();
    void on_openColorDefFileBtn_clicked();

    void on_openQssFileBtn_clicked();


    void on_applyBtn_clicked();

    void on_regLineEdit_textChanged(const QString &arg1);

    void on_replaceBtn_clicked();

    void on_saveTextBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;
    QMap<QString,QString> defs;
    QString pattern;

    QStandardItemModel *defsTableModel {nullptr};

    QStringListModel *defListsModel {nullptr};
    QStringList colorDefs;

    ColorDefTableModel *colorDefModel {nullptr};

    QssHighlighter *qssHighlighter { nullptr };

    ConfigDialog *m_configDialog {nullptr};
};
#endif // MAINWINDOW_H
