#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QIcon>
#include "config.h"
#include "path.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //AA_UseDesktopOpenGL  AA_UseOpenGLES AA_UseSoftwareOpenGL AA_ShareOpenGLContexts


    Config::getInstance()->setConfigFilePathName(Path::getInstance()->configFilePath());
    Config::getInstance()->readAllConfig();
    //qApp->setStyleSheet();
    MainWindow w;
    QIcon icon(":/icons/qsseditor.png");
    w.setWindowTitle("Qss Creator");
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
