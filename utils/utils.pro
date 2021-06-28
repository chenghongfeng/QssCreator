QT += gui widgets

TEMPLATE = lib
DEFINES += UTILS_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colordeftablemodel.cpp \
    config.cpp \
    oshelper.cpp \
    path.cpp \
    qss_helper.cpp \
    qsshighlighter.cpp \
    utils.cpp

HEADERS += \
    colordeftablemodel.h \
    config.h \
    oshelper.h \
    path.h \
    qss_helper.h \
    qsshighlighter.h \
    singleton.h \
    utils_global.h \
    utils.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
