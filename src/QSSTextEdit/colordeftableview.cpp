#include "colordeftableview.h"
#include <QColorDialog>



ColorDefTableView::ColorDefTableView(QWidget *parent)
    :QTableView(parent)
{

}

bool ColorDefTableView::edit(const QModelIndex &index, QAbstractItemView::EditTrigger trigger, QEvent *event)
{
    if(index.column() == 1 && trigger == QAbstractItemView::EditTrigger::DoubleClicked)
    {

        QColor color = index.data().value<QColor>();
        QColorDialog colorDialog(color);
        QColor resultColor = colorDialog.getColor(color);
        if(resultColor.isValid())
        {
            this->model()->setData(index,resultColor);
        }

    }
    else
    {
        return QTableView::edit(index,trigger,event);
    }

}
