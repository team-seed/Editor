#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "line.h"
#include "Player.h"
#include "FileIO.h"
#include "linemodel.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<LineModel>("Line",1,0,"LineModel");
    qmlRegisterType<Player>("Player",1,0,"Player");
    qmlRegisterType<FileIO>("FileIO",1,0,"FileIO");
    qmlRegisterUncreatableType<Line>("Line",1,0,"Line",
        QStringLiteral("Line should not be created in QML"));

    Line line;
    Line line2;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("line"),&line);     //向qml設定line屬性
    engine.rootContext()->setContextProperty(QStringLiteral("line2"),&line2);     //向qml設定line2屬性

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
