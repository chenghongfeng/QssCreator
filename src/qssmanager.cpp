#include "qssmanager.h"

#include <QFile>

#include "path.h"
#include "qss_helper.h"

QssManager::QssManager()
{
    initDefs();
}

void QssManager::initDefs()
{
    QString filePathName = Path::getInstance()->colorDefFilePath();
    QFile file(filePathName);
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText,"");
    }
}
