#ifndef QSSHELPER_H
#define QSSHELPER_H

#include <QStringList>
#include <QMap>
#include <QString>

class QssHelper
{
public:
    QssHelper();

    //使用 pattern 从 defsText 获取 def,value 的map ,默认形式 $def = value
    static QMap<QString,QString> getColorDefineFromQStr(const QString &defsText,const QString &pattern = "");
    //使用定义变量替换qssText
    static void replaceDefsWithValues(QString &qssText, const QMap<QString,QString> &defsMap);
    //将str转换成匹配这个str的正则表达式
    static QRegularExpression strToReplaceRegexp(const QString &str);
    static QString codeToPattern(QString code);
    //将字符串写入文件
    static void writeQStrTofile(const QString &str, const QString &fileName);

};

#endif // QSSHELPER_H
