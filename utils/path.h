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
    QString exeDir() const{
        return m_exeDir;
    }
    QString appDir() const{
        return m_appDir;
    }
    QString dataDir() const{
        return m_dataDir;
    }
    QString configFilePath() const{
        return m_configFilePath;
    }
    QString qssDir() const{
        return m_qssDir;
    }
    QString confDir() const{
        return m_confDir;
    }
    QString qssKeywordFilePath() const{
        return m_qssKeywordFilePath;
    }
    QString colorDefFilePath() const{
        return m_colorDefFilePath;
    }
    QString qssFilePath() const{
        return m_qssFilePath;
    }
    QString qClassKeyWordFilePath() const{
        return m_qClassKeywordFilePath;
    }
private:
    Path();

private:
    QString m_exeDir;
    QString m_appDir;
    QString m_dataDir;
    QString m_confDir;
    QString m_configFilePath;
    QString m_qssDir;
    QString m_colorDefFilePath;
    QString m_qssFilePath;
    QString m_qssKeywordFilePath;
    QString m_qClassKeywordFilePath;
};

#endif // PATH_H
