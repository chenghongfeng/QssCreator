#include "qss_helper.h"

#include <QFile>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

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
        exp = "([$]\\w+)\\s*=\\s*([#(),.\\w]*)\\s*[\\r\\n;\\\/]+";
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
        defs[match.captured(1)] = match.captured(2);
    }
    return defs;

}

void QssHelper::replaceDefsWithValues(QString &qssText, const QMap<QString, QString> &defsMap)
{
    auto iter = defsMap.constBegin();
    while (iter != defsMap.constEnd())
    {
        qssText.replace(strToReplaceRegexp(iter.key()),iter.value());
        iter++;
    }
}

QRegularExpression QssHelper::strToReplaceRegexp(const QString &str)
{
    QString regExp = str;
    regExp.replace("$","");
    regExp = QString("(\\$%1)").arg(regExp);
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
