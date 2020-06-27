
/*
 FFT 변환 후 필터링 -> FFT 역변환(IFFT)
 **/

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
