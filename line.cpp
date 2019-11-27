#include "line.h"
#include <math.h>
#include <QDebug>
#include <QJsonArray>

Line::Line(QObject *parent) : QObject(parent)
{
    mTotalHeight = 0;
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

        if(mItems[i].type==1)   {       // type Hold;
            while(mItems[i].turningPoint!=-1){
                temp+="|";
                i = mItems[i].turningPoint;
                temp+= QString::number(qRound(mItems[i].time))+":";
                temp+= QString::number(mItems[i].left)+":";
                temp+= QString::number(mItems[i].right+1);
            }
        }
        else if(mItems[i].type==2){
            temp+="|";
            temp+=QString::number(mItems[i].direction);
        }
        Output.push_back(temp);
    }
    return Output;
}
QVector<QString>Line::editorFileSave()
{

    QVector<QString>temp;
    QString str;
    for(int i=0;i<mItems.size();i++){
        str="";
        for(int j=0;j<16;j++){
            str+=QString::number(mItems[i].checked[j]);
        }
        str+=',';
        str+=QString::number(mItems[i].type);
        str+=',';
        str+=QString::number(mItems[i].turningPoint);
        str+=',';
        str+=QString::number(mItems[i].previous);
        str+=',';
        str+=QString::number(mItems[i].left);
        str+=',';
        str+=QString::number(mItems[i].right);
        str+=',';
        str+=QString::number(mItems[i].gesture);
        str+=',';
        str+=QString::number(mItems[i].time);
        str+=',';
        str+=QString::number(mItems[i].bold);
        str+=',';
        str+=QString::number(mItems[i].direction);
        str+=',';
        str+=QString::number(mItems[i].buttonHeight);
        str+=',';
        str+=QString::number(mItems[i].beat_index);
        str+=',';
        str+=QString::number(mItems[i].deletable);
        str+=',';
        str+=QString::number(mItems[i].checkable);
        str+=',';
        str+=mItems[i].color;
        temp.push_back(str);
    }

    return temp;
}

bool Line::editorFileOpen(QStringList file)
{
    int position = 0;
    for(int i=0;i<file.size();i++){
        emit preItemAppended(position);
        LineItem item;
        QString str = file[i];
        QStringList temp;
        temp  = str.split(',');
        for(int i=0;i<16;i++){
            if(temp[0][i]=='1') item.checked[i] = true;
            else item.checked[i] = false;
        }
        item.type = temp[1].toInt();
        item.turningPoint = temp[2].toInt();
        item.previous = temp[3].toInt();
        item.left = temp[4].toInt();
        item.right = temp[5].toInt();
        item.gesture = temp[6].toInt();
        item.time = temp[7].toDouble();
        item.bold = temp[8].toInt();
        item.direction = temp[9].toInt();
        item.buttonHeight = temp[10].toInt();
        item.beat_index = temp[11].toInt();
        if(temp[12]=='1') item.deletable = true;
        else item .deletable = false;
        if(temp[13]=='1') item.checkable = true;
        else item.checkable = false;
        item.color = temp[14];
        mItems.insert(position++,item);
        emit postItemAppended();
    }
    double temp = 0;
    for(int i=0;i<mItems.size();i++){
        temp += mItems[i].buttonHeight;
    }
    mTotalHeight = temp;
    return true;
}
bool Line::setItemAt(int index, const LineItem &item,int role)
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

