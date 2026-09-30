#ifndef POINT_H
#define POINT_H

#include "Intrinsics.h"
#include "Math.h"

#define POINT_INVALID    -1E38f

typedef Vec2 Point;

Point Point_New(float x,float y){
    Point p;
    p.x = x;
    p.y = y;
    return p;
}
char Point_Valid(Point p){
    return (p.x!=POINT_INVALID || p.y!=POINT_INVALID);
}
float Point_Sign(Point p1, Point p2, Point p3){
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

float Point_Dist2(Point p1,Point p2){
    const float dx = p1.x - p2.x;
    const float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}
float Point_Dist(Point p1,Point p2){
    return sqrtf(Point_Dist2(p1,p2));
}

void Point_Render(unsigned int* Target,int Target_Width,int Target_Height,Point p,unsigned int c){
	if(p.x>=0.0f && p.x<Target_Width && p.y>=0.0f && p.y<Target_Height){
        Target[(int)p.y * Target_Width + (int)p.x] = c;
    }
}
void Point_RenderX(unsigned int* Target,int Target_Width,int Target_Height,float x,float y,unsigned int c){
	Point_Render(Target,Target_Width,Target_Height,(Point){ x,y },c);
}

void Point_RenderAlpha(unsigned int* Target,int Target_Width,int Target_Height,Point p,unsigned int c){
    if (p.x >= 0 && p.x < Target_Width && p.y >= 0 && p.y < Target_Height){
		unsigned int Dst = Target[(int)p.y * Target_Width + (int)p.x];
        float Src_alpha = (float)(c >> 24) / 255.0f;
        float Dest_alpha = (float)(Dst >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((c >> 16) & 0xFF) * Src_alpha + (float)((Dst >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((c >> 8) & 0xFF)  * Src_alpha + (float)((Dst >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(c & 0xFF)         * Src_alpha + (float)(Dst & 0xFF)         * Part_alpha) / Out_alpha);

        Target[(int)p.y * Target_Width + (int)p.x] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
	}
}
void Point_RenderXAlpha(unsigned int* Target,int Target_Width,int Target_Height,float x,float y,unsigned int c){
    Point_RenderAlpha(Target,Target_Width,Target_Height,(Vec2){ x,y },c);
}

#endif // !RAY_H