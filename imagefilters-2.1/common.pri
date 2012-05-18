infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtimagefilters-uselib
TEMPLATE += fakelib
QTIMAGEFILTERS_LIBNAME = $$qtLibraryTarget(QtSolutions_ImageFilters-2.1)
TEMPLATE -= fakelib
QTIMAGEFILTERS_LIBDIR = $$PWD/lib
unix:qtimagefilters-uselib:!qtimagefilters-buildlib:QMAKE_RPATHDIR += $$QTIMAGEFILTERS_LIBDIR
