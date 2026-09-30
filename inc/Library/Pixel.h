#ifndef PIXEL_H
#define PIXEL_H

#include <math.h>
#include "Float.h"

#define CLEAR		    0x00000000
#define BLACK		    0xFF000000
#define DARK_GRAY	    0xFF444444
#define GRAY		    0xFF888888
#define LIGHT_GRAY	    0xFFBBBBBB
#define WHITE		    0xFFFFFFFF
#define DARK_RED        0xFF990000
#define RED             0xFFFF0000
#define LIGHT_RED       0xFFFF7777
#define DARK_GREEN      0xFF009900
#define GREEN           0xFF00FF00
#define LIGHT_GREEN     0xFF77FF77
#define DARK_BLUE       0xFF000099
#define BLUE            0xFF0000FF
#define LIGHT_BLUE      0xFF88CCFF
#define DARK_YELLOW     0xFF888800
#define YELLOW          0xFFFFFF00
#define LIGHT_YELLOW    0xFFFFFF55
#define DARK_CYAN       0xFF008888
#define CYAN            0xFF00FFFF
#define LIGHT_CYAN      0xFF55FFFF
#define DARK_MAGENTA    0xFF880088
#define MAGENTA         0xFFFF00FF
#define LIGHT_MAGENTA   0xFFFF55FF
#define DARK_ORANGE     0xFFAA6200
#define ORANGE          0xFFFFA500
#define LIGHT_ORANGE    0xFFFFE833
#define DARK_VIOLETTE   0xFF400040
#define VIOLETTE        0xFF800080
#define LIGHT_VIOLETTE  0xFF922292
#define DARK_ROSE       0xFFAA8087
#define ROSE            0xFFFFC0CB
#define LIGHT_ROSE      0xFFFFE4ED
#define DARK_BROWN      0xFF533D1E
#define BROWN           0xFF927553
#define LIGHT_BROWN     0xFFC7A358
#define GOLD            0xFFFFD700
#define SILBER          0xFFC0C0C0

typedef unsigned int Pixel;

const float PIXEL_TO_F32 = 1.0f / 255.0f;


typedef union Color{
    struct {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };
    Pixel p;
} Color;

typedef struct FColor{
    float b;
    float g;
    float r;
    float a;
} FColor;

FColor FColor_Alpha(FColor c1,FColor c2){
    const float pa = c2.a * (1.0f - c1.a);
    const float oa = c1.a + pa;
    const float ioa = 1.0f / oa;

    return (FColor){
        (c1.r * c1.a + c2.r * pa) * ioa,
        (c1.g * c1.a + c2.g * pa) * ioa,
        (c1.b * c1.a + c2.b * pa) * ioa,
        c1.a + oa
    };
}
int Pixel_R(Pixel p){
    return (Color){ .p = p }.r;
}
int Pixel_G(Pixel p){
    return (Color){ .p = p }.g;
}
int Pixel_B(Pixel p){
    return (Color){ .p = p }.b;
}
int Pixel_A(Pixel p){
    return (Color){ .p = p }.a;
}

Pixel Pixel_SetA(Pixel p,unsigned char a){
    Color col = { .p = p };
    col.a = a;
    return col.p;
}



