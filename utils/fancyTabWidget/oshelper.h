#ifndef OSHELPER_H
#define OSHELPER_H

#include <utils_global.h>

class UTILS_EXPORT OsHelper
{
public:
    OsHelper();

    // 判断文件是否存在
    static bool isFileExist(const QString& file);

    // 删除文件
    static bool removeFile(const QString& file);

    static qint64 fileSize(const QString& file);

};

#endif // OSHELPER_H
