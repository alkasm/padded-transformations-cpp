#include "padtransf.h"

using namespace cv;
using namespace std;

void warpPerspectivePadded(
    const Mat& src, const Mat& dst, const Mat& M, // input matrices
    Mat& src_warped, Mat& dst_padded, // output matrices
    int flags, int borderMode, const Scalar& borderValue) // OpenCV params
{
    
    Mat transf = M/M.at<float>(2, 2); // ensure a legal homography
    if (flags == WARP_INVERSE_MAP || 
        flags == INTER_LINEAR + WARP_INVERSE_MAP || 
        flags == INTER_NEAREST + WARP_INVERSE_MAP)
    {
        invert(transf, transf);
        flags -= WARP_INVERSE_MAP;
    }

    // it is enough to find where the corners of the image go to find
    // the padding bounds; points in clockwise order from origin
    int src_h = src.rows;
    int src_w = src.cols;
    Vec<Point2f, 4> init_pts, transf_pts;
    init_pts[0] = Point2f (0, 0);
    init_pts[1] = Point2f (src_w, 0);
    init_pts[2] = Point2f (src_w, src_h);
    init_pts[3] = Point2f (0, src_h);
    perspectiveTransform(init_pts, transf_pts, transf);

    // find min and max points
    int min_x, min_y, max_x, max_y;
    min_x = floor(min(
        min(transf_pts[0].x, transf_pts[1].x),
        min(transf_pts[2].x, transf_pts[3].x)));
    min_y = floor(min(
        min(transf_pts[0].y, transf_pts[1].y),
        min(transf_pts[2].y, transf_pts[3].y)));
    max_x = ceil(max(
        max(transf_pts[0].x, transf_pts[1].x),
        max(transf_pts[2].x, transf_pts[3].x)));
    max_y = ceil(max(
        max(transf_pts[0].y, transf_pts[1].y),
        max(transf_pts[2].y, transf_pts[3].y)));
 
    // add translation to the transformation matrix to shift to positive values
    int anchor_x, anchor_y = 0;
    Mat transl_transf = Mat::eye(3, 3, CV_32F);
    if (min_x < 0)
    {
        anchor_x = -min_x;
        transl_transf.at<float>(0,2) += anchor_x;
    }
    if (min_y < 0)
    {
        anchor_y = -min_y;
        transl_transf.at<float>(1,2) += anchor_y;
    }
    transf = transl_transf * transf;
    transf /= transf.at<float>(2, 2);

    // create padded destination image
    int dst_h = dst.rows;
    int dst_w = dst.cols;
    int pad_top = anchor_y;
    int pad_bot = max(max_y, dst_h) - dst_h;
    int pad_left = anchor_x;
    int pad_right = max(max_x, dst_w) - dst_w;
    copyMakeBorder(dst, dst_padded, pad_top, pad_bot, pad_left, pad_right, borderMode, borderValue);

    // transform src into larger window
    int dst_pad_h = dst_padded.rows;
    int dst_pad_w = dst_padded.cols;
    warpPerspective(src, src_warped, transf, Size (dst_pad_w, dst_pad_h), flags, borderMode, borderValue);
}

void warpAffinePadded(
    const Mat& src, const Mat& dst, const Mat& M, // input matrices
    Mat& src_warped, Mat& dst_padded, // output matrices
    int flags, const int borderMode, const Scalar& borderValue) // OpenCV params
{

    Mat transf(M); //copy M
    if (flags == WARP_INVERSE_MAP || 
        flags == INTER_LINEAR + WARP_INVERSE_MAP || 
        flags == INTER_NEAREST + WARP_INVERSE_MAP)
    {
        invertAffineTransform(M, M);
        flags -= WARP_INVERSE_MAP;
    }

    // it is enough to find where the corners of the image go to find
    // the padding bounds; points in clockwise order from origin
    int src_h = src.rows;
    int src_w = src.cols;
    Vec<Point2f, 4> init_pts, transf_pts;
    init_pts[0] = Point2f (0, 0);
    init_pts[1] = Point2f (src_w, 0);
    init_pts[2] = Point2f (src_w, src_h);
    init_pts[3] = Point2f (0, src_h);
    transform(init_pts, transf_pts, M);

    // find min and max points
    int min_x, min_y, max_x, max_y;
    min_x = floor(min(
        min(transf_pts[0].x, transf_pts[1].x),
        min(transf_pts[2].x, transf_pts[3].x)));
    min_y = floor(min(
        min(transf_pts[0].y, transf_pts[1].y),
        min(transf_pts[2].y, transf_pts[3].y)));
    max_x = ceil(max(
        max(transf_pts[0].x, transf_pts[1].x),
        max(transf_pts[2].x, transf_pts[3].x)));
    max_y = ceil(max(
        max(transf_pts[0].y, transf_pts[1].y),
        max(transf_pts[2].y, transf_pts[3].y)));

    // add translation to the transformation matrix to shift to positive values
    int anchor_x = 0, anchor_y = 0;
    if (min_x < 0)
    {
        anchor_x = -min_x;
    }
    if (min_y < 0)
    {
        anchor_y = -min_y;
    }
    transf.at<float>(0, 2) += anchor_x;
    transf.at<float>(1, 2) += anchor_y;

    // create padded destination image
    int dst_h = dst.rows;
    int dst_w = dst.cols;
    int pad_top = anchor_y;
    int pad_bot = max(max_y, dst_h) - dst_h;
    int pad_left = anchor_x;
    int pad_right = max(max_x, dst_w) - dst_w;
    copyMakeBorder(dst, dst_padded, pad_top, pad_bot, pad_left, pad_right, borderMode, borderValue);

    // transform src into larger window
    int dst_pad_h = dst_padded.rows;
    int dst_pad_w = dst_padded.cols;
    warpAffine(src, src_warped, transf, Size (dst_pad_w, dst_pad_h), flags, borderMode, borderValue);
}
