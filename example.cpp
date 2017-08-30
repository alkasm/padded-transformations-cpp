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
        transf.push_back(x);
    }
    transfFile.close();
    transf = transf.reshape(1, 3);

    float angle = 45;
    float data[2][3] = {{cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}};
    Mat affineTransf(2, 3, CV_32F, &data);

    // display cropped warp
    float alpha = 0.5;
    float beta = 1 - alpha;
    float gamma = 1.0;

    Mat src_warped, dst_padded;
    warpPerspectivePadded(src, dst, transf, src_warped, dst_padded,
        WARP_INVERSE_MAP, BORDER_REFLECT_101);
    Mat blended_persp;
    addWeighted(src_warped, alpha, dst_padded, beta, gamma, blended_persp);
    imshow("Blended warp, standard crop", blended_persp);
    waitKey();

    destroyAllWindows();

    return 0;
}

