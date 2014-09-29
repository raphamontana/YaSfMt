#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>

#include "calibration/CameraCalibration.h"
#include "FeatureExtractor.h"
#include "FeatureMatcher.h"
#include "FundamentalMatrix.h"
#include "Triangulation.h"

using namespace cv;

class Vision
{
public:

    /**
     *
     */
    Vision();

    /**
     * Deallocate CameraCalibration.
     */
    ~Vision();

    /**
     *
     */
    void setInitialFrame( Mat frame );

    /**
     * \brief Given a pair of images, calculate the motion map.
     * \return Number of features matched.
     */
    int getMotionMap( Mat &frame );

    /**
     *
     */
    void estimateMotion();

    /**
     *
     */
    void reconstructionTriangulation();

    /**
     * Calibrate the camera.
     */
    void calibrateCamera();

private:

    CameraCalibration *cameraCalibration;
    Mat lastFrame;
    Mat currentFrameDescriptors;
    Mat lastFrameDescriptors;
    vector<KeyPoint> currentFrameKeypoints;
    vector<KeyPoint> lastFrameKeypoints;
    FeatureExtractor featureExtractor;
    FeatureMatcher featureMatcher;
    //FundamentalMatrix fundamentalMatrix;
    Triangulation triang;

    vector<DMatch> good_matches;

    Mat fundamentalMatrix;
    Mat essentialMatrix;

    Mat K; /// Camera Matrix.
    Mat_<double> Kinv;

};

#endif // VISION_H
