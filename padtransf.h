#ifndef PADTRANSF_H // make sure function isn't duplicated
#define PADTRANSF_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

void warpPerspectivePadded(
    Mat, // source image, to be warped
    Mat, // destination image, to be padded
    Mat, // 3x3 perspective transformation
    Mat, // warped source image
    Mat, // padded destination image
    int flags=INTER_LINEAR,             // flags for copyMakeBorder & warpPerspective
    int borderMode=BORDER_CONSTANT,     // mode for copyMakeBorder & warpPerspective
    const Scalar& borderValue=Scalar()  // if border_constant for copyMakeBorder & warpPerspective
);

void warpAffinePadded(
    Mat, // source image, to be warped
    Mat, // destination image, to be padded
    Mat, // 3x3 perspective transformation
    Mat, // warped source image
    Mat, // padded destination image
    int flags=INTER_LINEAR,             // flags for copyMakeBorder & warpPerspective
    int borderMode=BORDER_CONSTANT,     // mode for copyMakeBorder & warpPerspective
    const Scalar& borderValue=Scalar()  // if border_constant for copyMakeBorder & warpPerspective
);

#endif
