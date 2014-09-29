/**
 * \file CameraVideoStream.h
 *
 * \author Raphael Montanari
 * \date 29/05/2013
 *
 * CameraVideoStream input description.
 */

#ifndef CAMERAVIDEOSTREAM_H
#define CAMERAVIDEOSTREAM_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>

#include "VideoStream.h"

using namespace std;

class CameraVideoStream : public VideoStream
{
public:

    /**
     * Open the default camera.
     */
    explicit CameraVideoStream();

    ~CameraVideoStream();

    Mat getFrame();

    int getFPS();

    bool isOpened();

    bool isRunning();

    void close();

private:

    VideoCapture cap;

};

#endif // CAMERAVIDEOSTREAM_H
