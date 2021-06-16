#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>

#include "qss_helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDefs()
{
    defs = QssHelper::getColorDefineFromFile(m_strColorDefFile);
}


void MainWindow::on_openColorDefFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/SourceCode/MyProject/QssHelper/qssFile", "颜色定义文件(*.def)");
    m_strColorDefFile = fileName;

//    if (!fileName.isEmpty()) {
//        QFile file(fileName);

//        if (file.open(QFile::ReadOnly)) {
//            QString str = file.readAll();
//            m_strColorDefFile = fileName;
//        }
//    }
}

void MainWindow::on_openQssFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", qApp->applicationDirPath() + "/qss", "皮肤文件(*.qss)");
    m_strQssFile = fileName;
//    if (!fileName.isEmpty()) {
//        QFile file(fileName);

//        if (file.open(QFile::ReadOnly)) {
//            QString str = file.readAll();
//            m_strQssFile = fileName;
//        }
//    }
}

void MainWindow::on_applyBtn_clicked()
{
    getDefs();
}
