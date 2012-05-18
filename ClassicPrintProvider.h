#ifndef CLASSICPRINTQML_CLASSICPRINTPROVIDER_H
#define CLASSICPRINTQML_CLASSICPRINTPROVIDER_H

#include <QtCore>
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrint.h"

class ClassicPrintProvider : public QDeclarativeImageProvider {
    private:
        ClassicPrint classicPrint;

    public:
        ClassicPrintProvider()
            : QDeclarativeImageProvider(QDeclarativeImageProvider::Image),
              classicPrint()
        {
            classicPrint.load(":/classicPrintData/settings.xml");
        }

        QImage requestImage(const QString &id, QSize *size,
                const QSize &requestedSize)
        {
            Q_UNUSED(size);

            QImage source("/" + id);
            size->setWidth(source.width());
            size->setHeight(source.height());

            QImage destination(requestedSize, source.format());
            classicPrint.process(source,
                    requestedSize.width(),
                    requestedSize.height(),
                    destination);

            return destination;
        }

        static void init() {
            ClassicPrint::init();
        }

        static void addToView(QDeclarativeView *view) {
            view->engine()->addImageProvider(QLatin1String("classicPrint"),
                    new ClassicPrintProvider);
        }
};

#endif
