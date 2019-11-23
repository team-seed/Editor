#ifndef FILEIO_H
#define FILEIO_H
#include <QObject>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
using namespace std;
class FileIO : public QObject{
    Q_OBJECT

    QString get_path(QUrl url){
        QString filename = url.path();
        filename.remove(0,1); //cut first '/'
        return filename;
    }

public slots:
    QString get_filename(QUrl a){
        return a.fileName();
    }

    QJsonObject openchart(QUrl fileurl){
        QFile file(get_path(fileurl));
        QJsonObject json_data;

        file.open(QIODevice::ReadOnly);
        if(!file.isOpen()){
            qDebug()<<"open file fail";
            return json_data;
        }

        QJsonParseError jsonError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(), &jsonError);
        if (!jsonDocument.isNull() && (jsonError.error == QJsonParseError::NoError))
            json_data=jsonDocument.object();

        file.close();
        return json_data;
    }

    bool savechart(QUrl fileurl, QJsonObject chart){
        QFile file(get_path(fileurl));
        QByteArray data=QJsonDocument(chart).toJson(QJsonDocument::Indented);

        file.open(QIODevice::WriteOnly);
        if(!file.isOpen()){
            qDebug()<<"open file fail";
            return 0;
        }

        file.write(data);
        file.close();
        return 1;
    }
    void test(QJsonObject a){
        //Qobject can connect with qml
        QByteArray array=QJsonDocument(a).toJson(QJsonDocument::Indented);
        qDebug()<<a["BPM"];
    }
};

#endif // FILEIO_H
