
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrintProvider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;

    ClassicPrintProvider::init();
    ClassicPrintProvider::addToView(&view);

    view.setSource(QUrl("qrc:/classicprintqml.qml"));
    view.show();

    return app.exec();
}

