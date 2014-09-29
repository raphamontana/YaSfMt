add_subdirectory( calibration )
set( VISION_SRC_LIST vision/FeatureExtractor.cpp vision/FeatureExtractor.h vision/FeatureMatcher.cpp vision/FeatureMatcher.h vision/FundamentalMatrix.cpp vision/FundamentalMatrix.h vision/Vision.cpp vision/Vision.h vision/Triangulation.cpp vision/Triangulation.h ${VISION_CALIBRATION_SRC_LIST} PARENT_SCOPE )
