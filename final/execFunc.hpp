
/*
 void addNoise(Mat& img, int nNoise);   점 노이즈 추가하는 함수
 void addLineNoise(int nLine);          선 노이즈 추가하는 함수
 #include "funcFFT.hpp"                 FFT 기능 헤더파일
    void fft(Mat& img);
 #include "funcSKIN.hpp"                피부영역 검출 기능 헤더파일
    Mat skinDetect(Mat& img);
 #include "funcHistogram.hpp"           히스토그램 기능 헤더파일
    void histogram(Mat& img);
 void command_func(int mode);           funcMenu 수행 함수
 Canny(tmp, tmp, 100, 150);             Edge 검출
*/

void addPointNoise(int nNoise) {
    int nGenPoints = 0;
    while (nGenPoints < nNoise) {
        int x = (int)(((float)rand() / RAND_MAX) * canvasImage.cols);
        int y = (int)(((float)rand() / RAND_MAX) * canvasImage.rows);
        
        if (x >= canvasImage.cols || y >= canvasImage.rows)
            continue;
        
        if ((float)rand() / RAND_MAX > 0.5) {
            canvasImage.at<Vec3b>(y, x) = 0;
        }
        else {
            canvasImage.at<Vec3b>(y, x) = 255;
        }
        nGenPoints++;
    }
}

void addLineNoise(Mat& img, int nLine) {
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

void fft(Mat& img) {
    // zero 패딩
    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    
    Mat padded;
    getZeroPaddedImage(imgGray, padded);
    
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
    
    // 필터링
    myFilter(magI);
    
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
    
    //cvtColor(plane[0], img, COLOR_GRAY2BGR);
    
    imshow("FFT", planes[0]);
}

Mat skinDetect(Mat& img) {
    Mat BGR_img = img;
    Mat HSV_img, ycrcb[3], bgr[3];
    Mat Skin_img, Skin_img_color;
    cvtColor(BGR_img, HSV_img, CV_BGR2YCrCb);     // BGR -> HSV
    split(HSV_img, ycrcb);
    split(BGR_img, bgr);
    
    
    isSkinArea(ycrcb, Skin_img);
    
    Matx<uchar, 5, 5> mask;
    mask <<
        0, 1, 1, 1, 0,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        0, 1, 1, 1, 0;
    
    //morphologyEx(Skin_img, Skin_img, MORPH_ERODE, mask);
    //morphologyEx(Skin_img, Skin_img, MORPH_DILATE, mask);
    
    morphologyEx(Skin_img, Skin_img, MORPH_OPEN, mask);
    morphologyEx(Skin_img, Skin_img, MORPH_CLOSE, mask);
    
    bgr[0] = bgr[0].mul(Skin_img / 255);
    bgr[1] = bgr[1].mul(Skin_img / 255);
    bgr[2] = bgr[2].mul(Skin_img / 255);
    
    merge(bgr, 3, Skin_img_color);
    
    //imshow("BGR", BGR_img);
    //imshow("Skin", Skin_img);
    //imshow("SkinDetect", Skin_img_color);
    return Skin_img_color;
}

void histogram(Mat& img) {
    Mat image = img;
    cvtColor(image, image, COLOR_BGR2GRAY);
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
    
    //imshow("image", image);
    //imshow("img_hist", hist_img);
    
    imshow("equalizeHist", dst1);
    imshow("histogram", hist_img1);
    
    //imshow("dst2-OpenCV", dst1);
    //imshow("OpenCV_hist", hist_img2);
}

void command_func(int mode) {                    // 일반 명령 수행 함수
    if (mode == ADD_POINTNOISE) {
        cout << "ADD_NOISE btn" << endl;
        addPointNoise(100);
    } else if (mode == ADD_LINENOSE) {
        addLineNoise(canvasImage, 20);
    } else if (mode == FFT) {
        fft(canvasImage);
    } else if (mode == GRAY) {
        Mat imgGrayColor;
        cvtColor(canvasImage, imgGrayColor, COLOR_BGR2GRAY);
        imshow("Gray2Color", imgGrayColor);
    } else if (mode == ICOLOR) {
        imshow("Gray2Color", canvasImage);
    } else if (mode == BLUR) {
        blur(canvasImage, canvasImage, Size(5, 5));
    } else if (mode == XFLIP) {
        flip(canvasImage, canvasImage, 0);
    } else if (mode == YFLIP) {
        flip(canvasImage, canvasImage, 1);
    } else if (mode == XYFLIP) {
        flip(canvasImage, canvasImage, -1);
    } else if (mode == IS_SKINAREA) {
        Mat tmp = skinDetect(canvasImage);
        tmp.copyTo(canvasImage);
    } else if (mode == HISTOGRAM) {
        histogram(canvasImage);
    } else if (mode == CANNY) {
        Mat tmp;
        cvtColor(canvasImage, tmp, COLOR_BGR2GRAY);
        Canny(tmp, tmp, 100, 150);
        imshow("Edge", tmp);
    }
    
    imshow("Palette", paletteImage);
}
