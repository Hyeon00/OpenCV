//
//  main.cpp
//  week11
//
//  Created by 임성현 on 2020/05/30.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

void ex00();
void isSkinArea(Mat ycrcb[], Mat &Skin_img);

void ex01();
Mat conv(Mat img, Matx<float, 3, 3> f);       // f는 3 x 3 으로 가정.

int main(int argc, const char * argv[]) {
    // insert code here...
    ex01();
    return 0;
}

Mat conv(Mat img, Matx<float, 3, 3> f) {
    Mat ret = Mat(img.rows, img.cols, CV_8U);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            // 모든 img의 픽셀에 대해서 밑의 작업을 수행.
            int px, py;
            float sum = 0;
            for (int k = 0; k < 3; k++) {
                for (int m = 0; m < 3; m++) {
                    py = i + k - 1;
                    px = j + m - 1;
                    if (px < 0 || py < 0 || px >= img.cols || py >= img.rows)
                        continue;
                    sum += ((float)(img.at<uchar>(py, px)) * f(k, m));
                }
            }
            ret.at<uchar>(i, j) = (int)sum;
        }
    }
    return ret;
}

void ex01() {
    /*
    Matx<uchar, 5, 5> img(0, 0, 255, 255, 255,
                          0, 0, 255, 255, 255,
                          0, 0, 255, 255);
    */
    
    Mat img(100, 100, CV_8U, Scalar(0));
    for (int i = 25; i < 75; i++)
        for (int j = 25; j < 75; j++)
            img.at<uchar>(i, j) = 255;
    
    
    Matx<float, 3, 3> m((1.0/9), (1.0/9), (1.0/9),
                        (1.0/9), (1.0/9), (1.0/9),
                        (1.0/9), (1.0/9), (1.0/9));
    
    Mat m2 = conv(img, m);
    
    namedWindow("test", WINDOW_NORMAL);
    imshow("test", img);
    namedWindow("conv", WINDOW_NORMAL);
    imshow("conv", m2);
    waitKey(0);
}


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

void ex00() {
    Mat BGR_img = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week11/A.jpg");
    Mat HSV_img, ycrcb[3], bgr[3];
    Mat Skin_img, Skin_img_color;
    cvtColor(BGR_img, HSV_img, CV_BGR2YCrCb);     // BGR -> HSV
    split(HSV_img, ycrcb);
    split(BGR_img, bgr);
    
    
    isSkinArea(ycrcb, Skin_img);
    
    bgr[0] = bgr[0].mul(Skin_img / 255);
    bgr[1] = bgr[1].mul(Skin_img / 255);
    bgr[2] = bgr[2].mul(Skin_img / 255);
    
    merge(bgr, 3, Skin_img_color);
    
    imshow("BGR", BGR_img);
    imshow("Skin", Skin_img);
    imshow("Skin Color", Skin_img_color);
    //imshow("Y", ycrcb[0]);
    //imshow("Cr", ycrcb[1]);
    //imshow("Cb", ycrcb[2]);

    waitKey(0);
}
