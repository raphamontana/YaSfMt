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
	if (sift.empty()) {
		CV_Error(CV_StsNotImplemented,
				"OpenCV has been compiled without SIFT support");
	}
	sift->set("contrastThreshold", 0.01f);

	// Inicializa o extrator SURF.
	surf = Algorithm::create<Feature2D>("Feature2D.SURF");
	if (surf.empty()) {
		CV_Error(CV_StsNotImplemented,
				"OpenCV has been compiled without SURF support");
	}
	surf->set("hessianThreshold", 400);
	//surf->set("nOctaves", 3);
	//surf->set("nOctaveLayers", 4);
}

void FeatureExtractor::extractORB(Mat image, Mat *descriptors,
		vector<KeyPoint> *keypoints) {
	orb(image, Mat(), *keypoints, *descriptors);
}

void FeatureExtractor::extractSIFT(Mat image, Mat *descriptors,
		vector<KeyPoint> *keypoints) {
	(*sift)(image, noArray(), *keypoints, *descriptors);
}

void FeatureExtractor::extractSURF(Mat image, Mat *descriptors,
		vector<KeyPoint> *keypoints) {
	(*surf)(image, noArray(), *keypoints, *descriptors);
}

void FeatureExtractor::showKeypoints(Mat image, vector<KeyPoint> keypoints) {
	Mat outImg;
	drawKeypoints(image, keypoints, outImg, Scalar(255, 0, 0));
	imshow("test", outImg);
	waitKey();
}
