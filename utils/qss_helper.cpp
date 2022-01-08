#include "qss_helper.h"

#include <QFile>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextStream>
#include <utility>
const QString DefaultPattern = "(?<key>[$]\\w+)\\s*=\\s*(?<value>[#(),.\\w]*)\\s*[\\r\\n;\\/]+";

QssHelper::QssHelper()
{

}

QMap<QString,QString> QssHelper::getColorDefineFromQStr(const QString &defsText, const QString &pattern)
{
    //\s  包括空格、制表符、换页符等空白字符的其中任意一个
    //([$]\w+)\s*=[ \t]*([#(),.\w]*)[\t ]*[\r\n;\/]+
    //      group1                                  group2
    // =================                          ============
    // (  [$]       \w+)     \s*      = \s*       ([#(),.\w]*)   \s*[\r\n;\/]+
    //     $     任意字符     空白字符  =
    QString exp;
    if (pattern == "")
    {
        exp = DefaultPattern;
    }
    else
    {
        exp = pattern;
    }
    QRegularExpression reg(exp);

//    QRegularExpression::MatchType matchType = QRegularExpression::NormalMatch;
//    QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;
//    QRegularExpression::MatchOptions matchOptions = QRegularExpression::NoMatchOption;
    //reg.setPatternOptions(patternOptions);

    QMap<QString,QString> defs;
    if (!reg.isValid())
    {
        return defs;
    }
    QRegularExpressionMatchIterator iterator = reg.globalMatch(defsText/*, 0,matchType, matchOptions*/);
    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        defs[match.captured("key")] = match.captured("value");
    }
    return defs;

}

QMap<QString, QString> QssHelper::getColorDefineFromQStr(const QString &defsText, ColorDefInfos &infos, const QString &pattern)
{
    //\s  包括空格、制表符、换页符等空白字符的其中任意一个
    //([$]\w+)\s*=[ \t]*([#(),.\w]*)[\t ]*[\r\n;\/]+
    //      group1                                  group2
    // =================                          ============
    // (  [$]       \w+)     \s*      = \s*       ([#(),.\w]*)   \s*[\r\n;\/]+
    //     $     任意字符     空白字符  =
    QString exp;
    if (pattern == "")
    {
        exp = DefaultPattern;
    }
    else
    {
        exp = pattern;
    }
    QRegularExpression reg(exp);

//    QRegularExpression::MatchType matchType = QRegularExpression::NormalMatch;
//    QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;
//    QRegularExpression::MatchOptions matchOptions = QRegularExpression::NoMatchOption;
    //reg.setPatternOptions(patternOptions);

    QMap<QString,QString> defs;
    if (!reg.isValid())
    {
        return defs;
    }
    QRegularExpressionMatchIterator iterator = reg.globalMatch(defsText/*, 0,matchType, matchOptions*/);
    QList<QRegularExpressionMatch> matchs;
    while (iterator.hasNext())
    {

        QRegularExpressionMatch match = iterator.next();
        defs[match.captured("key")] = match.captured("value");

        ColorDefInfo info;
        info.original_key = match.captured("key");
        info.original_value = match.captured("value");
        info.key = info.original_key;
        info.value = info.original_value;
        info.original_key_start = match.capturedStart("key");
        info.original_value_start = match.capturedStart("value");
        infos.push_back(std::move(info));
    }
    return defs;
}

void QssHelper::getOneDefineFromQStr(const QString &defsText, ColorDefInfo &info, const QString &pattern)
{
    QString exp;
    if (pattern == "")
    {
        exp = DefaultPattern;
    }
    else
    {
        exp = pattern;
    }
    QRegularExpression reg(exp);

    if (!reg.isValid())
    {
        return;
    }
    QRegularExpressionMatchIterator iterator = reg.globalMatch(defsText/*, 0,matchType, matchOptions*/);
    QList<QRegularExpressionMatch> matchs;
    //while (iterator.hasNext())
    {

        QRegularExpressionMatch match = iterator.next();
        info.original_key = match.captured("key");
        info.original_value = match.captured("value");
        info.key = info.original_key;
        info.value = info.original_value;
        info.original_key_start = match.capturedStart("key");
        info.original_value_start = match.capturedStart("value");
    }
    return;
}

void QssHelper::replaceDefsWithValues(QString &qssText, const QMap<QString, QString> &defsMap)
{
    auto iter = defsMap.constBegin();
    while (iter != defsMap.constEnd())
    {
        //要加上匹配到的作为非"_"字符的group2
        QString replaceText = iter.value() + "\\2";
        qssText.replace(strToReplaceRegexp(iter.key()),replaceText);
        iter++;
    }
}

QRegularExpression QssHelper::strToReplaceRegexp(const QString &str)
{
    QString regExp = str;
    regExp.replace("$","");
    //匹配不精确 同时定义了border和border_disabled时将出现问题
    //修改成这个样子会匹配对应字符串且后一个字符不是_ 这样的话$xxxx变量就不能出现在文件尾;
    regExp = QString("(\\$%1)([^_])").arg(regExp);
    QRegularExpression reg(regExp);
    return reg;
}

QString QssHelper::codeToPattern(QString code)
{
    for (int i = 0; i < code.size(); ++i) {
        if (code.at(i) == QLatin1Char('\\'))
            code.remove(i, 1);
    }
    if (code.startsWith(QLatin1Char('"')) && code.endsWith(QLatin1Char('"'))) {
        code.chop(1);
        code.remove(0, 1);
    }
    return code;
}

void QssHelper::writeQStrTofile(const QString &str, const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(/*QIODevice::Truncate|*/QIODevice::WriteOnly))
    {
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<str;
    file.close();
}
