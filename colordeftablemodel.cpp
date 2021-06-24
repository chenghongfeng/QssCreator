#include "colordeftablemodel.h"

#include <QFont>
#include <QBrush>
#include <QColor>
#include <QColorDialog>


ColorDefTableModel::ColorDefTableModel(QMap<QString, QString> &defMap, QObject *parent)
    : QAbstractTableModel(parent),
      defMap_(defMap)
{
    defs_ = defMap_.keys();
    values_ = defMap_.values();
}

QVariant ColorDefTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Key");
        case 1:
            return QString("Value");
        }
    }
    return QVariant();
}

int ColorDefTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return defMap_.values().count();
}

int ColorDefTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ColorDefTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
//    qDebug() << QString("row %1, col%2, role %3")
//            .arg(row).arg(col).arg(role);

    switch (role) {
    case Qt::DisplayRole:
        if (col == 0)
        {
            return defs_[row];
        }
        else if(col == 1)
        {
            return values_[row];
        }
        break;
    case Qt::BackgroundRole:
        if (col == 1)
        {

            QColor color(values_[row]);
            return QBrush(color);
        }
        break;
    case Qt::ForegroundRole:
        if (col == 1)
        {
            QColor color(values_[row]);
            //计算出灰度 如果小于100前景设置为白色
            if (qGray(color.rgb()) < 100)
            {
                return QBrush(Qt::white);
            }
            else
            {
                return QBrush(Qt::black);
            }

        }
        break;
    case Qt::TextAlignmentRole:
        if (col == 0) //change text alignment only for cell(1,1)
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == 1)
        {
            return int(Qt::AlignCenter);
        }
        break;
    default:
        return QVariant();
    }
    return QVariant();
}

bool ColorDefTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() != 1)
        return false;
    if (role == Qt::EditRole)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    if (!checkIndex(index)) //checkIndex在5.11版本引入
        return false;
#endif
        QColor color(values_[index.row()]);
        QColorDialog colorDialog(color);
        QColor resultColor = colorDialog.getColor(color);
        return true;

    }
}

Qt::ItemFlags ColorDefTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
