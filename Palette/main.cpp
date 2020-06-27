//
//  main.cpp
//  Palette
//
//  Created by 임성현 on 2020/06/19.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* 전역변수 */
vector<Rect> icons;                     // 아이콘 버튼 사각형들
Mat image, canvas;                      // 그림판 영상, 캔버스 영상
int hue;                                // hue
int mouse_mode = 0, draw_mode = 0;      // 마우스 상태, 그리기 모드
Point pt1, pt2;                         // 마우스 이벤트 좌표 값들
Scalar Color(0, 0, 0);                  // 색상변수 - 전역변수 지정
int thickness = 3;                      // 선 굵기 - 전역변수

#include "icon_flag.hpp"                // 아이콘 상수 헤더파일
#include "menu.hpp"                     // 아이콘 버튼 추가 헤더파일
#include "draw.hpp"                     // 그리기 및 명령 파일
#include "event.hpp"                    // 마우스 이벤트 파일

void  onTrackbar(int value, void*) {
    mouse_mode = 0;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    image = Mat(500, 800, CV_8UC3, Scalar(255, 255, 255));
    
    place_icons(Size(60, 60));                              // 아이콘 배치, 아이콘 크기
    
    Rect last_icon = *(icons.end() - 1);                    // 아이콘 사각형 마지막 원소
    Point start_palette = Point(0, last_icon.br().y + 5);   // 팔레트 시작 위치
    
    icons.push_back(Rect(start_palette, Size(100, 100)));   // 팔레트 사각형 추가
    icons.push_back(Rect(start_palette + Point(105, 0), Size(15, 100)));
                                                            // 색상 인덱스 사각형
    
    create_hueIndex(icons[HUE_IDX]);                        // 팔레트 생성
    create_palette(start_palette.y, icons[PALETTE]);        // 색상 인덱스 생성
    
    imshow("그림판", image);
    setMouseCallback("그림판", onMouse);
    createTrackbar("선 굵기", "그림판", &thickness, 20, onTrackbar);
    int  x = icons[1].br().x;
    Rect canvas_rect(x, 0, image.cols - x, image.rows);
    canvas = image(canvas_rect);
    
    while (1) {
        if (mouse_mode == 1) {                  // 마우스 버튼 떼기
            draw(image, Color);                 // 원본에 그림
        } else if (mouse_mode == 3) {           // 마우스 드래그
            if (draw_mode == DRAW_BRUSH || draw_mode == ERASE) {
                draw(image, Color);             // 원본에 그림
            } else {
                draw(image.clone(), Scalar(200, 200, 200)); // 복사본에 회색으로 그림
            }
        }
        if (waitKey(30) == 27)  break;          // ESC 키 누르면 종료
    }
    
    waitKey(0);
    
    return 0;
}









