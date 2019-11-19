#ifndef KB_H
#define KB_H
#include <QKeyEvent>
#include <QQuickWidget>
#include <QDebug>
#include <QApplication>
#include <QQmlEngine>
#include "Effect_play.h"
class Kb : public QQuickWidget{
    Q_OBJECT
signals:
    void keypress_signal(QVariant);
    void click_play();
public:
    Kb(){
        ep = new Effect_play;
        state = true; //true == press
        QObject::connect(engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    }

    void init () {
        QObject::connect(this, SIGNAL(keypress_signal(QVariant)),(QObject*)rootObject() , SLOT(keypress_slot(QVariant)));
        ep->init(10,QUrl::fromLocalFile("drum-hitnormal.wav"));
    }

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->isAutoRepeat())
            return;
        switch (event->key()) {
            case Qt::Key_Up:
                ct->play();
                emit keypress_signal(++count);
                //qDebug()<<"up pressed ";
                break;
            case Qt::Key_Down:
                //qDebug()<<"down pressed ";
                break;
        }
    }

    void keyReleaseEvent(QKeyEvent *event)
    {

        if(event->isAutoRepeat())
            return;
        switch (event->key()) {
            case Qt::Key_Up:

                qDebug()<<"up release ";
                break;
            case Qt::Key_Down:
                qDebug()<<"down release ";
                break;
        }
    }
private:
    int count = 0;
    Effect_play *ep;
    QKeyEvent *event;
    bool state;
};
#endif // KB_H
