/**
 * \file Video.cpp
 *
 * \author Raphael Montanari
 * \date 20/06/2013
 *
 * Video source file.
 */

#include "Video.h"

Video::Video( Gui *gui )
{
    frameNumber = 0;
    this->gui = gui;
    ready = false;
}

bool Video::isRunning()
{
    return( t.joinable() );
}

bool Video::captureFrom( InputSource source, string sourceName, string *message )
{
    this->source = source;
    if ( isRunning() ) {
        stop();
    }
    switch ( source ) {
        case InputSource::CAMERA:
            videoStream = new CameraVideoStream();
            if ( videoStream->isOpened() ) {
                *message = "Recording camera.";
            } else {
                *message = "Camera not found.";
                return( false );
            }
            break;
        case InputSource::FILE:
            videoStream = new FileVideoStream( sourceName );
            if ( videoStream->isOpened() ) {
                const char * pch;
                pch = strrchr( sourceName.c_str(), '/' );
                *message = "Opened the video: ";
                message->append( ++pch );
            } else {
                *message = "File not found.";
                return( false );
            }
            break;
        case InputSource::ROS:
            videoStream = new ROSVideoStream( sourceName );
            if ( videoStream->isOpened() ) {
                *message = "Subscribed to ROS node.";
            } else {
                *message = "ROS node not found.";
                return( false );
            }
            break;
        default:
            *message = "Video source was not recognized.";
            return( false );
            break;
    }
    t = thread( &Video::run, this );

    //Stop processing until thread is ready.
    {
        unique_lock<mutex> lock( readyMutex );
        cv.wait( lock, [&](){ return ready; } );
    }
    return( true );
}

void Video::run()
{
    Mat newFrame;
    frame = videoStream->getFrame();
    {
        lock_guard<mutex> lk( readyMutex );
        ready = true;
        cv.notify_one();
    }
    while ( ready ) {
        newFrame = videoStream->getFrame();
        if ( isValidFrame( newFrame ) ) {
            {
                lock_guard<mutex> lock( frameMutex );
                frame = newFrame;
                frameNumber++;
            }
            gui->setCameraView( newFrame.clone() );
        }
        else {
            cout << "Blank frame." << endl;
        }
    }
}

void Video::stop()
{
    if ( isRunning() ) {
        {
            lock_guard<mutex> lock( readyMutex );
            ready = false;
        }
        try {
            videoStream->close();
            t.join();
            #ifdef _DEBUG
            cout << "# Video stopped." << endl;
            #endif
        } catch ( const exception &e ) {
            cerr << "An exception occurred. Exception Nr.: " << e.what() << endl;
        }
        CameraVideoStream * cvs = (CameraVideoStream*) videoStream;
        FileVideoStream * fvs = (FileVideoStream*) videoStream;
        ROSVideoStream * rvs = (ROSVideoStream*) videoStream;
        switch ( source ) {
            case InputSource::CAMERA:
                delete( cvs );
                break;
            case InputSource::FILE:
                delete( fvs );
                break;
            case InputSource::ROS:
                delete( rvs );
                break;
            default:
                break;
        }
    }
}

long long int Video::getFrame( Mat *image )
{
    lock_guard<mutex> lock( frameMutex );
    *image = frame.clone();
    return( frameNumber );
}

bool Video::isValidFrame( Mat image ) {
    return( image.data && image.cols > 0 && image.rows > 0 && !image.empty() );
}
