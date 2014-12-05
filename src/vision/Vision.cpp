#include "Vision.h"

Vision::Vision()
{
    firstFrame = true;
    cameraCalibration = new CameraCalibration();
    cameraCalibration->readCameraInfo();
}


Vision::~Vision()
{
    delete( cameraCalibration );
}


bool Vision::isFirstFrame()
{
    return( firstFrame );
}


void Vision::setFirstFrame( bool first )
{
    firstFrame = first;
}


void Vision::setLastFrame( Mat frame )
{
    cvtColor( frame, lastFrame, COLOR_RGB2GRAY );
    featureExtractor.extractORB( lastFrame, &lastFrameDescriptors, &lastFrameKeypoints );
    lastFrameDescriptors.convertTo( lastFrameDescriptors, CV_32F );
}


SfMStatus Vision::detectMotion( Mat &frame )
{
    if ( !isFirstFrame() ) {
        return( motionDetector.detect( lastFrame, frame ) ? SfMStatus::MAPPING : SfMStatus::NOT_ENOUGH_MOTION );
    }
    else {
        setLastFrame( frame );
        setFirstFrame( false );
        return( SfMStatus::INITIATING );
    }
}


int Vision::getMotionMap( Mat &frame )
{
    Mat currentFrame;

    cvtColor( frame, currentFrame, COLOR_RGB2GRAY );
    featureExtractor.extractORB( currentFrame, &currentFrameDescriptors, &currentFrameKeypoints );
    // Convert the descriptors to the FLANN matcher format.
    currentFrameDescriptors.convertTo( currentFrameDescriptors, CV_32F );

    double max_dist, min_dist;
    vector<DMatch> matches = featureMatcher.matchDescriptors( lastFrameDescriptors, currentFrameDescriptors );
    featureMatcher.maxMinDistances( matches, &max_dist, &min_dist );

    for ( int i = 0; i < lastFrameDescriptors.rows; i++ ) {
        if ( matches[i].distance <= max( 2 * min_dist, 0.02 ) ) {
            good_matches.push_back( matches[i] );
        }
    }

    //-- Draw only "good" matches
    //Mat img_matches;
    //drawMatches( lastFrame, lastFrameKeypoints, currentFrame, currentFrameKeypoints, good_matches,
    //        img_matches, Scalar::all( -1 ), Scalar::all( -1 ), vector<char> (),
    //        DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //featureMatcher.drawGoodMatches( frame, frame, lastFrameDescriptors, descriptors, lastFrameKeypoints, keypoints, matches );
    //gui.setPointCloudView( img_matches );

    drawKeypoints( frame, currentFrameKeypoints, frame, Scalar(255, 0, 0), DrawMatchesFlags::DEFAULT ); // Blue keypoints.

    return( good_matches.size() );
}

