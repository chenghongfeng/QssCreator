#include "qss_helper.h"

#include <QFile>
#include <QRegularExpression>

QssHelper::QssHelper()
{

}

QStringList QssHelper::getColorDefineFromFile(const QString &fileName)
{
    QStringList defs = QStringList();
    if (fileName.isEmpty())
    {
        return defs;
    }
    QFile file(fileName);
    QRegularExpression reg("[$](\w+)\s*=[ \t]*([#(),.\w]*)[\t ]*[\r\n;\/]+");
    //QRegExp reg("[$](\w+)\s*=[ \t]*");
    if (file.open(QFile::ReadOnly))
    {
        QString str = file.readAll();
//        defs = str.split(reg);
        QStringList list;
        QRegularExpressionMatch match = reg.match(str);
        if(match.hasMatch())
        {
            QString matched = match.captured(0);
        }


    }

    return defs;

}
