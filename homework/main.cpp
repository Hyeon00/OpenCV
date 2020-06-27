//
//  main.cpp
//  homework
//
//  Created by 임성현 on 2020/04/14.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    Mat image(1200, 1800, CV_8UC1, Scalar(255));
    
    Point2f houseCenter(450, 600), housePts[4];
    Point2f doorCenter(450, 975), doorPts[4], doorHook(415, 975);
    Point2f head(900, 600), body(900, 800), leftArm(975, 800), rightArm(825, 800),
    leftLeg(870, 980), rightLeg(930, 980);
    Point2f bodyPts[4], leftArmPts[4], rightArmPts[4], leftLegPts[4], rightLegPts[4];
    
    Size2f houseSize(400, 900), doorSize(150, 150);
    Size2f bodySize(100, 250), armSize(25, 150), legSize(25, 100);
    
    int angle = 0;
    RotatedRect houseRect(houseCenter, houseSize, angle);
    RotatedRect doorRect(doorCenter, doorSize, angle);
    
    RotatedRect bodyRect(body, bodySize, angle);
    RotatedRect leftArmRect(leftArm, armSize, angle);
    RotatedRect rightArmRect(rightArm, armSize, angle);
    RotatedRect leftLegRect(leftLeg, legSize, angle);
    RotatedRect rightLegRect(rightLeg, legSize, angle);
    
    Rect bound_houseRect = houseRect.boundingRect();
    Rect bound_doorRect = doorRect.boundingRect();
    
    Rect bound_bodyRect = bodyRect.boundingRect();
    Rect bound_leftArmRect = leftArmRect.boundingRect();
    Rect bound_rightArmRect = rightArmRect.boundingRect();
    Rect bound_leftLegRect = leftLegRect.boundingRect();
    Rect bound_rightLegRect = rightLegRect.boundingRect();
    
    rectangle(image, bound_houseRect, Scalar(0), 1);
    rectangle(image, bound_doorRect, Scalar(0), 1);
    
    rectangle(image, bound_bodyRect, Scalar(0), 1);
    rectangle(image, bound_leftArmRect, Scalar(0), 1);
    rectangle(image, bound_rightArmRect, Scalar(0), 1);
    rectangle(image, bound_leftLegRect, Scalar(0), 1);
    rectangle(image, bound_rightLegRect, Scalar(0), 1);
    
    circle(image, doorHook, 1, Scalar(0), 5);
    
    circle(image, head, 70, Scalar(0), 2);
    
    houseRect.points(housePts);
    doorRect.points(doorPts);
    
    bodyRect.points(bodyPts);
    leftArmRect.points(leftArmPts);
    rightArmRect.points(rightArmPts);
    leftLegRect.points(leftLegPts);
    rightLegRect.points(rightLegPts);
    
    
    for (int i = 0; i < 4; i++) {
        cout << "housePts[" << i << "] = " << housePts[i] << endl;
        cout << "doorPts[" << i << "] = " << doorPts[i] << endl;
        
        line(image, housePts[i], housePts[(i + 1) % 4], Scalar(0), 2);
        line(image, doorPts[i], doorPts[(i + 1) % 4], Scalar(0), 2);
        
        line(image, bodyPts[i], bodyPts[(i + 1) % 4], Scalar(0), 2);
        line(image, leftArmPts[i], leftArmPts[(i + 1) % 4], Scalar(0), 2);
        line(image, rightArmPts[i], rightArmPts[(i + 1) % 4], Scalar(0), 2);
        line(image, leftLegPts[i], leftLegPts[(i + 1) % 4], Scalar(0), 2);
        line(image, rightLegPts[i], rightLegPts[(i + 1) % 4], Scalar(0), 2);
    }
    
    // 창문
    int x = 450, y = 820;
    Point2f windowPts[4];
    Size2f windowSize(80, 70);
    
    for (int i = y; i > 150; i -= 100) {
        Point2f windowCenter(x, i);
        RotatedRect windowRect(windowCenter, windowSize, angle);
        Rect bound_windowRect = windowRect.boundingRect();
        rectangle(image, bound_windowRect, Scalar(0), 1);
        windowRect.points(windowPts);
        
        for (int j = 0; i < 4; i++) {
            line(image, windowPts[j], windowPts[(j + 1) % 4], Scalar(0), 2);
        }
    }
    
    x = 350;
    for (int i = y; i > 150; i -= 100) {
        Point2f windowCenter(x, i);
        RotatedRect windowRect(windowCenter, windowSize, angle);
        Rect bound_windowRect = windowRect.boundingRect();
        rectangle(image, bound_windowRect, Scalar(0), 1);
        windowRect.points(windowPts);
        
        for (int j = 0; i < 4; i++) {
            line(image, windowPts[j], windowPts[(j + 1) % 4], Scalar(0), 2);
        }
    }
    
    x = 550;
    for (int i = y; i > 150; i -= 100) {
        Point2f windowCenter(x, i);
        RotatedRect windowRect(windowCenter, windowSize, angle);
        Rect bound_windowRect = windowRect.boundingRect();
        rectangle(image, bound_windowRect, Scalar(0), 1);
        windowRect.points(windowPts);
        
        for (int j = 0; i < 4; i++) {
            line(image, windowPts[j], windowPts[(j + 1) % 4], Scalar(0), 2);
        }
    }
    
    // 나무
    Point2f leftA(1250, 500), leftB(1250, 1000);
    Point2f rightA(1450, 500), rightB(1450, 1000);
    Point2f leaf1(1250, 400), leaf2(1350, 300), leaf3(1450, 400);
    
    line(image, leftA, leftB, Scalar(0), 2);
    line(image, rightA, rightB, Scalar(0), 2);
    
    circle(image, leaf1, 125, Scalar(0), 2);
    circle(image, leaf2, 125, Scalar(0), 2);
    circle(image, leaf3, 125, Scalar(0), 2);
    
    
    imshow("Rotated Rect", image);
    waitKey(0);
    return 0;
}
