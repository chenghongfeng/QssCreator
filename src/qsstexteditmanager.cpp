#include "qsstexteditmanager.h"

#include <QFile>
#include <utility>
#include <QTextStream>

#include "path.h"
#include "config.h"
#include "fileHelper.h"
#include "qss_helper.h"
#include "constants.h"

QString QssTextEditManager::getCurDefsText() const
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->colorDefFilePath()).toString();
    QFile file(filePath);
    QString curDefsText;
    if(!file.open(QFile::ReadOnly))
    {
        return QString();
    }
    else
    {
        int lineNum = 0;
        int defInfoCount = 0;
        QTextStream in(&file);
        QString line;
        line = in.readLine();
        //对从源文件获取的信息进行修改和处理
        while(!line.isNull())
        {
            if(m_defInfos.size() > defInfoCount && m_defInfos[defInfoCount].fromLineNum == lineNum)
            {
                switch (m_defInfos[defInfoCount].status) {
                case ColorDefInfo::DefStatus::Default:
                    break;
                    //理论上进入不了这一步,因为新加的fromLineNum不可能等于lineNum
                case ColorDefInfo::DefStatus::Append:
                    line = QString("\n%1=%2;")
                            .arg(m_defInfos[defInfoCount].key).arg(m_defInfos[defInfoCount].value);
                    break;
                case ColorDefInfo::DefStatus::Deprecated:
                    line = "";
                    break;
                case ColorDefInfo::DefStatus::Modification:
                    if(m_defInfos[defInfoCount].value != m_defInfos[defInfoCount].original_value)
                    {
                        line.replace(m_defInfos[defInfoCount].original_value_start,m_defInfos[defInfoCount].original_value.length(),m_defInfos[defInfoCount].value);
                    }
                    if(m_defInfos[defInfoCount].key != m_defInfos[defInfoCount].original_key)
                    {
                        line.replace(m_defInfos[defInfoCount].original_key_start,m_defInfos[defInfoCount].original_key.length(),m_defInfos[defInfoCount].key);
                    }
                    break;
                default:
                    break;
                }
                defInfoCount++;
            }
            curDefsText.append(line);
            curDefsText.append("\n");
            line = in.readLine();
            lineNum++;
        }
        //对用户新加的colorDef进行处理
        while(m_defInfos.size() > defInfoCount)
        {
            if(m_defInfos[defInfoCount].status == ColorDefInfo::DefStatus::Append)
            {
                curDefsText.append(QString("\n%1=%2;")
                                   .arg(m_defInfos[defInfoCount].key).arg(m_defInfos[defInfoCount].value));
            }
            defInfoCount++;
        }
    }
    file.close();

    return curDefsText;
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
        if((defInfo.key == Constants::NEW_DEFINE_NAME) && (defInfo.status != ColorDefInfo::DefStatus::Deprecated))
        {
            return false;
        }
    }
    newInfo.status = ColorDefInfo::DefStatus::Append;
    newInfo.key = Constants::NEW_DEFINE_NAME;
    newInfo.value = Constants::NEW_DEFINE_VALUE;
    m_defInfos.append(std::move(newInfo));
    emit defsUpdated();
    return true;
}

void QssTextEditManager::removeDef(const QString &key)
{
    for(auto &defInfo:m_defInfos){
        if(defInfo.key == key)
        {
            defInfo.status = ColorDefInfo::DefStatus::Deprecated;
            break;
        }
    }
    emit defsUpdated();
}

const QStringList &QssTextEditManager::qtClassKeywords() const
{
    return m_qtClassKeywords;
}

const QStringList &QssTextEditManager::qssKeywords() const
{
    return m_qssKeywords;
}

void QssTextEditManager::setSourceTextVisible(bool isVisible)
{
    emit sourceTextEditVisibleChange(isVisible);
}

void QssTextEditManager::updateDefs()
{
    QString filePath = Config::getInstance()->value("Qss/UserColorDefineFile",
                                                    Path::getInstance()->qssDefExampleFilePathName()).toString();
    setDefsFile(filePath);
}

void QssTextEditManager::slot_Config_valueUpdated(const QString &key, const QVariant &value)
{
    if (key == "Qss/UserColorDefineFile")
        setDefsFile(value.toString());

}

QssTextEditManager::QssTextEditManager()
{
    m_qtClassKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qClassKeyWordFilePath());
    m_qssKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qssKeywordFilePath());
    connect(Config::getInstance(), &Config::valueUpdated, this, &QssTextEditManager::slot_Config_valueUpdated);
    initDefs();//此时是不能发送defUpdate的信号的
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

        QTextStream in(&file);
        QString line;
        line = in.readLine();
        int lineNum = 0;
        while(!line.isNull())
        {
            ColorDefInfo info;
            QssHelper::getOneDefineFromQStr(line,info);
            if(info.key != "")
            {
                info.fromLineNum = lineNum;
                m_defInfos.push_back(std::move(info));
            }
            line = in.readLine();
            lineNum++;
        }

        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText);
    }
    emit defsUpdated();
}
