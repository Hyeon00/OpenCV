
/*
 static void onMouse_paint(int event, int x, int y, int flags, void*);마우스 이벤트 함수
*/

static void onMouse_paint(int event, int x, int y, int flags, void*) {
    Point2f pt(x, y);
    if (event == EVENT_LBUTTONUP) {                         // 왼쪽 버튼 떼기
        for (int i = 0; i < (int)icons.size(); i++) {       // 메뉴 아이콘 사각형 조회
            if (icons[i].contains(pt)) {                    // 메뉴 클릭 여부 검사
                if (i < 7) {                                // 그리기 명령이면
                    mouse_mode = 0;                         // 마우스 상태 초기화
                    draw_mode = i;                          // 그리기 모드 활성화
                } else {
                    command(i);                             // 일반 명령이면
                }
                return;
            }
        }
        pt2 = pt;                                           // 종료 좌표 저장
        mouse_mode = 1;
        //cout << "pt2: " << pt2 << endl;
    } else if (event == EVENT_LBUTTONDOWN) {                // 왼쪽 버튼 누르기
        pt1 = pt;                                           // 시작 좌표 저장
        mouse_mode = 2;
        //cout << "pt1: " << pt1 << endl;
    }
    
    if (mouse_mode >= 2) {                                  // 왼쪽 버튼 누르기 or 드래그
        Rect rect(0, 0, 125, paletteImage.rows);
        mouse_mode = (rect.contains(pt)) ? 0 : 3;           // 마우스 상태 지정
        pt2 = pt;
    }
}

