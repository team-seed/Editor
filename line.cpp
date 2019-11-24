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
        temp +=QString::number(mItems[i].type);

        if(mItems[i].type==1)   {       // type Hold
            //qDebug()<<"index "<<i<<"turn "<<mItems[i].turningPoint;
            while(mItems[i].turningPoint!=-1){
                temp+="|";
                i = mItems[i].turningPoint;
                temp+= QString::number(mItems[i].time)+":";
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
    //qDebug()<<"Ges"<<mGesture<<"Type"<<mType<<"Direction"<<mDirection;
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
    qDebug()<<"ModelIndex: "<<index<<"BeatIndex: "<<mItems[index].beat_index;
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
        status[i] ='1';
    }
  //qDebug()<<"第"<<mItems.size()-index<<"條BeatLine, 時間: "<<item.time<<
    //         "狀態 "<<status<<" left:"<<mItems[index].left<<" right "<<mItems[index].right;
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
   // qDebug()<<"index "<<index<<" turning "<<mItems[index].turningPoint;
    return true;
}

void Line::resetItemAt(int index)
{

  //qDebug()<<"Reset Item "<<index;

    if(mItems[index].type==1){
        /*由下往上取消*/
        while(mItems[index].turningPoint!=-1){
          //qDebug()<<" turnPoint"<<mItems[index].turningPoint;
            resetItemAt(mItems[index].turningPoint);
        }
        /*取消指向自己的turningPoint*/
        int previous = mItems[index].previous;
        if(previous!=-1) mItems[previous].turningPoint = -1;
        mItems[index].previous = -1;
       // qDebug()<<" previous"<<previous;
       // qDebug()<<"Pop: "<<holdList.back();
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

    for(int i=0;i<16;i++)   mItems[index].checked[i] = false;
    mItems[index].left = -1;
    mItems[index].right = -1;
    mItems[index].type = -1;
    mItems[index].gesture = -1;
    mItems[index].direction = -1;
}

bool Line::loadNotes(int time,QJsonObject input)
{

    int bpm = input.value("BPM").toInt();
    int beat = input.value("BEAT").toInt();
    int offset = input.value("OFFSET").toInt();
  //qDebug()<<"time: "<<time<<"beat: "<<beat<<"bpm: "<<bpm
   //        <<"offset: "<<offset;
    setBeatLines(time,bpm,beat,offset);

  //qDebug()<<"Size: 1"<<mItems.size();
    QJsonArray notes = input.value("NOTES").toArray();

    //qDebug()<<notes;    
    for(int i=0;i<notes.size();i++){
        //qDebug()<<notes[i];
        QStringList temp;
        temp  = notes[i].toString().split(',');

        int time = temp[0].toInt();   //time
        int gesture = temp[1].toInt();   //gesture
        int left = temp[2].toInt();   //left
        int right = temp[3].toInt()-1;   //right
        int type = temp[4].mid(0,1).toInt();   //type
      //qDebug()<<"temp[4]: "<<temp[4]<<" type: "<<type;
        int currentLine = -1;
        for(int i=0;i<mItems.size();i++){
            if(mItems[i].time==time){
                currentLine = i;
            }
        }
        if(currentLine==-1) return false;
      //qDebug()<<"current: "<<currentLine;
        mItems[currentLine].gesture = gesture;
        mItems[currentLine].left = left;
        mItems[currentLine].right = right;
        mItems[currentLine].type = type;
        if(type==2){    //swipe
          //qDebug()<<"temp[4]:"<<temp[4];
            mItems[currentLine].direction = temp[4].mid(2,1).toInt();
          //qDebug()<<" dir: "<<mItems[currentLine].direction;
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
                    if(mItems[i].time==HoldTime){
                        HoldcurrentLine = i;
                    }
                }
                if(currentLine==HoldPreviousLine)
                    mItems[currentLine].turningPoint = HoldcurrentLine;
              //qDebug()<<"Holdcurrent: "<<HoldcurrentLine;
                if(HoldcurrentLine==-1) return false;
                if(NextHoldTime!=-1){
                    HoldNextLine = -1;
                    for(int i=0;i<mItems.size();i++){      //find next line
                        if(mItems[i].time==NextHoldTime){
                            HoldNextLine = i;
                        }
                    }
                    if(HoldNextLine==-1) return false;
                }

                if(HoldcurrentLine==-1) return false;        //type,gesture,left,right,previous,turningPoint
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

void Line::setBeatLines(int time,int bpm,int beat,int offset)
{
    moffset = offset;
    //Remove old line
    if(!mItems.empty()){
        emit preItemRemoved(0,mItems.size()-1);
        mItems.clear();
        emit postItemRemoved();
    }

    //Add new Line (time/(60/bpm*1000))
    int count = ceil( time/((double)60/bpm*1000) );
    int spacing = qRound((double)60/bpm*1000);
    //int height = count * spacing;
    //qDebug()<<"Count: "<<count<<"Total Height: "<<height;
    int Linetime = (count-mItems.size())*spacing+moffset;
    for(int i=0;i<count;i++){
        if(i%beat==0){
           appendItem(Linetime,10,spacing,"royalblue",false,mItems.size());
         }
        else{
           appendItem(Linetime,5,spacing,"lightblue",false,mItems.size());
        }
    }
    appendItem(Linetime,2,offset,"black",false,mItems.size());
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

void Line::appendItem(int time,int bold,int height,QString color,bool deletable,int position)
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

    int currentHeight = mItems[index].buttonHeight;
    int slicedHeight  = qRound((double)currentHeight/slice);

    int currentTime = mItems[index].time;
    int slicedTime = currentTime-qRound((double)mItems[index].buttonHeight/slice);

    mItems[index].buttonHeight = slicedHeight;
    for(int i=1;i<slice;i++){
        appendItem(slicedTime,lineSize,slicedHeight,lineColor,true,index+i);
    }
    for(int i=index+slice;i<mItems.size();i++){         //重設beat_index
        mItems[i].beat_index = i;
    }

    for(int i=0;i<mItems.size();i++){                   //重設turningPoint && previous
        if(mItems[i].turningPoint>=index+1 && mItems[i].turningPoint<mItems.size())
            mItems[i].turningPoint +=(slice-1);
        else if(mItems[i].previous>=index+1 && mItems[i].previous<mItems.size())
            mItems[i].previous +=(slice-1);
    }
    for(int i=0;i<mItems.size();i++){
        qDebug()<<"turn :"<<mItems[i].turningPoint<<"pre: "<<mItems[i].previous;
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
int Line::shapeHeight(int previous,int bpm,int spacing)
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
  //qDebug()<<"turn "<<turning<<" current "<<current<<" height "<<height;
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
    int height = (previous-current)*spacing;
   // qDebug()<<"pre "<<previous<<" current "<<current<<" height "<<height;
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
    for(int i=0;i<mItems.size();i++){
        if(mItems[i].turningPoint>=index && mItems[i].turningPoint<mItems.size())
            mItems[i].turningPoint -= 1;
        if(mItems[i].previous>=index && mItems[i].previous<mItems.size())
            mItems[i].previous -= 1;
    }
    //qDebug()<<"New: "<<mItems[index].time;
    return true;
}

bool Line::deletable(int index){
    if(index>=mItems.size())    return false;
    if(mItems[index].deletable==false)  return false;
    return true;
}

