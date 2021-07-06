#include "mainwindow.h"

#include <QApplication>

#include "config.h"
#include "path.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Config::getInstance()->setConfigFilePathName(Path::getInstance()->configFilePath());
    Config::getInstance()->readAllConfig();
    Config::getInstance()->setValue("Test/1", "测试1");
    QString text1 = Config::getInstance()->value("Test/1").toString();
    MainWindow w;
    w.show();
    return a.exec();
}
