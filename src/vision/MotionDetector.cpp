#include "MotionDetector.h"

MotionDetector::MotionDetector( int thereIsMotion, int maxDeviation )
    : thereIsMotion( thereIsMotion ),
      maxDeviation( maxDeviation )
{
    //this->thereIsMotion = 15;
    this->maxDeviation = 50;
    kernelEro = getStructuringElement( MORPH_RECT, Size( 2, 2 ) );
}


bool MotionDetector::detect( Mat prev_frame, Mat next_frame )
{
    // Motion for calculating the differences
    Mat motion;

    // Take images and convert them to gray
    cvtColor( next_frame, next_frame, COLOR_RGB2GRAY );

    // Calc differences between the images and do threshold image, low differences are ignored (ex. contrast change due to sunlight)
    absdiff( prev_frame, next_frame, motion );
    threshold( motion, motion, 35, 255, THRESH_BINARY );
    erode( motion, motion, kernelEro );

    // calculate the standard deviation
    Scalar mean, stddev;
    meanStdDev( motion, mean, stddev );

    // if too much changes then the motion is real.
    return( stddev[0] > maxDeviation );

    /*
    // number_of_changes, the amount of changes in the result matrix.
    int number_of_changes = detectMotion( motion );

    // If a lot of changes happened, we assume something changed.
    return( number_of_changes >= thereIsMotion );
    */
}


int MotionDetector::detectMotion( const Mat & motion )
{
    // Detect motion in window
    int x_start = 0, x_stop = motion.cols - 1;
    int y_start = 0, y_stop = motion.rows - 1;

    // calculate the standard deviation
    Scalar mean, stddev;
    meanStdDev( motion, mean, stddev );
    // if not too much changes then the motion is real.
    if ( stddev[0] < maxDeviation ) {
        int number_of_changes = 0;
        // loop over image and detect changes
        for ( int j = y_start; j < y_stop; j += 2 ) { // height
            for ( int i = x_start; i < x_stop; i += 2 ) { // width
                // check if at pixel (j,i) intensity is equal to 255
                // this means that the pixel is different in the sequence
                // of images (prev_frame, current_frame, next_frame)
                if ( static_cast<int>( motion.at<uchar>( j, i ) ) == 255 ) {
                    number_of_changes++;
                }
            }
        }
        cout << number_of_changes << endl;
        return( number_of_changes );
    }
    return( 0 );
}
