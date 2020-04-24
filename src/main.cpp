#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include "gl/myglitem.h"
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
    qmlRegisterType<MyGLItem>("GLComponents", 1,0, "MyGLItem");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.
            load(url);
    qDebug() <<  "main (GUI) thread: " << QThread::currentThreadId();

    return app.exec();

//    bool win = false;
//    GLBoard* gl = new GLBoard(cl_Red);

//    gl->fillBoard();
//    gl->addToken(0,1);
//    gl->addToken(1,2);
//    gl->addToken(1,1);
//    gl->addToken(2,2);
//    gl->addToken(3,1);
//    gl->addToken(2,2);
//    gl->addToken(2,1);
//    gl->addToken(3,2);
//    gl->addToken(3,1);
//    win = gl->checkForWin();

//    gl->printBoard();

//    std::cout << std::endl << "win: " << win << std::endl;

    return 0;
}

