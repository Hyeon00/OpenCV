
/*
 isSkinArea 피부 영역 검출 헤더파일
 **/

void isSkinArea(Mat ycrcb[], Mat &Skin_img) {
    Skin_img = Mat(ycrcb[0].rows, ycrcb[0].cols, CV_8U);
    for (int i = 0; i < ycrcb[0].rows; i++) {
        for (int j = 0; j < ycrcb[0].cols; j++) {
            if (ycrcb[0].at<uchar>(i, j) > 80 &&
                ycrcb[1].at<uchar>(i, j) > 135 && ycrcb[1].at<uchar>(i, j) < 180 && ycrcb[2].at<uchar>(i, j) > 85 && ycrcb[2].at<uchar>(i, j) < 135) {
                Skin_img.at<uchar>(i, j) = 255;
            } else {
                Skin_img.at<uchar>(i, j) = 0;
            }
        }
    }
}
