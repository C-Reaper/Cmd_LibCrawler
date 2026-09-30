#ifndef RAY_H
#define RAY_H

#include "Intrinsics.h"
#include "Line.h"
#include "Point.h"
#include "Math.h"

typedef struct Ray{
    float m;
    float t;
} Ray;

Ray Ray_New(float m,float t){
    Ray r;
    r.m = m;
    r.t = t;
    return r;
}
float Ray_Y(Ray r,float x){
    return r.m * x + r.t;
}
float Ray_X(Ray r,float y){
    return (y - r.t) / r.m;
}
Ray Ray_ByLine(Line l){
    Vec2 d = Vec2_Sub(l.e,l.s);
    float m = INFINITY;
    if(d.x!=0.0f) m = d.y / d.x;
    float t = -m * l.s.x + l.s.y;
    return Ray_New(m,t);
}

void Ray_Render(unsigned int* Target,int Target_Width,int Target_Height,Ray r,unsigned int c,F32 StrokeSize){
	Vec2 pt = { Ray_X(r,0.0f),0.0f };
    Vec2 pb = { Ray_X(r,Target_Height),Target_Height };
    Line_RenderX(Target,Target_Width,Target_Height,pt,pb,c,StrokeSize);
}
void Ray_RenderX(unsigned int* Target,int Target_Width,int Target_Height,float m,float t,unsigned int c,F32 StrokeSize){
	Ray r = Ray_New(m,t);
    Ray_Render(Target,Target_Width,Target_Height,r,c,StrokeSize);
}

void Ray_DirRender(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,float len,float a,unsigned int c,F32 StrokeSize){
	// l = (ux * m - px * m + py - uy) / (vy - vx * m)
    
    const Vec2 v = Vec2_OfAngle(a);
    const float tx = p.x + v.x * len;
    const float ty = p.y + v.y * len;
    Line_RenderX(Target,Target_Width,Target_Height,p,(Vec2){ tx,ty },c,StrokeSize);
}

#endif // !RAY_H