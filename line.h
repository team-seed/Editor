#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QVector>
struct LineItem{        /*定義一個Line*/
    bool checked[16];
};

class Line : public QObject
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);

    QVector<LineItem> items() const;

    bool setItemAt(int index,const LineItem &item);
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int size);
    void postItemRemoved();

public slots:
    void setBeatLines(int ,int );
    void appendItem();

private:
    QVector <LineItem> mItems;
};

#endif // LINE_H
