/**
 * \file Core.h
 *
 * \author Raphael Montanari
 * \date 01/05/2013
 *
 * Core header file.
 */

#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <chrono>
#include <thread>

#include "../gui/Gui.h"
#include "../utils/SignalType.h"
#include "../utils/SystemStatus.h"
#include "../video/Video.h"
#include "../vision/Vision.h"

using namespace std;

/**
 * \class Core
 * \brief Major system class.
 *
 * The Core manages the communication with all modules of the system.
 */
class Core
{

public:

    /**
     * \brief The class constructor.
     *
     * Sets initial system parameters and init the GUI.
     */
    explicit Core();

    /**
     * \brief Start system modules.
     *
     * Execute the main loop routine.
     */
    void exec();

private:

    /**
     * Initialize the system.
     */
    void init();

    /**
     * Fetch a signal.
     */
    void fetch();

    /**
     * Decode the fetched signal.
     */
    void decode();

    /**
     * Does the data acquisition, processing and publication of information.
     */
    void execute();

    /**
     * Halt the system.
     */
    void halt();

    Gui gui;
    long long int currentFrame;
    bool firstFrame;
    SystemStatus systemStatus;
    Video video;
    Vision vision;

};

#endif /* CORE_H */
