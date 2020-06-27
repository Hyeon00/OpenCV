
/*
 void place_funcMenu(Size size);       메뉴 배치 함수
*/

void place_funcMenu(Size size) {              // size: 아이콘 크기
    vector<string> funcMenu_name = {                      // 아이콘 파일 이름
        "AddPointNoise", "AddLineNoise", "FFT", "Gray", "Color", "Blur",
        "XFlip", "YFlip", "XYFlip", "SkinDetect", "Histogram", "Edge"
    };
    
    int btn_rows = (int)cvCeil(funcMenu_name.size() / 3.0);     // 2열 버튼의 행 수.
    
    for (int i = 0, k = 0; i < btn_rows; i++) {
        for (int j = 0; j < 3; j++, k++) {
            Point pt(j * size.width, i * size.height);      // 각 아이콘 시작 위치
            funcMenu.push_back(Rect(pt, size));
            
            rectangle(funcMenuImage, Rect(pt, size), Scalar(0), 1);
            putText(funcMenuImage, funcMenu_name[k], Point(pt.x + 30, pt.y + 55), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0), 2);
            
            //resize(menuBtn, menuBtn, size);                  // 아이콘 영상 크기 동일
            //menuBtn.copyTo(menuImage(menu2[k]));             // 아이콘 영상 복사
        }
    }
}
