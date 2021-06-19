#ifndef QSSHELPER_H
#define QSSHELPER_H

#include <QStringList>
#include <QMap>
#include <QString>

class QssHelper
{
public:
    QssHelper();

    //ʹ�� pattern �� defsText ��ȡ def,value ��map ,Ĭ����ʽ $def = value
    static QMap<QString,QString> getColorDefineFromQStr(const QString &defsText,const QString &pattern = "");
    //ʹ�ö�������滻qssText
    static void replaceDefsWithValues(QString &qssText, const QMap<QString,QString> &defsMap);
    //��strת����ƥ�����str��������ʽ
    static QRegularExpression strToReplaceRegexp(const QString &str);
    static QString codeToPattern(QString code);
    //���ַ���д���ļ�
    static void writeQStrTofile(const QString &str, const QString &fileName);

};

#endif // QSSHELPER_H
