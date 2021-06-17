#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTableView>

#include "qss_helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->defsTableWidget->setColumnCount(2);
    //ui->defsTableWidget->setRowCount(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDefs()
{
    defs = QssHelper::getColorDefineFromFile(m_strColorDefFile,pattern);

}

void MainWindow::setUpDefsTableView()
{
//    defsTableModel = new QStandardItemModel(ui->defsTableView);

//    //设置行
//    defsTableModel->setColumnCount(2);
//    defsTableModel->setHeaderData(0,Qt::Horizontal,tr("Def"));
//    defsTableModel->setHeaderData(1,Qt::Horizontal,tr("Values"));
//    defsTableModel->setSortRole(1);

//    ui->defsTableView->setModel(defsTableModel);
//    ui->defsTableView->setColumnWidth(0,10);
//    ui->defsTableView->setColumnWidth(1,20);
//    ui->defsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->defsTableView->setSelectionMode(QAbstractItemView::SingleSelection);//
//    ui->defsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读，不能进行编辑

//    ui->defsTableView->verticalHeader()->setDefaultSectionSize(20);
//    ui->defsTableView->verticalHeader()->setHidden(true);

//    defsTableModel->setItem(1, 1, new QStandardItem("teste"));
//    defsTableModel->setItem(1, 1, new QStandardItem("testes"));
}


void MainWindow::on_openColorDefFileBtn_clicked()
{
    QString fileName = "F:/MyGitProject/qssHelper/qssFile/color.def";
    //QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/SourceCode/MyProject/QssHelper/qssFile", "颜色定义文件(*.def)");
    m_strColorDefFile = fileName;

}

void MainWindow::on_openQssFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "F:/MyGitProject/qssHelper/qssFile/", "皮肤文件(*.qss)");
    m_strQssFile = fileName;
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            ui->qssTextEdit->setText(str);
        }
    }
}

void MainWindow::on_applyBtn_clicked()
{
    getDefs();
    ui->defsTableWidget->clear();
    ui->defsTableWidget->setRowCount(0);
    auto iter = defs.constBegin();
    int i = 0;
    while (iter != defs.constEnd())
    {
        ui->defsTableWidget->insertRow(i);
        QTableWidgetItem *keyItem = new QTableWidgetItem(iter.key());
        QTableWidgetItem *valueItem = new QTableWidgetItem(iter.value());
        ui->defsTableWidget->setItem(i,0,keyItem);
        ui->defsTableWidget->setItem(i,1,valueItem);
        iter++;
        i++;
    }

}

void MainWindow::on_regLineEdit_textChanged(const QString &arg1)
{
    pattern = arg1;
}


void MainWindow::on_replaceBtn_clicked()
{
    QString str = QssHelper::replaceDefsWithValues(ui->qssTextEdit->toPlainText(),defs);
    ui->qssTextEdit->setText(str);
}
