#include "config.h"

#include <QStringList>
#include <QSettings>

Config::~Config()
{
    m_settings->sync();
}

void Config::readAllConfig()
{
    if (m_configFilePathName == "")
        return;
    m_settings = new QSettings(m_configFilePathName, QSettings::Format::IniFormat);
    m_settings->setIniCodec("UTF-8");
    QStringList settings = m_settings->allKeys();

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
