#ifndef COLORDEFTABLEMODEL_H
#define COLORDEFTABLEMODEL_H

#include "utils_global.h"
#include <QAbstractTableModel>
#include <QMap>

class UTILS_EXPORT ColorDefTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ColorDefTableModel(QMap<QString,QString> &defMap,QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //edit
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QMap<QString, QString> defMap_;
    QStringList defs_;
    QStringList values_;
};

#endif // COLORDEFTABLEMODEL_H
