#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <dirent.h>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

class MotionDetector
{
public:

    //MotionDetector();
    /**
     * Constructor.
     */
    explicit MotionDetector( int thereIsMotion = 5, int maxDeviation = 20 );

    /**
     * \param prev_frame, first image to compare.
     * \param next_frame, second image to compare.
     * \return true if detected changes in the images, false otherwise.
     */
    bool detect( Mat prev_frame, Mat next_frame );

private:

    /**
     * Check if there is motion in the result matrix.
     * \see <a href="http://blog.cedric.ws/opencv-simple-motion-detection">OpenCV Simple Motion Detection </a>
     * \author Cédric Verstraeten.
     * \return The number of changes.
     */
    int detectMotion( const Mat & motion );

    /**
     * If more than 'there_is_motion' pixels are changed, we say there is motion.
     */
    int thereIsMotion;

    /**
     * Maximum deviation of the image, the higher the value, the more motion is allowed
     */
    int maxDeviation;

    /**
     * Erode kernel
     */
    Mat kernelEro;

};

#endif // MOTIONDETECTOR_H
