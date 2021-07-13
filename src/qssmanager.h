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

protected:
    QssManager();
private:
    void initDefs();


private:
    QMap<QString,QString> m_defs;
};

#endif // QSSMANAGER_H
