QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4){
    # QT5
#    QT       += webengine
#    QT       += webenginewidgets
#    QT       += axcontainer
    !contains(QMAKE_TARGET.arch, x86_64){
        CONFIG(release, debug|release):DESTDIR = $$PWD/../bin
        else:DESTDIR = $$PWD/../bin_D
    }else{
        CONFIG(release, debug|release):DESTDIR = $$PWD/../bin_x64
        else:DESTDIR = $$PWD/../bin_x64_D
    }
}else{
    # QT4
#    QT      += webkit
#    CONFIG  += qaxcontainer
    CONFIG(release, debug|release):DESTDIR = $$PWD/../bin_qt4
    else:DESTDIR = $$PWD/../bin_qt4_D
}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colordefwidget.cpp \
    main.cpp \
    configdialog.cpp \
    mainwindow.cpp \
    QssTextEdit/colordeftablemodel.cpp \
    QssTextEdit/qsshighlighter.cpp \
    QssTextEdit/qssproxymodel.cpp \
    QssTextEdit/qsstextedit.cpp \
    mytabwidget.cpp \
    qssmanager.cpp

HEADERS += \
    colordefwidget.h \
    configdialog.h \
    mainwindow.h \
    QssTextEdit/colordeftablemodel.h \
    QssTextEdit/qsshighlighter.h \
    QssTextEdit/qssproxymodel.h \
    QssTextEdit/qsstextedit.h \
    mytabwidget.h \
    qssmanager.h

FORMS += \
    colordefwidget.ui \
    configdialog.ui \
    mainwindow.ui \
    mytabwidget.ui

TRANSLATIONS += \
    QssHelper_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils
