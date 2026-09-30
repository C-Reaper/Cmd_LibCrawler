#ifndef CIRCLE_H
#define CIRCLE_H

#include "Intrinsics.h"
#include "Line.h"
#include "Math.h"

typedef struct Circle{
    Vec2 p;
    F32 r;
} Circle;

Circle Circle_New(Vec2 p,F32 r){
    Circle c;
    c.p = p;
    c.r = r;
    return c;
}

char Circle_Point(Circle* c,Vec2 p){
    float d = Vec2_Mag(Vec2_Sub(p,c->p));
    return d < c->r;
}
char Circle_Circle_Overlap(Circle* b1,Circle* b2){
    float d = Vec2_Mag(Vec2_Sub(b2->p,b1->p));
    return d < (b1->r + b2->r);
}
void Circle_Circle_Static(Circle* b1,Circle* b2){
    Vec2 d = Vec2_Sub(b1->p,b2->p);
	float h = Vec2_Mag(d);
	float Overlap = 0.5f * (h - b1->r - b2->r );
	b1->p = Vec2_Sub(b1->p,Vec2_Divf(Vec2_Mulf(d,Overlap),h));
	b2->p = Vec2_Add(b2->p,Vec2_Divf(Vec2_Mulf(d,Overlap),h));
}
void Circle_Circle_Dynamic(Circle* b1,Vec2* v1,float m1,Circle* b2,Vec2* v2,float m2,float eff){
    Circle_Circle_Static(b1,b2);
	
    Vec2 d = Vec2_Sub(b1->p,b2->p);
	float h = Vec2_Mag(d);

	Vec2 n = Vec2_Divf(d,h);
	Vec2 t = Vec2_Perp(n);
	
	float d1 = Vec2_Dot(*v1,t);
	float d2 = Vec2_Dot(*v2,t);
	float dn1 = Vec2_Dot(*v1,n);
	float dn2 = Vec2_Dot(*v2,n);
	
	float bm1 = eff * (dn1 * (m1 - m2) + 2.0f * m2 * dn2) / (m1 + m2);
	float bm2 = eff * (dn2 * (m2 - m1) + 2.0f * m1 * dn1) / (m1 + m2);
	
	*v1 = Vec2_Add(Vec2_Mulf(t,d1),Vec2_Mulf(n,bm1));
	*v2 = Vec2_Add(Vec2_Mulf(t,d2),Vec2_Mulf(n,bm2));
}

// C - curved rect with circular corners
// R - ratio between w / h -> Oval

