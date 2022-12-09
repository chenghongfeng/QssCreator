#include "textsettingswidget.h"
#include "ui_textsettingswidget.h"

#include <QIntValidator>
#include <QPainter>
#include "config.h"

TextSettingsWidget::TextSettingsWidget(const QFont &f, QWidget *parent):
    QWidget(parent),
    ui(new Ui::TextSettingsWidget)
{
    ui->setupUi(this);
    m_settings.fontFamily = f.family();
    m_settings.fontSize = f.pointSize();
    m_settings.isAntialias = true;
    if((f.styleStrategy() & QFont::NoAntialias) == QFont::NoAntialias)
    {
        m_settings.isAntialias = false;
    }
    m_lastSettings = m_settings;

    auto sizeValidator = new QIntValidator(ui->fontSizeComboBox);
    sizeValidator->setBottom(0);
    ui->fontSizeComboBox->setValidator(sizeValidator);



    connect(ui->fontFamilyComboBox, &QFontComboBox::currentFontChanged,
            this, &TextSettingsWidget::fontSelected);
    connect(ui->fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TextSettingsWidget::fontSizeSelected);
    connect(ui->fontZoomSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TextSettingsWidget::fontZoomChanged);
    connect(ui->fontAntialias, &QCheckBox::toggled,
            this, &TextSettingsWidget::fontAntialiasChanged);

    updateUi();
    //TODO tabs control support
    ui->tabSettingsGroupBox->setVisible(false);
}

TextSettingsWidget::~TextSettingsWidget()
{
    delete ui;
}

void TextSettingsWidget::apply()
{
    m_lastSettings = m_settings;
    emit fontSsttingsChanged(m_settings);
}

void TextSettingsWidget::canel()
{
    m_settings = m_lastSettings;
    updateUi();
}

void TextSettingsWidget::updateUi()
{
    ui->fontFamilyComboBox->setCurrentFont(QFont(m_settings.fontFamily));
    ui->fontAntialias->setChecked(m_settings.isAntialias);
    updatePointSizes();
    ui->fontZoomSpinBox->setValue(m_settings.fontZoom);
}

void TextSettingsWidget::fontSelected(const QFont &font)
{
    m_settings.fontFamily = font.family();
    updatePointSizes();
}

void TextSettingsWidget::fontSizeSelected(int index)
{
    const QString sizeString = ui->fontSizeComboBox->itemText(index);
    bool ok = true;
    const int size = sizeString.toInt(&ok);
    if (ok) {
        m_settings.fontSize = size;
    }
}

void TextSettingsWidget::fontZoomChanged()
{
    m_settings.fontZoom = ui->fontZoomSpinBox->value();
}

void TextSettingsWidget::fontAntialiasChanged()
{
    m_settings.isAntialias = ui->fontAntialias->isChecked();
}

QList<int> TextSettingsWidget::pointSizesForSelectedFont() const
{
    QFontDatabase db;
    const QString familyName = ui->fontFamilyComboBox->currentFont().family();
    QList<int> sizeLst = db.pointSizes(familyName);
    if (!sizeLst.isEmpty())
        return sizeLst;

    QStringList styles = db.styles(familyName);
    if (!styles.isEmpty())
        sizeLst = db.pointSizes(familyName, styles.first());
    if (sizeLst.isEmpty())
        sizeLst = QFontDatabase::standardSizes();

    return sizeLst;
}

void TextSettingsWidget::updatePointSizes()
{
    const int oldSize = m_settings.fontSize;
    ui->fontSizeComboBox->clear();
    const QList<int> sizeLst = pointSizesForSelectedFont();
    int idx = -1;
    int i = 0;
    for (; i < sizeLst.count(); ++i) {
        if (idx == -1 && sizeLst.at(i) >= oldSize) {
            idx = i;
            if (sizeLst.at(i) != oldSize)
                ui->fontSizeComboBox->addItem(QString::number(oldSize));
        }
        ui->fontSizeComboBox->addItem(QString::number(sizeLst.at(i)));
    }
    if (idx != -1)
        ui->fontSizeComboBox->setCurrentIndex(idx);
}

void TextSettingsWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void TextSettingsWidget::initUiBySettings()
{
    //字体部分
//    {
//        m_settings.fontFamily = Config:
//    }4
}

void TextSettingsWidget::on_canelButton_clicked()
{
    canel();
}

void TextSettingsWidget::on_applyButton_clicked()
{
    apply();
}
