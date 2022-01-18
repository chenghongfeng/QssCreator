#ifndef THEME_H
#define THEME_H

#include "utils_global.h"

#include <QVariant>
#include <QObject>
#include "singleton.h"
namespace Utils {
class UTILS_EXPORT Theme
        :public QObject,public Singleton<Theme>
{
    Q_OBJECT
public:
    enum Color{
        FancyToolBarSeparatorColor,
        FancyTabBarBackgroundColor,
        FancyTabBarSelectedBackgroundColor,
        FancyTabWidgetDisabledSelectedTextColor,
        FancyTabWidgetDisabledUnselectedTextColor,
        FancyTabWidgetEnabledSelectedTextColor,
        FancyTabWidgetEnabledUnselectedTextColor,
        FancyToolButtonHoverColor,
        FancyToolButtonSelectedColor,
        PanelTextColorLight,
        InfoBarBackground,
        ProgressBarTitleColor,
        PanelStatusBarBackgroundColor,
        PanelTextColorDark,
        PanelTextColorMid,
        BackgroundColorNormal,
        /*QssTextEdit*/
        QssTextEditPaletteText,
        QssTextEditPaletteBase,
        QssTextEditQtClassTextColor,
        QssTextEditQssKeywordTextColor,
        QssTextEditCommentTextColor,
        QssTextEditUserDefineWordTextColor,

        PaletteText,

        SideBkgColor,
        SideBorderColor,


        /* Icons */

        IconsBaseColor,
        IconsDisabledColor,
        IconsInfoColor,
        IconsInfoToolBarColor,
        IconsWarningColor,
        IconsWarningToolBarColor,
        IconsErrorColor,
        IconsErrorToolBarColor,
        IconsRunColor,
        IconsRunToolBarColor,
        IconsStopColor,
        IconsStopToolBarColor,
        IconsInterruptColor,
        IconsInterruptToolBarColor,
        IconsDebugColor,
        IconsNavigationArrowsColor,
        IconsBuildHammerHandleColor,
        IconsBuildHammerHeadColor,
        IconsModeWelcomeActiveColor,
        IconsModeEditActiveColor,
        IconsModeDesignActiveColor,
        IconsModeDebugActiveColor,
        IconsModeProjectActiveColor,
        IconsModeAnalyzeActiveColor,
        IconsModeHelpActiveColor,

        /* Code model Icons */

        IconsCodeModelKeywordColor,
        IconsCodeModelClassColor,
        IconsCodeModelStructColor,
        IconsCodeModelFunctionColor,
        IconsCodeModelVariableColor,
        IconsCodeModelEnumColor,
        IconsCodeModelMacroColor,
        IconsCodeModelAttributeColor,
        IconsCodeModelUniformColor,
        IconsCodeModelVaryingColor,
        IconsCodeModelOverlayBackgroundColor,
        IconsCodeModelOverlayForegroundColor
    };
    enum Flag {
        DrawTargetSelectorBottom,
        DrawSearchResultWidgetFrame,
        DrawIndicatorBranch,
        DrawToolBarHighlights,
        DrawToolBarBorders,
        ComboBoxDrawTextShadow,
        DerivePaletteFromTheme,
        ApplyThemePaletteGlobally,
        FlatToolBars,
        FlatSideBarIcons,
        FlatProjectsMode,
        FlatMenuBar,
        ToolBarIconShadow,
        WindowColorAsBase,
        DarkUserInterface
    };
    Q_ENUM(Color)
    Q_ENUM(Flag)
public:
    Q_INVOKABLE void readColors();
    Q_INVOKABLE bool flag(Flag f) const;
    Q_INVOKABLE QColor color(Color role) const;

protected:
    Theme();

private:
    QPair<QColor, QString> readNamedColor(const QString &color) const;

private:
    friend class Singleton<Theme>;
    QMap<QString,QString> m_defs;
    QVector<QPair<QColor, QString> > m_colors;

};
}
#endif // THEME_H
