QT += gui widgets

TEMPLATE = lib
DEFINES += UTILS_LIBRARY
CONFIG += dll

CONFIG += c++11

!contains(QMAKE_TARGET.arch, x86_64){
    CONFIG(release, debug|release):DLLDESTDIR = $$PWD/../bin
    else:DLLDESTDIR = $$PWD/../bin_D
}else{
    CONFIG(release, debug|release):DLLDESTDIR = $$PWD/../bin_x64
    else:DLLDESTDIR = $$PWD/../bin_x64_D
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colordeftablemodel.cpp \
    config.cpp \
    fileHelper.cpp \
    oshelper.cpp \
    path.cpp \
    qss_helper.cpp \
    qsscompleter.cpp \
    qsshighlighter.cpp \
    qssproxymodel.cpp \
    qsstextedit.cpp \
    utils.cpp

HEADERS += \
    colordeftablemodel.h \
    config.h \
    fileHelper.h \
    oshelper.h \
    path.h \
    qss_helper.h \
    qsscompleter.h \
    qsshighlighter.h \
    qssproxymodel.h \
    qsstextedit.h \
    singleton.h \
    utils_global.h \
    utils.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
