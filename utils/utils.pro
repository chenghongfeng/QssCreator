QT += gui widgets

TEMPLATE = lib
DEFINES += UTILS_LIBRARY
CONFIG += dll

CONFIG += c++17

DLLDESTDIR = $$PWD/../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    fancyTabWidget/fancyactionbar.cpp \
    fancyTabWidget/fancytabwidget.cpp \
    fancyTabWidget/hostosinfo.cpp \
    fancyTabWidget/icon.cpp \
    fancyTabWidget/id.cpp \
    fancyTabWidget/infobar.cpp \
    fancyTabWidget/oshelper.cpp \
    fancyTabWidget/qtcassert.cpp \
    fancyTabWidget/stringutils.cpp \
    fancyTabWidget/styledbar.cpp \
    fancyTabWidget/stylehelper.cpp \
    theme.cpp \
    fancyTabWidget/utilsicons.cpp \
    fileHelper.cpp \
    path.cpp \
    qss_helper.cpp \
    tabwidget.cpp \
    utils.cpp \

HEADERS += \
    config.h \
    constants.h \
    fancyTabWidget/fancyactionbar.h \
    fancyTabWidget/fancytabwidget.h \
    fancyTabWidget/hostosinfo.h \
    fancyTabWidget/icon.h \
    fancyTabWidget/id.h \
    fancyTabWidget/infobar.h \
    fancyTabWidget/optional.h \
    fancyTabWidget/oshelper.h \
    fancyTabWidget/osspecificaspects.h \
    fancyTabWidget/porting.h \
    fancyTabWidget/predicates.h \
    fancyTabWidget/qtcassert.h \
    fancyTabWidget/stringutils.h \
    fancyTabWidget/styledbar.h \
    fancyTabWidget/stylehelper.h \
    theme.h \
    fancyTabWidget/utilsicons.h \
    fileHelper.h \
    path.h \
    qss_helper.h \
    singleton.h \
    tabwidget.h \
    utils_global.h \
    utils.h \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    utils.qrc

