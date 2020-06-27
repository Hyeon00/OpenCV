//
//  main.cpp
//  week8
//
//  Created by 임성현 on 2020/05/06.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int x, y;
string szTitle = "창 제어";
string szTitle2 = "Selection";
//Mat img_global;
Mat m2;

void ex00() {
    //Mat image1(h, w, CV_8U, Scalar(255));
    string szTitle = "창 제어";
    namedWindow(szTitle, WINDOW_NORMAL);
    //imshow(szTitle, image1);
    waitKey(0);
    moveWindow(szTitle, 100, 200);
    resizeWindow(szTitle, 400, 300);
    waitKey(0);
}

void onMouse(int event, int mx, int my, int flag, void* param) {
    Mat* img = (Mat*)param;
    switch (event) {
        case EVENT_LBUTTONDOWN:
            circle(*img, Point(mx, my), 5, Scalar(0));
            imshow(szTitle, *img);
            break;
    }
    moveWindow(szTitle, x, y);
}

void ex01() {
    Mat image1(300, 400, CV_8U, Scalar(255));
    //img_global = image1;
    
    namedWindow(szTitle, WINDOW_NORMAL);
    imshow(szTitle, image1);
    setMouseCallback(szTitle, onMouse, &image1);
    
    waitKey(0);
}

void onChange(int value, void* userdata) {
    Mat *m = (Mat*)userdata;
    m2 = *m - 130 + value;      // 사본 이미지.
                                    // 중첩되는 것을 방지하기 위해 사본이미지를 사용한다.
    imshow(szTitle, m2);
}

void onMouse2(int event, int x, int y, int flag, void* param) {
    if (event == EVENT_MOUSEWHEEL) {
        Rect rect = getWindowImageRect(szTitle);
        resizeWindow(szTitle, rect.width + 10, rect.height + 10);
    }
}

void ex02() {
    int value = 128;
    Mat image1 = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week6/road.jpg",
                        IMREAD_GRAYSCALE);
    
    namedWindow(szTitle, WINDOW_NORMAL);
    setMouseCallback(szTitle, onMouse2);
    createTrackbar("밝기", szTitle, &value, 255, onChange, &image1);
    imshow(szTitle, image1);
    
    waitKey();
    if (!m2.empty()) {
        imwrite("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week8/test.jpg", m2);
    }
}

void onMouse3(int event, int x, int y, int flag, void* param) {
    Mat *m = (Mat*)param;
    if (event == EVENT_MOUSEWHEEL) {
        Rect rect = getWindowImageRect(szTitle);
        resizeWindow(szTitle, rect.width + 10, rect.height + 10);
    } else if (event == EVENT_LBUTTONDOWN) {
        int ymin = (y - 50);
        int ymax = (y + 50);
        int xmin = (x - 50);
        int xmax = (x + 50);
        if (ymin < 0) {
            ymin = 0;
            ymax = 100;
        }
        if (ymax > m->size().height) {
            ymax = m->size().height;
            ymin = ymax - 100;
        }
        if (xmin < 0) {
            xmin = 0;
            xmax = 100;
        }
        if (xmax > m->size().width) {
            xmax = m->size().width;
            xmin = xmax - 100;
        }
        
        Mat m2 = (*m)(Range(ymin, ymax), Range(xmin, xmax));
        imshow(szTitle2, m2);
    }
}

void ex03() {
    int value = 128;
    Mat image1 = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week6/road.jpg",
                        IMREAD_GRAYSCALE);
    
    namedWindow(szTitle, WINDOW_NORMAL);
    namedWindow(szTitle2, WINDOW_NORMAL);
    setMouseCallback(szTitle, onMouse3, &image1);
    createTrackbar("밝기", szTitle, &value, 255, onChange, &image1);
    imshow(szTitle, image1);
    
    waitKey();
//    if (!m2.empty()) {
//        imwrite("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week8/test.jpg", m2);
//    }
}

void ex04() {
    string myTitle = "original";
    string myTitle2 = "change";
    string myTitle3 = "transpose";
    Mat img = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week8/figure.png");
    Mat img_2, img_3;
    //Range r1(0, 250), r2(0, 250), r3(0, 250), r4(250, 500),
            //r5(250, 500), r6(0, 250), r7(250, 500), r8(250, 500);
    imshow(myTitle, img);
    repeat(img, 10, 10, img_2);
    flip(img_2, img_2, 1);
    imshow(myTitle2, img_2);
    transpose(img_2, img_3);
    /*
    Mat img1 = img_2(r1, r2);
    Mat img2 = img_2(r3, r4);
    Mat img3 = img_2(r5, r6);
    Mat img4 = img_2(r7, r8);

    flip(img1, img_3, 1);
    flip(img2, img_3, 1);
    flip(img3, img_3, 1);
    flip(img4, img_3, 1);
    */
    
    imshow(myTitle3, img_3);
    waitKey();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ex04();
    return 0;
}