Pixel Pixel_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a){
    return (Pixel)b | ((Pixel)g << 8) | ((Pixel)r << 16) | ((Pixel)a << 24);
}
Pixel Pixel_RGBA_C(int r,int g,int b,int a){
    r = r<0 ? 0 : (r>255 ? 255 : r);
    g = g<0 ? 0 : (g>255 ? 255 : g);
    b = b<0 ? 0 : (b>255 ? 255 : b);
    a = a<0 ? 0 : (a>255 ? 255 : a);
    return b | (g << 8) | (r << 16) | (a << 24);
}
Pixel Pixel_toRGBA(float r,float g,float b,float a){
    return (Pixel)(b * 255.0f) | ((Pixel)(g * 255.0f) << 8) | ((Pixel)(r * 255.0f) << 16) | ((Pixel)(a * 255.0f) << 24);
}
Pixel Pixel_Alpha(Pixel p1,Pixel p2){
    Pixel a;
    float Src_alpha = (float)(p1 >> 24) / 255.0f;
    float Dest_alpha = (float)(p2 >> 24) / 255.0f;

    float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
    float Out_alpha  = (Src_alpha + Part_alpha);

    unsigned int Out_red    = (unsigned int)(((float)((p1 >> 16) & 0xFF) * Src_alpha + (float)((p2 >> 16) & 0xFF) * Part_alpha) / Out_alpha);
    unsigned int Out_green  = (unsigned int)(((float)((p1 >> 8) & 0xFF)  * Src_alpha + (float)((p2 >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
    unsigned int Out_blue   = (unsigned int)(((float)(p1 & 0xFF)         * Src_alpha + (float)(p2 & 0xFF)         * Part_alpha) / Out_alpha);

    a = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    return a;
}
Pixel Pixel_byFColor(FColor fc){
    Color c;
    c.b = (unsigned char)(F32_Clamp(fc.b,0.0f,1.0f) * 255.0f);
    c.g = (unsigned char)(F32_Clamp(fc.g,0.0f,1.0f) * 255.0f);
    c.r = (unsigned char)(F32_Clamp(fc.r,0.0f,1.0f) * 255.0f);
    c.a = (unsigned char)(F32_Clamp(fc.a,0.0f,1.0f) * 255.0f);
    return c.p;
}

FColor Pixel_toFColor(Pixel p){
    FColor c;
    c.b = (float)((p) & 0xFF) / 255.0f;
    c.g = (float)((p >> 8) & 0xFF) / 255.0f;
    c.r = (float)((p >> 16) & 0xFF) / 255.0f;
    c.a = (float)((p >> 24) & 0xFF) / 255.0f;
    return c;
}
FColor FColor_Black(){
    FColor c;
    c.b = 0.0f;
    c.g = 0.0f;
    c.r = 0.0f;
    c.a = 1.0f;
    return c;
}
FColor FColor_White(){
    FColor c;
    c.b = 1.0f;
    c.g = 1.0f;
    c.r = 1.0f;
    c.a = 1.0f;
    return c;
}
FColor FColor_Add(FColor c1,FColor c2){
    FColor c;
    c.b = c1.b + c2.b;
    c.g = c1.g + c2.g;
    c.r = c1.r + c2.r;
    c.a = c1.a + c2.a;
    return c;
}
FColor FColor_Mul(FColor c1,FColor c2){
    FColor c;
    c.b = c1.b * c2.b;
    c.g = c1.g * c2.g;
    c.r = c1.r * c2.r;
    c.a = c1.a * c2.a;
    return c;
}
FColor FColor_Mulf(FColor c,float a){
    FColor out;
    out.b = c.b * a;
    out.g = c.g * a;
    out.r = c.r * a;
    out.a = c.a;
    return out;
}
float FColor_Avg(FColor c){
    float l = (c.r + c.g + c.b) / 3.0f;
    l = l >= 0.0f ? l : 0.0f;
    l = l <= 1.0f ? l : 1.0f;
    return l;
}

Pixel Pixel_Mul(Pixel p1,Pixel p2){
    FColor c1 = Pixel_toFColor(p1);
    FColor c2 = Pixel_toFColor(p2);
    FColor c = FColor_Mul(c1,c2);
    return Pixel_byFColor(c);
}
Pixel Pixel_Inter(Pixel p1,Pixel p2,float a){
    float p1_b = (float)(p1 & 0xFF)                 / 255.0f;
    float p1_g = (float)((p1 & 0xFF00) >> 8)        / 255.0f;
    float p1_r = (float)((p1 & 0xFF0000) >> 16)     / 255.0f;
    float p1_a = (float)((p1 & 0xFF000000) >> 24)   / 255.0f;

    float p2_b = (float)(p2 & 0xFF)                 / 255.0f;
    float p2_g = (float)((p2 & 0xFF00) >> 8)        / 255.0f;
    float p2_r = (float)((p2 & 0xFF0000) >> 16)     / 255.0f;
    float p2_a = (float)((p2 & 0xFF000000) >> 24)   / 255.0f;

    return Pixel_toRGBA(p1_r + (p2_r - p1_r) * a,p1_g + (p2_g - p1_g) * a,p1_b + (p2_b - p1_b) * a,p1_a + (p2_a - p1_a) * a);;
}
Pixel Pixel_Avg(Pixel p1,Pixel p2){
    Color c1 = { .p = p1 };
    Color c2 = { .p = p2 };
    Color c = {
        .r = (c1.r + c2.r) >> 1,
        .g = (c1.g + c2.g) >> 1,
        .b = (c1.b + c2.b) >> 1,
        .a = (c1.a + c2.a) >> 1
    };
    return c.p;
}
Pixel Pixel_AvgC(Pixel* p,int c){
    Color col = { .p = 0x00000000 };
    for(int i = 0;i<c;i++){
        col.r = (col.r + (Color){ .p = p[i] }.r) >> 1;
        col.g = (col.g + (Color){ .p = p[i] }.g) >> 1;
        col.b = (col.b + (Color){ .p = p[i] }.b) >> 1;
        col.a = (col.a + (Color){ .p = p[i] }.a) >> 1;
    }
    return col.p;
}
Pixel Pixel_Sub(Pixel p,unsigned char v){
    Color col = { .p = p };
    col.r = ((short)col.r - (short)v) >= 0 ? col.r - v : 0;
    col.g = ((short)col.g - (short)v) >= 0 ? col.g - v : 0;
    col.b = ((short)col.b - (short)v) >= 0 ? col.b - v : 0;
    return col.p;
}
Pixel Pixel_SubA(Pixel p,unsigned char v){
    Color col = { .p = p };
    col.r = ((short)col.r - (short)v) >= 0 ? col.r - v : 0;
    col.g = ((short)col.g - (short)v) >= 0 ? col.g - v : 0;
    col.b = ((short)col.b - (short)v) >= 0 ? col.b - v : 0;
    col.a = ((short)col.a - (short)v) >= 0 ? col.a - v : 0;
    return col.p;
}

float Pixel_Lightness_RGB(Pixel p){
    FColor fc = Pixel_toFColor(p);
    return (fc.r + fc.g + fc.b) / 3.0f;
}
float Pixel_Lightness_N(Pixel p){
    FColor fc = Pixel_toFColor(p);
    return (fc.r * 0.33f + fc.g * 0.42f + fc.b * 0.25f);
}
float Pixel_Lightness_N2(Pixel p){
    FColor fc = Pixel_toFColor(p);
    return (fc.r * 0.2162f + fc.g * 0.7152f + fc.b * 0.0722f);
}

Pixel Pixel_Add(Pixel p,unsigned char v){
    Color col = { .p = p };
    col.r = ((short)col.r + (short)v) <= 255U ? col.r + v : 255U;
    col.g = ((short)col.g + (short)v) <= 255U ? col.g + v : 255U;
    col.b = ((short)col.b + (short)v) <= 255U ? col.b + v : 255U;
    return col.p;
}
Pixel Pixel_Mulf(Pixel p,float a){
    if(a<0.0 || a>1.0f) return p;
    
    FColor col = Pixel_toFColor(p);
    col.r = col.r * a;
    col.g = col.g * a;
    col.b = col.b * a;
    return Pixel_byFColor(col);
}

Pixel Pixel_Gray_LRGBA(Pixel p){
    float l = Pixel_Lightness_RGB(p);
    return Pixel_toRGBA(l,l,l,1.0f);
}
Pixel Pixel_Gray_LN(Pixel p){
    float l = Pixel_Lightness_N(p);
    return Pixel_toRGBA(l,l,l,1.0f);
}
Pixel Pixel_Gray_L2(Pixel p){
    float l = Pixel_Lightness_N2(p);
    return Pixel_toRGBA(l,l,l,1.0f);
}
Pixel Pixel_Dist(Pixel p1,Pixel p2){
    Color col1 = { .p = p1 };
    Color col2 = { .p = p2 };
    col1.r = (col1.r>col2.r ? col1.r : col2.r) - (col1.r<col2.r ? col1.r : col2.r);
    col1.g = (col1.g>col2.g ? col1.g : col2.g) - (col1.g<col2.g ? col1.g : col2.g);
    col1.b = (col1.b>col2.b ? col1.b : col2.b) - (col1.b<col2.b ? col1.b : col2.b);
    return col1.p;
}
float Pixel_DistF(Pixel p1,Pixel p2){
    const Color delta = { .p = Pixel_Dist(p1,p2) };
    float length = sqrtf((float)(
        (unsigned int)delta.r * (unsigned int)delta.r +
        (unsigned int)delta.g * (unsigned int)delta.g +
        (unsigned int)delta.b * (unsigned int)delta.b
    ));
    return length;
}
float Pixel_Gray_Dir(Pixel p1,Pixel p2){
    float l1 = Pixel_Lightness_N2(p1);
    float l2 = Pixel_Lightness_N2(p2);
    return l1 - l2;
}

Pixel Pixel_BW_LRGBA(Pixel p){
    float l = Pixel_Lightness_RGB(p);
    return l>0.5 ? WHITE : BLACK;
}
Pixel Pixel_BW_LN(Pixel p){
    float l = Pixel_Lightness_N(p);
    return l>0.5 ? WHITE : BLACK;
}
Pixel Pixel_BW_L2(Pixel p){
    float l = Pixel_Lightness_N2(p);
    return l>0.5 ? WHITE : BLACK;
}

Pixel Pixel_GQ_1(Pixel p){
    return Pixel_BW_L2(p);
}
Pixel Pixel_GQ_N(Pixel p,int n){
    float l = Pixel_Lightness_N2(p);
    l = (float)((int)(l * n + 0.5f)) / n;
    return Pixel_toRGBA(l,l,l,1.0f);
}

Pixel Pixel_CQ_N(Pixel p,int n){
    Color c = { .p = p };
    c.r = (unsigned char)((float)((int)(((float)c.r * PIXEL_TO_F32) * n + 0.5f)) / (n * PIXEL_TO_F32));
    c.g = (unsigned char)((float)((int)(((float)c.g * PIXEL_TO_F32) * n + 0.5f)) / (n * PIXEL_TO_F32));
    c.b = (unsigned char)((float)((int)(((float)c.b * PIXEL_TO_F32) * n + 0.5f)) / (n * PIXEL_TO_F32));
    return c.p;
}


Pixel Pixel_CCQ_N(Pixel p,Pixel* palette,int n){
    const Color c = { .p = p };
    const Color* paletteC = (Color*)palette;
    
    Pixel Selected = CLEAR;
    float ClosestSq = 10.0E9f;

    for(int i = 0;i<n;i++){
        const float dR = (float)c.r - (float)paletteC[i].r;
        const float dG = (float)c.g - (float)paletteC[i].g;
        const float dB = (float)c.b - (float)paletteC[i].b;
        const float DistSq = dR * dR + dG * dG + dB * dB;
        
        if(DistSq < ClosestSq){
            ClosestSq = DistSq;
            Selected = paletteC[i].p;
        }
    }

    return Selected;
}

#endif // PIXEL_H