#include "mainwindow.h"

#include <QApplication>

#include "config.h"
#include "path.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //AA_UseDesktopOpenGL  AA_UseOpenGLES AA_UseSoftwareOpenGL AA_ShareOpenGLContexts
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Config::getInstance()->setConfigFilePathName(Path::getInstance()->configFilePath());
    Config::getInstance()->readAllConfig();
    Config::getInstance()->setValue("Test/1", "测试1");
    //Config::getInstance()->setSkin(Path::getInstance()->qssDir() + "/default.qss", Path::getInstance()->qssDir() + "/default.qssdef");
    QString text1 = Config::getInstance()->value("Test/1").toString();
    //qApp->setStyleSheet();
    MainWindow w;
    w.show();
    return a.exec();
}
