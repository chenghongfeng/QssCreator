#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QWidget>

namespace Ui {
class MytabWidget;
}

class MytabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MytabWidget(QWidget *parent = nullptr);
    ~MytabWidget();

private:
    Ui::MytabWidget *ui;
};

#endif // MYTABWIDGET_H
