#ifndef FILEHELPER_H
#define FILEHELPER_H
#include "utils_global.h"
#include <QFile>
#include <QStringList>

namespace Utils {

class UTILS_EXPORT FileHelper
{
public:
    FileHelper();
    static QStringList readLinesFromFile(const QString &fileName);
};



}

#endif // FILEHELPER_H
