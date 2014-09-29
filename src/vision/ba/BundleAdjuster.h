/*
 * BundleAdjustment.h
 *
 *  Created on: 06/03/2013
 *      Author: Raphael Montanari
 */

#ifndef BUNDLEADJUSTER_H_
#define BUNDLEADJUSTER_H_

#include <opencv2/opencv.hpp>
#include <vector>

#include <pcl-1.6/pcl/io/pcd_io.h>
#include <pcl-1.6/pcl/point_types.h>
#include <pcl-1.6/pcl/visualization/pcl_visualizer.h>

using namespace cv;
using namespace std;

class BundleAdjuster {
public:
	BundleAdjuster();
	virtual ~BundleAdjuster();
};

#endif /* BUNDLEADJUSTER_H_ */
