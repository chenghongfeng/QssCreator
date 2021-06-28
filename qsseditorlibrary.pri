TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

win32 {
    dlltarget.path = $$INSTALL_BIN_PATH
    INSTALLS += dlltarget
} else {
    target.path = $$INSTALL_LIBRARY_PATH
    INSTALLS += target
}
