/*
 * FeatureExtractor.h
 *
 *  Created on: 16/01/2013
 *      Author: Raphael Montanari
 */

#ifndef FEATUREEXTRACTOR_H_
#define FEATUREEXTRACTOR_H_

//#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
//#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/nonfree/features2d.hpp>

#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/xfeatures2d.hpp"
using xfeatures2d::SURF;
#endif

//#ifdef HAVE_OPENCV_NONFREE
//#include "opencv2/nonfree/nonfree.hpp"
//static bool makeUseOfNonfree = cv::initModule_nonfree();
//#else
//#  error Non free module not found.
//#endif

using namespace cv;
using namespace std;

class FeatureExtractor {
public:
	FeatureExtractor();
	void extractSIFT(Mat image, Mat *descriptors, vector<KeyPoint> *keypoints);
	void extractSURF(Mat image, Mat *descriptors, vector<KeyPoint> *keypoints);
	void extractORB(Mat image, Mat *descriptors, vector<KeyPoint> *keypoints);
	void showKeypoints(Mat image, vector<KeyPoint> keypoints);

private:
	Ptr<Feature2D> sift;
	Ptr<Feature2D> surf;
    Ptr<Feature2D> orb;
};

#endif /* FEATUREEXTRACTOR_H_ */
