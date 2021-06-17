#ifndef QSSHELPER_H
#define QSSHELPER_H

#include <QStringList>
#include <QMap>
#include <QString>

class QssHelper
{
public:
    QssHelper();

    //使用 pattern 从 filename 获取 def,value 的map ,默认形式 $def = value
    static QMap<QString,QString> getColorDefineFromQStr(const QString &defsText,const QString &pattern = "");
    static void replaceDefsWithValues(QString &qssText, const QMap<QString,QString> &defsMap);
    static QRegularExpression strToReplaceRegexp(const QString &str);
    static QString codeToPattern(QString code);
    static void wirteQStrTofile(const QString &str, const QString &fileName);

};

#endif // QSSHELPER_H
