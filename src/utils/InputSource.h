/**
 * \file InputSource.h
 *
 * \author Raphael Montanari
 * \date 23/05/2013
 *
 * Enum InputSource header file.
 */

#ifndef INPUT_SOURCE_H
#define INPUT_SOURCE_H

/**
 * \enum InputSource
 * \brief The source video input.
 **/
enum class InputSource {
    NONE,    /**< Not capturing images. */
    CAMERA,  /**< Capturing images from a camera. */
    FILE,    /**< Capturing images from a video file. */
    ROS      /**< Capturing images from a ROS Node. */
};

#endif /* INPUT_SOURCE_H */
