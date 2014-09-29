/**
 * \file FileVideoStream.h
 *
 * \author Raphael Montanari
 * \date 29/05/2013
 *
 * FileVideoStream input description.
 */

#ifndef FILEVIDEOSTREAM_H
#define FILEVIDEOSTREAM_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include "VideoStream.h"

using namespace std;

class FileVideoStream : public VideoStream
{
public:

    /**
     * Open a file.
     */
    explicit FileVideoStream( string fileName );

    ~FileVideoStream();

    Mat getFrame();

    int getFPS();

    bool isOpened();

    bool isRunning();

    void close();

    int numberOfFrames;

private:

    VideoCapture cap;

};

#endif // FILEVIDEOSTREAM_H
