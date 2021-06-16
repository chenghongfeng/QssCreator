#ifndef QSSHELPER_H
#define QSSHELPER_H

#include <QStringList>

class QssHelper
{
public:
    QssHelper();

    static QStringList getColorDefineFromFile(const QString &fileName);
};

#endif // QSSHELPER_H
