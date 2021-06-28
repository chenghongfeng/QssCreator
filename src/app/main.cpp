#include "mainwindow.h"

#include <QApplication>

#include "config.h"
#include "path.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Config::getInstance()->setConfigFilePathName(Path::getInstance()->configFilePath());
    Config::getInstance()->readAllConfig();
    MainWindow w;
    w.show();
    return a.exec();
}