void Circle_Render(unsigned int* Target,int Target_Width,int Target_Height,Circle c,unsigned int col){
    const float yS = F32_Clamp(c.p.y - c.r,0.0f,Target_Height);
    const float yM = F32_Clamp(c.p.y + c.r,0.0f,Target_Height);
    
    for(int i = (int)yS;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - c.p.y) / c.r,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(c.p.x - cosf(a) * c.r,0.0f,Target_Width);
        const float x2 = F32_Clamp(c.p.x + cosf(a) * c.r,0.0f,Target_Width);
        Memset_i32((unsigned int*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_RenderX(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,F32 r,unsigned int c){
    Circle_Render(Target,Target_Width,Target_Height,Circle_New(p,r),c);
}
void Circle_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Circle c,unsigned int col,float STOKE){
    for(float i = 0.0f;i<2*3.1415f;i+=1.0f/c.r){
        const float x = cosf(i) * c.r + c.p.x;
        const float y = sinf(i) * c.r + c.p.y;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
}
void Circle_RenderXWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,F32 r,unsigned int c,float STOKE){
    Circle_RenderWire(Target,Target_Width,Target_Height,Circle_New(p,r),c,STOKE);
}
void Circle_R_Render(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,unsigned int col){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);
    const float ratio = d.x / d.y;

    for(int i = (int)yS;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - p.y) / d.y,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - cosf(a) * d.y * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + cosf(a) * d.y * ratio,0.0f,Target_Width);
        Memset_i32((unsigned int*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_R_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,unsigned int col,float STOKE){
    if(d.x <= 0.0f || d.y <= 0.0f) return;
    
    const float max = F32_Max(d.x,d.y);

    for(float i = 0.0f;i<F32_PI2;i+=1.0f/max){
        const float x = p.x + cosf(i) * d.x;
        const float y = p.y + sinf(i) * d.y;
        
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
}
void Circle_C_Render(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,Vec2 curve,unsigned int col){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);

    const float xS = F32_Clamp(p.x - d.x,0.0f,Target_Width);
    const float xM = F32_Clamp(p.x + d.x,0.0f,Target_Width);

    const float ratio = (d.x * curve.x) / (d.y * curve.y);
    const float xCurve = curve.x * d.x;
    const float yCurve = curve.y * d.y;

    const float xmid = (d.x - xCurve);
    
    const float hrect = (yM - yS - yCurve * 2.0f);
    
    int i = (int)yS;
    for(;i<(int)(yS + yCurve);i++){
        const float v = F32_Clamp((float)(i - (yS + yCurve)) / yCurve,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - xmid - cosf(a) * yCurve * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + xmid + cosf(a) * yCurve * ratio,0.0f,Target_Width);
        Memset_i32((unsigned int*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
    for(;i<(int)(yS + yCurve + hrect);i++){
        Memset_i32((unsigned int*)(Target+(int)(i)*Target_Width+(int)(xS)),col,(int)(xM-xS));
    }
    for(;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - (yS + yCurve + hrect)) / yCurve,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - xmid - cosf(a) * yCurve * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + xmid + cosf(a) * yCurve * ratio,0.0f,Target_Width);
        Memset_i32((unsigned int*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_C_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,Vec2 curve,unsigned int col,float STOKE){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);

    const float xS = F32_Clamp(p.x - d.x,0.0f,Target_Width);
    const float xM = F32_Clamp(p.x + d.x,0.0f,Target_Width);

    const float ratio = curve.x / curve.y;
    const float xCurve = curve.x * d.x;
    const float yCurve = curve.y * d.y;

    const float xmid = (d.x - xCurve);
    const float ymid = (d.y - yCurve);
    
    const float hrect = (yM - yS - yCurve * 2.0f);
    
    const float max = F32_Max(d.x,d.y);

    for(float i = F32_PI;i<F32_PI * 1.5f;i+=1.0f/max){
        const float x = (p.x - xmid) + cosf(i) * xCurve;
        const float y = (p.y - ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    for(float i = F32_PI * 1.5f;i<F32_PI2;i+=1.0f/max){
        const float x = (p.x + xmid) + cosf(i) * xCurve;
        const float y = (p.y - ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    
    const float x1 = F32_Clamp(p.x - xmid,0.0f,Target_Width);
    const float x2 = F32_Clamp(p.x + xmid,0.0f,Target_Width);
    Memset_i32((unsigned int*)(Target+(int)(yS)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    
    for(int i = (int)yS + yCurve;i<(int)(yS + yCurve + hrect);i++){
        if(xS>=0.0f && xS<Target_Width && i>=0 && i<Target_Height)
           Target[i * Target_Width + (int)xS] = col;
        if(xM>=0.0f && xM<Target_Width && i>=0 && i<Target_Height)
           Target[i * Target_Width + (int)xM] = col;
    }
    
    for(float i = F32_PI05;i<F32_PI;i+=1.0f/max){
        const float x = (p.x - xmid) + cosf(i) * xCurve;
        const float y = (p.y + ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    for(float i = 0.0f;i<F32_PI05;i+=1.0f/max){
        const float x = (p.x + xmid) + cosf(i) * xCurve;
        const float y = (p.y + ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    Memset_i32((unsigned int*)(Target+(int)(yM)*Target_Width+(int)(x1)),col,(int)(x2-x1));
}


void Circle_RenderG(float* Target,int Target_Width,int Target_Height,Circle c,float col){
    const float yS = F32_Clamp(c.p.y - c.r,0.0f,Target_Height);
    const float yM = F32_Clamp(c.p.y + c.r,0.0f,Target_Height);
    
    for(int i = (int)yS;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - c.p.y) / c.r,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(c.p.x - cosf(a) * c.r,0.0f,Target_Width);
        const float x2 = F32_Clamp(c.p.x + cosf(a) * c.r,0.0f,Target_Width);
        Memset_f32((float*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_RenderGX(float* Target,int Target_Width,int Target_Height,Vec2 p,F32 r,float c){
    Circle_RenderG(Target,Target_Width,Target_Height,Circle_New(p,r),c);
}
void Circle_RenderGWire(float* Target,int Target_Width,int Target_Height,Circle c,float col,float STOKE){
    for(float i = 0.0f;i<2*3.1415f;i+=1.0f/c.r){
        const float x = cosf(i) * c.r + c.p.x;
        const float y = sinf(i) * c.r + c.p.y;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
}
void Circle_RenderGXWire(float* Target,int Target_Width,int Target_Height,Vec2 p,F32 r,float c,float STOKE){
    Circle_RenderGWire(Target,Target_Width,Target_Height,Circle_New(p,r),c,STOKE);
}
void Circle_R_RenderG(float* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,float col){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);
    const float ratio = d.x / d.y;

    for(int i = (int)yS;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - p.y) / d.y,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - cosf(a) * d.y * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + cosf(a) * d.y * ratio,0.0f,Target_Width);
        Memset_f32((float*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_R_RenderGWire(float* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,float col,float STOKE){
    if(d.x <= 0.0f || d.y <= 0.0f) return;
    
    const float max = F32_Max(d.x,d.y);

    for(float i = 0.0f;i<F32_PI2;i+=1.0f/max){
        const float x = p.x + cosf(i) * d.x;
        const float y = p.y + sinf(i) * d.y;
        
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
}
void Circle_C_RenderG(float* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,Vec2 curve,float col){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);

    const float xS = F32_Clamp(p.x - d.x,0.0f,Target_Width);
    const float xM = F32_Clamp(p.x + d.x,0.0f,Target_Width);

    const float ratio = (d.x * curve.x) / (d.y * curve.y);
    const float xCurve = curve.x * d.x;
    const float yCurve = curve.y * d.y;

    const float xmid = (d.x - xCurve);
    
    const float hrect = (yM - yS - yCurve * 2.0f);
    
    int i = (int)yS;
    for(;i<(int)(yS + yCurve);i++){
        const float v = F32_Clamp((float)(i - (yS + yCurve)) / yCurve,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - xmid - cosf(a) * yCurve * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + xmid + cosf(a) * yCurve * ratio,0.0f,Target_Width);
        Memset_f32((float*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
    for(;i<(int)(yS + yCurve + hrect);i++){
        Memset_f32((float*)(Target+(int)(i)*Target_Width+(int)(xS)),col,(int)(xM-xS));
    }
    for(;i<(int)yM;i++){
        const float v = F32_Clamp((float)(i - (yS + yCurve + hrect)) / yCurve,-1.0f,1.0f);
        const float a = asinf(v);
        const float x1 = F32_Clamp(p.x - xmid - cosf(a) * yCurve * ratio,0.0f,Target_Width);
        const float x2 = F32_Clamp(p.x + xmid + cosf(a) * yCurve * ratio,0.0f,Target_Width);
        Memset_f32((float*)(Target+(int)(i)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    }
}
void Circle_C_RenderGWire(float* Target,int Target_Width,int Target_Height,Vec2 p,Vec2 d,Vec2 curve,float col,float STOKE){
    const float yS = F32_Clamp(p.y - d.y,0.0f,Target_Height);
    const float yM = F32_Clamp(p.y + d.y,0.0f,Target_Height);

    const float xS = F32_Clamp(p.x - d.x,0.0f,Target_Width);
    const float xM = F32_Clamp(p.x + d.x,0.0f,Target_Width);

    const float ratio = curve.x / curve.y;
    const float xCurve = curve.x * d.x;
    const float yCurve = curve.y * d.y;

    const float xmid = (d.x - xCurve);
    const float ymid = (d.y - yCurve);
    
    const float hrect = (yM - yS - yCurve * 2.0f);
    
    const float max = F32_Max(d.x,d.y);

    for(float i = F32_PI;i<F32_PI * 1.5f;i+=1.0f/max){
        const float x = (p.x - xmid) + cosf(i) * xCurve;
        const float y = (p.y - ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    for(float i = F32_PI * 1.5f;i<F32_PI2;i+=1.0f/max){
        const float x = (p.x + xmid) + cosf(i) * xCurve;
        const float y = (p.y - ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    
    const float x1 = F32_Clamp(p.x - xmid,0.0f,Target_Width);
    const float x2 = F32_Clamp(p.x + xmid,0.0f,Target_Width);
    Memset_f32((float*)(Target+(int)(yS)*Target_Width+(int)(x1)),col,(int)(x2-x1));
    
    for(int i = (int)yS + yCurve;i<(int)(yS + yCurve + hrect);i++){
        if(xS>=0.0f && xS<Target_Width && i>=0 && i<Target_Height)
           Target[i * Target_Width + (int)xS] = col;
        if(xM>=0.0f && xM<Target_Width && i>=0 && i<Target_Height)
           Target[i * Target_Width + (int)xM] = col;
    }
    
    for(float i = F32_PI05;i<F32_PI;i+=1.0f/max){
        const float x = (p.x - xmid) + cosf(i) * xCurve;
        const float y = (p.y + ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    for(float i = 0.0f;i<F32_PI05;i+=1.0f/max){
        const float x = (p.x + xmid) + cosf(i) * xCurve;
        const float y = (p.y + ymid) + sinf(i) * yCurve * ratio;
        if(x>=0.0f && x<Target_Width && y>=0.0f && y<Target_Height)
           Target[(int)(y) * Target_Width + (int)(x)] = col;
    }
    Memset_f32((float*)(Target+(int)(yM)*Target_Width+(int)(x1)),col,(int)(x2-x1));
}

#endif // !CIRCLE_Hatic/Library/Stb_Imageà;