#ifndef CLASSICPRINTQML_CLASSICPRINTDECLARATIVE_H
#define CLASSICPRINTQML_CLASSICPRINTDECLARATIVE_H

#include <QtCore>
#include <QtGui>
#include <QtDeclarative>

#include "ClassicPrint.h"
#include "ClassicPrintProcessing.h"

class ClassicPrintDeclarative : public QObject {
    Q_OBJECT

    public:
        ClassicPrintDeclarative(QObject *parent=NULL)
            : QObject(parent),
              m_sequence(0),
              m_timer()
        {
            m_timer.setInterval(500);
            m_timer.setSingleShot(true);

            QObject::connect(&m_timer, SIGNAL(timeout()),
                    this, SIGNAL(sequenceChanged()));

            QObject::connect(this, SIGNAL(contrastChanged()),
                    this, SLOT(contentUpdated()));

            QObject::connect(this, SIGNAL(colourisationChanged()),
                    this, SLOT(contentUpdated()));

            QObject::connect(this, SIGNAL(frameSizeChanged()),
                    this, SLOT(contentUpdated()));

            QObject::connect(this, SIGNAL(lightLeakChanged()),
                    this, SLOT(contentUpdated()));
        }

        static void init() {
            classicPrint = new ClassicPrint;
            classicPrint->load(":/classicPrintData/settings.xml");
            qmlRegisterType<ClassicPrintDeclarative>("org.maemo.classicprint",
                    1, 0, "ClassicPrint");
        }

        static ClassicPrint *getClassicPrint() {
            return classicPrint;
        }

        qreal contrast() {
            return getClassicPrint()->getCurrentProcessing()->contrast()/100.;
        }

        void setContrast(qreal contrast) {
            getClassicPrint()->getCurrentProcessing()->setContrast(contrast*100.);
            emit contrastChanged();
        }

        Q_PROPERTY(qreal contrast
                READ contrast
                WRITE setContrast
                NOTIFY contrastChanged)

        qreal colourisation() {
            return getClassicPrint()->getCurrentProcessing()->colourisationPercent()/100.;
        }

        void setColourisation(qreal colourisation) {
            getClassicPrint()->getCurrentProcessing()->setColourisationPercent(colourisation*100.);
            emit colourisationChanged();
        }

        Q_PROPERTY(qreal colourisation
                READ colourisation
                WRITE setColourisation
                NOTIFY colourisationChanged)

        qreal frameSize() {
            return getClassicPrint()->getCurrentProcessing()->frameSizePercent()/15.;
        }

        void setFrameSize(qreal frameSize) {
            getClassicPrint()->getCurrentProcessing()->setFrameSizePercent(frameSize*15.);
            emit frameSizeChanged();
        }

        Q_PROPERTY(qreal frameSize
                READ frameSize
                WRITE setFrameSize
                NOTIFY frameSizeChanged)

        int lightLeak() {
            QString value = getClassicPrint()->getCurrentProcessing()->lightLeak();
            if (value == "Light Leak 1.jpg") {
                return 1;
            } else if (value == "Light Leak 3.jpg") {
                return 2;
            } else if (value == "Random") {
                return -1;
            } else {
                return 0;
            }
        }

        void setLightLeak(int lightLeak) {
            if (this->lightLeak() != lightLeak) {
                QString value;
                switch (lightLeak) {
                    case 1:
                        value = "Light Leak 1.jpg";
                        break;
                    case 2:
                        value = "Light Leak 3.jpg";
                        break;
                    case -1:
                        value = "Random";
                        break;
                    default:
                        value = "";
                        break;
                }

                getClassicPrint()->getCurrentProcessing()->setLightLeak(value);
                emit lightLeakChanged();
            }
        }

        Q_PROPERTY(int lightLeak
                READ lightLeak
                WRITE setLightLeak
                NOTIFY lightLeakChanged)


        int sequence() { return m_sequence; }
        Q_PROPERTY(int sequence READ sequence NOTIFY sequenceChanged)

    signals:
        void contrastChanged();
        void colourisationChanged();
        void frameSizeChanged();
        void lightLeakChanged();

        void sequenceChanged();

    public slots:
        void contentUpdated() {
            m_sequence++;
            // wait a bit to avoid too many updates
            m_timer.start();
        }

    private:
        static ClassicPrint *classicPrint;

        int m_sequence;
        QTimer m_timer;
};

#endif
