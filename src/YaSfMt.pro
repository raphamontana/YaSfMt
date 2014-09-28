QT       += core gui opengl

greaterThan( QT_MAJOR_VERSION, 4 ) : QT += widgets

TARGET    = YaSfMt
TEMPLATE  = app

QMAKE_CXXFLAGS += -std=c++0x

CONFIG(release, release|debug):DEFINES += _DEBUG
#CONFIG += qt warn_on debug staticlib
#CONFIG += link_pkgconfig
#PKGCONFIG += opencv

SOURCES += src/main.cpp \
        src/core/Core.cpp \
        src/gui/CVWidget.cpp \
        src/gui/Gui.cpp \
        src/gui/MainWindow.cpp \
        src/pcnn/Legion.cpp \
        src/pcnn/Principal.cpp \
        src/video/CameraVideoStream.cpp \
        src/video/FileVideoStream.cpp \
        src/video/ROSVideoStream.cpp \
        src/video/Video.cpp \
#        src/viewer/pclviewer.cpp \
        src/vision/calibration/CameraCalibration.cpp \
        src/vision/calibration/Settings.cpp \
        src/vision/FeatureExtractor.cpp \
        src/vision/FeatureMatcher.cpp \
        src/vision/FundamentalMatrix.cpp \
        src/vision/Vision.cpp \
        src/vision/Triangulation.cpp

HEADERS += src/core/Core.h \
        src/gui/CVWidget.h \
        src/gui/Gui.h \
        src/gui/MainWindow.h \
        src/pcnn/Legion.h \
        src/pcnn/Principal.h \
        src/utils/InputSource.h \
        src/utils/SignalType.h \
        src/utils/SystemStatus.h \
        src/video/CameraVideoStream.h \
        src/video/FileVideoStream.h \
        src/video/ROSVideoStream.h \
        src/video/VideoStream.h \
        src/video/Video.h \
#        src/viewer/pclviewer.h \
        src/vision/calibration/CameraCalibration.h \
        src/vision/calibration/Settings.h \
        src/vision/FeatureExtractor.h \
        src/vision/FeatureMatcher.h \
        src/vision/FundamentalMatrix.h \
        src/vision/Vision.h \
        src/vision/Triangulation.h

#FORMS    += src/viewer/pclviewer.ui

RESOURCES = resources.qrc

PKGCONFIG += opencv pcl_common-1.6 pcl_features-1.6 pcl_filters-1.6 pcl_geometry-1.6 pcl_io-1.6 pcl_kdtree-1.6 pcl_keypoints-1.6 pcl_octree-1.6 pcl_registration-1.6 pcl_sample_consensus-1.6 pcl_search-1.6 pcl_segmentation-1.6 pcl_surface-1.6 pcl_tracking-1.6 pcl_visualization-1.6 flann
#PKGCONFIG += opencv pcl_apps-1.7 pcl_common-1.7 pcl_features-1.7 pcl_filters-1.7 pcl_geometry-1.7 pcl_io-1.7 pcl_kdtree-1.7 pcl_keypoints-1.7 pcl_octree-1.7 pcl_registration-1.7 pcl_sample_consensus-1.7 pcl_search-1.7 pcl_segmentation-1.7 pcl_surface-1.7 pcl_tracking-1.7 pcl_visualization-1.7 flann

LIBS    += `pkg-config opencv --cflags --libs` -lGL -lGLU -lm -lpthread -L/usr/local/lib -lGL -lGLU -lm

#INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/include \
               /usr/include/eigen3 \
               /usr/include/pcl-1.6 \
               /usr/include/pcl-1.7 \
               /usr/include/vtk-5.8 \
               /usr/include/flann
