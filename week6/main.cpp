//
//  main.cpp
//  week6
//
//  Created by 임성현 on 2020/04/28.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void print_vectorInfo(string v_name, vector<int> v);
void ex00();
void ex01();
void ex02();
void ex03();


int main(int argc, const char * argv[]) {
    
    ex03();
    return 0;
}

void print_vectorInfo(string v_name, vector<int> v) {
    cout << "[" << v_name << "] = ";
    if (v.empty())  cout << "Empty" << endl;
    else            cout << ((Mat)v).reshape(1, 1) << endl;
    
    cout << ".size() = " << v.size() << endl;
}

void ex00() {
    int arr[] = { 10, 20, 30, 40, 50 };
    vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
    vector<int> v2;
    v2.reserve(100);
    v2.push_back(3);
    v2.push_back(4);
    v2.push_back(5);
    
    print_vectorInfo("v2", v2);
    cout << ".capacity() = " << v2.capacity() << endl << endl;
    v1.insert(v1.begin() + 2, 100);
    print_vectorInfo("v2 after insert", v2);
    
    cout << ".capacity() = " << v2.capacity() << endl << endl;
    
}

void ex01() {
    /*
    Range r1(1, 3);
    Range r2 = Range::all();
    cout << r2;
     */
    
    Mat m1 = (Mat_<uchar> (3, 4) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    Range r(1, 2);
    Mat m2 = m1.col(0);
    m2 = m1.rowRange(r);
    //m2.at<uchar>(0,0) = 255;
    Size wholesize;
    Point ofs;
    m2.locateROI(wholesize, ofs);
    m2.adjustROI(0, 0, 1, -1);
    //cout << "wholesize: " << wholesize << " ofs: " << ofs;
    cout << "wholesize: " << wholesize << " ofs: " << ofs;
    
    Range r1(2, 3), r2(1, 3);
    Mat m3 = m1(r1, r2);
    
}

void ex02() {
    Mat img = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week6/people.jpg", IMREAD_GRAYSCALE);
    Range r_row(50, 100), r_col(250, 300);
    Mat roi = img(r_row, r_col);
    roi.adjustROI(-40, 5, -20, 20);
    for (int i = 0; i < roi.rows; i++) {
        for (int j = 0; j < roi.cols; j++) {
            roi.at<uchar>(i, j) = 0;
        }
    }
    imshow("image", img);
    waitKey(0);
}

void ex03() {
    Mat img = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/week6/road.jpg", IMREAD_GRAYSCALE);
    
    Range r_row(300, 1200), r_col(300, 800);
    
    Mat roi = img(r_row, r_col);
    
    for (int i = 0; i < roi.rows; i++) {
        for (int j = 0; j < roi.cols; j++) {
            roi.at<uchar>(i, j) += 50;
        }
    }
    
    cout << roi << endl;
    
    imshow("image", img);
    waitKey(0);
}














