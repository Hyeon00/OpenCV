//
//  main.cpp
//  week5
//
//  Created by 임성현 on 2020/04/18.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void ex00();
void ex01();
void ex02();
void ex03();
void ex04();
void ex05();
void print_matInfo(string m_name, Mat m);
void ex06();
void ex07();
void ex08();

int main(int argc, const char * argv[]) {
    // insert code here...
    
    //ex01();
    //ex02();
    //ex03();
    //ex04();
    //ex05();
    //ex06();
    //ex07();
    ex08();
    return 0;
}

void ex00() {
    Mat m1 = 200*Mat::ones(300, 500, CV_8UC1);
    Mat m2 = Mat::zeros(300, 500, CV_8UC1);
    Mat m3 = 200*Mat::eye(300, 300, CV_8UC1);
    
    //cout << "M1 = " << endl << m1 << endl;
    //cout << "M2 = " << endl << m2 << endl;
    //cout << "M3 = " << endl << m3 << endl;
    
    imshow("Mat", m3);
    waitKey(0);
}

void ex01() {
    double a = 32.12345678, b = 2.7;
    short c = 400;
    float d = 10.f, e = 11.f, f = 13.f;
    
    Mat_<int> m1(2, 4);
    Mat_<uchar> m2(3, 4, 100);
    Mat_<short> m3(4, 5, c);
    
    m1 << 1, 2, 3, 4, 5, 6;
    Mat m4 = (Mat_<double>(2, 3) << 1, 2, 3, 4, 5, 6);
    Mat m5 = (Mat_<float>(2, 3) << a, b, c, d, e, f);
    
    cout << "m1 = " << endl << m1 << endl;
    cout << "m2 = " << endl << m2 << endl;
    cout << "m3 = " << endl << m3 << endl;
    cout << "m4 = " << endl << m4 << endl;
    cout << "m5 = " << endl << m5 << endl;
}

void ex02() {
    Matx<int, 3, 3> m1(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Matx<float, 2, 3> m2(1, 2, 3, 4, 5, 6);
    Matx<double, 3, 5> m3(3, 4, 5, 7);
    
    Matx23d m4(3, 4, 5, 6, 7, 8);
    Matx34d m5(1, 2, 3, 10, 11, 12, 13, 14, 15);
    Matx66f m6(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    
    cout << m1;
}

void ex03() {
    Mat m1(4, 3, CV_32FC3);
    
    cout << "차원 수 : " << m1.dims << endl;
    cout << "행 수 : " << m1.rows << endl;
    cout << "열 수 : " << m1.cols << endl;
    cout << "행렬 크기 : " << m1.size() << endl;
    
    cout << "전체 원소 수 : " << m1.total() << endl;
    cout << "한 원소의 크기 : " << m1.elemSize() << endl;
    cout << "채널 당 한 원소의 크기 : " << m1.elemSize1() << endl;
    
    
}

void ex04() {
    Mat m1(2, 3, CV_8U, 2);
    Mat m2(2, 3, CV_8U, Scalar(10));
    
    Mat m3 = m1 + m2;
    Mat m4 = m2 - 6;
    Mat m5 = m1;
    
    cout << "m1 = " << endl << m1 << endl;
    cout << "m5 = " << endl << m5 << endl << endl;
    m5 = 100;
    cout << "m1 = " << endl << m1 << endl;
    cout << "m5 = " << endl << m5 << endl << endl;
}

void ex05() {
    Mat m = (Mat_<uchar>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    cout << "m = " << endl << m << endl << endl;
    
    m.resize(1);
    cout << "m.resize(1) = " << endl << m << endl << endl;
    m.resize(3);
    cout << "m.resize(3) = " << endl << m << endl << endl;
    m.resize(5, Scalar(50));
    cout << "m.resize(5, Scalar(50)) = " << endl << m << endl << endl;
}

void print_matInfo(string m_name, Mat m) {
    cout << "[" << m_name << " 행렬]" << endl;
    cout << "   채널 수 = " << m.channels() << endl;
    cout << "   행 x 열 = " << m.rows << " x " << m.cols << endl << endl;
}

void ex06() {
    Mat m1(2, 6, CV_8U);
    Mat m2 = m1.reshape(2);
    Mat m3 = m1.reshape(3, 2);
    
    print_matInfo("m1(2, 6)", m1);
    print_matInfo("m2 = m1_reshape(2)", m2);
    print_matInfo("m3 = m1_reshape(3, 2)", m3);
    
    m1.create(3, 5, CV_16S);
    print_matInfo("m1.create(3, 5)", m1);
}

void ex07() {
    double data[] = {
        1.1, 2.2, 3.3, 4.4,
        5.5, 6.6, 7.7, 8.8,
        9.9, 10, 11, 12
    };
    
    Mat m1(3, 4, CV_64F, data);
    Mat m2 = m1.clone();
    Mat m3, m4;
    m1.copyTo(m3);
    m1.convertTo(m4, CV_8U);
    
    cout << "m1 =\n" << m1 << endl;
    cout << "m2 =\n" << m2 << endl;
    cout << "m3 =\n" << m3 << endl;
    cout << "m4 =\n" << m4 << endl;
}

void ex08() {
    vector<Point> v1;
    v1.push_back(Point(10, 20));
    v1.push_back(Point(20, 30));
    v1.push_back(Point(50, 60));
    
    vector<float> v2(3, 9.25);
    Size arr_size[] = { Size(2,2), Size(3, 3), Size(4, 4) };
    int arr_int[] = { 10, 20, 30, 40, 50 };
    
    vector<Size> v3(arr_size, arr_size + sizeof(arr_size) / sizeof(Size));
    vector<int> v4(arr_int + 2, arr_int + sizeof(arr_int) / sizeof(int));
    
    cout << "v1" << (Mat)v1 << endl << endl;
    cout << "v2" << ((Mat)v2).reshape(1, 1) << endl << endl;
    cout << "v3" << ((Mat)v3).reshape(1, 1) << endl << endl;
    cout << "v4" << ((Mat)v4).reshape(1, 1) << endl << endl;
}






