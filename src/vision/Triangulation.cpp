#include "Triangulation.h"

Triangulation::Triangulation()
{
}


Mat_<double> Triangulation::linearLSTriangulation( Point3d u, Matx34d P, Point3d u1, Matx34d P1 )
{
    // Build matrix A for homogenous equation system Ax = 0
    // assume X = (x,y,z,1), for Linear-LS method
    // which turns it into a AX = B system, where A is 4x3, X is 3x1 and B is 4x1
    Matx43d A( u.x *  P( 2, 0 ) -  P( 0, 0 ),  u.x *  P( 2, 1 ) -  P( 0, 1 ),  u.x *  P( 2, 2 ) -  P( 0, 2 ),
               u.y *  P( 2, 0 ) -  P( 1, 0 ),  u.y *  P( 2, 1 ) -  P( 1, 1 ),  u.y *  P( 2, 2 ) -  P( 1, 2 ),
              u1.x * P1( 2, 0 ) - P1( 0, 0 ), u1.x * P1( 2, 1 ) - P1( 0, 1 ), u1.x * P1( 2, 2 ) - P1( 0, 2 ),
              u1.y * P1( 2, 0 ) - P1( 1, 0 ), u1.y * P1( 2, 1 ) - P1( 1, 1 ), u1.y * P1( 2, 2 ) - P1( 1, 2 ) );
    Matx41d B( - ( u.x *  P( 2, 3 ) -  P( 0, 3 ) ),
               - ( u.y *  P( 2, 3 ) -  P( 1, 3 ) ),
               - (u1.x * P1( 2, 3 ) - P1( 0, 3 ) ),
               - (u1.y * P1( 2, 3 ) - P1( 1, 3 ) ) );

    Mat_<double> X;
    solve( A, B, X, DECOMP_SVD );
    return( X );
}


Mat_<double> Triangulation::iterativeLinearLSTriangulation( Point3d u, Matx34d P, Point3d u1, Matx34d P1 )
{
    double EPSILON = 0.2;
    double wi = 1, wi1 = 1;
    Mat_<double> X( 4, 1 );
    for ( int i = 0; i < 10; i++ ) { //Hartley suggests 10 iterations at most
        Mat_<double> X_ = linearLSTriangulation( u, P, u1, P1 );
        X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X(3) = 1.0;

        // Recalculate weights
        double p2x  = Mat_<double>( Mat_<double>( P  ).row( 2 ) * X )( 0 );
        double p2x1 = Mat_<double>( Mat_<double>( P1 ).row( 2 ) * X )( 0 );

        // Breaking point
        if ( fabsf( wi - p2x ) <= EPSILON && fabsf( wi1 - p2x1 ) <= EPSILON ) {
            break;
        }

        wi = p2x;
        wi1 = p2x1;

        // Reweight equations and solve
        Matx43d A((u.x*P(2,0)-P(0,0))/wi,		(u.x*P(2,1)-P(0,1))/wi,			(u.x*P(2,2)-P(0,2))/wi,
                  (u.y*P(2,0)-P(1,0))/wi,		(u.y*P(2,1)-P(1,1))/wi,			(u.y*P(2,2)-P(1,2))/wi,
                  (u1.x*P1(2,0)-P1(0,0))/wi1,	(u1.x*P1(2,1)-P1(0,1))/wi1,		(u1.x*P1(2,2)-P1(0,2))/wi1,
                  (u1.y*P1(2,0)-P1(1,0))/wi1,	(u1.y*P1(2,1)-P1(1,1))/wi1,		(u1.y*P1(2,2)-P1(1,2))/wi1
                  );
        Mat_<double> B = (Mat_<double>(4,1) <<	  -(u.x*P(2,3)	-P(0,3))/wi,
                                                  -(u.y*P(2,3)	-P(1,3))/wi,
                                                  -(u1.x*P1(2,3)	-P1(0,3))/wi1,
                                                  -(u1.y*P1(2,3)	-P1(1,3))/wi1
                          );

        solve( A, B, X_, DECOMP_SVD );
        X( 0 ) = X_( 0 ); X( 1 ) = X_( 1 ); X( 2 ) = X_( 2 ); X( 3 ) = 1.0;
    }
    return( X );
}


