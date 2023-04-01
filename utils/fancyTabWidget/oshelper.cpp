#include "oshelper.h"

#include <QFile>

OsHelper::OsHelper()
{

}

bool OsHelper::removeFile(const QString& file)
{
    QFile tempFile(file);
    if (tempFile.exists())
    {
        return tempFile.remove();
    }
    return false;
}

// 判断文件是否存在
bool OsHelper::isFileExist(const QString& file)
{
    QFile tempFile(file);
    return tempFile.exists();
}

qint64 OsHelper::fileSize(const QString& file)
{
    QFile tempFile(file);
    return tempFile.bytesAvailable();
}
