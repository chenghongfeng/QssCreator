#include "colordeftableview.h"
#include <QColorDialog>



ColorDefTableView::ColorDefTableView(QWidget *parent)
    :QTableView(parent)
{
    //关闭选中高亮
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //启用右键菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //启用排序,这样可以点击表头就进行排序
    //this->setSortingEnabled(true);
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
            return true;
        }
        return false;
    }
    return QTableView::edit(index,trigger,event);
}
