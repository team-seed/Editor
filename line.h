#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QVector>
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
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int size);
    void postItemRemoved();

public slots:
    QVector<QString> noteOutput();
    void setBeatLines(int ,int,int);
    void setType(int);
    void setGesture(int);
    void setDirection(int);
    void appendItem(int,int,int);
    int shapeLeft(int);
    int shapeRight(int);
    int shapeHeight(int,int,double);

private:
    QVector <LineItem> mItems;
    QVector <int> holdList;
    int mType;
    int mGesture;
    int mDirection;
    int height;
};

#endif // LINE_H
