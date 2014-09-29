/*
 * BundleAdjuster.cpp
 *
 *  Created on: 06/03/2013
 *      Author: Raphael Montanari
 */

#include "BundleAdjuster.h"

BundleAdjuster::BundleAdjuster() {
	// camera related parameters
	int N = 11;
	int numCamera = N;
	Size cameraRes(640, 480);
	Mat
			cameraMat =
					(cv::Mat_<double>(3, 3) << cameraRes.width / 4, 0, cameraRes.width
							/ 2, 0, cameraRes.height / 4, cameraRes.height / 2, 0, 0, 1);
	RNG rng(0);

	// variables for sba
	vector<Point3d> points_true, points_init, points_opt;
	vector<vector<Point2d> > imagePoints;
	vector<vector<int> > visiblity;
	vector<Mat> cameraMatrix;
	vector<Mat> R_true, R_init, R_opt;
	vector<Mat> T_true, T_init, T_opt;
	vector<Mat> distCoeffs;
	TermCriteria criteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 70, 1e-10);

	// generate points
	for (double y = -10; y < 10; y += 1) {
		double r = 4 + sin(3 * M_PI * y / 10);
		for (double theta = 0; theta <= 2 * M_PI; theta += M_PI / 4) {
			Point3d
					point_true(r * cos(theta) + y /*+ rng.gaussian(1)*/,
							5 - y /*+ rng.gaussian(1)*/, r * sin(theta) + y /*+ rng.gaussian(1)*/);
			Point3d point_init(point_true.x + rng.gaussian(5), point_true.y
					+ rng.gaussian(5), point_true.z + rng.gaussian(5));
			points_true.push_back(point_true);
			points_init.push_back(point_init);
			points_opt.push_back(point_init);
		}
	}

	// define cameras
	for (int i = 0; i < numCamera; i++) {
		cameraMatrix.push_back(cameraMat);
		distCoeffs.push_back((Mat_<double> (4, 1) << 0, 0, 0, 0));

		Mat _R_true(3, 3, CV_64F), _R_init(3, 3, CV_64F);
		_R_true = (Mat_<double> (3, 3) << cos(i * 2 * M_PI / N), 0, sin(i * 2
				* M_PI / N), 0, 1, 0, -sin(i * 2 * M_PI / N), 0, cos(i * 2
				* M_PI / N));
		rng.fill(_R_init, RNG::NORMAL, Scalar(0), Scalar(0.5));
		_R_init += _R_true;
		R_true.push_back(_R_true);
		R_init.push_back(_R_init);
		R_opt.push_back(_R_init);

		Mat _T_true(3, 1, CV_64F), _T_init(3, 1, CV_64F);
		_T_true = (Mat_<double> (3, 1) << 0, 0, 30);
		rng.fill(_T_init, RNG::NORMAL, Scalar(0), Scalar(0.5));
		_T_init += _T_true;
		T_true.push_back(_T_true);
		T_init.push_back(_T_init);
		T_opt.push_back(_T_init);
	}

	// project points to image coordinates
	for (int i = 0; i < cameraMatrix.size(); i++) {
		// project
		vector<Point2d> imagePoint;
		projectPoints(points_true, R_true[i], T_true[i], cameraMatrix[i],
				distCoeffs[i], imagePoint);

		// check if the point is in cameras
		vector<int> vis(points_true.size(), 0);
		for (int j = 0; j < imagePoint.size(); j++) {
			// if the image point is within camera resolution then the point is visible
			if ((0 <= imagePoint[j].x) && (imagePoint[j].x <= cameraRes.width)
					&& (0 <= imagePoint[j].y) && (imagePoint[j].y
					<= cameraRes.height) && (rng.uniform(-1, 9))) { // add randomness

				// perturbate
				vis[j] = 1;
				imagePoint[j].x += rng.gaussian(3);
				imagePoint[j].y += rng.gaussian(3);
			}
			// else, the point is not visible
			else {

				vis[j] = 0;
				imagePoint[j].x = -1;
				imagePoint[j].y = -1;
			}
		}
		imagePoints.push_back(imagePoint);
		visiblity.push_back(vis);
	}

	// run bunble adjustment
	LevMarqSparse lms;
	lms.bundleAdjust(points_opt, imagePoints, visiblity, cameraMatrix, R_opt,
			T_opt, distCoeffs, criteria);

	// set up viewer
	pcl::visualization::PCLVisualizer viewer("3D Viewer");
	viewer.setBackgroundColor(0, 0, 0);
	viewer.addCoordinateSystem(2.0);
	viewer.initCameraParameters();

	// Fill in the true point data
	pcl::PointCloud<pcl::PointXYZRGB> cloud_true;
	cloud_true.width = points_true.size();
	cloud_true.height = 1;
	cloud_true.is_dense = false;
	cloud_true.points.resize(cloud_true.width * cloud_true.height);

	for (size_t i = 0; i < cloud_true.points.size(); ++i) {
		cloud_true.points[i].x = points_true[i].x;
		cloud_true.points[i].y = points_true[i].y;
		cloud_true.points[i].z = points_true[i].z;
		cloud_true.points[i].r = 0;
		cloud_true.points[i].g = 255;
		cloud_true.points[i].b = 0;
	}
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> green(
			cloud_true.makeShared());

	// Fill in the initial point data
	pcl::PointCloud<pcl::PointXYZRGB> cloud_init;
	cloud_init.width = points_init.size();
	cloud_init.height = 1;
	cloud_init.is_dense = false;
	cloud_init.points.resize(cloud_init.width * cloud_init.height);

	for (size_t i = 0; i < cloud_init.points.size(); ++i) {
		cloud_init.points[i].x = points_init[i].x;
		cloud_init.points[i].y = points_init[i].y;
		cloud_init.points[i].z = points_init[i].z;
		cloud_init.points[i].r = 255;
		cloud_init.points[i].g = 0;
		cloud_init.points[i].b = 0;
	}
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> red(
			cloud_init.makeShared());

	// Fill in the optimised point data
	pcl::PointCloud<pcl::PointXYZRGB> cloud_opt;
	cloud_opt.width = points_opt.size();
	cloud_opt.height = 1;
	cloud_opt.is_dense = false;
	cloud_opt.points.resize(cloud_opt.width * cloud_opt.height);

	for (size_t i = 0; i < cloud_opt.points.size(); ++i) {
		cloud_opt.points[i].x = points_opt[i].x;
		cloud_opt.points[i].y = points_opt[i].y;
		cloud_opt.points[i].z = points_opt[i].z;
		cloud_opt.points[i].r = 255;
		cloud_opt.points[i].g = 255;
		cloud_opt.points[i].b = 255;
	}
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> white(
			cloud_opt.makeShared());

	// add points
	viewer.addPointCloud<pcl::PointXYZRGB> (cloud_true.makeShared(), green,
			"true points");
	viewer.setPointCloudRenderingProperties(
			pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "true points");

	viewer.addPointCloud<pcl::PointXYZRGB> (cloud_init.makeShared(), red,
			"initial points");
	viewer.setPointCloudRenderingProperties(
			pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "initial points");

	viewer.addPointCloud<pcl::PointXYZRGB> (cloud_opt.makeShared(), white,
			"optimized points");
	viewer.setPointCloudRenderingProperties(
			pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3,
			"optimized points");

	// add camera coordinates
	for (int i = 0; i < cameraMatrix.size(); i++) {
		Mat Rw, Tw;
		Eigen::Matrix4f _t;
		Eigen::Affine3f t;

		// true camera coordinate
		Rw = R_true[i].t();
		Tw = -Rw * T_true[i];

		_t << Rw.at<double> (0, 0), Rw.at<double> (0, 1), Rw.at<double> (0, 2), Tw.at<
				double> (0, 0), Rw.at<double> (1, 0), Rw.at<double> (1, 1), Rw.at<
				double> (1, 2), Tw.at<double> (1, 0), Rw.at<double> (2, 0), Rw.at<
				double> (2, 1), Rw.at<double> (2, 2), Tw.at<double> (2, 0), 0.0, 0.0, 0.0, 1.0;

		t = _t;
		viewer.addCoordinateSystem(2.0, t);
		/*
		 // initial camera coordinate
		 Rw =  R_init[i].t();
		 Tw = -Rw*T_init[i];

		 _t << Rw.at<double>(0,0), Rw.at<double>(0,1), Rw.at<double>(0,2), Tw.at<double>(0,0),
		 Rw.at<double>(1,0), Rw.at<double>(1,1), Rw.at<double>(1,2), Tw.at<double>(1,0),
		 Rw.at<double>(2,0), Rw.at<double>(2,1), Rw.at<double>(2,2), Tw.at<double>(2,0),
		 0.0, 0.0, 0.0, 1.0;

		 t = _t;
		 viewer.addCoordinateSystem(2.0, t);
		 */
		// optimized camera coordinate
		Rw = R_opt[i].t();
		Tw = -Rw * T_opt[i];

		_t << Rw.at<double> (0, 0), Rw.at<double> (0, 1), Rw.at<double> (0, 2), Tw.at<
				double> (0, 0), Rw.at<double> (1, 0), Rw.at<double> (1, 1), Rw.at<
				double> (1, 2), Tw.at<double> (1, 0), Rw.at<double> (2, 0), Rw.at<
				double> (2, 1), Rw.at<double> (2, 2), Tw.at<double> (2, 0), 0.0, 0.0, 0.0, 1.0;

		t = _t;
		viewer.addCoordinateSystem(5.0, t);
	}

	// summary
	// cout << "points: \n" << points << endl;
	// cout << "image points: \n";
	// for (int i=0; i<imagePoints.size(); i++) {
	//   cout << imagePoints[i] << endl;
	// }
	// cout << "visiblity: \n";
	// for (int i=0; i<visiblity.size(); i++) {
	//   cout << "[";
	//   for (int j=0; j<visiblity[i].size(); j++) {
	//     cout << visiblity[i][j] << ", ";
	//   }
	//   cout << "];" << endl;
	// }
	// cout << "T: \n";
	// for (int i=0; i<T_true.size(); i++) {
	//   cout << i << ":\n";
	//   cout << T_true[i] << endl;
	//   cout << T_init[i] << endl;
	//   cout << T_opt[i] << endl;
	// }
	// cout << "R: \n";
	// for (int i=0; i<R_true.size(); i++) {
	//   cout << i << ":\n";
	//   cout << R_true[i] << endl;
	//   cout << R_init[i] << endl;
	//   cout << R_opt[i] << endl;
	// }

	// view
	viewer.spin();
}

BundleAdjuster::~BundleAdjuster() {
}
