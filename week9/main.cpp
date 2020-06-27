//
//  main.cpp
//  week9
//
//  Created by 임성현 on 2020/05/17.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void ex00() {
    vector<Mat> img_bgr_src;
    
    vector<string> szNameWindows_bgr;
    string szNameWindow1 = "source";
    szNameWindows_bgr.push_back("windows_B");
    szNameWindows_bgr.push_back("windows_G");
    szNameWindows_bgr.push_back("windows_R");
    
    Mat img_src = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week9/people.jpg");
    imshow(szNameWindow1, img_src);
    split(img_src, img_bgr_src);
    
    for (int i = 0; i < 3; i++)
        imshow(szNameWindows_bgr[i], img_bgr_src[i]);
    
    // 분리된 이미지를 컬러 영상으로 변경.
    Mat img_bgr_split[3];
    vector<Mat> img_tmp;
    Mat img_black = Mat::zeros(img_src.rows, img_src.cols, CV_8UC1);
    
    img_tmp.push_back(img_bgr_src[0]);
    img_tmp.push_back(img_black);
    img_tmp.push_back(img_black);
    
    merge(img_tmp, img_bgr_split[0]);
    
    img_tmp.clear();        // 비우기.
    
    imshow(szNameWindows_bgr[0], img_bgr_split[0]);
    
    waitKey(0);
}

void ex01() {
    string szNameWindow1 = "A";
    string szNameWindow2 = "B";
    string szNameWindow3 = "result";
    Mat img_A = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week9/people.jpg");
    Mat img_B = img_A.clone();
    Mat img_C;
    subtract(img_A, img_B, img_C);
    imshow(szNameWindow1, img_A);
    imshow(szNameWindow2, img_B);
    imshow(szNameWindow3, img_C);
    
    waitKey(0);
}

void ex02() {
    string szNameWindow1 = "A";
    string szNameWindow2 = "B";
    string szNameWindow3 = "result";
    Mat img_A = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week9/people.jpg");
    Mat img_B;
    for (int i = 0; i < 10; i++) {
        img_B = Mat::zeros(img_A.rows, img_A.cols, CV_8UC3);
        
        ellipse(img_B, Point(80 + i*10, 50 + i*10), Size(50, 50), 0, 0, 360, Scalar(255, 255, 255), -1);
        Mat img_C;
        bitwise_and(img_A, img_B, img_C);
        imshow(szNameWindow1, img_A);
        imshow(szNameWindow2, img_B);
        imshow(szNameWindow3, img_C);
        
        waitKey(0);
    }
}


string szNameWindow1 = "A";
string szNameWindow2 = "B";
float sx = 0, sy = 0, ex = 0, ey = 0;
int bMouseDown = 0;

Mat img_A;
Mat img_B;

void MyMerge() {
    bitwise_or(img_A, img_B, img_B);
}

void onMouse(int event, int mx, int my, int flags, void *param) {
    Mat img_tmp = img_A.clone();
    Mat mask = Mat::zeros(img_A.rows, img_A.cols, CV_8UC3);
    RotatedRect rect;
    
    switch (event) {
        case EVENT_LBUTTONDOWN:
            sx = (float)mx;
            sy = (float)my;
            bMouseDown = 1;     // 마우스를 누른 상태인 지 확인하는 변수.
            break;
        case EVENT_MOUSEMOVE:
            if (bMouseDown == 1) {
                ex = (float)mx;
                ey = (float)my;
                rect = RotatedRect(Point2f((sx + ex) / 2, (sy + ey) / 2),
                                        Size(abs(ex - sx), abs(ey - sy)), 0);
                ellipse(img_tmp, rect, Scalar(0, 0, 0), 2);
                imshow(szNameWindow1, img_tmp);
            }
            break;
            
        case EVENT_LBUTTONUP:
            ex = (float)mx;
            ey = (float)my;
            rect = RotatedRect(Point2f((sx + ex) / 2, (sy + ey) / 2),
                               Size(abs(ex - sx), abs(ey - sy)), 0);
            ellipse(mask, rect, Scalar(255, 255, 255), -1);
            bitwise_and(mask, img_A, img_A);
            bitwise_and(Scalar(255, 255, 255) - mask, img_B, img_B);
            imshow(szNameWindow1, img_A);
            bMouseDown = 0;
            MyMerge();      // 두 이미지 합성
            imshow(szNameWindow2, img_B);
            break;
            
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    img_A = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week9/smile.jpg");
    img_B = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week9/landscape.jpg");
    resize(img_B, img_B, Size(img_A.cols, img_A.rows));
    
    imshow(szNameWindow1, img_A);
    setMouseCallback(szNameWindow1, onMouse, &img_A);
    
    imshow(szNameWindow2, img_B);
    waitKey(0);
    return 0;
}
