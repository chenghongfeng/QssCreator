#ifndef QSSHELPER_H
#define QSSHELPER_H

/* Warning
1.变量命名要使用$开头
2.变量不要使用_作为最后一个字符
3.value结尾推荐使用 ';'*/

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
