#include "path.h"

#include <QApplication>
#include <QDir>

QString Path::cdUp(const QString &folder)
{
    QDir tempFolder(folder);
    if (!tempFolder.cdUp())
    {
        return folder;
    }
    return tempFolder.absolutePath();
}

Path::Path()
{
    m_exeDir = QApplication::applicationDirPath();
    m_appDir = cdUp(m_exeDir);
    m_dataDir = QString("%1/data").arg(m_appDir);
    m_confDir = QString("%1/conf").arg(m_appDir);
    m_configFilePath = QString("%1/config.ini").arg(m_confDir);
    m_qssDir = QString("%1/qss").arg(m_confDir);
    m_qssFilePath = QString("%1/default.qss").arg(m_qssDir);
    m_colorDefFilePath = QString("%1/default.qssdef").arg(m_qssDir);
    m_qssKeywordFilePath = QString("%1/qssKeyWord.txt").arg(m_confDir);
    m_qClassKeywordFilePath = QString("%1/QtClassKeyWord.txt").arg(m_confDir);
}
