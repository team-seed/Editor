#include "linemodel.h"

#include "line.h"
#include <QDebug>

LineModel::LineModel(QObject *parent)
    : QAbstractListModel(parent)
    ,mLine(nullptr)
{
}

int LineModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mLine)
        return 0;

    return mLine->items().size();
}

QVariant LineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mLine)
        return QVariant();

    const LineItem item = mLine->items().at(index.row());

    return QVariant(item.checked[role%16]);
}

bool LineModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!mLine)
        return false;
    LineItem item = mLine->items().at(index.row());
    item.checked[role%16] = value.toBool();
    if (mLine->setItemAt(index.row(), item,role%16)) {
        emit dataChanged(index, index, QVector<int>());
        return true;
    }
    return false;
}

Qt::ItemFlags LineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> LineModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[B1] = "B1";names[B2] = "B2";names[B3] = "B3";names[B4] = "B4";
    names[B5] = "B5";names[B6] = "B6";names[B7] = "B7";names[B8] = "B8";
    names[B9] = "B9";names[B10] = "B10";names[B11] = "B11";names[B12] = "B12";
    names[B13] = "B13";names[B14] = "B14";names[B15] = "B15";names[B16] = "B16";
    return names;
}

Line *LineModel::mline() const
{
    return mLine;
}

void LineModel::setMline(Line *mline)
{
    beginResetModel();

    if(mLine){
        mLine->disconnect(this);
    }

    mLine = mline;

    if(mLine){
        connect(mLine,&Line::preItemAppended,this,[=](){
            const int index = mLine->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(mLine,&Line::postItemAppended,this,[=](){
           endInsertRows();
        });
        connect(mLine,&Line::preItemRemoved,this,[=](int size){
           beginRemoveRows(QModelIndex(),0,size);
        });
        connect(mLine,&Line::postItemRemoved,this,[=](){
           endRemoveRows();
        });
    }

    endResetModel();
}
