QT += gui widgets

TEMPLATE = lib
DEFINES += UTILS_LIBRARY
CONFIG += dll

CONFIG += c++17

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
    config.cpp \
    fancyactionbar.cpp \
    fancytabwidget.cpp \
    fileHelper.cpp \
    hostosinfo.cpp \
    icon.cpp \
    id.cpp \
    infobar.cpp \
    oshelper.cpp \
    path.cpp \
    qss_helper.cpp \
    qtcassert.cpp \
    stringutils.cpp \
    styledbar.cpp \
    stylehelper.cpp \
    tabwidget.cpp \
    theme.cpp \
    utils.cpp \
    utilsicons.cpp

HEADERS += \
    algorithm.h \
    config.h \
    constants.h \
    fancyactionbar.h \
    fancytabwidget.h \
    fileHelper.h \
    hostosinfo.h \
    icon.h \
    id.h \
    infobar.h \
    optional.h \
    oshelper.h \
    osspecificaspects.h \
    path.h \
    porting.h \
    predicates.h \
    qss_helper.h \
    qtcassert.h \
    singleton.h \
    styledbar.h \
    stylehelper.h \
    tabwidget.h \
    theme.h \
    theme_p.h \
    utils_global.h \
    utils.h \
    utilsicons.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    utils.qrc

