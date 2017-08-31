#ifndef PADTRANSF_H
#define PADTRANSF_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void warpPerspectivePadded(
    const cv::Mat&, // source image, to be warped
    const cv::Mat&, // destination image, to be padded
    const cv::Mat&, // 3x3 perspective transformation
    cv::Mat&, // warped source image
    cv::Mat&, // padded destination image
    int flags=cv::INTER_LINEAR,         // flags for OpenCV funcs
    int borderMode=cv::BORDER_CONSTANT, // mode for OpenCV funcs
    const cv::Scalar& borderValue=cv::Scalar()  // border val
);

void warpAffinePadded(
    const cv::Mat&, // source image, to be warped
    const cv::Mat&, // destination image, to be padded
    const cv::Mat&, // 2x3 affine transformation
    cv::Mat&, // warped source image
    cv::Mat&, // padded destination image
    int flags=cv::INTER_LINEAR,         // flags for OpenCV funcs
    int borderMode=cv::BORDER_CONSTANT, // mode for OpenCV funcs
    const cv::Scalar& borderValue=cv::Scalar()  // border val
);

#endif
