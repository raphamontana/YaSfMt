/**
 * \file SystemStatus.h
 *
 * \author Raphael Montanari
 * \date 23/05/2013
 *
 * Enum SystemStatus header file.
 */

#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H

/**
 * \enum SystemStatus
 * \brief Possible statuses of the system.
 **/
enum class SystemStatus {
    INITIATING, /**< Initial state.                             */
    FETCHING,   /**< Waiting for the user to signal the system. */
    DECODING,   /**< Processes the received signal.             */
    EXECUTING,  /**< Processing images.                         */
    HALTED      /**< Execution finished.                        */
};

#endif /* SYSTEM_STATUS_H */