/*設定按下的位置如果是已選取的範圍則為取消該範圍*/
    QString status="";
    if(role>=mItems[index].left && role<=mItems[index].right)
        status = "0000000000000000";
    else{
        mItems[index] = item;
        for(int i=0;i<16;i++){
            if(mItems[index].checked[i]==true){
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

/*設定-----turningPoint-----利用QVector<int>HoldList儲存上一次轉折點------*/
    if(mType==1){                                    //Hold Type
        if(!holdList.contains(index)){               // 若是第一次才會加入HoldList
            if(!holdList.empty()){
                if(holdList.back()<index){           // 若是上一次最後Hold位置在這條線之後(不支援由上往下新增Hold)，則清空HoldList
                    holdList.clear();
                }
            }
            holdList.append(index);
            int previousLine = -1;
            if(holdList.size()>=2){                  // 設定previousLine 前面至少要先有一條Line
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
    return true;
}

void Line::resetItemAt(int index)
{
    if(mItems[index].type==1){
        /*由下往上取消*/
        if(mItems[index].turningPoint!=-1){
          if(index == mItems[index].turningPoint){
            qDebug()<<index<<": turningPoint 指向自己";
            mItems[index].turningPoint = -1;
          }else{
            resetItemAt(mItems[index].turningPoint);
          }
        }

        /*取消指向自己的turningPoint*/
        int previous = mItems[index].previous;
        if(previous!=-1) {
            mItems[previous].turningPoint = -1;
            mItems[index].previous = -1;
        }
        if(!holdList.empty()){
            int find = -1;
            for(int i=0;i<holdList.size();i++){
                if(holdList[i]==index)
                    find = i;
            }
            if(find!=-1){
                holdList.erase(holdList.begin()+find);
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

    for(int i=0;i<notes.size();i++){
        QStringList temp;
        temp  = notes[i].toString().split(',');

        int time = temp[0].toInt();
        int gesture = temp[1].toInt();
        int left = temp[2].toInt();
        int right = temp[3].toInt()-1;
        int type = temp[4].mid(0,1).toInt();
        int currentLine = -1;

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
        if(currentLine==-1) continue;                     // 找不到這條線
        mItems[currentLine].gesture = gesture;
        mItems[currentLine].left = left;
        mItems[currentLine].right = right;
        mItems[currentLine].type = type;
        if(type==2){
            mItems[currentLine].direction = temp[4].mid(2,1).toInt();
        }else if(type == 1){
            QStringList temp2 = temp[4].split('|');
            int HoldPreviousLine = currentLine;
            int HoldcurrentLine = -1;
            int HoldNextLine = -1;

            for(int j=1;j<temp2.size();j++){
                QStringList temp3 = temp2[j].split(':');
                int HoldTime = temp3[0].toInt();
                int Holdleft = temp3[1].toInt();
                int Holdright = temp3[2].toInt()-1;

                HoldcurrentLine = -1;
                if(HoldNextLine == -1){
                    for(int i=0;i<mItems.size();i++){       //find current line
                        if(qRound(mItems[i].time)==HoldTime){
                            HoldcurrentLine = i;
                        }
                    }
                    if(HoldcurrentLine==-1){    //若Beat上找不到，往1/2 1/4 1/6 1/8 1/12Beat上找

                        for(int i=0;i<mItems.size()-1;i++){
                            double CurrentTimeLine = mItems[i].time;
                            double PreviousTimeLine = mItems[i+1].time;

                            if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)/4))==HoldTime){   // 1/4線
                                sliceAt(i,2);
                                sliceAt(i+1,2);
                                HoldcurrentLine = i+2;
                                break;
                            }
                            else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)/2))==HoldTime){   // 1/2線
                                sliceAt(i,2);
                                HoldcurrentLine = i+1;
                                break;
                            }
                            else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)*1/3))==HoldTime){   // 1/3線
                                    sliceAt(i,3);
                                    HoldcurrentLine = i+2;
                                    break;
                            }
                            else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)*2/3))==HoldTime){   // 2/3線
                                    sliceAt(i,3);
                                    HoldcurrentLine = i+1;
                                    break;
                            }
                        }
                    }
                }else{
                    HoldcurrentLine = HoldNextLine;
                }
                if(HoldcurrentLine==-1) continue;           //找不到HoldCurrentLine

                int NextHoldTime = -1;
                if(j+1<temp2.size())
                    NextHoldTime = temp2[j+1].split(':')[0].toInt();
                HoldNextLine = -1;
                if(NextHoldTime!=-1){
                    for(int i=0;i<mItems.size();i++){      //find next line
                        if(qRound(mItems[i].time)==NextHoldTime){
                            HoldNextLine = i;
                        }
                    }
                    for(int i=0;i<mItems.size()-1;i++){
                        double CurrentTimeLine = mItems[i].time;
                        double PreviousTimeLine = mItems[i+1].time;
                        if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)/4))==NextHoldTime){   // 1/4線
                            sliceAt(i,2);
                            sliceAt(i+1,2);
                            HoldNextLine = i+2;
                            break;
                        }
                        else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)/2))==NextHoldTime){   // 1/2線
                            sliceAt(i,2);
                            HoldNextLine = i+1;
                            break;
                        }
                        else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)*1/3))==NextHoldTime){   // 1/3線
                                sliceAt(i,3);
                                HoldNextLine = i+2;
                                break;
                        }
                        else if(qRound((PreviousTimeLine+(CurrentTimeLine-PreviousTimeLine)*2/3))==NextHoldTime){   // 2/3線
                                sliceAt(i,3);
                                HoldNextLine = i+1;
                                break;
                        }
                    }
                }

                if(currentLine==HoldPreviousLine)
                    mItems[currentLine].turningPoint = HoldcurrentLine;     //將起始線的tp指向這條
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
        mTotalHeight += qRound(spacing);
    }
    if(offset!=0){
        appendItem(Linetime,2,offset,"black",false,mItems.size(),true);
        mTotalHeight += offset;
    }
    mTotalHeight += 167;
    appendItem(0,2,167,"black",false,mItems.size(),false);       //底線
}

void Line::setType(int type)
{
    mType = type;
}

void Line::setGesture(int gesture)
{
    mGesture = gesture;
}

void Line::setDirection(int direction)
{
    mDirection = direction;
}

void Line::appendItem(double time,int bold,int height,QString color,bool deletable,int position,bool checkable)
{
    emit preItemAppended(position);

    LineItem item;
    for(int i=0;i<16;i++)
        item.checked[i] = false;
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
    item.buttonHeight = height;
    mItems.insert(position,item);
    emit postItemAppended();
}

void Line::sliceAt(int index, int slice)
{
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

    mItems[index].buttonHeight = slicedHeight;              //將原高度設成切過的高度

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
        }
        if(mItems[i].previous>=(index+slice-1) && mItems[i].previous<mItems.size()){
            mItems[i].previous += slice-1;
        }
    }

    //HoldList值更新
    for(int i=0;i<holdList.size();i++){
        if(holdList[i]>=index && holdList[i]<mItems.size()){
            holdList[i] += slice-1;
        }
    }
}

int Line::shapeLeft(int previous)
{

    if(previous==-1) return 0;
    return mItems[previous].left;
}

int Line::shapeRight(int previous)
{
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
    for(int i=current;i<previous;i++){      //計算這兩條線之間的高度差
        height+=mItems[i].buttonHeight;
    }
    return height;
}

double Line::getTotalHeight()
{
    return mTotalHeight;
}

bool Line::removeLineAt(int index)
{
    if(index>=mItems.size())    return false;
    if(mItems[index].deletable==false)  return false;
    mItems[index-1].buttonHeight += mItems[index].buttonHeight;     // 將刪掉這條線的高度加回去上一條線

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

