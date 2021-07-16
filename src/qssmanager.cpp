#include "qssmanager.h"

#include <QFile>

#include "path.h"
#include "config.h"
#include "qss_helper.h"

void QssManager::updateDefs()
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->colorDefFilePath()).toString();
    setDefsFile(filePath);
}

void QssManager::slot_Config_valueUpdated(const QString &key, const QVariant &value)
{
    if (key == "Qss/UserColorDefineFile")
        setDefsFile(value.toString());

}

QssManager::QssManager()
{
    connect(Config::getInstance(), &Config::valueUpdated, this, &QssManager::slot_Config_valueUpdated);
    initDefs();
}

void QssManager::initDefs()
{
    updateDefs();
}

void QssManager::setDefsFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText,"");
    }
    emit defsUpdated();
}
