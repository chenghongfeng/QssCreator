#ifndef QssTextEditManager_H
#define QssTextEditManager_H

#include <QMap>
#include <singleton.h>
#include "qss_helper.h"

class QssTextEditManager
        : public QObject, public Singleton<QssTextEditManager>
{
    friend class Singleton<QssTextEditManager>;
    Q_OBJECT
public:
    const QMap<QString,QString> &getDefs() const{
        return m_defs;
    }
    QMap<QString,QString> getCurDefs() const {
        QMap<QString,QString> curDefs;
        for(const auto &info: m_defInfos)
        {
            if(info.status != ColorDefInfo::DefStatus::Deprecated)
            {
                curDefs.insert(info.key, info.value);
            }
        }
        return curDefs;
    }
    ColorDefInfos & getDefInfos(){
        return m_defInfos;
    }
    QString getCurDefsText() const;
    void saveDefsToFile();
    bool addNewDef();
    void removeDef(const QString &key);
    const QStringList &qtClassKeywords() const;
    const QStringList &qssKeywords() const;

signals:
    void defsUpdated();
    void sourceTextEditVisibleChange(bool);
public slots:
    void setSourceTextVisible(bool isVisible);
    void updateDefs();
    void slot_Config_valueUpdated(const QString &key, const QVariant &value);

protected:
    QssTextEditManager();
private:
    void initDefs();
    void setDefsFile(const QString &fileName);



private:
    //基于源文件获取的内容,源文件更改时才更改
    QMap<QString,QString> m_defs;
    //基于源文件获取的内容,可以用来更改源文件
    ColorDefInfos m_defInfos;
    QStringList m_qtClassKeywords;
    QStringList m_qssKeywords;
};

#endif // QssTextEditManager_H
