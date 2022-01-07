#ifndef COLORDEFTABLEVIEW_H
#define COLORDEFTABLEVIEW_H

#include <QTableView>

class ColorDefTableView : public QTableView
{
    Q_OBJECT
public:
    ColorDefTableView(QWidget *parent = nullptr);

protected:
    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) override;
};

#endif // COLORDEFTABLEVIEW_H
