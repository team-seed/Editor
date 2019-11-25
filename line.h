#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QColor>

struct LineItem{        /*定義一個Line*/
    bool checked[16];
    int type;           /*(0,Click),(1,Swipe),(2,Hold)*/
    int turningPoint;   /*hold的下一個轉折點*/
    int previous;       /*hold的上一個轉折點*/
    int left;
    int right;
    int gesture;
    int time;
    int bold;
    int direction;
    int buttonHeight;
    int beat_index;
    bool deletable;     // 1/2 ,1/4 beat線等等
    QString color;
};

class Line : public QObject
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);
    QVector<LineItem> items() const;
    bool setItemAt(int index,const LineItem &item,int role);
    void resetItemAt(int index);
signals:
    void preItemAppended(int index);
    void postItemAppended();

    void preItemRemoved(int begin,int end);
    void postItemRemoved();

public slots:
    bool deletable(int);            // 判斷此條線是否為可刪除 (1/2, 1/4拍線)

    bool loadNotes(int,QJsonObject );
    QVector<QString> noteOutput();

    void setBeatLines(int ,int,int,int);
    void setType(int);
    void setGesture(int);
    void setDirection(int);

    void appendItem(int,int,int,QString,bool,int);
    void sliceAt(int,int);
    bool removeLineAt(int);

    int shapeLeft(int);
    int shapeRight(int);
    int shapeHeight(int,int,int);


private:
    QVector <LineItem> mItems;
    QVector <int> holdList;
    int moffset;
    int mType;
    int mGesture;
    int mDirection;
};

#endif // LINE_H
