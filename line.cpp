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
        temp +=QString::number(qRound(mItems[i].time))+',';
        temp +=QString::number(mItems[i].gesture)+',';
        temp +=QString::number(mItems[i].left)+',';
        temp +=QString::number(mItems[i].right+1)+',';
        temp +=QString::number(mItems[i].type);

        if(mItems[i].type==1)   {       // type Hold
            //qDebug()<<"index "<<i<<"turn "<<mItems[i].turningPoint;
            while(mItems[i].turningPoint!=-1){
                temp+="|";
                i = mItems[i].turningPoint;
                temp+= QString::number(qRound(mItems[i].time))+":";
                temp+= QString::number(mItems[i].left)+":";
                temp+= QString::number(mItems[i].right+1);
                //qDebug()<<"index "<<i<<"turn "<<mItems[i].turningPoint;
            }
        }
        else if(mItems[i].type==2){
            temp+="|";
            temp+=QString::number(mItems[i].direction);
        }
        Output.push_back(temp);
        //qDebug()<<i;
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
    qDebug()<<"Set At : "<<index;
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
    for(int i=mItems[index].left;i<=mItems[index].right;i++){
        mItems[index].checked[i] = true;
    }
    /*設定type gesture direction*/
    mItems[index].type = mType;
    mItems[index].gesture = mGesture;
    if(mType==2)    mItems[index].direction = mDirection;
    else mItems[index].direction = -1;

    /*設定 turningPoint */
    if(mType==1){           //Hold Type
        if(!holdList.contains(index)){          // 若是第一次才會加入HoldList
            qDebug()<<"First: "<<index;

            if(!holdList.empty()){              // 若是上一次最後Hold位置在這條線之後(不支援由上往下新增Hold)，則清空HoldList
                if(holdList.back()<index){
                    holdList.clear();
                    qDebug()<<"Clear: "<<index;
                }
            }
            holdList.append(index);
            int previousLine = -1;
            if(holdList.size()>=2){         // 設定previousLine 前面至少要先有一條Line
                previousLine = holdList[holdList.size()-2];
                mItems[previousLine].turningPoint = index;
                mItems[index].previous = previousLine;
            }

        }
    }
    else{
        mItems[index].turningPoint = -1;
        holdList.clear();
    }
    qDebug()<<"index "<<index<<" turning "<<mItems[index].turningPoint<<" previous "<<mItems[index].previous;
    return true;
}

void Line::resetItemAt(int index)
{

  qDebug()<<"Reset Item "<<index<<" TP: "<<mItems[index].turningPoint;

    if(mItems[index].type==1){
        /*由下往上取消*/
        if(mItems[index].turningPoint!=-1){
          qDebug()<<" turnPoint"<<mItems[index].turningPoint;
          if(index == mItems[index].turningPoint){
            qDebug()<<index<<": turningPoint 指向自己";
            mItems[index].turningPoint = -1;
          }else{
            qDebug()<<"ResetFuck "<<index<<" TP: "<<mItems[index].turningPoint;
            resetItemAt(mItems[index].turningPoint);
          }
        }

        /*取消指向自己的turningPoint*/
        int previous = mItems[index].previous;
        if(previous!=-1) mItems[previous].turningPoint = -1;
        mItems[index].previous = -1;
        qDebug()<<" previous"<<previous;

        if(!holdList.empty()){
            //qDebug()<<"Pop: "<<holdList.back();
            int find = -1;
            for(int i=0;i<holdList.size();i++){
                if(holdList[i]==index)
                    find = i;
            }
            if(find!=-1){
                holdList.erase(holdList.begin()+find);
               // qDebug()<<"Erase At: "<<find;
            }
        }
    }

    for(int i=0;i<16;i++)   mItems[index].checked[i] = false;
    mItems[index].left = -1;
    mItems[index].right = -1;
    mItems[index].type = -1;
    mItems[index].gesture = -1;
    mItems[index].direction = -1;
    qDebug()<<"Reset"<<index<<"OK";
}

