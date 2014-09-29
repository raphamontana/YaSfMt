#ifndef FUNDAMENTALMATRIX_H
#define FUNDAMENTALMATRIX_H

#include <opencv2/opencv.hpp>

using namespace cv;

class FundamentalMatrix
{
public:
    FundamentalMatrix();
    void find( InputArray points1, InputArray points2 );

private:
    Mat fundamentalMatrix;
};

#endif // FUNDAMENTALMATRIX_H
