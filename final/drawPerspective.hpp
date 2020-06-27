
Mat display_image;
Point2f p[4];
int nPoint = 0;

Mat getPerspectiveMatrix() {
    Mat m;
    
    int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2));
    int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y, 2));
    Point2f p_new[4];
    p_new[1] = p[1];
    p_new[0].x = p[1].x;
    p_new[0].y = p[1].y - L1;
    p_new[2].x = p[1].x + L2;
    p_new[2].y = p[1].y;
    p_new[3].x = p[1].x + L2;
    p_new[3].y = p[1].y - L1;
    
    m = getPerspectiveTransform(p, p_new);
    
    return m;
}
