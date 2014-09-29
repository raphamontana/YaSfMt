/*
 * FeatureMatcher.h
 *
 *  Created on: 16/01/2013
 *      Author: Raphael Montanari
 */

#ifndef FEATUREMATCHER_H_
#define FEATUREMATCHER_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class FeatureMatcher {
public:
	FeatureMatcher();
	~FeatureMatcher();

    vector<DMatch> matchDescriptors(Mat descriptors_1, Mat descriptors_2);

    /**
     * Calculation of max and min distances between keypoints.
     */
    void maxMinDistances( vector<DMatch> matches, double *max_dist, double *min_dist);

    void drawGoodMatches( Mat img_1, Mat img_2, Mat descriptors_1, vector<KeyPoint> keypoints_1,
			vector<KeyPoint> keypoints_2, vector<DMatch> matches);
private:
	FlannBasedMatcher *matcher;
};

#endif /* FEATUREMATCHER_H_ */
