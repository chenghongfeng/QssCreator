#include "config.h"

#include <QStringList>

void Config::readAllConfig()
{
    if (m_configFilePathName == "")
        return;
    m_settings = new QSettings(m_configFilePathName, QSettings::Format::IniFormat);
    QStringList settings = m_settings->allKeys();

}
