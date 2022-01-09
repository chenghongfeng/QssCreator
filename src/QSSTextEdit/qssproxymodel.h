#ifndef COLORDEFTABLEPROXYMODEL_H
#define COLORDEFTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>

class ColorDefTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum class SortRole{
        KeyAscendingOrder,
        KeyDescendingOrder,
        Color
    };
    ColorDefTableProxyModel(QObject *parent = nullptr);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
};

#endif // COLORDEFTABLEPROXYMODEL_H
