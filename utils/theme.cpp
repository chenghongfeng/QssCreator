#include "theme.h"
#include <QFile>
#include <QMetaEnum>

#include "config.h"
#include "qss_helper.h"
const char* SideBkgColorDefText = "$side_bkg";
const char* SideBorderColorDefText = "$side_border";
const char* FancyTabBarSelectedBackground = "$FancyTabBarSelectedBackgroundColor";

void Utils::Theme::readColors()
{

    QFile file(Config::getInstance()->themeFilePathName());
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText);
    }

    const QMetaObject &m = *metaObject();
    m_colors.resize(m.enumerator(m.indexOfEnumerator("Color")).keyCount());

    QMetaEnum e = m.enumerator(m.indexOfEnumerator("Color"));
    for (int i = 0, total = e.keyCount(); i < total; ++i) {
        const QString key = QLatin1String(e.key(i));
        m_colors[i] = readNamedColor(key);
    }
}

bool Utils::Theme::flag(Utils::Theme::Flag f) const
{
    return true;
}

QColor Utils::Theme::color(Utils::Theme::Color role) const
{
    if(role > m_colors.size())
        return Qt::black;
    return m_colors[role].first;
}

Utils::Theme::Theme()
{
    readColors();

}

QPair<QColor, QString> Utils::Theme::readNamedColor(const QString &key) const
{
    return qMakePair(QColor(m_defs.value(QString("%1%2").arg("$").arg(key), "#FFFFFF")), key);
}
