/*
 * FeatureMatcher.cpp
 *
 *  Created on: 18/01/2013
 *      Author: Raphael Montanari
 */

#include "FeatureMatcher.h"

FeatureMatcher::FeatureMatcher()
{
	matcher = new FlannBasedMatcher();
}

FeatureMatcher::~FeatureMatcher()
{
    delete ( matcher );
}

vector<DMatch> FeatureMatcher::matchDescriptors( Mat descriptors_1,
        Mat descriptors_2 )
{
    //FlannBasedMatcher flannBasedmatcher;
    //vector<vector<DMatch > > matches;
    //flannBasedmatcher.knnMatch(lastFrameDescriptors, descriptors, matches, 2);

    //BFMatcher bForceMatcher( NORM_L2 );
    //vector<DMatch> matches;
    //bForceMatcher.match( lastFrameDescriptors, descriptors, matches );

	vector<DMatch> matches;
    matcher->match( descriptors_1, descriptors_2, matches );
    return( matches );
}

void FeatureMatcher::maxMinDistances( vector<DMatch> matches,
        double *max_dist, double *min_dist )
{
	*max_dist = 0.0;
	*min_dist = 100.0;
    for ( unsigned int i = 0; i < matches.size(); i++ ) {
        double dist = matches[i].distance;
        *max_dist = max( dist, *max_dist );
        *min_dist = min( dist, *min_dist );
    }
}

void FeatureMatcher::drawGoodMatches( Mat img_1, Mat img_2, Mat descriptors_1,
        vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2, vector<DMatch> matches )
{
	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
	//-- PS.- radiusMatch can also be used here.
	vector<DMatch> good_matches;

    double max_dist = 0, min_dist = 100;
    maxMinDistances( matches, &max_dist, &min_dist );

    for ( int i = 0; i < descriptors_1.rows; i++ ) {
        if ( matches[i].distance < 2 * min_dist ) {
            good_matches.push_back( matches[i] );
		}
	}

	//-- Draw only "good" matches
	Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2, good_matches,
            img_matches, Scalar::all( -1 ), Scalar::all( -1 ), vector<char> (),
            DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Show detected matches
    imshow( "Good Matches", img_matches );
    waitKey( 0 );
}
