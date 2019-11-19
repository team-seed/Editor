#ifndef PLAYER_H
#define PLAYER_H
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QQmlContext>
#include<QString>
#include<QTime>
using namespace::std;

class Player:public QObject{
    Q_OBJECT
    Q_PROPERTY(qint64 time READ timeRead NOTIFY timeChanged)
    Q_PROPERTY(qint64 position READ positionRead NOTIFY positionChanged)

public:
    Player(){
        time=0;
        url="";
        loop=0;
        start=0;
    }

    qint64 timeRead ()const {
        return time;
    }
    qint64 positionRead() const{
        return music.position();
    }

public slots:
    void displayErrorMessage(){
        qDebug()<<music.errorString();
    }

    void set_music(QUrl url = QUrl(""), bool loop = false, int start = 0){
        if(!plist.isEmpty())
            plist.clear();
        plist.addMedia(url);
        if(loop)
            plist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        music.setPlaylist(&plist);
        music.setVolume(100);
        music.setPosition(start);
        music.stop();
        connect(&music,QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
                this,&Player::displayErrorMessage);
        connect(&music,SIGNAL(durationChanged(qint64)),this,SLOT(get_duration()));
    }

    void play(qint64 x){
        music.setPosition(x);
        music.play();
    }

    void play_from (int x) {
        music.setPosition(x);
    }

    void pause(){
        music.pause();
        emit positionChanged();
    }

    qint64 get_position() {
        return music.position();
    }

    bool isready(){
        return music.isAudioAvailable();
    }

    void get_duration(){
        time=music.duration();
        emit timeChanged();
    }

signals:
    void timeChanged();
    void positionChanged();
private:
    qint64 time;
    QUrl url;
    bool loop;
    int start;
    QMediaPlaylist plist;
    QMediaPlayer music;
};
#endif // PLAYER_H