double Triangulation::triangulatePoints( const vector<KeyPoint>& pt_set1, const vector<KeyPoint>& pt_set2,
                                         const Mat& K, const Mat& Kinv, const Mat& distcoeff,
                                         const Matx34d& P, const Matx34d& P1,
                                         vector<CloudPoint>& pointcloud,
                                         vector<KeyPoint>& correspImg1Pt)
{
//	pointcloud.clear();
    correspImg1Pt.clear();

    Matx44d P1_( P1(0,0),P1(0,1),P1(0,2),P1(0,3),
                 P1(1,0),P1(1,1),P1(1,2),P1(1,3),
                 P1(2,0),P1(2,1),P1(2,2),P1(2,3),
                 0,		0,		0,		1);
    Matx44d P1inv( P1_.inv() );

    cout << "Triangulating...";
    double t = getTickCount();
    vector<double> reproj_error;
    unsigned int pts_size = pt_set1.size();

    //Using OpenCV's triangulation
    //convert to Point2f
    vector<Point2f> _pt_set1_pt, _pt_set2_pt;
    KeyPointsToPoints( pt_set1, _pt_set1_pt );
    KeyPointsToPoints( pt_set2, _pt_set2_pt );

    //undistort
    Mat pt_set1_pt,pt_set2_pt;
    undistortPoints(_pt_set1_pt, pt_set1_pt, K, distcoeff);
    undistortPoints(_pt_set2_pt, pt_set2_pt, K, distcoeff);

    //triangulate
    Mat pt_set1_pt_2r = pt_set1_pt.reshape(1, 2);
    Mat pt_set2_pt_2r = pt_set2_pt.reshape(1, 2);
    Mat pt_3d_h(1,pts_size,CV_32FC4);
    cv::triangulatePoints(P,P1,pt_set1_pt_2r,pt_set2_pt_2r,pt_3d_h);

    //calculate reprojection
    vector<Point3f> pt_3d;
    convertPointsHomogeneous(pt_3d_h.reshape(4, 1), pt_3d);
    cv::Mat_<double> R = (cv::Mat_<double>(3,3) << P(0,0),P(0,1),P(0,2), P(1,0),P(1,1),P(1,2), P(2,0),P(2,1),P(2,2));
    Vec3d rvec; Rodrigues(R ,rvec);
    Vec3d tvec(P(0,3),P(1,3),P(2,3));
    vector<Point2f> reprojected_pt_set1;
    projectPoints(pt_3d,rvec,tvec,K,distcoeff,reprojected_pt_set1);

    for (unsigned int i=0; i<pts_size; i++) {
        CloudPoint cp;
        cp.pt = pt_3d[i];
        pointcloud.push_back(cp);
        reproj_error.push_back(norm(_pt_set1_pt[i]-reprojected_pt_set1[i]));
    }

    Scalar mse = mean(reproj_error);
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Done. ("<<pointcloud.size()<<"points, " << t <<"s, mean reproj err = " << mse[0] << ")"<< endl;

    return( mse[0] );
}


