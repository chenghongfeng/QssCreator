#include "qssproxymodel.h"

qssProxyModel::qssProxyModel(QObject *parent)
{

}

bool qssProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return true;
}
