#ifndef QSSPROXYMODEL_H
#define QSSPROXYMODEL_H

#include <QSortFilterProxyModel>

class qssProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum class SortType{
        KeyAscendingOrder,
        KeyDescendingOrder,
        Color
    };
    qssProxyModel(QObject *parent = nullptr);
protected:
    //bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // QSSPROXYMODEL_H