bool Line::loadNotes(double time,QJsonObject input)
{

    double bpm = input.value("BPM").toDouble();
    int beat = input.value("BEATS").toInt();
    int offset = input.value("OFFSET").toInt();
    QJsonArray notes = input.value("NOTES").toArray();

    setBeatLines(time,bpm,beat,offset);
    double beatspacing = 60000/bpm;

    for(int i=0;i<notes.size();i++){
        QStringList temp;
        temp  = notes[i].toString().split(',');

        int time = temp[0].toInt();   //time
        int gesture = temp[1].toInt();   //gesture
        int left = temp[2].toInt();   //left
        int right = temp[3].toInt()-1;   //right
        int type = temp[4].mid(0,1).toInt();   //type
        int currentLine = -1;           //  find currentLine

        for(int i=0;i<mItems.size();i++){           //從Beat上找
            if(qRound(mItems[i].time)==time){
                currentLine = i;
                break;
            }
        }

        if(currentLine==-1){    //若Beat上找不到，往1/2 1/4 1/6 1/8 1/12Beat上找

            for(int i=0;i<mItems.size()-1;i++){
                if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/4))==time){   // 1/4線
                    sliceAt(i,2);
                    sliceAt(i+1,2);
                    currentLine = i+2;
                    break;
                }
                else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/2))==time){   // 1/2線
                    sliceAt(i,2);
                    currentLine = i+1;
                    break;
                }
                else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*1/3))==time){   // 1/3線
                        sliceAt(i,3);
                        currentLine = i+2;
                        break;
                }
                else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*2/3))==time){   // 2/3線
                        sliceAt(i,3);
                        currentLine = i+1;
                        break;
                }
            }
        }
        qDebug()<<"Current: "<<currentLine;
        if(currentLine==-1) continue;
        mItems[currentLine].gesture = gesture;
        mItems[currentLine].left = left;
        mItems[currentLine].right = right;
        mItems[currentLine].type = type;
        if(type==2){    //swipe
            mItems[currentLine].direction = temp[4].mid(2,1).toInt();
        }else if(type == 1){     //hold
            QStringList temp2 = temp[4].split('|');
            int HoldPreviousLine = currentLine;
            int HoldcurrentLine = -1;
            int HoldNextLine = -1;

            for(int j=1;j<temp2.size();j++){
                QStringList temp3 = temp2[j].split(':');
                int HoldTime = temp3[0].toInt();
                int Holdleft = temp3[1].toInt();
                int Holdright = temp3[2].toInt()-1;
                int NextHoldTime = -1;
                if(j+1<temp2.size())
                    NextHoldTime = temp2[j+1].split(':')[0].toInt();

                HoldcurrentLine = -1;
                for(int i=0;i<mItems.size();i++){       //find current line
                    if(qRound(mItems[i].time)==HoldTime){
                        HoldcurrentLine = i;
                    }
                }
                if(HoldcurrentLine==-1){    //若Beat上找不到，往1/2 1/4 1/6 1/8 1/12Beat上找

                    for(int i=0;i<mItems.size()-1;i++){
                        if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/4))==HoldTime){   // 1/4線
                            sliceAt(i,2);
                            sliceAt(i+1,2);
                            HoldcurrentLine = i+2;
                            break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/2))==HoldTime){   // 1/2線
                            sliceAt(i,2);
                            HoldcurrentLine = i+1;
                            break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*1/3))==HoldTime){   // 1/3線
                                sliceAt(i,3);
                                HoldcurrentLine = i+2;
                                break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*2/3))==HoldTime){   // 2/3線
                                sliceAt(i,3);
                                HoldcurrentLine = i+1;
                                break;
                        }
                    }
                }
                if(HoldcurrentLine==-1) continue;
                if(currentLine==HoldPreviousLine)
                    mItems[currentLine].turningPoint = HoldcurrentLine;

                if(NextHoldTime!=-1){
                    HoldNextLine = -1;
                    for(int i=0;i<mItems.size();i++){      //find next line
                        if(qRound(mItems[i].time)==NextHoldTime){
                            HoldNextLine = i;
                        }
                    }
                    for(int i=0;i<mItems.size()-1;i++){
                        if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/4))==NextHoldTime){   // 1/4線
                            sliceAt(i,2);
                            sliceAt(i+1,2);
                            HoldNextLine = i+2;
                            break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)/2))==NextHoldTime){   // 1/2線
                            sliceAt(i,2);
                            HoldNextLine = i+1;
                            break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*1/3))==NextHoldTime){   // 1/3線
                                sliceAt(i,3);
                                HoldNextLine = i+2;
                                break;
                        }
                        else if(qRound((double)(mItems[i+1].time+(mItems[i].time-mItems[i+1].time)*2/3))==NextHoldTime){   // 2/3線
                                sliceAt(i,3);
                                HoldNextLine = i+1;
                                break;
                        }
                    }
                }

                mItems[HoldcurrentLine].type = type;
                mItems[HoldcurrentLine].gesture = gesture;
                mItems[HoldcurrentLine].left = Holdleft;
                mItems[HoldcurrentLine].right = Holdright;
                mItems[HoldcurrentLine].previous = HoldPreviousLine;
                if(NextHoldTime!=-1)
                    mItems[HoldcurrentLine].turningPoint = HoldNextLine;
                else
                    mItems[HoldcurrentLine].turningPoint =-1;
                HoldPreviousLine = HoldcurrentLine;
            }
        }
    }
    //Set checked
    for(int i=0;i<mItems.size();i++){
        if(mItems[i].left==-1 || mItems[i].right==-1)
            continue;
        for(int j=mItems[i].left;j<=mItems[i].right;j++){
            mItems[i].checked[j] = true;
        }
    }
    return true;
}

