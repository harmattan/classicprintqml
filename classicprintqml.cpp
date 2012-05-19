
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrint.h"
#include "ClassicPrintProvider.h"
#include "ClassicPrintDeclarative.h"

//#define CLASSICPRINTQML_DESKTOP

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ClassicPrint::init();
    ClassicPrintDeclarative::init();

    QDeclarativeView view;

    ClassicPrintProvider::addToView(&view);

#if defined(CLASSICPRINTQML_DESKTOP)
    QDir dcim("/home/thp/Pictures/Webcam/");
    ClassicPrintDeclarative::destinationFolder = "/home/thp/Desktop/Classic Print/";
#else
    QDir dcim("/home/user/MyDocs/DCIM/");
    ClassicPrintDeclarative::destinationFolder = "/home/user/MyDocs/Pictures/Classic Print/";
#endif
    QDir(ClassicPrintDeclarative::destinationFolder).mkpath(".");

    QStringList photos;
    photos << "*.jpg";
    QStringList fileModel = dcim.entryList(photos, QDir::Files, QDir::Time);

    view.rootContext()->setContextProperty("dcimFolder", dcim.absolutePath());
    view.rootContext()->setContextProperty("fileModel", fileModel);
    view.setSource(QUrl("qrc:/classicprintqml.qml"));
#if defined(CLASSICPRINTQML_DESKTOP)
    view.scale(.8, .8);
    view.resize(view.size() * .8);
    view.show();
#else
    view.showFullScreen();
#endif

    return app.exec();
}

