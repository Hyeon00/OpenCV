//
//  main.cpp
//  week13
//
//  Created by 임성현 on 2020/06/14.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void ex00();
void ex01();
void ex02();

Mat src;    // 원본 이미지
Mat display_image;
string szTitle_src = "src";
Point2f p[4];
int nPoint = 0;

void onMouse(int event, int x, int y, int flag, void*);
void drawSelectedPoints();
//Mat getAffineMatrix();
Mat getPerspectiveMatrix();
void ex03();

void ex04();

int main(int argc, const char * argv[]) {
    // insert code here...
//    ex00();
//    ex01();
//    ex02();
    ex03();
//    ex04();
    
    waitKey(0);
    return 0;
}
/*
Mat getAffineMatrix() {
    Mat m;
    
    int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2));
    int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y, 2));
    Point2f p_new[4];
    p_new[1] = p[1];
    p_new[0].x = p[1].x;
    p_new[0].y = p[1].y - L1;
    p_new[2].x = p[1].x + L2;
    p_new[2].y = p[1].y;
    
    m = getAffineTransform(p, p_new);
    
    return m;
}
 */
Mat getPerspectiveMatrix() {
    Mat m;
    
    int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2));
    int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y, 2));
    Point2f p_new[4];
    p_new[1] = p[1];
    p_new[0].x = p[1].x;
    p_new[0].y = p[1].y - L1;
    p_new[2].x = p[1].x + L2;
    p_new[2].y = p[1].y;
    p_new[3].x = p[1].x + L2;
    p_new[3].y = p[1].y - L1;
    
    m = getPerspectiveTransform(p, p_new);
    
    return m;
}

void drawSelectedPoints() {
    int size_srect = 20;
    display_image = src.clone();
    
    for (int i = 0; i < nPoint; i++) {
        
        Rect rect(p[i].x-size_srect/2, p[i].y-size_srect/2, size_srect, size_srect);
        rectangle(display_image, rect, Scalar(0, 0, 0), 4);
    }
    
    if (nPoint == 4) {
        Mat m = getPerspectiveMatrix();
        Mat dst;
        //warpAffine(src, dst, m, Size(src.cols, src.rows));
        warpPerspective(src, dst, m, Size(src.cols, src.rows));
        imshow("dst", dst);
    }
    
    
    imshow(szTitle_src, display_image);
}

void onMouse(int event, int x, int y, int flag, void*) {
    if (event == EVENT_LBUTTONUP) {
        p[nPoint].x = x;
        p[nPoint].y = y;
        nPoint = nPoint + 1 > 4? 4 : (nPoint + 1);
        drawSelectedPoints();
    }
}

void ex03() {
    src = imread("/Users/seonghyeon/xCode/opencv/opencv/week13/F.jpeg");
    namedWindow(szTitle_src, WINDOW_NORMAL);
    namedWindow("dst", WINDOW_NORMAL);
    setMouseCallback(szTitle_src, onMouse, 0);
    
    imshow(szTitle_src, src);
}

void ex02() {
    int tx = 100, ty = 0;
    
    Mat image = imread("/Users/seonghyeon/xCode/opencv/opencv/week13/A.jpg");
    Point2f center(image.cols/2, image.rows/2);
    // 회전변환 매트릭스 계산
    Mat m1 = getRotationMatrix2D(center, -30, 1);
    Matx13d mat_add(0, 0, 1);
    m1.push_back((Mat)mat_add);
    // 이동 매트릭스 계산
    Matx33d m2(1, 0, tx, 0, 1, ty, 0, 0, 1);
    // 확대(resize) 매트릭스 계산
    Matx33d m3(0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5);
    // shear(찌그러짐) 매트릭스 계산
    Matx33d m4(1, 1.5, 0, 0, 1, 0, 0, 0, 1);
    // 회전 후 resize, 이동 매트릭스 계산
    Mat m = (Mat)m4 * (Mat)m3 * (Mat)m2 * m1;
    m.pop_back();
    
    Mat dst1;
    warpAffine(image, dst1, m, Size(image.cols, image.rows));
    imshow("src", image);
    imshow("dst1", dst1);
}

void ex01() {
    Mat image = imread("/Users/seonghyeon/xCode/opencv/opencv/week13/A.jpg");
    Point2f center(image.cols/2, image.rows/2);
    Mat m = getRotationMatrix2D(center, -30, 1);
    Mat dst1;
    warpAffine(image, dst1, m, Size(image.cols, image.rows));
    
    imshow("src", image);
    imshow("dst1", dst1);
}

void ex00() {
    Mat image = imread("/Users/seonghyeon/xCode/opencv/opencv/week13/A.jpg");
    CV_Assert(image.data);
    Mat dst3, dst4;
    
    float factor = 0.2;
    
    resize(image, dst3, Size((int)(image.cols * factor), (int)(image.rows * factor)), 0, 0, INTER_LINEAR);
    resize(image, dst4, Size((int)(image.cols * factor), (int)(image.rows * factor)), 0, 0, INTER_CUBIC);
    
    imshow("src", image);
    imshow("nearest", dst3);
    imshow("cubic", dst4);
}
