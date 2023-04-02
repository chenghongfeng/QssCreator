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

QString Path::exampleFilePath() const
{
    return QString("%1/UserData/%2").arg(m_appDir).arg("exampleFiles");
}

QString Path::qssExampleFilePathName() const
{
    return QString("%1/%2").arg(exampleFilePath()).arg("example.qss");
}

QString Path::qssDefExampleFilePathName() const
{
    return QString("%1/%2").arg(exampleFilePath()).arg("example.qssdef");
}

Path::Path()
{
    m_exeDir = QApplication::applicationDirPath();
    m_appDir = m_exeDir;
    m_dataDir = QString("%1/UserData").arg(m_appDir);
    m_confDir = QString("%1/conf").arg(m_appDir);
    m_configFilePath = QString("%1/config.ini").arg(m_dataDir);
    m_qssDir = QString("%1/style").arg(m_confDir);
    m_qssFilePath = QString("%1/default.qss").arg(m_qssDir);
    m_colorDefFilePath = QString("%1/default.qssdef").arg(m_qssDir);
    m_qssKeywordFilePath = QString("%1/qssKeyWord.txt").arg(m_confDir);
    m_qClassKeywordFilePath = QString("%1/QtClassKeyWord.txt").arg(m_confDir);
}
