#ifndef THEME_P_H
#define THEME_P_H


#include "theme.h"
#include "utils_global.h"

#include <QColor>
#include <QMap>

namespace Utils {
class UTILS_EXPORT ThemePrivate
{
public:
    ThemePrivate();

    QString id;
    QString fileName;
    QString displayName;
    QStringList preferredStyles;
    QString defaultTextEditorColorScheme;
    QVector<QPair<QColor, QString> > colors;
    QVector<QString> imageFiles;
    QVector<QGradientStops> gradients;
    QVector<bool> flags;
    QMap<QString, QColor> palette;
};

UTILS_EXPORT void setCreatorTheme(Theme *theme);
UTILS_EXPORT void setThemeApplicationPalette();
}

#endif // THEME_P_H
