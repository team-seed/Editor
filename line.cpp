#include "line.h"
#include <QDebug>

Line::Line(QObject *parent) : QObject(parent)
{
}

QVector<LineItem> Line::items() const
{
    return mItems;
}


bool Line::setItemAt(int index, const LineItem &item,int role)   //設定mItems[index]
{
    if(index <=0 || index >= mItems.size()) //設定index 0為終止線，故無法修改
        return false;

    //檢查item是否有更新資料，若無則return false
    const LineItem &oldItem = mItems.at(index);
    bool flag = false;
    for(int i=0;i<16;i++){
        if(item.checked[i]!=oldItem.checked[i])
            flag = true;
    }
    if(flag==false) return false;
    QString status="";
    /*設定按下的位置如果是已選取的範圍則為取消該範圍*/
    if(role>=mItems[index].left && role<=mItems[index].right)
        status = "0000000000000000";
    else{   /*設定左右界*/
        mItems[index] = item;
        for(int i=0;i<16;i++){
            if(item.checked[i]==true){
                status+='1';
                if(mItems[index].left==-1 || mItems[index].left>i)
                    mItems[index].left = i;
                if(mItems[index].right<i)
                    mItems[index].right = i;

            }else   status+='0';
        }
    }
    if(status=="0000000000000000"){
        for(int i=0;i<16;i++)   mItems[index].checked[i] = false;
        mItems[index].left = -1;
        mItems[index].right = -1;
    }

    /*將left~right範圍內全部設為true*/
    for(int i=mItems[index].left;i<mItems[index].right;i++){
        mItems[index].checked[i] = true;
        status[i] ='1';
    }
    qDebug()<<"第"<<mItems.size()-index<<"條BeatLine, 時間: "<<item.time<<
              "狀態 "<<status<<" left:"<<mItems[index].left<<" right "<<mItems[index].right;
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
    double count = time/((double)60/bpm*1000);
    for(int i=0;i<count+1;i++){
        appendItem((int)count+1,bpm);
    }
}

void Line::setType(int, int)
{

}

void Line::appendItem(int count,int bpm)
{
    emit preItemAppended();

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
    mItems.append(item);
    item.time = (count-mItems.size()+1)*((double)60/bpm*1000);
    emit postItemAppended();
}


