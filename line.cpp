#include "line.h"
#include <math.h>
#include <QDebug>
#include <QJsonArray>

Line::Line(QObject *parent) : QObject(parent)
{
    mType = -1;
    mGesture = -1;
    mDirection = -1;
}

QVector<LineItem> Line::items() const
{
    return mItems;
}

QVector<QString>Line::noteOutput()
{
    QVector<QString> Output;
    QString temp ;

    for(int i=mItems.size()-1;i>0;i--){
        if(mItems[i].type==-1)  continue;
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
                qDebug()<<"turning"<<i;
            }
        }
        else if(mItems[i].type==2){
            temp+="|";
            temp+=QString::number(mItems[i].direction);
        }
        Output.push_back(temp);
        qDebug()<<i;
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
    /*檢查是否有設定Gesture/Type/Direction*/
   // qDebug()<<"Ges"<<mGesture<<"Type"<<mType<<"Direction"<<mDirection;
    if(mGesture==-1){
        qDebug()<<"未設定Gesture";
        return false;
    }else{
        if(mType==-1){
            qDebug()<<"未設定Type";
            return false;
        }
        else if(mType==2 && mDirection==-1){
            qDebug()<<"未設定Direction";
            return false;
        }
    }

    /*設定按下的位置如果是已選取的範圍則為取消該範圍*/
    QString status="";
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
        resetItemAt(index);
        return true;
    }

    /*將left~right範圍內全部設為true*/
    for(int i=mItems[index].left;i<mItems[index].right;i++){
        mItems[index].checked[i] = true;
        status[i] ='1';
    }
    qDebug()<<"第"<<mItems.size()-index<<"條BeatLine, 時間: "<<item.time<<
             "狀態 "<<status<<" left:"<<mItems[index].left<<" right "<<mItems[index].right;
    /*設定type gesture direction*/
    mItems[index].type = mType;
    mItems[index].gesture = mGesture;
    if(mType==2)    mItems[index].direction = mDirection;
    else mItems[index].direction = -1;

    /*設定 turningPoint */
    if(mType==1){    //Hold Type
        if(!holdList.contains(index)){
            if(!holdList.empty()){
                if(holdList.back()<index)
                    holdList.clear();
            }
            holdList.append(index);
            int previousLine = -1;
           // qDebug()<<"Size: "<<holdList.size();
            if(holdList.size()>=2){
                previousLine = holdList[holdList.size()-2];
                mItems[previousLine].turningPoint = index;
                mItems[index].previous = previousLine;
            }
           // qDebug()<<"previous "<<previousLine;

        }
        //qDebug()<<"index:"<<index<<" "
    }
    else{
        mItems[index].turningPoint = -1;
        holdList.clear();
    }
    qDebug()<<"index "<<index<<" turning "<<mItems[index].turningPoint;
    return true;
}

void Line::resetItemAt(int index)
{

    qDebug()<<"Reset Item "<<index;

    if(mItems[index].type==1){
        /*由下往上取消*/
        while(mItems[index].turningPoint!=-1){
            qDebug()<<" turnPoint"<<mItems[index].turningPoint;
            resetItemAt(mItems[index].turningPoint);
        }
        /*取消指向自己的turningPoint*/
        int previous = mItems[index].previous;
        if(previous!=-1) mItems[previous].turningPoint = -1;
        mItems[index].previous = -1;
        qDebug()<<" previous"<<previous;
        qDebug()<<"Pop: "<<holdList.back();
        int find = -1;
        for(int i=0;i<holdList.size();i++){
            if(holdList[i]==index)
                find = i;
        }
        if(find!=-1){
            holdList.erase(holdList.begin()+find);
            qDebug()<<"Erase At: "<<find;
        }
    }

    for(int i=0;i<16;i++)   mItems[index].checked[i] = false;
    mItems[index].left = -1;
    mItems[index].right = -1;
    mItems[index].type = -1;
    mItems[index].gesture = -1;
    mItems[index].direction = -1;
}

void Line::loadNotes(QJsonObject input)
{
    qDebug()<<input.value("NOTES");
    QJsonArray notes = input.value("NOTES").toArray();
    //qDebug()<<notes;
    for(int i=0;i<notes.size();i++){
        qDebug()<<notes[i];
    }
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
    int count = ceil( time/((double)60/bpm*1000) );

    double spacing = (double)60/bpm*1000;
    double height = count * spacing;

    //qDebug()<<"Count: "<<count<<"Total Height: "<<height;
    for(int i=0;i<count;i++){
        if((i+1)%beat==0){
            appendItem((int)count,bpm,10);
        }
        else
            appendItem((int)count,bpm,2);
    }
}

void Line::setType(int type)
{
   // qDebug()<<type;
    mType = type;
}

void Line::setGesture(int gesture)
{
   // qDebug()<<gesture;
    mGesture = gesture;
}

void Line::setDirection(int direction)
{
   // qDebug()<<direction;
    mDirection = direction;
}

void Line::appendItem(int count,int bpm,int bold)
{
    emit preItemAppended();

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
    item.bold = bold;
    item.time = (count-mItems.size()+1)*((double)60/bpm*1000);
    item.type = -1;
    item.left = -1;
    item.right = -1;
    item.turningPoint = -1;
    item.previous = -1;
    item.gesture = -1;
    mItems.append(item);
    emit postItemAppended();
}

int Line::shapeLeft(int previous)
{
    /*由下往上畫
    if(turning==-1) return 0;
    //qDebug()<<"turn "<<turning<<" left "<<mItems[turning].left;
    return mItems[turning].left;
    */
    if(previous==-1) return 0;
    return mItems[previous].left;
}

int Line::shapeRight(int previous)
{
    /*  由下往上畫
    if(turning==-1) return 0;
    //qDebug()<<"turn "<<turning <<"right "<<mItems[turning].right;
    return mItems[turning].right;
    */
    if(previous==-1) return 0;
    return mItems[previous].right;
}
int Line::shapeHeight(int previous,int bpm,double spacing)
{
    /*  由下往上畫
    if(turning==-1) return 0;
    int current = -1;

    for(int i=0;i<mItems.size();i++)
        if(mItems[i].turningPoint == turning)
             current = i;
    if(current == -1) return 0;
    double spacing = (double)60/bpm*1000;
    //qDebug()<<"spacing "<<spacing;
    int height = (turning-current)*(int)spacing;
    qDebug()<<"turn "<<turning<<" current "<<current<<" height "<<height;
    return height;
    */
   // qDebug()<<"pre"<<previous;

    if(previous==-1) return 0;
    int current = -1;

    for(int i=0;i<mItems.size();i++)
        if(mItems[i].previous == previous)
             current = i;
   // qDebug()<<"current"<<current;
    if(current == -1) return 0;
    //qDebug()<<"spacing "<<spacing;
    int height = (previous-current)*(int)spacing;
   // qDebug()<<"pre "<<previous<<" current "<<current<<" height "<<height;
    return height;

}


