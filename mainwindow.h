#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
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

private slots:
    void on_openColorDefFileBtn_clicked();

    void on_openQssFileBtn_clicked();


    void on_applyBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;
    QStringList defs;
};
#endif // MAINWINDOW_H
