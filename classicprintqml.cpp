
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

    QStringList fileModel;


#if defined(CLASSICPRINTQML_DESKTOP)
    QDir dcim("/home/thp/Pictures/Webcam/");
#else
    QDir dcim("/home/user/MyDocs/DCIM/");
#endif

    QStringList photos;
    photos << "*.jpg" << "*.png";

    QStringList fileNames = dcim.entryList(photos, QDir::Files, QDir::Time);
    QString fileName;
    foreach (fileName, fileNames) {
        fileModel << (dcim.absolutePath() + "/" + fileName);
    }

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

