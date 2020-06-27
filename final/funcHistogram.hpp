
/*
 히스토그램 계산 및 그리기 함수들.
 **/

Mat calc_myHistogram(Mat m, int nBins) {
    Mat hist;
    int channels[] = { 0 };     // 채널 목록
    int hitSize[] = { nBins };  // bin 개수
    float range[] = { 0, 256 }; // 0 - 255까지, 조각내자.
    const float* ranges[] = { range };
    calcHist(&m, 1, channels, Mat(), hist, 1, hitSize, ranges);
    return hist;
}

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

void create_hist(Mat img, Mat &hist, Mat &hist_img) {
    int histsize = 256, range = 256;
    hist = calc_myHistogram(img, histsize);
    draw_histo(hist, hist_img);
}

