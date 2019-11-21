#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QVector>
struct LineItem{        /*定義一個Line*/
    bool checked[16];
    int type;           /*(0,Click),(1,Swipe),(2,Hold)*/
    int turningPoint;   /*hold的下一個轉折點*/
    int left;
    int right;
    int gesture;
    int time;

};

class Line : public QObject
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);

    QVector<LineItem> items() const;

    bool setItemAt(int index,const LineItem &item,int role);
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int size);
    void postItemRemoved();

public slots:
    void setBeatLines(int ,int );
    void setType(int,int);
    void appendItem(int,int);

private:
    QVector <LineItem> mItems;
};

#endif // LINE_H
