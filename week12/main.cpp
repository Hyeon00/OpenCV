//
//  main.cpp
//  week12
//
//  Created by 임성현 on 2020/06/09.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

void ex00();

void isSkinArea(Mat ycrcb[], Mat &Skin_img) {
    Skin_img = Mat(ycrcb[0].rows, ycrcb[0].cols, CV_8U);
    for (int i = 0; i < ycrcb[0].rows; i++) {
        for (int j = 0; j < ycrcb[0].cols; j++) {
            if (ycrcb[0].at<uchar>(i, j) > 80 &&
                ycrcb[1].at<uchar>(i, j) > 135 && ycrcb[1].at<uchar>(i, j) < 180 && ycrcb[2].at<uchar>(i, j) > 85 && ycrcb[2].at<uchar>(i, j) < 135) {
                Skin_img.at<uchar>(i, j) = 255;
            } else {
                Skin_img.at<uchar>(i, j) = 0;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    
    Mat BGR_img = imread("/Users/seonghyeon/xCode/opencv/opencv/week12/D.jpeg");
    Mat HSV_img, ycrcb[3], bgr[3];
    Mat Skin_img, Skin_img_color;
    cvtColor(BGR_img, HSV_img, CV_BGR2YCrCb);     // BGR -> HSV
    split(HSV_img, ycrcb);
    split(BGR_img, bgr);
    
    
    isSkinArea(ycrcb, Skin_img);
    
    Matx<uchar, 5, 5> mask;
    mask <<
        0, 1, 1, 1, 0,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        0, 1, 1, 1, 0;
    
    //morphologyEx(Skin_img, Skin_img, MORPH_ERODE, mask);
    //morphologyEx(Skin_img, Skin_img, MORPH_DILATE, mask);
    
    morphologyEx(Skin_img, Skin_img, MORPH_OPEN, mask);
    morphologyEx(Skin_img, Skin_img, MORPH_CLOSE, mask);
    
    bgr[0] = bgr[0].mul(Skin_img / 255);
    bgr[1] = bgr[1].mul(Skin_img / 255);
    bgr[2] = bgr[2].mul(Skin_img / 255);
    
    merge(bgr, 3, Skin_img_color);
    
    
    
    imshow("BGR", BGR_img);
    imshow("Skin", Skin_img);
    imshow("Skin Color", Skin_img_color);
    
    waitKey(0);
    
    return 0;
}

void ex00() {
    Mat m = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week12/D.jpeg");
    Mat dst;
    //Sobel(m, dst, CV_32F, 1, 0, 3);
    Matx<int, 3, 3> mask(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    filter2D(m, dst, CV_32F, mask);
    convertScaleAbs(dst, dst);
    imshow("src", m);
    imshow("dst", dst);
    
}