void Vision::estimateMotion()
{
    // Camera matrix.
    Mat cameraMatrix = cameraCalibration->getCameraMatrix();
#ifdef _DEBUG
    cout << "Camera matrix" << endl << cameraMatrix << endl;
#endif

    vector<int> pointIndexesLeft, pointIndexesRight;
    for ( vector<DMatch>::const_iterator it = good_matches.begin(); it != good_matches.end(); ++it ) {
        // Get the indexes of the selected matched keypoints
        pointIndexesLeft.push_back( it->queryIdx );
        pointIndexesRight.push_back( it->trainIdx );
    }

    // Convert keypoints into Point2f
    vector<Point2f> selPointsLeft, selPointsRight;
    KeyPoint::convert( lastFrameKeypoints, selPointsLeft,  pointIndexesLeft );
    KeyPoint::convert( currentFrameKeypoints, selPointsRight, pointIndexesRight );

    // Compute F matrix from n >= 8 matches
    Mat fundamental = findFundamentalMat(
        Mat( selPointsLeft ),  // points in first image
        Mat( selPointsRight ), // points in second image
        FM_8POINT );        // 8-point method
#ifdef _DEBUG
    cout << "Fundamental matrix" << endl << fundamental << endl;
#endif

    // Essential matrix
    ///TODO verificar o tipo de dados da matriz float ou double
    essentialMatrix = cameraMatrix.t() * fundamental * cameraMatrix;
#ifdef _DEBUG
    cout << "Essential matrix" << endl << essentialMatrix << endl;
#endif

    if ( countNonZero( essentialMatrix ) >= 0 ) {
        // Now let's assume one camera is P = [I|0],
        // meaning it hasn't moved or rotated,
        // getting the second camera matrix,
        // P' = [R|t], is done as follows:
        SVD svd( essentialMatrix );
        Matx33d W( 0,-1, 0,   //HZ 9.13
                   1, 0, 0,
                   0, 0, 1 );
        //Matx33d Winv( 0, 1, 0,
        //             -1, 0, 0,
        //              0, 0, 1);
        Mat R = svd.u * Mat( W ) * svd.vt; //HZ 9.19
        Vec3d t = svd.u.col( 2 ); //u3
        Matx34d P1( R.at<double>(0,0), R.at<double>(0,1), R.at<double>(0,2), t(0),
                    R.at<double>(1,0), R.at<double>(1,1), R.at<double>(1,2), t(1),
                    R.at<double>(2,0), R.at<double>(2,1), R.at<double>(2,2), t(2));

        // CheckCoherentRotation
        bool coherentRotation;
        if ( fabsf( determinant( R ) ) - 1.0 > 1e-07 ) {
            cerr << "det(R) != +-1.0, this is not a rotation matrix" << endl;
            coherentRotation = false;
        }
        else {
            coherentRotation = true;
        }
        // CheckCoherentRotation

        if ( !coherentRotation ) {
            cout << "resulting rotation is not coherent\n";
        }

#ifdef _DEBUG
        cout << "Rotation matrix" << endl << R << endl;
        cout << "Translation matrix" << endl << t << endl;
#endif

        //-- Step 7: Reprojection Matrix and rectification data
        Mat R1, R2, P1_, P2_, Q;
        Rect validRoi[2];
        double dist[] = { -0.03432, 0.05332, -0.00347, 0.00106, 0.00000};
        Mat D(1, 5, CV_64F, dist);
        //stereoRectify( K, D, K, D, lastFrame.size(), R, t, R1, R2, P1_, P2_, Q, CV_CALIB_ZERO_DISPARITY, 1, lastFrame.size(),  &validRoi[0], &validRoi[1] );
    }

    /*
    unsigned int pts_size = good_matches.size();
    for ( unsigned int i = 0; i < pts_size; i++ )
        Point2f kp = good_matches[i];
        Point3d u( kp.x, kp.y, 1.0 );
        Mat_ um = Kinv * Mat_( u );
        u = um.at(0);

        Point2f kp1 = good_matches[i];
        Point3d u1( kp1.x, kp1.y, 1.0 );
        Mat_ um1 = Kinv * Mat_( u1 );
        u1 = um1.at(0);

        Mat_ X = IterativeLinearLSTriangulation( u, cameraMatrix, u1, cameraMatrix );
    */

}

void Vision::reconstructionTriangulation()
{
    const Mat& K = cameraCalibration->getCameraMatrix();
    const Mat& Kinv = K.inv();
    const Mat& distcoeff = cameraCalibration->getDistortionCoefficients();
    vector<KeyPoint> imgpts1 = currentFrameKeypoints;
    vector<KeyPoint> imgpts2 = lastFrameKeypoints;
    Matx34d P(1,0,0,0,
              0,1,0,0,
              0,0,1,0);
    Matx34d P1(1,0,0,0,
              0,1,0,0,
              0,0,1,0);
    vector<DMatch> matches = good_matches;
    vector<CloudPoint> outCloud;

    triang.findCameraMatrices( K, Kinv, distcoeff, fundamentalMatrix, essentialMatrix, imgpts1, imgpts2, P, P1, matches, outCloud );
}

void Vision::calibrateCamera()
{
    cameraCalibration->calibrate();
}


SfMStatus Vision::processStructureFromMotion( Mat &frame )
{
    // Processing()
    // -> Calibrate camera
    // -> Extrai características locais.
    // -> Faz correspondência entre os pontos.
    // -> Find fundamental matrix based on undistorted feature points.
    // -> Get essential matrix from fundamental matrix.
    // -> Compute projection matrix from cam1 to cam2
    // -> Triangulate
    // -> Calcula posição da câmera e dos pontos.
    // ---> Algoritmo dos 8 pontos.
    // ---> Matriz F.
    // ---> cv::LevMarqSparse lms;
    // ---> Triangulação

    int matches = 0;
    bool triangulou = false;
    switch ( detectMotion( frame ) ) {
        case SfMStatus::MAPPING: // Moviment detected.
            // Roda uma iteraçao do algoritmo
            matches = getMotionMap( frame );
            if ( matches >= 8 ) {
                estimateMotion();
                reconstructionTriangulation();
                triangulou = true;
            }
            if ( triangulou ) {
                setLastFrame( frame );
                return( SfMStatus::MAPPING );
            }
            else {
                return( SfMStatus::LOST_TRACKING );
            }
        case SfMStatus::NOT_ENOUGH_MOTION:
            return( SfMStatus::NOT_ENOUGH_MOTION );
        case SfMStatus::INITIATING:
            return( SfMStatus::INITIATING );
        default:
            return( SfMStatus::ERROR );
    }
}
