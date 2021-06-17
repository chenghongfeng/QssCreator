#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QStandardItemModel;
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
    void setUpDefsTableView();

private slots:
    void on_openColorDefFileBtn_clicked();

    void on_openQssFileBtn_clicked();


    void on_applyBtn_clicked();

    void on_regLineEdit_textChanged(const QString &arg1);

    void on_replaceBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;
    QMap<QString,QString> defs;
    QString pattern;

    QStandardItemModel *defsTableModel;
};
#endif // MAINWINDOW_H
