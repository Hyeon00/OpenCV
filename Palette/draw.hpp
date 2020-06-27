
/*
 void draw();               그리기 함수
 void command(int mode);    일반 명령 수행 함수
 
*/

void draw(Mat image, Scalar color = Scalar(200, 200, 200)) {
    if (draw_mode == DRAW_RECTANGLE) {
        rectangle(image, pt1, pt2, color, thickness);
    } else if (draw_mode == DRAW_LINE) {
        line(image, pt1, pt2, color, thickness);
    } else if (draw_mode == DRAW_BRUSH) {
        line(image, pt1, pt2, color, thickness * 3);
        pt1 = pt2;
    } else if (draw_mode == ERASE) {
        line(image, pt1, pt2, Scalar(255, 255, 255), thickness * 5);
        pt1 = pt2;
    } else if (draw_mode == DRAW_CIRCLE) {
        Point2d pt3 = pt1 - pt2;
        int radius = (int)sqrt(pt3.x * pt3.x + pt3.y * pt3.y);  // 두 좌표간 거리
        circle(image, pt1, radius, color, thickness);
    } else if (draw_mode == DRAW_ECLIPSE) {
        Point2d center = (pt1 + pt2) / 2.0;         // 두 좌표 중심
        Size_<int> size = (pt1 - pt2) / 2.0;          // 두 좌표 차분 - 타원 크기
        size.width = abs(size.width);
        size.height = abs(size.height);
        ellipse(image, center, size, 0, 0, 360, color, thickness);
    }
    
    imshow("그림판", image);
}

void command(int mode) {                    // 일반 명령 수행 함수
    if (mode == PALETTE) {                  // 팔레트 영역 클릭
        float ratio1 = 256.0f / icons[PALETTE].width;       // 너비로 채도 비율
        float ratio2 = 256.0f / icons[PALETTE].height;      // 높이로 명도 비율
        
        Point pt = pt2 - icons[PALETTE].tl();               // 팔레트 내 클릭 좌표
        int saturation = cvRound(pt.x * ratio1);
        int value = cvRound((icons[PALETTE].height - pt.y - 1) * ratio2);
        Scalar hsv(hue, saturation, value);                 // HSV 색상 지정
        
        Mat m_color = image(icons[COLOR]);                  // 색상 아이콘 참조
        m_color.setTo(hsv);
        cvtColor(m_color, m_color, COLOR_HSV2BGR);          // HSV -> BGR
        rectangle(image, icons[COLOR], Scalar(0, 0, 0), 1); // 색상 아이콘 테두리 색
        
        Color = Scalar(m_color.at<Vec3b>(10, 10));
    } else if (mode == HUE_IDX) {                   // 색상 인덱스 클릭 시
        create_palette(pt2.y, icons[PALETTE]);      // 팔레트 다시 그리기
    } else if (mode == CLEAR) {                     // 캔버스 영역 전체 지우기
        canvas.setTo(Scalar(255, 255, 255));
        mouse_mode = 0;
    } else if (mode == OPEN) {
        Mat tmp = imread("/Users/seonghyeon/xCode/opencv/opencv/Palette/image/G.jpg", 1);
        if (tmp.empty()) {
            cout << "Error opening image" << endl;
            exit(EXIT_FAILURE);
        }
        
        resize(tmp, tmp, canvas.size());
        tmp.copyTo(canvas);
    } else if (mode == SAVE) {
        imwrite("/Users/seonghyeon/xCode/opencv/opencv/Palette/image/my_save.jpg", canvas);
    } else if (mode == PLUS) {
        canvas += Scalar(10, 10, 10);           // 영상 밝게
    } else if (mode == MINUS) {
        canvas -= Scalar(10, 10, 10);           // 어둡게
    }
    
    imshow("그림판", image);
}
