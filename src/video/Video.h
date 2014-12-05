/**
 * \file Video.h
 *
 * \author Raphael Montanari
 * \date 20/06/2013
 *
 * Video header file.
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "../gui/Gui.h"
#include "../utils/InputSource.h"
#include "CameraVideoStream.h"
#include "FileVideoStream.h"
#include "ROSVideoStream.h"

using namespace std;

/**
 * @brief The Video starter class.
 */
class Video
{
public:

    /**
     * \brief Gui starter constructor.
     */
    Video( Gui *gui );

    /**
     * \brief Check if the thread is running.
     * \return True if the thread is active. False otherwise.
     */
    bool isRunning();

    /**
     * \brief Start the thread.
     */
    void start();

    /**
     * \brief Thread processing loop.
     */
    void run();

    /**
     * \brief Pause frame capture.
     */
    void pause();

    /**
     * \brief Resume frame capture.
     */
    void resume();

    /**
     * \brief Stop the thread.
     */
    void stop();

    /**
     * \brief Update the latest frame of the video stream and return the frames counter.
     * \return Current frame number.
     */
    long long int getFrame( Mat *image );

    /**
     * \brief Set the input source of the video.
     * \arg source The input source.
     * \arg sourceName The path to the input.
     * \return True if could open the stream. False otherwise.
     */
    bool captureFrom( InputSource source, string sourceName, string *message );

private:

    /**
     * \return True if the image is valid. False otherwise.
     */
    bool isValidFrame( Mat image );

    bool ready;
    condition_variable cv;
    Gui *gui;
    InputSource source;
    long long int frameNumber;
    Mat frame;
    mutex frameMutex;
    mutex readyMutex;
    thread t;
    VideoStream *videoStream;

};

#endif // VIDEO_H