bool Triangulation::findCameraMatrices( const Mat& K, const Mat& Kinv, const Mat& distcoeff, const Mat& F,
                                        const Mat& Eaux, vector<KeyPoint>& imgpts1_good, vector<KeyPoint>& imgpts2_good,
                                        Matx34d& P, Matx34d& P1, vector<DMatch>& matches, vector<CloudPoint>& outCloud )
{
    //Find camera matrices
    if ( matches.size() < 50 ) {
        cerr << "not enough inliers after F matrix" << endl;
        return( false );
    }

    //Essential matrix: compute then extract cameras [R|t]
    Mat_<double> E = Eaux; //according to HZ (9.12)

    //according to http://en.wikipedia.org/wiki/Essential_matrix#Properties_of_the_essential_matrix
    if ( fabsf( determinant( Eaux ) ) > 1e-07 ) {
        cout << "det(E) != 0 : " << determinant( Eaux ) << "\n";
        P1 = 0;
        return( false );
    }

    Mat_<double> R1(3,3);
    Mat_<double> R2(3,3);
    Mat_<double> t1(1,3);
    Mat_<double> t2(1,3);

    //decompose E to P' , HZ (9.19)
    {
        if ( !DecomposeEtoRandT( E, R1, R2, t1, t2 ) ) {
            cerr << "Could not decompose E" << endl;
            return( false );
        }

        if ( determinant( R1 ) + 1.0 < 1e-09 ) {
            //according to http://en.wikipedia.org/wiki/Essential_matrix#Showing_that_it_is_valid
            cout << "det(R) == -1 [" << determinant( R1 ) << "]: flip E's sign" << endl;
            E = -E;
            DecomposeEtoRandT( E, R1, R2, t1, t2 );
        }
        if ( !CheckCoherentRotation( R1 ) ) {
            cout << "resulting rotation is not coherent\n";
            P1 = 0;
            return( false );
        }

        P1 = Matx34d(R1(0,0),	R1(0,1),	R1(0,2),	t1(0),
                     R1(1,0),	R1(1,1),	R1(1,2),	t1(1),
                     R1(2,0),	R1(2,1),	R1(2,2),	t1(2));
        cout << "Testing P1 " << endl << Mat(P1) << endl;

        vector<CloudPoint> pcloud,pcloud1; vector<KeyPoint> corresp;

        double reproj_error1 = triangulatePoints( imgpts1_good, imgpts2_good, K, Kinv, distcoeff, P, P1, pcloud, corresp );
        double reproj_error2 = triangulatePoints( imgpts2_good, imgpts1_good, K, Kinv, distcoeff, P1, P, pcloud1, corresp );
        vector<uchar> tmp_status;
        //check if pointa are triangulated --in front-- of cameras for all 4 ambiguations
        if ( !TestTriangulation( pcloud, P1, tmp_status ) ||
             !TestTriangulation( pcloud1, P, tmp_status ) ||
             reproj_error1 > 100.0 || reproj_error2 > 100.0 ) {
            P1 = Matx34d(R1(0,0),	R1(0,1),	R1(0,2),	t2(0),
                         R1(1,0),	R1(1,1),	R1(1,2),	t2(1),
                         R1(2,0),	R1(2,1),	R1(2,2),	t2(2));
            cout << "Testing P1 "<< endl << Mat(P1) << endl;

            pcloud.clear(); pcloud1.clear(); corresp.clear();
            reproj_error1 = triangulatePoints( imgpts1_good, imgpts2_good, K, Kinv, distcoeff, P, P1, pcloud, corresp );
            reproj_error2 = triangulatePoints( imgpts2_good, imgpts1_good, K, Kinv, distcoeff, P1, P, pcloud1, corresp );

            if ( !TestTriangulation( pcloud, P1, tmp_status ) ||
                 !TestTriangulation( pcloud1, P, tmp_status ) ||
                 reproj_error1 > 100.0 || reproj_error2 > 100.0 ) {
                if ( !CheckCoherentRotation( R2 ) ) {
                    cout << "resulting rotation is not coherent\n";
                    P1 = 0;
                    return( false );
                }

                P1 = Matx34d(R2(0,0),	R2(0,1),	R2(0,2),	t1(0),
                             R2(1,0),	R2(1,1),	R2(1,2),	t1(1),
                             R2(2,0),	R2(2,1),	R2(2,2),	t1(2));
                cout << "Testing P1 "<< endl << Mat(P1) << endl;

                pcloud.clear(); pcloud1.clear(); corresp.clear();
                reproj_error1 = triangulatePoints( imgpts1_good, imgpts2_good, K, Kinv, distcoeff, P, P1, pcloud, corresp );
                reproj_error2 = triangulatePoints( imgpts2_good, imgpts1_good, K, Kinv, distcoeff, P1, P, pcloud1, corresp );

                if ( !TestTriangulation( pcloud, P1, tmp_status ) ||
                     !TestTriangulation( pcloud1, P, tmp_status ) ||
                     reproj_error1 > 100.0 || reproj_error2 > 100.0 ) {
                    P1 = Matx34d(R2(0,0),	R2(0,1),	R2(0,2),	t2(0),
                                 R2(1,0),	R2(1,1),	R2(1,2),	t2(1),
                                 R2(2,0),	R2(2,1),	R2(2,2),	t2(2));
                    cout << "Testing P1 "<< endl << Mat(P1) << endl;

                    pcloud.clear(); pcloud1.clear(); corresp.clear();
                    reproj_error1 = triangulatePoints( imgpts1_good, imgpts2_good, K, Kinv, distcoeff, P, P1, pcloud, corresp );
                    reproj_error2 = triangulatePoints( imgpts2_good, imgpts1_good, K, Kinv, distcoeff, P1, P, pcloud1, corresp );

                    if ( !TestTriangulation( pcloud, P1, tmp_status ) ||
                         !TestTriangulation( pcloud1, P, tmp_status ) ||
                         reproj_error1 > 100.0 || reproj_error2 > 100.0 ) {
                        cout << "Triangulation test failed." << endl;
                        return( false );
                    }
                }
            }
        }
        for (unsigned int i=0; i<pcloud.size(); i++) {
            outCloud.push_back(pcloud[i]);
        }
    }
    return( true );
}

