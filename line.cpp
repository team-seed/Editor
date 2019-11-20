#include "line.h"
#include <QDebug>

Line::Line(QObject *parent) : QObject(parent)
{
}

QVector<LineItem> Line::items() const
{
    return mItems;
}


bool Line::setItemAt(int index, const LineItem &item)   //設定mItems[index]
{
    if(index <0 || index >= mItems.size())
        return false;

    const LineItem &oldItem = mItems.at(index);
    bool flag = false;
    for(int i=0;i<16;i++){
        if(item.checked[i]!=oldItem.checked[i])
            flag = true;
    }
    if(flag==false) return false;
    mItems[index] = item;
    return true;
}

void Line::setBeatLines(int time,int bpm)
{
    //Remove old line
    if(!mItems.empty()){
        emit preItemRemoved(mItems.size()-1);
        mItems.clear();
        emit postItemRemoved();
    }
    //Add new Line (time/(60/bpm*1000))
    qDebug()<<"time "<<time<<" bpm "<<bpm;
    double count = time/((double)60/bpm*1000);
    qDebug()<<count;
    for(int i=0;i<count;i++){
        appendItem();
    }
}

void Line::appendItem()
{
    emit preItemAppended();

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
    mItems.append(item);

    emit postItemAppended();
}


