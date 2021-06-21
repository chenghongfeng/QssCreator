#include "qss_helper.h"

#include <QFile>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextStream>

QssHelper::QssHelper()
{

}

QMap<QString,QString> QssHelper::getColorDefineFromQStr(const QString &defsText, const QString &pattern)
{
    //\s  �����ո��Ʊ������ҳ���ȿհ��ַ�����������һ��
    //([$]\w+)\s*=[ \t]*([#(),.\w]*)[\t ]*[\r\n;\/]+
    //      group1                                  group2
    // =================                          ============
    // (  [$]       \w+)     \s*      = \s*       ([#(),.\w]*)   \s*[\r\n;\/]+
    //     $     �����ַ�     �հ��ַ�  =
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
        //Ҫ����ƥ�䵽����Ϊ��"_"�ַ���group2
        QString replaceText = iter.value() + "\\2";
        qssText.replace(strToReplaceRegexp(iter.key()),replaceText);
        iter++;
    }
}

QRegularExpression QssHelper::strToReplaceRegexp(const QString &str)
{
    QString regExp = str;
    regExp.replace("$","");
    //ƥ�䲻��ȷ ͬʱ������border��border_disabledʱ����������
    //�޸ĳ�������ӻ�ƥ���Ӧ�ַ����Һ�һ���ַ�����_ �����Ļ�$xxxx�����Ͳ��ܳ������ļ�β;
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
    //file.write(str.toLatin1());
    file.close();
}
