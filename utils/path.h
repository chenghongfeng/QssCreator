#ifndef PATH_H
#define PATH_H
#include "utils_global.h"
#include "singleton.h"

class UTILS_EXPORT Path : public Singleton<Path>
{
    friend class Singleton<Path>;
public:
    //func about path
    static QString cdUp(const QString &folder);
    //get path
    QString exeDir(){
        return m_exeDir;
    }
    QString appDir(){
        return m_appDir;
    }
    QString dataDir(){
        return m_dataDir;
    }
    QString configFilePath(){
        return m_configFilePath;
    }
private:
    Path();

private:
    QString m_exeDir;
    QString m_appDir;
    QString m_dataDir;
    QString m_configFilePath;
};

#endif // PATH_H
