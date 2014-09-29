QT       += core gui opengl

greaterThan( QT_MAJOR_VERSION, 4 ) : QT += widgets

TARGET    = YaSfMt
TEMPLATE  = app

SOURCES += main.cpp \
        core/Core.cpp \
        gui/CVWidget.cpp \
        gui/Gui.cpp \
        gui/MainWindow.cpp \
        pcnn/Legion.cpp \
        pcnn/Principal.cpp \
        video/CameraVideoStream.cpp \
        video/FileVideoStream.cpp \
        video/ROSVideoStream.cpp \
        video/Video.cpp \
#        viewer/pclviewer.cpp \
        vision/calibration/CameraCalibration.cpp \
        vision/calibration/Settings.cpp \
        vision/FeatureExtractor.cpp \
        vision/FeatureMatcher.cpp \
        vision/FundamentalMatrix.cpp \
        vision/Vision.cpp \
        vision/Triangulation.cpp

HEADERS += core/Core.h \
        gui/CVWidget.h \
        gui/Gui.h \
        gui/MainWindow.h \
        pcnn/Legion.h \
        pcnn/Principal.h \
        utils/InputSource.h \
        utils/SignalType.h \
        utils/SystemStatus.h \
        video/CameraVideoStream.h \
        video/FileVideoStream.h \
        video/ROSVideoStream.h \
        video/VideoStream.h \
        video/Video.h \
#        viewer/pclviewer.h \
        vision/calibration/CameraCalibration.h \
        vision/calibration/Settings.h \
        vision/FeatureExtractor.h \
        vision/FeatureMatcher.h \
        vision/FundamentalMatrix.h \
        vision/Vision.h \
        vision/Triangulation.h

RESOURCES   += resources.qrc

OTHER_FILES += CMakeLists.txt
