#ifndef QSSMANAGER_H
#define QSSMANAGER_H

#include <QMap>
#include <singleton.h>

class QssManager
        : public QObject, public Singleton<QssManager>
{
    friend class Singleton<QssManager>;
    Q_OBJECT
public:
    QMap<QString,QString> getDefs(){
        return m_defs;
    }

signals:
    void defsUpdated();
public slots:
    void updateDefs();
    void slot_Config_valueUpdated(const QString &key, const QVariant &value);

protected:
    QssManager();
private:
    void initDefs();
    void setDefsFile(const QString &fileName);


private:
    QMap<QString,QString> m_defs;
};

#endif // QSSMANAGER_H
