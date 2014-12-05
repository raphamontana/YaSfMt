#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>

#include "../utils/SfMStatus.h"
#include "calibration/CameraCalibration.h"
#include "FeatureExtractor.h"
#include "FeatureMatcher.h"
#include "FundamentalMatrix.h"
#include "MotionDetector.h"
#include "Triangulation.h"

using namespace cv;

class Vision
{
public:

    /**
     * Sets the class initial parameters and read the camera calibration settings.
     */
    Vision();

    /**
     * Deallocate CameraCalibration.
     */
    ~Vision();

    /**
     * Calibrate the camera.
     */
    void calibrateCamera();

    /**
     * \return true if frame was successfully processed.
     */
    SfMStatus processStructureFromMotion( Mat &frame );

private:

    /**
     * \return true, if none frame was processed.
     */
    bool isFirstFrame();

    /**
     * Sets the first frame.
     */
    void setFirstFrame( bool );

    /**
     * The process need a starter frame.
     */
    void setLastFrame( Mat frame );

    /**
     * Check if there was moviment on the scene.
     */
    SfMStatus detectMotion( Mat &frame );

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
    MotionDetector motionDetector;

    vector<DMatch> good_matches;

    Mat fundamentalMatrix;
    Mat essentialMatrix;

    Mat K; /// Camera Matrix.
    Mat_<double> Kinv;

    bool firstFrame;

};

#endif // VISION_H
