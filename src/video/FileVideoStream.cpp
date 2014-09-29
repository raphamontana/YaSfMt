/**
 * \file FileVideoInput.cpp
 *
 * \author Raphael Montanari
 * \date 30/05/2013
 *
 * FileVideoInput source file.
 */

#include "FileVideoStream.h"

FileVideoStream::FileVideoStream( string fileName )
{
    cap = VideoCapture();
    cap.open( fileName );
    cap.set( CV_CAP_PROP_POS_AVI_RATIO, 1 );
    numberOfFrames = cap.get( CV_CAP_PROP_POS_FRAMES );
}

FileVideoStream::~FileVideoStream() {
}

bool FileVideoStream::isOpened() {
    return (cap.isOpened());
}

Mat FileVideoStream::getFrame() {
    this_thread::sleep_for( chrono::milliseconds( 32 ) );
    Mat frame;
    cap >> frame;
    if ( frame.empty() ) {
        // Rewind video.
        cap.set( CV_CAP_PROP_POS_FRAMES, 0 );
    }
    return frame;
}

int FileVideoStream::getFPS() {
    return ( cap.get( CV_CAP_PROP_FPS ) );
}

void FileVideoStream::close() {

}
