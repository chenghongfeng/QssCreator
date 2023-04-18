#include "colordeftablemodel.h"

#include <QFont>
#include <QBrush>
#include <QColor>
#include <QColorDialog>
#include <qsstexteditmanager.h>


ColorDefTableModel::ColorDefTableModel(ColorDefInfos &infos, QObject *parent)
    : QAbstractTableModel(parent),
      defInfos_(&infos)
{

}


QVariant ColorDefTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Index:
            return QString(tr("Index"));
        case Key:
            return QString(tr("Key"));
        case Value:
            return QString(tr("Value"));
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

int ColorDefTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return defInfos_->size();
}

int ColorDefTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ColorDefTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
//    qDebug() << QString("row %1, col%2, role %3")
//            .arg(row).arg(col).arg(role);

    switch (role) {
    case ColorDefTableModel::KeyRole:
        return defInfos_->at(row).key;
        break;
    case ColorDefTableModel::ColorRole:
        return QColor(defInfos_->at(row).value);
        break;
    case ColorDefTableModel::DataStatusRole:
        return defInfos_->at(row).status;
        break;
    case Qt::EditRole:
        if(col == Key)
        {
            return defInfos_->at(row).key;
        }
        else if(col == Value)
        {
            return QColor(defInfos_->at(row).value);
        }
        break;
    case Qt::DisplayRole:
        if (col == Index)
        {
            return row+1;
        }
        else if (col == Key)
        {
            return defInfos_->at(row).key;
        }
        else if(col == Value)
        {
            return QColor(defInfos_->at(row).value);
        }
        break;
    case Qt::BackgroundRole:
        if (col == Value)
        {
            QColor color(defInfos_->at(row).value);
            return QBrush(color);
        }
        break;
    case Qt::ForegroundRole:
        if (col == Value)
        {
            QColor color(defInfos_->at(row).value);
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
        if (col == Key)
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == Index){
            return int(Qt::AlignCenter);
        }
        else if (col == Value)
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
    if (role == Qt::EditRole)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        if (!checkIndex(index)) //checkIndex在5.11版本引入
            return false;
#endif
        if(index.column() == Key ||index.column() == Value)
        {
            if(index.column() == Value)
            {
                (*defInfos_)[index.row()].value = value.toString();
            }
            else
            {
                (*defInfos_)[index.row()].key = value.toString();
            }
            //新加项被修改也不需要改变属性,因为新加项是从文件尾添加的;
            if((*defInfos_)[index.row()].status != ColorDefInfo::DefStatus::Append)
            {
                (*defInfos_)[index.row()].status = ColorDefInfo::DefStatus::Modification;
            }
            emit dataChanged(index,index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags ColorDefTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void ColorDefTableModel::resetDefMap(QMap<QString, QString> &map)
{
    defMap_ = map;
    defMapChanegd();

}

void ColorDefTableModel::resetDefInfos(ColorDefInfos &infos)
{
    beginResetModel();
    defInfos_ = &infos;
    endResetModel() ;

}

void ColorDefTableModel::defMapChanegd()
{
    beginResetModel();
    defs_ = defMap_.keys();
    values_ = defMap_.values();
    endResetModel() ;
}
