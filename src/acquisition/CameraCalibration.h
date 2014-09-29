/**
 * \file CameraCalibration.h
 *
 * \author Raphael Montanari
 * \date 07/08/2013
 *
 * CameraCalibration header file.
 */

#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <iostream>

using namespace std;

/**
 * \class CameraCalibration
 * \brief Manages the camera calibration.
 */
class CameraCalibration
{

public:

    /**
     * \brief The class constructor.
     */
    explicit CameraCalibration();

    void readSettings();

    bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,vector<vector<Point2f> > imagePoints );

private:

    void calcBoardCornerPositions( Size boardSize, float squareSize,
		vector<Point3f>& corners, Settings::Pattern patternType );

    double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
		const vector<vector<Point2f> >& imagePoints,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const Mat& cameraMatrix , const Mat& distCoeffs,
		vector<float>& perViewErrors);

};

#endif /* CAMERACALIBRATION_H */
