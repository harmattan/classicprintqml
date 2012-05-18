TEMPLATE = app
#CONFIG -= moc
INCLUDEPATH += .
QT += core

include(../../src/qtimagefilters.pri)

# Input
SOURCES += main.cpp imagetoolwidget.cpp
HEADERS += imagetoolwidget.h mirrorfilter.h

FORMS   += imagetoolwidget.ui
