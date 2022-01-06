#ifndef FANCYACTIONBAR_H
#define FANCYACTIONBAR_H
#include "utils_global.h"

#include <QToolButton>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE
namespace Utils {


class UTILS_EXPORT FancyToolButton : public QToolButton
{
    Q_OBJECT

    Q_PROPERTY(qreal fader READ fader WRITE setFader)
public:
    FancyToolButton(QAction *action, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    qreal fader() const { return m_fader; }
    void setFader(qreal value)
    {
        m_fader = value;
        update();
    }

    void setIconsOnly(bool iconsOnly);

    static void hoverOverlay(QPainter *painter, const QRect &spanRect);
private:
    void actionChanged();

    qreal m_fader = 0;
    bool m_iconsOnly = false;
};
class FancyActionBar : public QWidget
{
    Q_OBJECT

public:
    FancyActionBar(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void insertAction(int index, QAction *action);
    void addProjectSelector(QAction *action);
    QLayout *actionsLayout() const;
    QSize minimumSizeHint() const override;
    void setIconsOnly(bool iconsOnly);

private:
    QVBoxLayout *m_actionsLayout;
    bool m_iconsOnly = false;
};
}
#endif // FANCYACTIONBAR_H
