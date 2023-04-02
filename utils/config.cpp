#include "config.h"

#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QApplication>
#include <QDir>

#include "Constants.h"
#include "qss_helper.h"
#include "path.h"

Config::~Config()
{
    m_settings->sync();
}

void Config::readAllConfig()
{
    if (m_configFilePathName == "")
        return;
    m_settings = new QSettings(m_configFilePathName, QSettings::Format::IniFormat);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    m_settings->setIniCodec("UTF-8");
#endif
    QStringList settings = m_settings->allKeys();
    QString qssPath = Path::getInstance()->qssDir();
    QDir qssDir(qssPath);
    QFileInfoList fileInfos = qssDir.entryInfoList();
    for(auto &info:fileInfos){
        if(info.suffix() == "qssdef")
        {
            m_themes.push_back(info.baseName());
        }
    }

}

void Config::setSkin()
{
    //setSkin()
}

void Config::setSkin(const QString &qssFilePathName, const QString &colorDefFilePathName)
{

    QFile colorDefsFile(colorDefFilePathName);
    QMap<QString,QString> defs;
    if (colorDefsFile.open(QFile::ReadOnly))
    {
        QString defsText = colorDefsFile.readAll();
        defs = QssHelper::getColorDefineFromQStr(defsText);
        colorDefsFile.close();
    }

    QFile file(qssFilePathName);
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = file.readAll();
        //QssHelper::replaceDefsWithValues(styleSheet, defs);
        //styleSheet.replace("url(", QString("url(")+currentSkinDir+"/");
        //qApp->setStyleSheet(styleSheet);
        file.close();
        setSkin(styleSheet, defs);
    }
}

void Config::setSkin(QString &qss,const QMap<QString,QString> &defs)
{
    QssHelper::replaceDefsWithValues(qss, defs);
    qApp->setStyleSheet(qss);
}

void Config::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
    emit valueUpdated(key, value);
}

QVariant Config::value(const QString &key, const QVariant &defaultValue)
{
    return this->m_settings->value(key, defaultValue);
}

QString Config::themeFilePathName() const
{
    QString qssPath = Path::getInstance()->qssDir();
    QDir qssDir(qssPath);
    QFileInfoList fileInfos = qssDir.entryInfoList();
    for(auto &info:fileInfos){
        if(info.suffix() == "qssdef" && info.baseName() == themeName())
        {
            return info.absoluteFilePath();
        }
    }
    return QString();
}

QString Config::themeName() const
{
    return Config::getInstance()->value("Theme/name",Constants::DEFAULT_THEME_NAME).toString();
}
