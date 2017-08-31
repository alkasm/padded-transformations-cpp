#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "padtransf.h"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main() {

    // read src and dst image
    Mat src, dst;
    src = imread("test/img3.png", CV_LOAD_IMAGE_COLOR);
    dst = imread("test/img1.png", CV_LOAD_IMAGE_COLOR);

    // read transformation matrix
    Mat transf;
    ifstream transfFile;
    transfFile.open("test/H1to3p");
    float x;
    while (transfFile >> x) {
        transf.push_back(x*2);
    }
    transfFile.close();
    transf = transf.reshape(1, 3);

    // standard warpPerspective
    Mat crop_warped;
    warpPerspective(src, crop_warped, transf, crop_warped.size(), WARP_INVERSE_MAP);

    // blending parameters for display
    float alpha = 0.5;
    float beta = 1 - alpha;
    float gamma = 1.0;

    // display cropped warp
    Mat blended;
    addWeighted(crop_warped, alpha, dst, beta, gamma, blended);
    imshow("Blended warp, standard crop", blended);
    waitKey();

    // new warpPerspectivePadded
    Mat src_warped, dst_padded;
    warpPerspectivePadded(src, dst, transf, src_warped, dst_padded, WARP_INVERSE_MAP);

    // display padded warp
    Mat blended_padded;
    addWeighted(src_warped, alpha, dst_padded, beta, gamma, blended_padded);
    imshow("Blended warp, padded crop", blended_padded);
    waitKey();

    destroyAllWindows();

    return 0;
}