void Line::setBeatLines(double time,double bpm,int beat,int offset)
{
    //Remove old line
    if(!mItems.empty()){
        emit preItemRemoved(0,mItems.size()-1);
        mItems.clear();
        emit postItemRemoved();
    }

    moffset = offset;
    int count = ceil( time/((double)60000/bpm) );
    double spacing = (double)60000/bpm;
    double Linetime = (count-mItems.size())*spacing+moffset;

    int supportline = 1;
    if(offset!=0)   supportline+=1;

    for(int i=0;i<count;i++){
        if((count-i)%beat==0){
           appendItem(Linetime,10,qRound(spacing),"royalblue",false,mItems.size(),true);
         }
        else{
           appendItem(Linetime,5,qRound(spacing),"lightblue",false,mItems.size(),true);
        }
        Linetime = (count-mItems.size())*spacing+moffset;
    }
    if(offset!=0)
        appendItem(Linetime,2,offset,"black",false,mItems.size(),true);
    appendItem(Linetime,2,167,"black",false,mItems.size(),false);       //底線
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

void Line::appendItem(double time,int bold,int height,QString color,bool deletable,int position,bool checkable)
{
    emit preItemAppended(position);

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
  //qDebug()<<count;
    item.bold = bold;
    item.time = time;
    item.type = -1;
    item.left = -1;
    item.right = -1;
    item.turningPoint = -1;
    item.previous = -1;
    item.gesture = -1;
    item.color = color;
    item.deletable = deletable;
    item.beat_index = position;
    item.checkable = checkable;
  //qDebug()<<"test";
    item.buttonHeight = height;
  //qDebug()<<"time "<<item.time<<"height "<<item.buttonHeight;
    //mItems.append(item);
    mItems.insert(position,item);
    emit postItemAppended();
}

void Line::sliceAt(int index, int slice)
{
    //qDebug()<<"index: "<<index<<"slice: "<<slice;
    int lineSize;
    QString lineColor;
    if(slice==2){
        lineSize = 1;
        lineColor = "plum";             //設定1/2切線顏色
    }else if(slice==3){
        lineSize = 1;
        lineColor = "palegreen";                 //設定1/3切線顏色
    }else{
        return;
    }
    // slice前
    for(int i=0;i<mItems.size();i++){
        if(mItems[i].turningPoint!=-1)  qDebug()<<"slice前: "<<i<<": tp = "<< mItems[i].turningPoint;
        else if(mItems[i].previous!=-1)  qDebug()<<"slice前: "<<i<<": previous = "<< mItems[i].previous;
    }


    int currentHeight = mItems[index].buttonHeight;
    int slicedHeight  = qRound((double)currentHeight/slice);

    double currentTime = mItems[index].time;
    double previousTime = mItems[index+1].time;
    double slicedTime = currentTime-(double)(currentTime-previousTime)/slice;

    mItems[index].buttonHeight = slicedHeight;
    for(int i=1;i<slice;i++){
        appendItem(slicedTime,lineSize,slicedHeight,lineColor,true,index+i,true);
        slicedTime -= (double)currentHeight/slice;
    }
    for(int i=index+slice;i<mItems.size();i++){         //重設beat_index
        mItems[i].beat_index = i;
    }

    /* 在 index+1~size間 的 turningPoint 和 index+slice-1~size間 previous 需要加上slice-1 */

    for(int i=0;i<mItems.size();i++){                   //重設turningPoint && previous
        if(mItems[i].turningPoint>=(index+1) && mItems[i].turningPoint<mItems.size()){
            mItems[i].turningPoint += slice-1;
            qDebug()<<i<<" :Reset TP to "<<mItems[i].turningPoint;
        }
        if(mItems[i].previous>=(index+slice-1) && mItems[i].previous<mItems.size()){
            mItems[i].previous += slice-1;
             qDebug()<<i<<" :Reset previous to "<<mItems[i].previous;
        }
    }
    // 更新前
    for(int i=0;i<holdList.size();i++){
        qDebug()<<"Old"<<holdList[i];
    }
    //HoldList值更新
    for(int i=0;i<holdList.size();i++){
        if(holdList[i]>=index && holdList[i]<mItems.size()){
            holdList[i] += slice-1;
        }
    }
    //更新後
    for(int i=0;i<holdList.size();i++){
        qDebug()<<"New"<<holdList[i];
    }
    // slice後
    for(int i=0;i<mItems.size();i++){
        if(mItems[i].turningPoint!=-1)  qDebug()<<"slice後: "<<i<<": tp = "<< mItems[i].turningPoint;
        else if(mItems[i].previous!=-1)  qDebug()<<"slice後: "<<i<<": previous = "<< mItems[i].previous;
    }
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
int Line::shapeHeight(int previous)
{
    if(previous==-1) return 0;
    int current = -1;

    for(int i=0;i<mItems.size();i++)
        if(mItems[i].previous == previous)
             current = i;
    if(current == -1) return 0;
    int height = 0;
    for(int i=current;i<previous;i++){
        height+=mItems[i].buttonHeight;
    }
    return height;
}

bool Line::removeLineAt(int index)
{
    //qDebug()<<"index: "<<index;
    if(index>=mItems.size())    return false;
    if(mItems[index].deletable==false)  return false;
    mItems[index-1].buttonHeight += mItems[index].buttonHeight;

    emit preItemRemoved(index,index);
    mItems.remove(index);
    emit postItemRemoved();

    for(int i=index;i<mItems.size();i++)
        mItems[i].beat_index = i;
    // 重設turning , previous
    for(int i=0;i<mItems.size();i++){
        if(mItems[i].turningPoint>=index && mItems[i].turningPoint<mItems.size())
            mItems[i].turningPoint -= 1;
        if(mItems[i].previous>=index && mItems[i].previous<mItems.size())
            mItems[i].previous -= 1;
    }
    // 重設holdList
    for(int i=0;i<holdList.size();i++){
        if(holdList[i]>=index && holdList[i]<mItems.size())
            holdList[i] -=1;
    }
    //qDebug()<<"New: "<<mItems[index].time;
    return true;
}

void Line::holdClear()
{
    holdList.clear();
}

bool Line::deletable(int index){
    if(index>=mItems.size())    return false;
    if(mItems[index].deletable==false)  return false;
    return true;
}

