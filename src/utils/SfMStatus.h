/**
 * \file SystemStatus.h
 *
 * \author Raphael Montanari
 * \date 23/05/2013
 *
 * Enum SystemStatus header file.
 */

#ifndef SFMSTATUS_H
#define SFMSTATUS_H

/**
 * \enum SfMStatus
 * \brief Possible statuses of the Structure from Motion proccess.
 **/
enum class SfMStatus {
    INITIATING,        /**< Setting the first frame.                     */
    MAPPING,           /**< Mapping normally.                            */
    NOT_ENOUGH_MOTION, /**< Same frame as last.                          */
    LOST_TRACKING,     /**< Detected moviment but could not triangulate. */
    ERROR
};

#endif // SFMSTATUS_H