bool Triangulation::DecomposeEtoRandT(
    Mat_<double>& E,
    Mat_<double>& R1,
    Mat_<double>& R2,
    Mat_<double>& t1,
    Mat_<double>& t2 )
{
    // HZ E decomposition (using OpenCV's SVD)
    SVD svd( E, SVD::MODIFY_A );
    Mat svd_u = svd.u;
    Mat svd_vt = svd.vt;
    Mat svd_w = svd.w;

    //check if first and second singular values are the same (as they should be)
    double singular_values_ratio = fabsf(svd_w.at<double>(0) / svd_w.at<double>(1));
    if(singular_values_ratio>1.0) singular_values_ratio = 1.0/singular_values_ratio; // flip ratio to keep it [0,1]
    if (singular_values_ratio < 0.7) {
        cout << "singular values are too far apart\n";
        return false;
    }

    Matx33d W(0,-1,0,	//HZ 9.13
        1,0,0,
        0,0,1);
    Matx33d Wt(0,1,0,
        -1,0,0,
        0,0,1);
    R1 = svd_u * Mat(W) * svd_vt; //HZ 9.19
    R2 = svd_u * Mat(Wt) * svd_vt; //HZ 9.19
    t1 = svd_u.col(2); //u3
    t2 = -svd_u.col(2); //u3

    return( true );
}


bool Triangulation::CheckCoherentRotation( Mat_<double>& R )
{


    if(fabsf(determinant(R))-1.0 > 1e-07) {
        cerr << "det(R) != +-1.0, this is not a rotation matrix" << endl;
        return false;
    }

    return true;
}


bool Triangulation::TestTriangulation(const vector<CloudPoint>& pcloud, const Matx34d& P, vector<uchar>& status) {
    vector<Point3d> pcloud_pt3d = CloudPointsToPoints( pcloud );
    vector<Point3d> pcloud_pt3d_projected( pcloud_pt3d.size() );

    Matx44d P4x4 = Matx44d::eye();
    for(int i=0;i<12;i++) P4x4.val[i] = P.val[i];

    perspectiveTransform( pcloud_pt3d, pcloud_pt3d_projected, P4x4 );

    status.resize( pcloud.size(), 0 );
    for ( unsigned int i = 0; i < pcloud.size(); i++ ) {
        status[i] = (pcloud_pt3d_projected[i].z > 0) ? 1 : 0;
    }
    int count = countNonZero( status );

    double percentage = ((double)count / (double)pcloud.size());
    cout << count << "/" << pcloud.size() << " = " << percentage*100.0 << "% are in front of camera" << endl;
    if ( percentage < 0.75 ) {
        cerr << "Less than 75% of the points are in front of the camera." << endl;
        return( false );
    }
    return( true );
}

vector<Point3d> Triangulation::CloudPointsToPoints( const vector<CloudPoint> cpts ) {
    vector<Point3d> out;
    for ( unsigned int i = 0; i < cpts.size(); i++ ) {
        out.push_back( cpts[i].pt );
    }
    return( out );
}


void Triangulation::KeyPointsToPoints( const vector<KeyPoint>& kps, vector<Point2f>& ps ) {
    ps.clear();
    for ( unsigned int i = 0; i < kps.size(); i++ ) {
        ps.push_back( kps[i].pt );
    }
}
