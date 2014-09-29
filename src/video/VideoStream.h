/**
 * \file VideoStream.h
 *
 * \author Raphael Montanari
 * \date 20/01/2013
 *
 * VideoStream input description.
 */

#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class VideoStream {
public:

    /**
     * \brief Default destructor.
     */
    virtual ~VideoStream() {}

    /**
     * \brief Return the current frame.
     * \return A frame.
     */
    virtual Mat getFrame() = 0;

    /**
     * \brief Return the input count of frames per second.
     * \return The framerate.
     */
    virtual int getFPS() = 0;

    /**
     * \brief Check if succeeded with the input opening.
     * \return true if input is open. false otherwise.
     */
    virtual bool isOpened() = 0;

    /**
     * \brief Close the input.
     */
    virtual void close() = 0;

};

#endif /* VIDEO_STREAM_H */
