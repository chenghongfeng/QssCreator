#include "qss_helper.h"

#include <QFile>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

QssHelper::QssHelper()
{

}

QMap<QString,QString> QssHelper::getColorDefineFromFile(const QString &fileName, const QString &pattern)
{
    QMap<QString,QString> defs;
    if (fileName.isEmpty())
    {
        return defs;
    }
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
    if (!reg.isValid())
    {
        return defs;
    }
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        const QString str = file.readAll();
        QRegularExpressionMatchIterator iterator = reg.globalMatch(str/*, 0,matchType, matchOptions*/);
        while (iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            defs[match.captured(1)] = match.captured(2);
        }
    }
    return defs;

}

QString QssHelper::replaceDefsWithValues(QString &qssText, const QMap<QString, QString> &defsMap)
{
    auto iter = defsMap.constBegin();
    while (iter != defsMap.constEnd())
    {
        qssText.replace(strToReplaceRegexp(iter.key()),iter.value());
        iter++;
    }
    return qssText;
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
