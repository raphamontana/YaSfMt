#include "FundamentalMatrix.h"

FundamentalMatrix::FundamentalMatrix()
{

}

void FundamentalMatrix::find( InputArray points1, InputArray points2 )
{
    int point_count = 100;
    //vector<Point2f> points1( point_count );
    //vector<Point2f> points2( point_count );

    int method = FM_RANSAC;
    double param1 = 3.;   // Maximum distance from a point to an epipolar line in pixels.
    double param2 = 0.99; // Level of confidence that the estimated matrix is correct.

    // initialize the points here ... */
    for( int i = 0; i < point_count; i++ )
    {
        //points1[i] = ...;
        //points2[i] = ...;
    }

    fundamentalMatrix = findFundamentalMat( points1, points2, method, param1, param2 );
}
