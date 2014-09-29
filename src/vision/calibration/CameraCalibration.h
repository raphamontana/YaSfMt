#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include "Settings.h"

class CameraCalibration
{
public:

    /**
     * \brief Read the camera settings file.
     */
    CameraCalibration();

    /**
     * \brief Run the calibration.
     */
    void calibrate();

    /**
     * \brief Get the camera matrix.
     * \return The camera matrix.
     */
    Mat getCameraMatrix();

    /**
     * \brief Get the camera distortion coeff matrix.
     * \return The camera distortion coeff matrix.
     */
    Mat getDistortionCoefficients();

    /**
     * \brief Read camera parameters from file.
     * \return true if could read the camera matrix and distortion coefficients matrix, else otherwise.
     */
    bool readCameraInfo();

private:

    enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

    /**
     * \brief Read a node of the settings file.
     */
    void read( const FileNode& node, Settings& x, const Settings& default_value = Settings() );

    /**
     * \brief Call the camera calibration method and save the results.
     * \return true if ...; false otherwise.
     */
    bool runCalibrationAndSave( Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs, vector<vector<Point2f> > imagePoints );

    /**
     * \brief Compute reprojection errors.
     * \return the reprojection error.
     */
    double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
            const vector<vector<Point2f> >& imagePoints, const vector<Mat>& rvecs, const vector<Mat>& tvecs,
            const Mat& cameraMatrix , const Mat& distCoeffs, vector<float>& perViewErrors );

    /**
     * \brief Find the corner positions on the chessboard.
     */
    void calcBoardCornerPositions( Size boardSize, float squareSize, vector<Point3f>& corners,
                                         Settings::Pattern patternType );

    /**
     * \brief Do the camera calibration.
     * \return true if succesfully calibrated the camera; false otherwise.
     */
    bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                                vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                                vector<float>& reprojErrs,  double& totalAvgErr);

    /**
     * \brief Print camera parameters to the output file.
     */
    void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                                  const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                  const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                                  double totalAvgErr );

    /**
     * \brief Matrix containing four parameters
     * the camera focal lengths (f_x, f_y) and
     * the optical centers (c_x, c_y) expressed in pixels coordinates.
     */
    //Matx33d
    Mat cameraMatrix;

    /**
     * \brief Distortion coefficients.
     */
    Mat distortionCoefficients;

    /**
     * \brief Camera settings.
     */
    Settings s;

};

#endif // CAMERACALIBRATION_H
