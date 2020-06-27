
/*
 static void onMouse_func(int event, int x, int y, int flags, void*); 마우스 이벤트 함수
*/

static void onMouse_func(int event, int x, int y, int flags, void*) {
    Point pt(x, y);
    if (event == EVENT_LBUTTONDOWN) {                          // 왼쪽 버튼 누르기
        for (int i = 0; i < (int)funcMenu.size(); i++) {       // 메뉴 아이콘 사각형 조회
            if (funcMenu[i].contains(pt)) {                    // 메뉴 클릭 여부 검사
                command_func(i);
                return;
            }
        }
    }
}
