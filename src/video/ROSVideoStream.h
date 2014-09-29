/**
 * \file ROSVideoStream.h
 *
 * \author Raphael Montanari
 * \date 29/05/2013
 *
 * ROSVideoStream input description.
 */

#ifndef ROSVIDEOSTREAM_H
#define ROSVIDEOSTREAM_H


#include <condition_variable>
#include <mutex>
#include <thread>
//#include <ros/ros.h>
#include "VideoStream.h"

using namespace std;

class ROSVideoStream : public VideoStream
{
public:

    /**
     * Subscribe to a ROS node.
     */
    explicit ROSVideoStream( string node );

    ~ROSVideoStream();

    Mat getFrame();

    int getFPS();

    bool isOpened();

    bool isRunning();

    void close();

};

#endif // ROSVIDEOSTREAM_H
