/**
 * \file Signal.h
 *
 * \author Raphael Montanari
 * \date 31/05/2013
 *
 * Signals from the GUI.
 */

#ifndef SIGNAL_TYPE_H
#define SIGNAL_TYPE_H

/**
 * \enum Signal
 * \brief Possible signals of the system.
 **/
enum class SignalType {
    NONE,
    NEW,
    OPEN,
    SAVE,
    CALIBRATE,
    CAPTURE,
    FILE,
    ROS,
    PAUSE,
    RESUME,
    QUIT
};

#endif // SIGNAL_TYPE_H
