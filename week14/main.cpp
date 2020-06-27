//
//  main.cpp
//  week14
//
//  Created by 임성현 on 2020/06/18.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat readImage();
void ex00();
void getZeroPaddedImage(Mat& src, Mat& dst);
void getLogMag(Mat planes[], Mat& magI, Mat& angI);
void rearrange(Mat magI);
void addNoise(Mat& img, int nNoise);
void addLines(Mat& img, int nLine);
void myFilter(Mat& magI);

int main(int argc, const char * argv[]) {
    // insert code here...
    ex00();
    
    waitKey(0);
    return 0;
}

void ex00() {
    // 이미지 불러오기.
    Mat I = readImage();
    addNoise(I, 1000);
    addLines(I, 20);
    
    // zero 패딩
    Mat padded;
    getZeroPaddedImage(I, padded);
//    cout << "src: " << I.cols << ", " << I.rows << endl;
//    cout << "padded: " << padded.cols << ", " << padded.rows << endl;
    
    // dft를 위한 2채널(실수부, 허수부) 매트릭스 생성
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);
    
    // dft를 수행하고 채널 분리 planes[0]: 실수부, planes[1]: 허수부
    dft(complexI, complexI);    // 내부적으로는 fft를 함. complexI 안에는 (실수부, 허수부)로 나뉨.
    
    split(complexI, planes);
    
    // 실수부와 허수부 값으로부터 크기 계산하고 log 취하여 정규화, 각도도 함께 계산.
    Mat magI, angI;
    getLogMag(planes, magI, angI);
    
    // 역변환을 위한 최대 최소값 계산
    double min_mag, max_mag, min_ang, max_ang;
    minMaxLoc(magI, &min_mag, &max_mag);
    minMaxLoc(angI, &min_ang, &max_ang);
    
    // 정규화
    normalize(magI, magI, 0, 1, NORM_MINMAX);       // 0 ~ 1 정규화
    normalize(angI, angI, 0, 1, NORM_MINMAX);       // 0 ~ 1 정규화
    
    // 각 사분면의 위치를 재조정 (1-3사분면 교환, 2-4사분면 교환)
    rearrange(magI);
    rearrange(angI);
    
    namedWindow("source");
    namedWindow("zero-padding");
    imshow("source", I);
    imshow("zero-padding", padded);
    imshow("spectrum magnitude", magI);
    imshow("phase", angI);
    
    // 필터링
    myFilter(magI);
    
    imshow("filtered magnitude", magI);
    waitKey(0);
    
    /* IFFT를 위한 처리 시작 */
    // 각 사분면의 위치를 재조정 (1-3사분면 교환, 2-4사분면 교환)
    rearrange(magI);
    rearrange(angI);
    
    // Denormalization
    magI = magI * (max_mag - min_mag) + min_mag;
    angI = angI * (max_ang - min_ang) + min_ang;
    
    // 로그의 역변환인 지수변환 수행
    exp(magI, magI);
    magI -= 1;
    
    // 극 좌표계 -> 직교 좌표계로 변환 후 inverse FFT 수행.
    polarToCart(magI, angI, planes[0], planes[1], true);
    merge(planes, 2, complexI);
    
    // Inverse FFT
    dft(complexI, complexI, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
    split(complexI, planes);
    normalize(planes[0], planes[0], 0, 1, NORM_MINMAX); // 0 ~ 1 정규화.
    
    imshow("filtered", planes[0]);
}

// 필터링 함수
void myFilter(Mat& magI) {
    //Mat f = Mat::zeros(magI.rows, magI.cols, CV_32F);
    Mat f = Mat::ones(magI.rows, magI.cols, CV_32F);
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    circle(f, Point(cx, cy), 200, Scalar(1), -1);   // 200은 저주파를 남기는 정도.
    circle(f, Point(cx, cy), 3, Scalar(0), -1);
    int k = 11;
    for (int i = 0; i < 18; i++) {
        circle(f, Point(cx+k, cy), 3, Scalar(0), -1);
        k += 20;
    }
    k = -11;
    for (int i = 0; i < 18; i++) {
        circle(f, Point(cx+k, cy), 3, Scalar(0), -1);
        k -= 20;
    }
//    circle(f, Point(cx+11, cy), 3, Scalar(0), -1);
//    circle(f, Point(cx+31, cy), 3, Scalar(0), -1);
//    circle(f, Point(cx+51, cy), 3, Scalar(0), -1);
//    circle(f, Point(cx+71, cy), 3, Scalar(0), -1);
//    circle(f, Point(cx+91, cy), 3, Scalar(0), -1);
    magI = magI.mul(f);
}

// 선 노이즈 추가 함수.
void addLines(Mat& img, int nLine) {
    int width = img.cols / nLine;
    
    for (int i = 0; i < nLine; i++) {
        int sx = i * width;
        int ex = (i + 1) * width - 1;
        
        if (ex >= img.cols) {
            ex = img.cols - 1;
        }
        
        Mat tmp(img, Rect(sx, 0, ex - sx + 1, img.rows));
        
        if (i % 2 == 0) {
            tmp += Scalar::all(100);
        } else {
            tmp -= Scalar::all(100);
        }
    }
}

// 점 노이즈 추가 함수. nNoise : n개의 노이즈.
void addNoise(Mat& img, int nNoise) {
    int nGenPoints = 0;
    while (nGenPoints < nNoise) {
        // 랜덤하게 점을 찍음.
        int x = (int)(((float)rand() / RAND_MAX) * img.cols);
        int y = (int)(((float)rand() / RAND_MAX) * img.rows);
        // rand() 위치 초과 방지.
        if (x >= img.cols || y >= img.rows) {
            continue;
        }
        
        // 흰점과 검은점 섞어서. 50퍼 확률로.
        if ((float)rand() / RAND_MAX > 0.5) {
            img.at<uchar>(y, x) = 0;
        } else {
            img.at<uchar>(y, x) = 255;
        }
        nGenPoints++;
    }
}

// 각 사분면의 위치를 재조정하여 재배열
void rearrange(Mat magI) {
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    
    Mat q0(magI, Rect(0, 0, cx, cy));       // top-left
    Mat q1(magI, Rect(cx, 0, cx, cy));      // top-right
    Mat q2(magI, Rect(0, cy, cx, cy));      // bottom-left
    Mat q3(magI, Rect(cx, cy, cx, cy));     // bottom-right
    
    Mat tmp;
    
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

// 실수부와 허수부 값으로부터 크기와 각도를 계산하고 크기에 log 취하여 돌려주는 함수.
void getLogMag(Mat planes[], Mat& magI, Mat& angI) {
    // cartToPolar: 직교 좌표계 -> 극 좌표계.
    cartToPolar(planes[0], planes[1], magI, angI, true);
    magI += Scalar::all(1);                     // 1을 더함. switch to logarithmic scale
    log(magI, magI);                            // 크기에 대해서만 log.
    
    // 스펙트럼 이미지의 크기(너비, 높이)가 홀수인 경우 제거. (잘라냄)
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
}

void getZeroPaddedImage(Mat& src, Mat& dst) {
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, dst, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
}

Mat readImage() {
    Mat I = imread("/Users/seonghyeon/xCode/opencv/opencv/week14/F.jpeg", IMREAD_GRAYSCALE);
    if (I.empty()) {
        cout << "Error opening image" << endl;
        exit(EXIT_FAILURE);
    }
    return I;
}
