
/*
 void place_icons(Size size);       아이콘 배치 함수
 void create_hueIndex(Rect rect);   색상 인덱스 함수. rect - 색상 인덱스 영역.
 void create_palette(int pos_y, Rect rect_palette); 색상 팔레트 생성 함수
                        pos_y: 마우스 클릭 y 좌표, rect_palette: 팔레트 영역.
*/

void place_icons(Size size) {           // size: 아이콘 크기
    vector<string> icon_name = {        // 아이콘 파일 이름
        "rect", "circle", "eclipe", "line", "brush", "eraser",
        "sticker", "clear", "open", "save", "plus", "minus",
        "color", ""
    };
    
    int btn_rows = (int)cvCeil(icon_name.size() / 2.0);     // 2열 버튼의 행 수.
    
    for (int i = 0, k = 0; i < btn_rows; i++) {
        for (int j = 0; j < 2; j++, k++) {
            Point pt(j * size.width, i * size.height);      // 각 아이콘 시작 위치
            icons.push_back(Rect(pt, size));
            
            Mat icon = imread("/Users/seonghyeon/xCode/opencv/opencv/final/icon/" + icon_name[k] + ".jpg", IMREAD_COLOR);
            
            if (icon.empty())
                continue;
            
            resize(icon, icon, size);                       // 아이콘 영상 크기 동일
            icon.copyTo(paletteImage(icons[k]));            // 아이콘 영상 복사
        }
    }
}

void create_hueIndex(Rect rect) {
    Mat m_hueIdx = paletteImage(rect);          // 색상 인덱스 영역 참조
    float ratio = 180.0f / rect.height;         // 색상 인덱스 세로크기의 hue 스케일
    
    for (int i = 0; i < rect.height; i++) {
        Scalar hue_color(i * ratio, 255, 255);  // HSV 색상 지정
        m_hueIdx.row(i).setTo(hue_color);       // 한 행의 색상 지정
    }
    cvtColor(m_hueIdx, m_hueIdx, COLOR_HSV2BGR);
}

void create_palette(int pos_y, Rect rect_palette) {
    Mat m_palette = paletteImage(rect_palette);     // 팔레트 영역 참조
    float ratio1 = 180.0f / rect_palette.height;    // 팔레트 높이에 따른 hue 비율
    float ratio2 = 256.0f / rect_palette.width;     // 팔레트 너비에 따른 saturation 비율
    float ratio3 = 256.0f / rect_palette.height;    // 팔레트 높이에 따른 intensity 비율
    
    hue = cvRound((pos_y - rect_palette.y) * ratio1);   // 색상팔레트 기본 색상
    
    for (int i = 0; i < m_palette.rows; i++) {      // 색상 팔레트 조회
        for (int j = 0; j < m_palette.cols; j++) {
            int saturation = cvRound(j * ratio2);   // 채도 계산(가로)
            int intensity = cvRound((m_palette.rows - i - 1) * ratio3); // 명도계산(세로)
            m_palette.at<Vec3b>(i, j) = Vec3b(hue, saturation, intensity);  // HSV색상 지정.
        }
    }
}
