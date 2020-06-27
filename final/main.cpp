//
//  main.cpp
//  final
//
//  Created by 임성현 on 2020/06/20.
//  Copyright © 2020 임성현. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

// 기본 이미지 전역변수
Mat paletteImage, canvasImage, funcMenuImage;

// 메뉴1 전역변수
vector<Rect> icons;                             // 아이콘 버튼 사각형들
int hue;                                        // hue
int mouse_mode = 0, draw_mode = 0;              // 마우스 상태, 그리기 모드
Point pt1, pt2;                                 // 마우스 이벤트 좌표 값들
Scalar Color(0, 0, 0);                          // 색상변수 - 전역변수 지정
int thickness = 3;                              // 선 굵기 - 전역변수

// 메뉴2 전역변수
vector<Rect> funcMenu;                          // menu 버튼 사각형들

// paintMenu 함수
void  onTrackbar(int value, void*) {            // 선 굵기 조절 트랙바.
    mouse_mode = 0;
}

// paintMenu 헤더파일
#include "paintMenu.hpp"
#include "icon_flag.hpp"                        // 아이콘 상수 헤더파일
#include "drawPerspective.hpp"
#include "drawPaint.hpp"
#include "paintMenuEvent.hpp"

// funcMenu 헤더파일
#include "funcMenu.hpp"
#include "func_flag.hpp"
#include "funcFFT.hpp"
#include "funcSKIN.hpp"
#include "funcHistogram.hpp"
#include "execFunc.hpp"
#include "funcMenuEvent.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    // 기본 이미지
    paletteImage = Mat(560, 800, CV_8UC3, Scalar(255, 255, 255));
    
    place_icons(Size(60, 60));                              // 아이콘 배치, 아이콘 크기
    
    Rect last_icon = *(icons.end() - 1);                    // 아이콘 사각형 마지막 원소
    Point start_palette = Point(0, last_icon.br().y + 5);   // 팔레트 시작 위치
    
    icons.push_back(Rect(start_palette, Size(100, 100)));   // 팔레트 사각형 추가
    icons.push_back(Rect(start_palette + Point(105, 0), Size(15, 100)));
                                                            // 색상 인덱스 사각형
    
    create_hueIndex(icons[HUE_IDX]);                        // 팔레트 생성
    create_palette(start_palette.y, icons[PALETTE]);        // 색상 인덱스 생성

    // funcMenu
    funcMenuImage = Mat(560, 600, CV_8UC3, Scalar(255, 255, 255));
    place_funcMenu(Size(200, 100));                       // 아이콘 배치, 아이콘 크기
    
    // Palette 창 만들고 paintMenu 마우스 이벤트, 선 굵기 트렉바 추가.
    namedWindow("Palette");
    setMouseCallback("Palette", onMouse_paint, &paletteImage);
    createTrackbar("Line Thickness", "Palette", &thickness, 20, onTrackbar);
    
    // funcMenu 창을 만들고 마우스 이벤트 추가
    namedWindow("funcMenu");
    setMouseCallback("funcMenu", onMouse_func, &funcMenuImage);
    
    imshow("Palette", paletteImage);
    imshow("funcMenu", funcMenuImage);
    
    int  x = icons[1].br().x;
    cout << "canvas x : " << x << endl;
    Rect canvas_rect(x, 0, paletteImage.cols - x, paletteImage.rows);
    canvasImage = paletteImage(canvas_rect);
    
    while (1) {
        if (mouse_mode == 1) {                  // 마우스 버튼 떼기
            draw(paletteImage, Color);                 // 원본에 그림
        } else if (mouse_mode == 3) {           // 마우스 드래그
            if (draw_mode == DRAW_BRUSH || draw_mode == ERASE) {
                draw(paletteImage, Color);             // 원본에 그림
            } else {
                draw(paletteImage.clone(), Scalar(200, 200, 200)); // 복사본에 회색으로 그림
            }
        }
        if (waitKey(30) == 27)  break;          // ESC 키 누르면 종료
    }
    
    waitKey();

    return 0;
}
