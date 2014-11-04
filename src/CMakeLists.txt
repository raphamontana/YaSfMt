cmake_minimum_required( VERSION 2.8.11 FATAL_ERROR )

# ======================================================
# Project YetAnotherStructureFromMotionToolkit
# ======================================================
project( YaSfMt )

# ======================================================
# Add sources directory
# ======================================================
add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( pcnn )
add_subdirectory( utils )
add_subdirectory( video )
add_subdirectory( vision )

set( SRC_LIST main.cpp ${CORE_SRC_LIST} ${GUI_SRC_LIST} ${PCNN_SRC_LIST} ${UTILS_SRC_LIST} ${VIDEO_SRC_LIST} ${VISION_SRC_LIST} )

# ======================================================
#  Version variables:
# ======================================================
set( YASFMT_VERSION_MAJOR 0 )
set( YASFMT_VERSION_MINOR 1 )
set( YASFMT_VERSION_REVISION 1 )
set( CMAKE_BUILD_TYPE DEBUG )
set( CMAKE_INCLUDE_CURRENT_DIR ON )    # Find includes in corresponding build directories
set( CMAKE_AUTOMOC ON )                # Instruct CMake to run moc automatically when needed.

# ======================================================
# Use the package PkgConfig to detect headers/library files
# ======================================================
find_package( PkgConfig REQUIRED )

# ======================================================
# Add Qt 5 library
# ======================================================
find_package( Qt5Widgets REQUIRED )
find_package( Qt5OpenGL REQUIRED )
qt5_add_resources( RSC_LIST resources.qrc )
set( QT_USE_QTOPENGL TRUE )
set( LIBRARIES ${LIBRARIES} Qt5::Widgets Qt5::OpenGL )

# ======================================================
# Add OpenGL library
# ======================================================
find_package( OpenGL REQUIRED )
set( LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES} )

# ======================================================
# Add OpenCV library
# ======================================================
find_package( OpenCV 2.4 REQUIRED )
set( LIBRARIES ${LIBRARIES} ${OpenCV_LIBS} opencv_nonfree opencv_viz )

# ======================================================
# Add VTK library
# ======================================================
find_package( VTK 5.8 REQUIRED NO_MODULE )
include( ${VTK_USE_FILE} )
set( LIBRARIES ${LIBRARIES} ${VTK_LIBRARIES} QVTK )

# ======================================================
# Add PCL library
# ======================================================
find_package( PCL 1.7.1 REQUIRED )
set( LIBRARIES ${LIBRARIES} ${PCL_LIBRARIES} )

# ======================================================
# Compiler flags
# ======================================================
add_definitions( -ansi -std=c++11 -Wall -Wextra )

# ======================================================
# Create the YaSfMt executable
# ======================================================
add_executable( ${PROJECT_NAME} ${SRC_LIST} ${RSC_LIST} )
target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} )

# ======================================================
# Unnecessary libs
# ======================================================
#set( LIBRARIES ${LIBRARIES} Xext X11 ICE SM GL GLU tbb rt pthread m dl )
