#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <tabwidget.h>

class centerWidget : public TabWidget
{
    Q_OBJECT
public:
    centerWidget();
protected:
    void initUi();
    void updateUi();
};

#endif // CENTERWIDGET_H
