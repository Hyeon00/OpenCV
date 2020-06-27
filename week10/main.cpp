//
//  main.cpp
//  week10
//
//  Created by 임성현 on 2020/05/24.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat img_src;
string szWinName1 = "Main";
string szTrackbarName1 = "brightness";
string szTrackbarName2 = "contrast";

void ex00();
int calcBrightness(Mat m);
void onChange_Brightness(int value, void*);
void onChange_Contrast(int value, void*);
void addBrightness(Mat m, int v);
void multiplyContrast(Mat m, double v);
double mymin(double a, double b);

void ex01();
Mat calc_myHistogram(Mat m, int nBins);
void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256, 200)) {
    hist_img = Mat(size, CV_8U, Scalar(255));               // 그래프 행렬
    float bin_width = (float)hist_img.cols / hist.rows;     // 한 계급의 너비(높이 / 너비)
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX);   // histogram 정규화
    
    for (int i = 0; i < hist.rows; i++) {
        float start_x = i * bin_width;
        float end_x = (i+1) * bin_width;
        Point2f pt1(start_x, 0);
        Point2f pt2(end_x, hist.at<float>(i));
        
        if (pt2.y > 0) {    // 0인 경우는 사각형을 그릴 필요가 없다.
            rectangle(hist_img, pt1, pt2, Scalar(0), -1);   // 막대 사각형 그리기
        }
    }
    flip(hist_img, hist_img, 0);                            // x축 기준으로 상하 반전.
}


void ex02();
void create_hist(Mat img, Mat &hist, Mat &hist_img);


int main(int argc, const char * argv[]) {
    ex02();
    return 0;
}

void create_hist(Mat img, Mat &hist, Mat &hist_img) {
    int histsize = 256, range = 256;
    hist = calc_myHistogram(img, histsize);
    draw_histo(hist, hist_img);
}

void ex02() {
    Mat image = imread("/Users/seonghyeon/xCode/opencv/opencv/week10/landscape.jpg", IMREAD_COLOR);
    CV_Assert(!image.empty());      // 예외처리
    
    Mat hist, dst1, dst2, hist_img, hist_img1, hist_img2;
    create_hist(image, hist, hist_img);   // 히스토그램 및 그래프 그리기
    
    // 히스토그램 누적 합 계산.
    Mat accum_hist = Mat(hist.size(), hist.type(), Scalar(0));
    accum_hist.at<float>(0) = hist.at<float>(0);
    for (int i = 1; i < hist.rows; i++) {
        accum_hist.at<float>(i) = accum_hist.at<float>(i-1) + hist.at<float>(i);
    }
    
    accum_hist /= sum(hist)[0];         // 누적합의 정규화
    accum_hist *= 255;
    dst1 = Mat(image.size(), CV_8U);
    
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int idx = image.at<uchar>(i, j);
            dst1.at<uchar>(i, j) = (uchar)accum_hist.at<float>(idx);
        }
    }
    
    //normalize(accum_hist, accum_hist, 0, 255, NORM_MINMAX);
    //accum_hist.convertTo(accum_hist, CV_8U);
    //LUT(image, accum_hist, dst1);
    
    equalizeHist(image, dst2);          // OpenCV 히스토그램 평활화
    create_hist(dst1, hist, hist_img1);
    create_hist(dst2, hist, hist_img2);
    
    imshow("image", image);
    imshow("img_hist", hist_img);
    
    imshow("dst1-User", dst1);
    imshow("User_hist", hist_img1);
    
    imshow("dst2-OpenCV", dst1);
    imshow("OpenCV_hist", hist_img2);
    
    waitKey(0);
}

void ex01() {
    Mat m_src = imread("/Users/seonghyeon/xCode/opencv/opencv/week10/landscape.jpg", IMREAD_GRAYSCALE);
    Mat m_src2 = imread("/Users/seonghyeon/xCode/opencv/opencv/week10/C.jpg", IMREAD_GRAYSCALE);
    Mat m_histo_img1, m_histo_img2;
    imshow("사진", m_src);
    
    Mat hist1 = calc_myHistogram(m_src, 256);
    Mat hist2 = calc_myHistogram(m_src2, 256);
    
    /*
    for (int i = 0; i < hist.rows; i++) {
        cout << i << ": " << hist.at<float>(i, 0) << endl;
    }
    */
    
    draw_histo(hist1, m_histo_img1);
    draw_histo(hist2, m_histo_img2);
    
    imshow("histogram", m_histo_img1);
    imshow("histogram2", m_histo_img2);
    
    waitKey(0);
}

Mat calc_myHistogram(Mat m, int nBins) {
    Mat hist;
    int channels[] = { 0 };     // 채널 목록
    int hitSize[] = { nBins };  // bin 개수
    float range[] = { 0, 256 }; // 0 - 255까지, 조각내자.
    const float* ranges[] = { range };
    calcHist(&m, 1, channels, Mat(), hist, 1, hitSize, ranges);
    return hist;
}


void onChange_Brightness(int value, void*) {
    int b_before = calcBrightness(img_src);
    addBrightness(img_src, value - b_before);
    imshow(szWinName1, img_src);
    //imwrite("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week10/C.jpg", img_src);
}

void onChange_Contrast(int value, void*) {
    double v = 1 + value * 0.5;
    Mat img_tmp = img_src.clone();
    multiplyContrast(img_tmp, v);
    imshow(szWinName1, img_tmp);
}

double mymin(double a, double b) {
    return (a < b) ? a : b;
}

void multiplyContrast(Mat m, double v) {
    m = m * v;
}

int calcBrightness(Mat m) {
    int sum = 0;
    uchar *p;
    for (int i = 0; i < m.rows; i++) {
        p = m.ptr<uchar>(i);
        for (int j = 0; j < m.cols; j++) {
            sum = sum + ((p+j*3)[0] + (p+j*3)[1] + (p+j*3)[2]) / 3;
        }
    }
    /*
     for (int i = 0; i < m.rows; i++) {
     for (int j = 0; j < m.cols; j++) {
     sum = sum + (m.at<Vec3b>(i, j)[0] + m.at<Vec3b>(i, j)[1] + m.at<Vec3b>(i, j)[2]) / 3;
     }
     }
     */
    return sum / (m.rows * m.cols);
}

void addBrightness(Mat m, int v) {
    uchar *p;
    for (int i = 0; i < m.rows; i++) {
        p = m.ptr<uchar>(i);
        for (int j = 0; j < m.cols; j++) {
            for (int k = 0; k < 3; k++) {
                if (v + (p)[j*3+k] > 255) {
                    (p)[j*3+k] = 255;
                } else if(v + (p)[j*3+k] < 0) {
                    (p)[j*3+k] = 0;
                } else {
                    (p)[j*3+k] += v;
                }
            }
        }
    }
}
 
void ex00() {
    namedWindow(szWinName1);        // 윈도우 생성
    createTrackbar(szTrackbarName1, szWinName1, 0, 255, onChange_Brightness);
    createTrackbar(szTrackbarName2, szWinName1, 0, 4, onChange_Contrast);
    img_src = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week10/landscape.jpg");
    int brightness = calcBrightness(img_src);
    imshow(szWinName1, img_src);
    
    setTrackbarPos(szTrackbarName1, szWinName1, brightness);
    
    waitKey(0);
}
