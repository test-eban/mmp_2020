#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include "gl/connectfour.h"
#include <QThread>

#include "gl/glboard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QTranslator translator;
    if (translator.load("mm2019_de.qm", ":/translations"))
         app.installTranslator(&translator);

    QQmlApplicationEngine engine;
    qmlRegisterType<ConnectFour>("GLComponents", 1,0, "MyGLItem");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    qDebug() <<  "main (GUI) thread: " << QThread::currentThreadId();

    return app.exec();

    return 0;
}

