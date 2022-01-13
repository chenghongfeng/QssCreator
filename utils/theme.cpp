#include "theme.h"
#include <QFile>

#include "path.h"
#include "config.h"
#include "qss_helper.h"
const char* SideBkgColorDefText = "$side_bkg";
const char* SideBorderColorDefText = "$side_border";

bool Utils::Theme::flag(Utils::Theme::Flag f) const
{
    return true;
}

QColor Utils::Theme::color(Utils::Theme::Color role) const
{
    QColor ret;
    bool isNoneTheme = "None" == Config::getInstance()->value("Theme/name","None").toString();
    switch (role) {
    case FancyToolBarSeparatorColor:
        ret = QColor(33, 34, 34);
        break;
    case FancyTabBarBackgroundColor:
        if(isNoneTheme){
            ret = Qt::white;
        }else{
            ret = m_defs.value(SideBkgColorDefText,"#404142");
        }
        break;
    case FancyTabBarSelectedBackgroundColor:
        ret = QColor("#212222");
        break;
    case FancyTabWidgetDisabledSelectedTextColor:
        ret = Qt::green;
        break;
    case FancyTabWidgetDisabledUnselectedTextColor:
        ret = Qt::green;
        break;
    case FancyTabWidgetEnabledSelectedTextColor:
        if(isNoneTheme){
            ret = Qt::white;
        }
        else{
            ret = QColor("#bec0c1");
        }
        break;
    case FancyTabWidgetEnabledUnselectedTextColor:
        ret = QColor("#bec0c1");
        break;
    case FancyToolButtonHoverColor:
        ret = QColor(94, 95, 96);
        break;
    case FancyToolButtonSelectedColor:
        ret = QColor(33, 34, 34);
        break;
    case PanelTextColorDark:
        ret = QColor("#bec0c1");
        break;
    case IconsDisabledColor:
        ret = QColor("#626264");
        break;
    case IconsBaseColor:
        ret = QColor("#7fc242");
        break;
    case SideBkgColor:
        if(isNoneTheme)
        {
            ret = Qt::white;
        }else{
            ret = m_defs.value(SideBkgColorDefText,"#404142");
        }
        break;
    case SideBorderColor:
        if(isNoneTheme)
        {
            ret = QColor("#404142");
        }else{
            ret = m_defs.value(SideBkgColorDefText,"#404142");
        }
        break;
    default:
        ret = Qt::red;

    }
    return ret;
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
