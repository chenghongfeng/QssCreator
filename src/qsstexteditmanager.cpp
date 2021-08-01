#include "qsstexteditmanager.h"

#include <QFile>

#include "path.h"
#include "config.h"
#include "fileHelper.h"
#include "qss_helper.h"

void QssTextEditManager::updateDefs()
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->colorDefFilePath()).toString();
    setDefsFile(filePath);
}

void QssTextEditManager::slot_Config_valueUpdated(const QString &key, const QVariant &value)
{
    if (key == "Qss/UserColorDefineFile")
        setDefsFile(value.toString());

}

QssTextEditManager::QssTextEditManager()
{
    connect(Config::getInstance(), &Config::valueUpdated, this, &QssTextEditManager::slot_Config_valueUpdated);
    initDefs();
    m_qtClassKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qClassKeyWordFilePath());
    m_qssKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qssKeywordFilePath());
}

void QssTextEditManager::initDefs()
{
    updateDefs();
}

void QssTextEditManager::setDefsFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText,"");
    }
    emit defsUpdated();
}
