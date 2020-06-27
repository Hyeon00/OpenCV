//
//  main.cpp
//  opencv
//
//  Created by 임성현 on 2020/03/31.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, const char * argv[]) {
    // insert code here...
    Mat image = imread("/Users/seonghyeon/Desktop/xCode/opencv/opencv/opencv/myname.jpg", IMREAD_COLOR);
    imshow("Display window", image);
    waitKey(0);
    return 0;
}
