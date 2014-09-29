/**
 * \file Gui.cpp
 *
 * \author Raphael Montanari
 * \date 19/06/2013
 *
 * Graphical User Interface starter.
 */

#ifndef GUI_THREAD_H
#define GUI_THREAD_H

#include <atomic>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include "MainWindow.h"

/**
 * @brief The Gui starter class.
 */
class Gui
{

public:

    /**
     * \brief Gui starter constructor.
     */
    Gui();

    /**
     * \brief Start the thread.
     */
    void start();

    /**
     * \brief Gets the emitted signal.
     * \return The signal.
     */
    SignalType getSignal();

    /**
     * \brief Test if a signal was emmited.
     * \return true if a signal was emmited, false otherwise.
     */
    bool gotSignal();

    /**
     * \brief Stop processing until signal is emmited.
     */
    void waitSignal();

    /**
     * \brief Reset capture menu actions.
     */
    void captureFailed();

    /**
     * \brief Gets the input stream.
     * \return The input stream.
     */
    InputSource getInputSource();

    /**
     * \brief Gets the filename.
     * \return The filename.
     */
    string getFilename();

    /**
     * \brief Sets a status message.
     * \param message String message to show.
     */
    void setStatusMessage( string message );

    /**
     * \brief Increases the number processed frames.
     */
    void incrementFramesCounter();

    /**
     * \brief
     * \param frame Frame to show.
     */
    void setCameraView( Mat frame );

    /**
     * \brief
     * \param frame Frame to show.
     */
    void setPointCloudView( Mat frame );

    /**
     * \brief
     * \param frame Frame to show.
     */
    void setLatestFrameView( Mat frame );

    /**
     * \brief Stop the thread.
     */
    void stop();

private:

    /**
     * \brief Check if the thread is running.
     * \return True if the thread is active. False otherwise.
     */
    bool isRunning();

    /**
     * \brief Run the events loop.
     */
    void run();

    /**
     * The instance of the main window.
     */
    MainWindow *mainWindow;

    atomic<bool> readySignal;
    bool ready;
    condition_variable cv;
    mutex m;
    thread t;

};

#endif // GUI_THREAD_H
