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
    QMap<QString,QString> getDefs(){
        return m_defs;
    }
    ColorDefInfos & getDefInfos(){
        return m_defInfos;
    }
    QString getCurDefsText() const;
    void saveDefsToFile();
    void addNewDef();

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
    QMap<QString,QString> m_defs;
    ColorDefInfos m_defInfos;
    QStringList m_qtClassKeywords;
    QStringList m_qssKeywords;
};

#endif // QssTextEditManager_H
