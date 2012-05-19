#ifndef CLASSICPRINTQML_CLASSICPRINTTHREAD_H
#define CLASSICPRINTQML_CLASSICPRINTTHREAD_H

#include <QtCore>

#include "ClassicPrint.h"

class ClassicPrintThread : public QThread {
    public:
        ClassicPrintThread(ClassicPrint *classicPrint,
                QString sourceFilename,
                QString destinationFilename,
                QObject *parent=NULL)
            : QThread(parent),
              m_classicPrint(classicPrint),
              m_sourceFilename(sourceFilename),
              m_destinationFilename(destinationFilename)
        {
        }

        void run() {
            QImage source(m_sourceFilename);
            QImage destination(source.size(), source.format());
            m_classicPrint->process(source, 0, 0, destination);
            destination.save(m_destinationFilename);
        }

    private:
        ClassicPrint *m_classicPrint;
        QString m_sourceFilename;
        QString m_destinationFilename;
};

#endif
