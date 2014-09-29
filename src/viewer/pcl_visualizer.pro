QT       += core gui

greaterThan( QT_MAJOR_VERSION, 4 ) : QT += widgets

TARGET    = pcl_visualizer
TEMPLATE  = app

SOURCES  += main.cpp\
            pclviewer.cpp

HEADERS  += pclviewer.h

FORMS    += pclviewer.ui
