#ifndef RECT_H
#define RECT_H

#include "Intrinsics.h"
#include "Line.h"
#include "Vector2.h"
#include "../Container/Vector.h"

typedef struct Rect {
    Vec2 p;
    Vec2 d;
} Rect;

Rect Rect_New(Vec2 p,Vec2 d){
    return (Rect){ p,d };
}
char Rect_Clip(Rect* r,float x, float y,float w,float h,float bx,float by,float bw,float bh){
	Rect r1 = Rect_New((Vec2){ x,y },(Vec2){ w,h });
    Rect r2 = Rect_New((Vec2){ bx,by },(Vec2){ bw,bh });
    if(r1.p.x<r2.p.x-r1.d.x || r1.p.y<r2.p.y-r1.d.y || r1.p.x>r2.p.x+r2.d.x || r1.p.y>r2.p.y+r2.d.y) 
        return 0;

    if(x < bx){
        w = x + w - bx;
        x = bx;
    }
    if(y < by){
        h = y + h - by;
        y = by;
    }
    if(x + w > bx + bw){
        w = bx + bw - x;
    }
    if(y + h > by + bh){
        h = by + bh - y;
    }
    *r = (Rect){ {x,y},{w,h} };
    return 1;
}
char Rect_Overlap(Rect r1,Rect r2){
    return !(
        (r1.p.x < r2.p.x - r1.d.x) ||
        (r1.p.x > r2.p.x + r2.d.x) ||
        (r1.p.y < r2.p.y - r1.d.y) ||
        (r1.p.y > r2.p.y + r2.d.y)
    );
}
char Rect_Contains(Rect r1,Rect r2){
	return (r2.p.x>=r1.p.x && r2.p.x+r2.d.x<r1.p.x+r1.d.x && r2.p.y>=r1.p.y && r2.p.y+r2.d.y<r1.p.y+r1.d.y);
}
char Rect_Point_Overlap(Rect r,Vec2 p){
    return (
        (p.x >= r.p.x) &&
        (p.y >= r.p.y) &&
        (p.x < r.p.x + r.d.x) &&
        (p.y < r.p.y + r.d.y)
    );
}

void Clip(float* x, float* y,float w,float h){
	if (*x < 0) 	*x = 0;
	if (*x >= w) 	*x = w;
	if (*y < 0) 	*y = 0;
	if (*y >= h) 	*y = h;
}
char Bounce(float x,float y,float w,float h){
    if(x>=0.0f && y>=0.0f && x<w && y<h) return 1;
    return 0;
}

void Rect_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Rect r,unsigned int col,float StrokeSize){
    if(!Rect_Clip(&r,r.p.x,r.p.y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height)){
        return;
    }
	
    Line_RenderX(Target,Target_Width,Target_Height,(Vec2){r.p.x,r.p.y},(Vec2){r.p.x+r.d.x,r.p.y},col,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,(Vec2){r.p.x,r.p.y},(Vec2){r.p.x,r.p.y+r.d.y},col,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,(Vec2){r.p.x+r.d.x,r.p.y},(Vec2){r.p.x+r.d.x,r.p.y+r.d.y},col,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,(Vec2){r.p.x,r.p.y+r.d.y},(Vec2){r.p.x+r.d.x,r.p.y+r.d.y},col,StrokeSize);
}
void Rect_RenderXWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,unsigned int col,float StrokeSize){
    Rect_RenderWire(Target,Target_Width,Target_Height,(Rect){ p,d },col,StrokeSize);
}
void Rect_RenderXXWire(unsigned int* Target,int Target_Width,int Target_Height,float x,float y,float w,float h,unsigned int col,float StrokeSize){
    Rect_RenderWire(Target,Target_Width,Target_Height,(Rect){ {x,y},{w,h} },col,StrokeSize);
}

void Rect_Render(unsigned int* Target,int Target_Width,int Target_Height,Rect r,unsigned int col){
    if(!Rect_Clip(&r,r.p.x,r.p.y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height)){
        return;
    }

	for (int py = (int)r.p.y; py < (int)r.p.y + (int)r.d.y; py++)
		Memset_i32((unsigned int*)(Target+py*Target_Width+(int)(r.p.x)),col,(int)(r.d.x));
}
void Rect_RenderX(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d, unsigned int col){
    Rect_Render(Target,Target_Width,Target_Height,(Rect){ p,d },col);
}
void Rect_RenderXX(unsigned int* Target,int Target_Width,int Target_Height,float x,float y,float w,float h,unsigned int col){
    Rect_Render(Target,Target_Width,Target_Height,(Rect){ {x,y},{w,h} },col);
}

void Rect_RenderAlpha(unsigned int* Target,int Target_Width,int Target_Height,Rect r,unsigned int col){
    if(!Rect_Clip(&r,r.p.x,r.p.y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height)){
        return;
    }

	for(int py = (int)r.p.y; py < (int)r.p.y + (int)r.d.y; py++)
		Memset_Alpha((unsigned int*)(Target+py*Target_Width+(int)(r.p.x)),col,(int)(r.d.x));
}
void Rect_RenderXAlpha(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d, unsigned int col){
    Rect_RenderAlpha(Target,Target_Width,Target_Height,(Rect){ p,d },col);
}
void Rect_RenderXXAlpha(unsigned int* Target,int Target_Width,int Target_Height,float x,float y,float w,float h,unsigned int col){
    Rect_RenderAlpha(Target,Target_Width,Target_Height,(Rect){ {x,y},{w,h} },col);
}



typedef struct RectD {
    Vdc2 p;
    Vdc2 d;
} RectD;

RectD RectD_New(Vdc2 p,Vdc2 d){
    return (RectD){ p,d };
}
char RectD_Point_Overlap(const RectD* r,const Vdc2 p){
    return (
        (p.x >= r->p.x) &&
        (p.y >= r->p.x) &&
        (p.x < r->p.x + r->d.x) &&
        (p.y < r->p.y + r->d.y)
    );
}

#endif