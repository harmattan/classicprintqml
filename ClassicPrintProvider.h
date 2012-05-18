#ifndef CLASSICPRINTQML_CLASSICPRINTPROVIDER_H
#define CLASSICPRINTQML_CLASSICPRINTPROVIDER_H

#include <QtCore>
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrint.h"
#include "ClassicPrintDeclarative.h"

class ClassicPrintProvider : public QDeclarativeImageProvider {
    public:
        ClassicPrintProvider()
            : QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
        {
        }

        QImage requestImage(const QString &id, QSize *size,
                const QSize &requestedSize)
        {
            if (id == "") {
                return QImage();
            }

            QString filename(id);
            int pos = -1;
            if ((pos = id.lastIndexOf("#")) != -1) {
                filename = filename.mid(0, pos);
            }

            QImage source(filename);
            size->setWidth(source.width());
            size->setHeight(source.height());

            QSize targetSize(requestedSize);

            if (targetSize.width() == 0) {
                targetSize.setWidth(source.width()*targetSize.height()/
                        source.height());
            } else if (targetSize.height() == 0) {
                targetSize.setHeight(source.height()*targetSize.width()/
                        source.width());
            }

            QImage destination(targetSize, source.format());
            ClassicPrintDeclarative::getClassicPrint()->process(
                    source.scaled(targetSize,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation),
                    targetSize.width(),
                    targetSize.height(),
                    destination);

            return destination;
        }

        static void addToView(QDeclarativeView *view) {
            view->engine()->addImageProvider(QLatin1String("classicPrint"),
                    new ClassicPrintProvider);
        }
};

#endif
