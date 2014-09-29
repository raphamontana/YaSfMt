#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct CloudPoint {
    cv::Point3d pt;
    std::vector<int> imgpt_for_img;
    double reprojection_error;
};

class Triangulation
{

public:

    Triangulation();

    bool findCameraMatrices( const Mat& K, const Mat& Kinv, const Mat& distcoeff, const Mat& F,
                             const Mat& E, vector<KeyPoint>& imgpts1_good, vector<KeyPoint>& imgpts2_good,
                             Matx34d& P, Matx34d& P1, vector<DMatch>& matches, vector<CloudPoint>& outCloud );
private:

    /**
     * \brief
     * \param u homogenous image point (u,v,1)
     * \param P camera 1 matrix
     * \param u1 homogenous image point in 2nd camera
     * \param P1 camera 2 matrix
     */
    Mat_<double> iterativeLinearLSTriangulation( Point3d u, Matx34d P, Point3d u1, Matx34d P1 );

    /**
     * \brief From "Triangulation", Hartley, R.I. and Sturm, P., Computer vision and image understanding, 1997
     * \param u homogenous image point (u,v,1)
     * \param P camera 1 matrix
     * \param u1 homogenous image point in 2nd camera
     * \param P1 camera 2 matrix
     */
    Mat_<double> linearLSTriangulation( Point3d u, Matx34d P, Point3d u1, Matx34d P1 );

    /**
     * Triagulate points.
     */
    double triangulatePoints( const vector<KeyPoint>& pt_set1, const vector<KeyPoint>& pt_set2,
                              const Mat& K, const Mat& Kinv, const Mat& distcoeff,
                              const Matx34d& P, const Matx34d& P1,
                              vector<CloudPoint>& pointcloud,
                              vector<KeyPoint>& correspImg1Pt );

    bool DecomposeEtoRandT( Mat_<double>& E, Mat_<double>& R1, Mat_<double>& R2, Mat_<double>& t1, Mat_<double>& t2 );

    bool CheckCoherentRotation( Mat_<double>& R );

    bool TestTriangulation(const vector<CloudPoint>& pcloud, const Matx34d& P, vector<uchar>& status );

    vector<Point3d> CloudPointsToPoints( const vector<CloudPoint> cpts );

    void KeyPointsToPoints( const vector<KeyPoint>& kps, vector<Point2f>& ps );
};

#endif // TRIANGULATION_H
