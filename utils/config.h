#ifndef CONFIG_H
#define CONFIG_H

#include "utils_global.h"

#include <QSettings>
#include <QVariant>
#include "singleton.h"


class UTILS_EXPORT Config
        : public QObject, public Singleton<Config>
{
    Q_OBJECT
public:
    ~Config();

    void setConfigFilePathName(const QString &pathName){
        m_configFilePathName = pathName;
    }
    void readAllConfig();
    void setSkin();
    void setSkin(const QString &qssFilePathName, const QString &colorDefFilePathName);
    void setSkin(QString &qss,const QMap<QString,QString> &defs);
    void setValue(const QString &key, const QVariant &values);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    QString themeFilePathName() const;
    QString themeName() const;
    const QVector<QString> themes(){
        return m_themes;
    }
signals:

    void valueUpdated(const QString &key, const QVariant &value);
protected:

private:
    QString m_configFilePathName;
    QSettings *m_settings {nullptr};
    QVector<QString> m_themes;
};

#endif // CONFIG_H
