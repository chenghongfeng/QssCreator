#ifndef COLORDEFTABLEMODEL_H
#define COLORDEFTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include "qss_helper.h"

class ColorDefTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ColorDefTableModel(ColorDefInfos &infos,QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //edit
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //
    void resetDefMap(QMap<QString,QString> &map);
    void resetDefInfos(ColorDefInfos &infos);

public slots:
    void defMapChanegd();

private:
    ColorDefInfos * defInfos_;
    QMap<QString, QString> defMap_;
    QStringList defs_;
    QStringList values_;
};

#endif // COLORDEFTABLEMODEL_H
