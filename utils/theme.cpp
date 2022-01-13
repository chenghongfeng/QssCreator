#include "theme.h"
#include <QFile>

#include "path.h"
#include "qss_helper.h"
const char* SideBkgColorDefText = "$side_bkg";
const char* SideBorderColorDefText = "$side_border";

bool Utils::Theme::flag(Utils::Theme::Flag f) const
{
    return true;
}

QColor Utils::Theme::color(Utils::Theme::Color role) const
{
    switch (role) {
    case FancyToolBarSeparatorColor:
        return QColor(33, 34, 34);
        break;
    case FancyTabBarBackgroundColor:
        return QColor(64, 65, 66);
        break;
    case FancyTabBarSelectedBackgroundColor:
        return QColor("#212222");
        break;
    case FancyTabWidgetDisabledSelectedTextColor:
        return Qt::green;
        break;
    case FancyTabWidgetDisabledUnselectedTextColor:
        return Qt::green;
        break;
    case FancyTabWidgetEnabledSelectedTextColor:
        return QColor("#bec0c1");
        break;
    case FancyTabWidgetEnabledUnselectedTextColor:
        return QColor("#bec0c1");
        break;
    case FancyToolButtonHoverColor:
        return QColor(94, 95, 96);
        break;
    case FancyToolButtonSelectedColor:
        return QColor(33, 34, 34);
        break;
    case PanelTextColorDark:
        return QColor("#bec0c1");
        break;
    case IconsDisabledColor:
        return QColor("#626264");
        break;
    case IconsBaseColor:
        return QColor("#7fc242");
        break;
    case SideBkgColor:
        return m_defs.value(SideBkgColorDefText,"#404142");
        break;
    case SideBorderColor:
        return m_defs.value(SideBorderColorDefText,"#000000");
        break;
    default:
        return Qt::red;

    }
}

Utils::Theme::Theme()
{
    QFile file(Path::getInstance()->colorDefFilePath());
    if(file.open(QFile::ReadOnly))
    {
        QString defsText = file.readAll();
        m_defs = QssHelper::getColorDefineFromQStr(defsText);
    }
}
