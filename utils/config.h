#ifndef CONFIG_H
#define CONFIG_H

#include "utils_global.h"

#include <QSettings>
#include "singleton.h"


class UTILS_EXPORT Config
        : public QObject, public Singleton<Config>
{
public:

    void setConfigFilePathName(const QString &pathName){
        m_configFilePathName = pathName;
    }
    void readAllConfig();
protected:

private:
    QString m_configFilePathName;
    QSettings *m_settings {nullptr};


};

#endif // CONFIG_H
