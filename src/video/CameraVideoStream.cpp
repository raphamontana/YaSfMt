/**
 * \file CameraVideoInput.cpp
 *
 * \author Raphael Montanari
 * \date 30/05/2013
 *
 * VideoInput source file.
 */

#include "CameraVideoStream.h"

CameraVideoStream::CameraVideoStream()
{
    // Try to open the standard error file descriptor.
    int oldStdErr = dup( fileno( stderr ) );
    if ( oldStdErr != -1 ) {
        // Ignores the VIDIOC_QUERYMENU invalid argument warning.
        dup2( (int) NULL, fileno( stderr ) );
        cap = VideoCapture( 0 );
        dup2( oldStdErr, fileno( stderr ) );
    }
    else {
        cap = VideoCapture( 0 );

    }
}

CameraVideoStream::~CameraVideoStream() {
    cap.release();
}

Mat CameraVideoStream::getFrame() {
    Mat frame;
    cap >> frame;
    return frame;
}

int CameraVideoStream::getFPS() {
    return( cap.get( CV_CAP_PROP_FPS ) );
}

bool CameraVideoStream::isOpened() {
    return( cap.isOpened() );
}

void CameraVideoStream::close() {

}
