#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QStringListModel>

#include "qss_helper.h"
#include "colordeftablemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->defsTableWidget->setColumnCount(2);
    defListsModel = new QStringListModel();

    ui->defsTableWidget->setVisible(false);
    ui->colorListView->setVisible(false);

//    myModel = new MyModel(this);
//    ui->colorTableView->setModel(myModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDefs()
{
    if (m_strColorDefFile.isEmpty())
    {
        return;
    }
    QFile file(m_strColorDefFile);
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        defs = QssHelper::getColorDefineFromQStr(defsText,pattern);
    }
}


void MainWindow::on_openColorDefFileBtn_clicked()
{
    //QString fileName = "F:/MyGitProject/qssHelper/qssFile/color.def";
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/SourceCode/MyProject/qssHelper/qssFile/", "颜色定义文件(*.qssdef)");
    m_strColorDefFile = fileName;

}

void MainWindow::on_openQssFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/SourceCode/MyProject/qssHelper/qssFile/", "皮肤文件(*.qss)");
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
    colorDefs.clear();
    auto iter = defs.constBegin();
    int i = 0;
    while (iter != defs.constEnd())
    {
        ui->defsTableWidget->insertRow(i);
        QTableWidgetItem *keyItem = new QTableWidgetItem(iter.key());
        QTableWidgetItem *valueItem = new QTableWidgetItem(iter.value());
        colorDefs.append(iter.key());
        ui->defsTableWidget->setItem(i,0,keyItem);
        ui->defsTableWidget->setItem(i,1,valueItem);
        iter++;
        i++;
    }
    defListsModel->setStringList(colorDefs);
    ui->colorListView->setModel(defListsModel);

    colorDefModel = new ColorDefTableModel(defs, this);
    ui->colorTableView->setModel(colorDefModel);

}

void MainWindow::on_regLineEdit_textChanged(const QString &arg1)
{
    pattern = arg1;
}


void MainWindow::on_replaceBtn_clicked()
{
    QString resultText = ui->qssTextEdit->toPlainText();
    QssHelper::replaceDefsWithValues(resultText,defs);
    ui->qssTextEdit->setText(resultText);
}

void MainWindow::on_saveTextBtn_clicked()
{
    //QString fileName = QFileDialog::getSaveFileName(this, "打开文件", QString("%1/%2.css").arg(qApp->applicationDirPath()).arg(dirName), "皮肤文件(*.css)");
    QString fileName = QFileDialog::getSaveFileName(this, "打开文件", "F:/MyGitProject/qssHelper/qssFile/default.qss", "皮肤文件(*.qss)");
    QssHelper::writeQStrTofile(ui->qssTextEdit->toPlainText(),fileName);
}
