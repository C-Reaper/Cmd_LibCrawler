#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Intrinsics.h"
#include "Point.h"
#include "Line.h"
#include "Math.h"
#include "../Container/Vector.h"

typedef struct Triangle{
    Vec2 p1;
    Vec2 p2;
    Vec2 p3;
} Triangle;

Triangle Triangle_New(Vec2 p1, Vec2 p2, Vec2 p3){
	return (Triangle){ p1,p2,p3 };
}
void Triangle_Render(unsigned int* Target,int Target_Width,int Target_Height,Triangle t,unsigned int c){
	// if((t.p1.x<0.0f || t.p1.x>=Target_Width || t.p1.y<0.0f || t.p1.y>=Target_Height) &&
	//    (t.p2.x<0.0f || t.p2.x>=Target_Width || t.p2.y<0.0f || t.p2.y>=Target_Height) &&
	//    (t.p3.x<0.0f || t.p3.x>=Target_Width || t.p3.y<0.0f || t.p3.y>=Target_Height))
	//    return;
	if((t.p1.x<0.0f && t.p2.x<0.0f && t.p3.x<0.0f) ||
	   (t.p1.y<0.0f && t.p2.y<0.0f && t.p3.y<0.0f) ||
	   (t.p1.x>=Target_Width  && t.p2.x>=Target_Width  && t.p3.x>=Target_Width) ||
	   (t.p1.y>=Target_Height && t.p2.y>=Target_Height && t.p3.y>=Target_Height))
	   return;
	
	Vec2 h1 = t.p1;
	Vec2 h2 = t.p2;
	Vec2 h3 = t.p3;
	if(t.p1.y<t.p2.y){
		if(t.p1.y<t.p3.y){
			h1 = t.p1;
			if(t.p2.y<t.p3.y){
				h2 = t.p2;
				h3 = t.p3;
			}else{
				h2 = t.p3;
				h3 = t.p2;
			}
		}else{
			h1 = t.p3;
			h2 = t.p1;
			h3 = t.p2;
		}
	}else{
		if(t.p2.y<t.p3.y){
			h1 = t.p2;
			if(t.p1.y<t.p3.y){
				h2 = t.p1;
				h3 = t.p3;
			}else{
				h2 = t.p3;
				h3 = t.p1;
			}
		}else{
			h1 = t.p3;
			h2 = t.p2;
			h3 = t.p1;
		}
	}

	if((int)h1.y==(int)h3.y) return;

	float dy1 = (h3.y - h1.y);
	float dy2 = (h2.y - h1.y);	
	if(dy1 < 1.0f) dy1 = 1.0f;
	if(dy2 < 1.0f) dy2 = 1.0f;
	float x1 = h1.x;
	float x2 = h1.x;
	float dir1 = (h3.x - h1.x) / dy1;
	float dir2 = (h2.x - h1.x) / dy2;
	int y = (int)h1.y;

	for(;y<(int)h2.y || F32_Abs(h2.x - x2)>1.0f;y++){
		if(y>=Target_Height) return;
		
		x1 += dir1;
		if(F32_Passes(x2,dir2,h2.x)) 	x2 = h2.x;
		else 							x2 += dir2;

		int sx = (int)F32_Clamp((x1<x2?x1:x2) - 2,0.0f,Target_Width);// Containing Borders
		int bx = (int)F32_Clamp((x1>x2?x1:x2) + 1,0.0f,Target_Width);
		if(bx==sx) continue;
		if(y*Target_Width+sx >= 0 && y*Target_Width+bx < Target_Width * Target_Height)
			Memset_i32((unsigned int*)(Target+y*Target_Width+sx),c,bx-sx);
	}

	dy2 = (h3.y - h2.y);
	if(dy2 < 1.0f) dy2 = 1.0f;
	dir2 = (h3.x - h2.x) / dy2;
	for(;y<(int)h3.y;y++){
		if(y>=Target_Height) return;

		if(F32_Passes(x1,dir1,h3.x)) 	x1 = h3.x;
		else 							x1 += dir1;
		if(F32_Passes(x2,dir2,h3.x))    x2 = h3.x;
		else 							x2 += dir2;
		
		int sx = (int)F32_Clamp((x1<x2?x1:x2) - 2,0.0f,Target_Width);// Containing Borders
		int bx = (int)F32_Clamp((x1>x2?x1:x2) + 1,0.0f,Target_Width);
		if(bx==sx) continue;
		if(y*Target_Width+sx >= 0 && y*Target_Width+bx < Target_Width * Target_Height)
			Memset_i32((unsigned int*)(Target+y*Target_Width+sx),c,bx-sx);
	}
}
void Triangle_RenderS(unsigned int* Target,int Target_Width,int Target_Height,Triangle t,unsigned int c){
	if((t.p1.x<0.0f && t.p2.x<0.0f && t.p3.x<0.0f) ||
	   (t.p1.y<0.0f && t.p2.y<0.0f && t.p3.y<0.0f) ||
	   (t.p1.x>=Target_Width  && t.p2.x>=Target_Width  && t.p3.x>=Target_Width) ||
	   (t.p1.y>=Target_Height && t.p2.y>=Target_Height && t.p3.y>=Target_Height))
	   return;
	
	Vec2 h1 = t.p1;
	Vec2 h2 = t.p2;
	Vec2 h3 = t.p3;
	if(t.p1.y<t.p2.y){
		if(t.p1.y<t.p3.y){
			h1 = t.p1;
			if(t.p2.y<t.p3.y){
				h2 = t.p2;
				h3 = t.p3;
			}else{
				h2 = t.p3;
				h3 = t.p2;
			}
		}else{
			h1 = t.p3;
			h2 = t.p1;
			h3 = t.p2;
		}
	}else{
		if(t.p2.y<t.p3.y){
			h1 = t.p2;
			if(t.p1.y<t.p3.y){
				h2 = t.p1;
				h3 = t.p3;
			}else{
				h2 = t.p3;
				h3 = t.p1;
			}
		}else{
			h1 = t.p3;
			h2 = t.p2;
			h3 = t.p1;
		}
	}

	if((int)h1.y==(int)h3.y) return;

	//const float xmin = F32_Max(0.0f,F32_Min(F32_Min(h1.x,h2.x),h3.x));
	//const float xmax = F32_Min(Target_Width,F32_Max(F32_Max(h1.x,h2.x),h3.x));
	
	const float x12min = F32_Min(h1.x,h2.x);
	const float x12max = F32_Max(h1.x,h2.x);
	const float x13min = F32_Min(h1.x,h3.x);
	const float x13max = F32_Max(h1.x,h3.x);
	const float x23min = F32_Min(h2.x,h3.x);
	const float x23max = F32_Max(h2.x,h3.x);

	const float cx12min = F32_Max(0.0f,F32_Min(h1.x,h2.x));
	const float cx12max = F32_Min(Target_Width,F32_Max(h1.x,h2.x));
	const float cx13min = F32_Max(0.0f,F32_Min(h1.x,h3.x));
	const float cx13max = F32_Min(Target_Width,F32_Max(h1.x,h3.x));
	const float cx23min = F32_Max(0.0f,F32_Min(h2.x,h3.x));
	const float cx23max = F32_Min(Target_Width,F32_Max(h2.x,h3.x));

	//printf("%f,%f %f,%f %f,%f\n",x12min,x12max,x13min,x13max,x23min,x23max);

	float dy1 = (h3.y - h1.y);
	float dy2 = (h2.y - h1.y);
	if(dy1 < 1.0f) dy1 = 1.0f;
	if(dy2 < 1.0f) dy2 = 1.0f;

	float x1 = h1.x;
	float x2 = h1.x;
	float dir1 = (h3.x - h1.x) / dy1;
	float dir2 = (h2.x - h1.x) / dy2;
	
	int y = (int)h1.y;

	for(;y<=(int)h2.y;y++){
		if(y>=Target_Height) return;
		
		x1 += dir1;
		x2 += dir2;

		x1 = F32_Clamp(x1,x13min,x13max);
		x2 = F32_Clamp(x2,x12min,x12max);

		int sx;
		if(x1<x2) 	sx = (int)F32_Clamp(x1 - F32_Abs(dir1),cx13min,cx13max);
		else		sx = (int)F32_Clamp(x2 - F32_Abs(dir2),cx12min,cx12max);

		int bx;
		if(x1>x2) 	bx = (int)F32_Clamp(x1 + F32_Abs(dir1),cx13min,cx13max);
		else		bx = (int)F32_Clamp(x2 + F32_Abs(dir2),cx12min,cx12max);

		if(bx<=sx)
			continue;

		if(y*Target_Width+sx >= 0 && y*Target_Width+bx < Target_Width * Target_Height)
			Memset_i32((unsigned int*)(Target+y*Target_Width+sx),c,bx-sx);
	}

	dy2 = (h3.y - h2.y);
	if(dy2 < 1.0f) dy2 = 1.0f;
	dir2 = (h3.x - h2.x) / dy2;

	for(;y<=(int)h3.y;y++){
		if(y>=Target_Height) return;

		x1 += dir1;
		x2 += dir2;

		x1 = F32_Clamp(x1,x13min,x13max);
		x2 = F32_Clamp(x2,x23min,x23max);
		
		int sx;
		if(x1<x2) 	sx = (int)F32_Clamp(x1 - F32_Abs(dir1),cx13min,cx13max);
		else		sx = (int)F32_Clamp(x2 - F32_Abs(dir2),cx23min,cx23max);

		int bx;
		if(x1>x2) 	bx = (int)F32_Clamp(x1 + F32_Abs(dir1),cx13min,cx13max);
		else		bx = (int)F32_Clamp(x2 + F32_Abs(dir2),cx23min,cx23max);

		if(bx<=sx) continue;

		if(y*Target_Width+sx >= 0 && y*Target_Width+bx < Target_Width * Target_Height)
			Memset_i32((unsigned int*)(Target+y*Target_Width+sx),c,bx-sx);
	}
}
void Triangle_RenderX(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p1,Vec2 p2,Vec2 p3,unsigned int c){
	Triangle_RenderS(Target,Target_Width,Target_Height,Triangle_New(p1,p2,p3),c);
}
void Triangle_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Triangle t,unsigned int c,F32 StrokeSize){
	Line_RenderX(Target,Target_Width,Target_Height,t.p1,t.p2,c,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,t.p2,t.p3,c,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,t.p3,t.p1,c,StrokeSize);
}
void Triangle_RenderXWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p1,Vec2 p2,Vec2 p3,unsigned int c,F32 StrokeSize){
	Line_RenderX(Target,Target_Width,Target_Height,p1,p2,c,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,p2,p3,c,StrokeSize);
    Line_RenderX(Target,Target_Width,Target_Height,p3,p1,c,StrokeSize);
}

#endif // !TRIANGLE_H