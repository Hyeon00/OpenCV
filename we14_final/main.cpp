//
//  main.cpp
//  we14_final
//
//  Created by 임성현 on 2020/06/18.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 기본 이미지 변수
Mat I;

// 메뉴 위치 설정
Rect rMenu1(20, 20, 160, 50);
Rect rMenu2(20, 90, 160, 50);

Mat readImage();                                // 이미지 불러오기
int bInsideRect(int x, int y, Rect rect);       // 점이 사각형 안에 있는지 체크
void addNoise(Mat& img, int nNoise);            // 노이즈 추가하는 함수
void onMouse(int event, int mx, int my, int flag, void* param);


int main()
{
    // 기본 이미지 불러오기
    I = readImage();

    // 메뉴 이미지 만들기
    Mat menu = Mat::zeros(512, 200, CV_8UC1);
    menu += 255;
    rectangle(menu, rMenu1, Scalar(0), 1);
    rectangle(menu, rMenu2, Scalar(0), 1);
    putText(menu, "Add Noise", Point(rMenu1.x + 15, rMenu1.y+30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0), 2);
    putText(menu, "My Test", Point(rMenu2.x + 15, rMenu2.y + 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0), 2);

    // 메뉴 창을 만들고 마우스 이벤트 추가
    namedWindow("Menu");
    setMouseCallback("Menu", onMouse, &menu);

    imshow("src", I);
    imshow("Menu", menu);
    waitKey();

    return 0;
}

Mat readImage() {
    Mat I = imread("/Users/seonghyeon/xCode/opencv/opencv/we14_final/G.jpg",IMREAD_GRAYSCALE);
    if (I.empty()) {
        cout << "Error opening image" << endl;
        exit(EXIT_FAILURE);
    }
    return I;
}

// 점이 사각형 안에 있는지 체크: 안에 있으면 1, 밖에 있으면 0
int bInsideRect(int x, int y, Rect rect) {
    if (x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height)
        return 1;
    else
        return 0;
}

// 노이즈 추가하는 함수
void addNoise(Mat& img, int nNoise) {
    int nGenPoints = 0;
    while (nGenPoints < nNoise) {
        int x = (int)(((float)rand() / RAND_MAX) * img.cols);
        int y = (int)(((float)rand() / RAND_MAX) * img.rows);
        if (x >= img.cols || y >= img.rows)
            continue;
        if ((float)rand() / RAND_MAX > 0.5)
            img.at<uchar>(y, x) = 0;
        else
            img.at<uchar>(y, x) = 255;
        nGenPoints++;
    }
}

// 마우스 이벤트 핸들러
void onMouse(int event, int mx, int my, int flag, void* param) {
    switch (event) {
    case EVENT_LBUTTONDOWN:
        if (bInsideRect(mx, my, rMenu1)) {
            addNoise(I, 100);
            imshow("src", I);
        }
        else if (bInsideRect(mx, my, rMenu2)) {
            putText(I, "This is The final Exam!", Point(100,100), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(255,255,255), 4);
            putText(I, "Add your functions!", Point(100, 200), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(255, 255, 255), 4);
            imshow("src", I);
        }
        break;
    }
}
