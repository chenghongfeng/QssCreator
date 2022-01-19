#ifndef PREVIEWQSSWIDGET_H
#define PREVIEWQSSWIDGET_H

#include <QWidget>

namespace Ui {
class PreviewQssWidget;
}

class PreviewQssWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewQssWidget(QWidget *parent = nullptr);
    ~PreviewQssWidget();


private slots:

    void updateProgress();

private:
    Ui::PreviewQssWidget *ui;
};

#endif // PREVIEWQSSWIDGET_H
