#include "line.h"
#include <QDebug>

Line::Line(QObject *parent) : QObject(parent)
{
}

QVector<LineItem> Line::items() const
{
    return mItems;
}

QVector<QString>Line::noteOutput()
{
    QVector<QString> Output;
    QString temp ;

    for(int i=1;i<mItems.size();i++){
        temp = "";
        temp +=QString::number(mItems[i].time)+',';
        temp +=QString::number(mItems[i].gesture)+',';
        temp +=QString::number(mItems[i].left)+',';
        temp +=QString::number(mItems[i].right+1)+',';
        temp +=QString::number(mItems[i].type)+',';

        if(mItems[i].type==1)   {       // type Hold
            while(mItems[i].turningPoint!=-1){
                temp+="|";
                i = mItems[i].turningPoint;
                temp+= QString::number(mItems[i].time)+":";
                temp+= QString::number(mItems[i].left)+":";
                temp+= QString::number(mItems[i].right+1);
            }
        }
        qDebug()<<"Line "<<mItems.size()-i<<" :"<<temp;
        Output.push_back(temp);
    }

    return Output;
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
    /*測試type*/
    mItems[index].type= (mItems[index].type+1)%3;
    return true;
}

void Line::setBeatLines(int time,int bpm,int beat)
{
    //Remove old line
    if(!mItems.empty()){
        emit preItemRemoved(mItems.size()-1);
        mItems.clear();
        emit postItemRemoved();
    }
    //Add new Line (time/(60/bpm*1000))
    double count = time/((double)60/bpm*1000);
    qDebug()<<beat;
    for(int i=0;i<count+1;i++){
        if((i+1)%beat==0){
            appendItem((int)count+1,bpm,10);
        }
        else
            appendItem((int)count+1,bpm,5);
    }
}

void Line::setType(int, int)
{

}

void Line::appendItem(int count,int bpm,int bold)
{
    emit preItemAppended();

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
    item.bold = bold;
    item.time = (count-mItems.size()+1)*((double)60/bpm*1000);
    item.type = 1;
    item.left = -1;
    item.right = -1;
    item.turningPoint = -1;
    item.gesture = 0;
    mItems.append(item);
    emit postItemAppended();
}


