
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrintProvider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;

    ClassicPrintProvider::init();
    ClassicPrintProvider::addToView(&view);

    QStringList fileModel;

    QDir dcim("/home/user/MyDocs/DCIM/");
    QStringList photos;
    photos << "*.jpg" << "*.png";

    QStringList fileNames = dcim.entryList(photos, QDir::Files, QDir::Time);
    QString fileName;
    foreach (fileName, fileNames) {
        fileModel << (dcim.absolutePath() + "/" + fileName);
    }

    view.rootContext()->setContextProperty("fileModel", fileModel);
    view.setSource(QUrl("qrc:/classicprintqml.qml"));
    view.showFullScreen();

    return app.exec();
}

