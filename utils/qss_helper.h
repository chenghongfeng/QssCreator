#ifndef QSSHELPER_H
#define QSSHELPER_H

/* Warning
1.变量命名要使用$开头
2.变量不要使用_作为最后一个字符
3.value结尾推荐使用 ';'*/
#include "utils_global.h"
#include <QStringList>
#include <QMap>
#include <QString>
//通过状态来判断保存ColorDef文件时的操作


struct ColorDefInfo
{
    enum DefStatus{
        //默认
        Default = 0,
        //新添加
        Append,
        //将删除
        Deprecated,
        //已修改
        Modification
    };
    QString key{""};
    QString value{""};
    int original_key_start{0};
    int original_value_start{0};
    QString original_key{""};
    QString original_value{""};
    int fromLineNum{-1};
    DefStatus status{DefStatus::Default};
};
using ColorDefInfos = QList<ColorDefInfo>;

class UTILS_EXPORT QssHelper
{
public:
    QssHelper();

    //使用 pattern 从 defsText 获取 def,value 的map ,默认形式 $def = value
    static QMap<QString,QString> getColorDefineFromQStr(const QString &defsText,const QString &pattern = "");
    static QMap<QString,QString> getColorDefineFromQStr(const QString &defsText, ColorDefInfos &infos, const QString &pattern = "");
    static void getOneDefineFromQStr(const QString &defsText, ColorDefInfo &info, const QString &pattern = "");
    //使用定义变量替换qssText
    static void replaceDefsWithValues(QString &qssText, const QMap<QString,QString> &defsMap);
    //将str转换成匹配这个str的正则表达式
    static QRegularExpression strToReplaceRegexp(const QString &str);
    static QString codeToPattern(QString code);
    //将字符串写入文件
    static void writeQStrTofile(const QString &str, const QString &fileName);

};

#endif // QSSHELPER_H
