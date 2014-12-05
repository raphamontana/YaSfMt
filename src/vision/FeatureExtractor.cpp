/*
 * FeatureExtractor.cpp
 *
 *  Created on: 16/01/2013
 *      Author: Raphael Montanari
 */

#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor() {
    /// ToDo
    //initModule_nonfree();

	// Inicializa o extrator SIFT.
    sift = Algorithm::create<Feature2D>("Feature2D.SIFT");
    //if ( sift.empty() ) {
    //    CV_Error( Error::StsNotImplemented,
    //			"OpenCV has been compiled without SIFT support");
    //}
    //sift->set("contrastThreshold", 0.01f);

    // Inicializa o extrator SURF.
    surf = Algorithm::create<Feature2D>("Feature2D.SURF");
    //if ( surf.empty() ) {
    //    CV_Error( Error::StsNotImplemented,
    //			"OpenCV has been compiled without SURF support");
    //}
    //surf->set("hessianThreshold", 400);

	//surf->set("nOctaves", 3);
	//surf->set("nOctaveLayers", 4);

    orb = ORB::create( 10000, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20 );
    if ( orb.empty() ) {
        CV_Error( Error::StsNotImplemented, "OpenCV has been compiled without SURF support" );
    }
}

void FeatureExtractor::extractORB( Mat image, Mat *descriptors,
        vector<KeyPoint> *keypoints ) {
    Mat roi(image.size(), CV_8UC1, Scalar(0));
    orb->detectAndCompute( image, roi, *keypoints, *descriptors );
}

void FeatureExtractor::extractSIFT( Mat image, Mat *descriptors,
        vector<KeyPoint> *keypoints ) {
    sift->detectAndCompute( image, noArray(), *keypoints, *descriptors );
}

void FeatureExtractor::extractSURF(Mat image, Mat *descriptors,
		vector<KeyPoint> *keypoints) {
    surf->detectAndCompute( image, noArray(), *keypoints, *descriptors );
}

void FeatureExtractor::showKeypoints(Mat image, vector<KeyPoint> keypoints) {
	Mat outImg;
	drawKeypoints(image, keypoints, outImg, Scalar(255, 0, 0));
	imshow("test", outImg);
	waitKey();
}
