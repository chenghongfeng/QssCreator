#include "qssproxymodel.h"

#include "colordeftablemodel.h"

ColorDefTableProxyModel::ColorDefTableProxyModel(QObject *parent)
{
    Q_UNUSED(parent);
}


bool ColorDefTableProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    //弃用的不显示
    return sourceModel()->data(index, ColorDefTableModel::Roles::DataStatusRole).toInt() != 2;
}

//bool ColorDefTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
//{
//    return sourceModel()->data(left, Qt::DisplayRole).toString() <
//            sourceModel()->data(right, ColorDefTableModel::Roles::KeyRole).toString();
////    switch (sortRole()) {
////    case ColorDefTableModel::Roles::KeyRole:
////        return sourceModel()->data(left, ColorDefTableModel::Roles::KeyRole).toString() <
////               sourceModel()->data(right, ColorDefTableModel::Roles::KeyRole).toString();
////        break;
////    case ColorDefTableModel::Roles::ColorRole:
////        return sourceModel()->data(left, ColorDefTableModel::Roles::ColorRole).toString() <
////               sourceModel()->data(right, ColorDefTableModel::Roles::ColorRole).toString();
////        break;
////    default:
////        return sourceModel()->data(left, ColorDefTableModel::Roles::KeyRole).toString() <
////               sourceModel()->data(right, ColorDefTableModel::Roles::KeyRole).toString();
////        break;

////    }

//}
