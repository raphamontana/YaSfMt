# Project
project( YetAnotherStructureFromMotionToolkit )
cmake_minimum_required( VERSION 2.8 )

# Source directory
aux_source_directory( src/ SRC_LIST )

set( YASFMT_VERSION_MAJOR 1 )
set( YASFMT_VERSION_MINOR 0 )
set( CMAKE_BUILD_TYPE RELEASE )
set( CMAKE_INCLUDE_CURRENT_DIR ON )    # Find includes in corresponding build directories
set( CMAKE_AUTOMOC ON )                # Instruct CMake to run moc automatically when needed.

set( CMAKE_PREFIX_PATH "/opt/Qt5.0.2/5.0.2/gcc_64/" )

# Add Qt 5 library
find_package( Qt5 COMPONENTS Widgets REQUIRED )
#set( QT_USE_QTOPENGL TRUE )
#include_directories( ${Qt5Widgets_INCLUDE_DIRS} )
#link_directories( ${Qt5Widgets_LIBRARIES} )

# Add OpenGL library
find_package( OpenGL REQUIRED )
#include_directories( ${OPENGL_INCLUDE_DIR} )
#link_directories( ${OPENGL_LIBRARIES} )

# Add OpenCV 2.4.4 library
find_package( OpenCV 2.4.4 REQUIRED )

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

# Add PCL 1.6 library
#find_package( PCL 1.6 REQUIRED )
#include_directories( ${PCL_INCLUDE_DIRS} )
#link_directories( ${PCL_LIBRARY_DIRS} )
#add_definitions( ${PCL_DEFINITIONS} )

set( CMAKE_CXX_COMPILER "g++-4.8" )

set( CMAKE_CXX_FLAGS "-Wextra -Wall -ansi -std=c++11" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/lib/x86_64-linux-gnu/mesa -lGL -lGLU" )

# Tell CMake to create the YaSfMt executable
add_executable( ${PROJECT_NAME} ${SRC_LIST} )
target_link_libraries( ${PROJECT_NAME}
		${OpenCV_LIBS}
		#${PCL_LIBRARIES}
	)

qt5_use_modules( ${PROJECT_NAME} Widgets )
qt5_use_modules( ${PROJECT_NAME} OpenGL )
