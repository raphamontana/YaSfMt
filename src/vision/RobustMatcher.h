/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:
   Computer Vision Programming using the OpenCV Library.
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify,
   and distribute this source code, or portions thereof, for any purpose, without fee,
   subject to the restriction that the copyright notice may not be removed
   or altered from any source or altered source distribution.
   The software is released on an as-is basis and without any warranties of any kind.
   In particular, the software is not guaranteed to be fault-tolerant or free from failure.
   The author disclaims all warranties with regard to this software, any use,
   and any consequent failure, is purely the responsibility of the user.

   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#ifndef ROBUSTMATCHER_H
#define ROBUSTMATCHER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

class RobustMatcher
{

public:

    RobustMatcher();

    /**
     * Set the feature detector.
     * \arg detect Feature detector
     */
    void setFeatureDetector( Ptr< FeatureDetector >& detect );

    /**
     * Set descriptor extractor.
     * \arg desc Descriptor extractor.
     */
    void setDescriptorExtractor( Ptr< DescriptorExtractor >& desc );

    /**
     * Set the minimum distance to epipolar in RANSAC.
     * \arg d Min distance to epipolar.
     */
    void setMinDistanceToEpipolar( double d );

    /**
     * Set confidence level in RANSAC.
     * \arg c Confidence level.
     */
    void setConfidenceLevel( double c );

    /**
     * Set the NN ratio.
     * \arg r ratio
     */
    void setRatio( float r );

    /**
     * If you want the F matrix to be recalculated.
     * \arg flag
     */
    void refineFundamental( bool flag );

    /**
     * Clear matches for which NN ratio is > than threshold
     * \arg matches
     * \return the number of removed points (corresponding entries being cleared, i.e. size will be 0).
     */
    int ratioTest( vector< vector< DMatch > >& matches );

    /**
     * Insert symmetrical matches in symMatches vector.
     * \arg matches1
     * \arg matches2
     * \arg symMatches
     */
    void symmetryTest( const vector< vector< DMatch > >& matches1, const vector< vector< DMatch > >& matches2, vector< DMatch >& symMatches );

    /**
     * Identify good matches using RANSAC.
     * \arg matches
     * \arg keypoints1
     * \arg keypoints2
     * \arg outMatches
     * \return fundamental matrix.
     */
    Mat ransacTest( const vector< DMatch >& matches, const vector< KeyPoint >& keypoints1, const vector< KeyPoint >& keypoints2, vector< DMatch >& outMatches );

    /**
     * Match feature points using symmetry test and RANSAC.
     * \arg image1 input image
     * \arg image2 input image
     * \arg matches output matches
     * \arg keypoints1 output keypoints
     * \arg keypoints2 output keypoints
     * \returns fundamental matrix.
     */
    Mat match( Mat& image1, Mat& image2, vector< DMatch >& matches, vector< KeyPoint >& keypoints1, vector< KeyPoint >& keypoints2 );

private:

    /**
     * Pointer to the feature point detector object.
     */
    Ptr< Feature2D > surf;
    Ptr< FeatureDetector > detector;

    /**
     * Pointer to the feature descriptor extractor object.
     */
    Ptr< DescriptorExtractor > extractor;

    /**
     * max ratio between 1st and 2nd NN.
     */
    float ratio;

    /**
     * if true will refine the F matrix.
     */
    bool refineF;

    /**
     * confidence level (probability).
     */
    double confidence;

    /**
     * Min distance to epipolar.
     */
    double distance;
};

#endif // ROBUSTMATCHER_H
