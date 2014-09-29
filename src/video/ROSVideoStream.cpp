/**
 * \file ROSVideoInput.cpp
 *
 * \author Raphael Montanari
 * \date 30/05/2013
 *
 * VideoInput source file.
 */

#include "ROSVideoStream.h"

ROSVideoStream::ROSVideoStream( string node )
{
    cout << node << endl;
}

ROSVideoStream::~ROSVideoStream() {
}

Mat ROSVideoStream::getFrame() {
    Mat frame;
    return( frame );
}

int ROSVideoStream::getFPS() {
    return( 0 );
}

bool ROSVideoStream::isOpened() {
    return( false );
}

void ROSVideoStream::close() {
}
