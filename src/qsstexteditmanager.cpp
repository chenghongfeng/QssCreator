#include "qsstexteditmanager.h"

#include <QFile>
#include <utility>

#include "path.h"
#include "config.h"
#include "fileHelper.h"
#include "qss_helper.h"
const QString NewDefineName = "$set_name";
const QString NewDefineValue = "#FFFFFF";

QString QssTextEditManager::getCurDefsText() const
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->colorDefFilePath()).toString();
    QFile file(filePath);
    QString defsText;
    if(!file.open(QFile::ReadWrite))
    {
        return QString();
    }
    defsText = file.readAll();
    file.close();
    for(auto iter = --m_defInfos.end();iter != --m_defInfos.begin();--iter)
    {
        if(iter->value != iter->original_value)
        {
            defsText.replace(iter->original_value_start,iter->original_value.length(),iter->value);
        }
        if(iter->key != iter->original_key)
        {
            defsText.replace(iter->original_key_start,iter->original_key.length(),iter->key);
        }
        if(iter->is_append)
        {
            defsText.append(QString("\n%1=%2\n").arg(iter->key).arg(iter->value));
        }
    }
    return defsText;
}

void QssTextEditManager::saveDefsToFile()
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->colorDefFilePath()).toString();
    QFile file(filePath);
    QString defsText = getCurDefsText();
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        return;
    }
    file.write(defsText.toStdString().c_str());
    file.close();
    updateDefs();
}

bool QssTextEditManager::addNewDef()
{
    ColorDefInfo newInfo;
    for(auto const &defInfo:m_defInfos){
        if(defInfo.key == NewDefineName)
        {
            return false;
        }
    }
    newInfo.is_append = true;
    newInfo.key = NewDefineName;
    newInfo.value = NewDefineValue;
    m_defInfos.append(std::move(newInfo));
    emit defsUpdated();
    return true;
}

void QssTextEditManager::removeDef(const QString &key)
{
    int index = 0;
    for(auto const &defInfo:m_defInfos){
        if(defInfo.key == key)
        {
            break;
        }
        index++;
    }
    m_defInfos.removeAt(index);
    emit defsUpdated();
}

void QssTextEditManager::setSourceTextVisible(bool isVisible)
{
    emit sourceTextEditVisibleChange(isVisible);
}

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
        m_defInfos.clear();
        m_defs.clear();
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText, m_defInfos, "");
    }
    emit defsUpdated();
}
