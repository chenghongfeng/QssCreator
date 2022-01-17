#ifndef TEXTSETTINGSWIDGET_H
#define TEXTSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class TextSettingsWidget;
}

struct FontSettings
{
    QString fontFamily;
    uint fontSize;
    uint fontZoom;
    bool isAntialias;
};

class TextSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextSettingsWidget(const QFont &f,QWidget *parent = nullptr);
    ~TextSettingsWidget();
    void apply();
    void canel();
    void updateUi();

    void fontSelected(const QFont &font);
    void fontSizeSelected(int index);
    void fontZoomChanged();
    void fontAntialiasChanged();
    QList<int> pointSizesForSelectedFont() const;

    void updatePointSizes();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void fontSsttingsChanged(const FontSettings &f);

private slots:
    void on_canelButton_clicked();

    void on_applyButton_clicked();

private:
    void initUiBySettings();
    void saveSettings();

private:
    Ui::TextSettingsWidget *ui;
    FontSettings m_settings;
    FontSettings m_lastSettings;
};

#endif // TEXTSETTINGSWIDGET_H
