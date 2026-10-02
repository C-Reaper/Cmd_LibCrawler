#ifndef WINDOWENGINEDEREF_H
#define WINDOWENGINEDEREF_H

/*

Author: Codeleaded[Alx]

Windows (GCC):
C: -mavx2
L: -lgdi32

Linux (GCC):
C: -O2 -mavx2
CO: -Ofast -mavx2 -funroll-loops -fpeel-loops -funswitch-loops -fpredictive-commoning -ftree-vectorize	-fweb -frename-registers -flto -fomit-frame-pointer
L: -lm -lX11 -lpng 

"Good Resolutions":
112,    62,     16, 16
170,    95,     12, 12
225,    125,    8,  8
450,    250,    4,  4
900,    500,    2,  2
1800,   1000,   1,  1

*/

#if defined __linux__
#include "AlxWindow_Linux.h"
#elif defined _WIN32
#include "AlxWindow_Windows.h"
#elif defined _WEB
#include "AlxWindow_Web.h"
#else
#error "Plattform not supported!"
#endif

#include "Math.h"
#include "String.h"
#include "Graphics.h"
#include "Intrinsics.h"

AlxWindow window;

#define WINDOW_STD_ARGS     window.Buffer,window.Width,window.Height

#define Clear(col)                                                   Graphics_Clear(window.Buffer,window.Width,window.Height,col)
#define RenderPixel(x,y,col)                                         Graphics_RenderPoint(window.Buffer,window.Width,window.Height,x,y,col)
#define RenderPixelAlpha(x,y,col)                                    Graphics_RenderPointAlpha(window.Buffer,window.Width,window.Height,x,y,col)
#define RenderLine(s,e,c,StrokeSize)                                 Graphics_RenderLine(window.Buffer,window.Width,window.Height,s,e,c,StrokeSize)
#define RenderRectWire(x,y,w,h,col,STROKE)                           Graphics_RenderRectWire(window.Buffer,window.Width,window.Height,x,y,w,h,col,STROKE)
#define RenderRect(x,y,w,h,col)                                      Graphics_RenderRect(window.Buffer,window.Width,window.Height,x,y,w,h,col)
#define RenderRectAlpha(x,y,w,h,col)                                 Graphics_RenderRectAlpha(window.Buffer,window.Width,window.Height,x,y,w,h,col)
#define RenderTriangle(p1,p2,p3,c)                                   Graphics_RenderTriangle(window.Buffer,window.Width,window.Height,p1,p2,p3,c)
#define RenderTriangleWire(p1,p2,p3,c,StrokeSize)                    Graphics_RenderTriangleWire(window.Buffer,window.Width,window.Height,p1,p2,p3,c,StrokeSize)
#define RenderCircle(p,r,c)                                          Graphics_RenderCircle(window.Buffer,window.Width,window.Height,p,r,c)
#define RenderCircleWire(p,r,c,STOKE)                                Graphics_RenderCircleWire(window.Buffer,window.Width,window.Height,p,r,c,STOKE)
#define RenderSprite(sp,x,y)                                         Graphics_RenderSprite(window.Buffer,window.Width,window.Height,sp,x,y)
#define RenderSpriteAlpha(sp,x,y)                                    Graphics_RenderSpriteAlpha(window.Buffer,window.Width,window.Height,sp,x,y)
#define RenderSpriteAlphaTint(sp,x,y,Tint)                           Graphics_RenderSpriteAlphaTint(window.Buffer,window.Width,window.Height,sp,x,y,Tint)
#define RenderSubSprite(sp,x,y,ox,oy,dx,dy)                          Graphics_RenderSubSprite(window.Buffer,window.Width,window.Height,sp,x,y,ox,oy,dx,dy)
#define RenderSubSpriteAlpha(sp,x,y,ox,oy,dx,dy)                     Graphics_RenderSubSpriteAlpha(window.Buffer,window.Width,window.Height,sp,x,y,ox,oy,dx,dy)
#define RenderSubSpriteAlphaTint(sp,x,y,ox,oy,dx,dy,Tint)            Graphics_RenderSubSpriteAlphaTint(window.Buffer,window.Width,window.Height,sp,x,y,ox,oy,dx,dy,Tint)
#define RenderSpriteResize(sp,x,y,w,h)                               Graphics_RenderSpriteResize(window.Buffer,window.Width,window.Height,sp,x,y,w,h)
#define RenderSpriteResizeAlpha(sp,x,y,w,h)                          Graphics_RenderSpriteResizeAlpha(window.Buffer,window.Width,window.Height,sp,x,y,w,h)
#define RenderSpriteResizeAlphaTint(sp,x,y,w,h,Tint)                 Graphics_RenderSpriteResizeAlphaTint(window.Buffer,window.Width,window.Height,sp,x,y,w,h,Tint)
#define RenderSubSpriteResize(sp,x,y,w,h,ox,oy,dx,dy)                Graphics_RenderSubSpriteResize(window.Buffer,window.Width,window.Height,sp,x,y,w,h,ox,oy,dx,dy)
#define RenderSubSpriteResizeAlpha(sp,x,y,w,h,ox,oy,dx,dy)           Graphics_RenderSubSpriteResizeAlpha(window.Buffer,window.Width,window.Height,sp,x,y,w,h,ox,oy,dx,dy)
#define RenderSubSpriteResizeAlphaTint(sp,x,y,w,h,ox,oy,dx,dy,Tint)  Graphics_RenderSubSpriteResizeAlphaTint(window.Buffer,window.Width,window.Height,sp,x,y,w,h,ox,oy,dx,dy,Tint)
#define RenderCharAlxFont(f,c,x,y,Color)                             Graphics_RenderCharAlxFont(window.Buffer,window.Width,window.Height,f,c,x,y,Color)
#define RenderCStrAlxFont(f,cstr,x,y,Color)                          Graphics_RenderCStrAlxFont(window.Buffer,window.Width,window.Height,f,cstr,x,y,Color)
#define RenderCStrSizeAlxFont(f,cstr,Size,x,y,Color)                 Graphics_RenderCStrSizeAlxFont(window.Buffer,window.Width,window.Height,f,cstr,Size,x,y,Color)
#define RenderCStrAlxFontTex(f,t,cstr,x,y)                           Graphics_RenderCStrAlxFontTex(window.Buffer,window.Width,window.Height,f,t,cstr,x,y)
#define RenderCStrSizeAlxFontTex(f,t,cstr,Size,x,y)                  Graphics_RenderCStrSizeAlxFontTex(window.Buffer,window.Width,window.Height,f,t,cstr,Size,x,y)

#define RenderChar(c,x,y,Color)                                      Graphics_RenderCharAlxFont(window.Buffer,window.Width,window.Height,&window.font,c,x,y,Color)
#define RenderCStr(cstr,x,y,Color)                                   Graphics_RenderCStrAlxFont(window.Buffer,window.Width,window.Height,&window.font,cstr,x,y,Color)
#define RenderCStrSize(cstr,Size,x,y,Color)                          Graphics_RenderCStrSizeAlxFont(window.Buffer,window.Width,window.Height,&window.font,cstr,Size,x,y,Color)
#define RenderCStrTex(t,cstr,x,y)                                    Graphics_RenderCStrAlxFontTex(window.Buffer,window.Width,window.Height,&window.font,t,cstr,x,y)
#define RenderCStrSizeTex(t,cstr,Size,x,y)                           Graphics_RenderCStrSizeAlxFontTex(window.Buffer,window.Width,window.Height,&window.font,t,cstr,Size,x,y)

int GetWidth(){
    return window.Width;
}
int GetHeight(){
    return window.Height;
}

Vec2 GetMouseBefore(){
    return (Vec2){ window.MouseBeforeX,window.MouseBeforeY };
}
Vec2 GetMouse(){
    return (Vec2){ window.MouseX,window.MouseY };
}
void SetMouse(Vec2 p){
    AlxWindow_Mouse_Set(&window,p);
}
Rect GetScreenRect(){
    return (Rect){ { 0.0f,0.0f },{ GetWidth(),GetHeight() } };
}


AlxFont* GetAlxFont(){
    return &window.font;
}
void SetAlxFont(Sprite s,int Columns,int Rows,int CharSizeX,int CharSizeY){
    AlxFont_Free(&window.font);
    window.font = AlxFont_New(s,Columns,Rows,CharSizeX,CharSizeY);
}
void SetAlxFontTo(AlxFont f,int CharSizeX,int CharSizeY){
    AlxFont_Free(&window.font);
    window.font = f;
    AlxFont_Resize(&window.font,CharSizeX,CharSizeY);
}
void ResizeAlxFont(int CharSizeX,int CharSizeY){
    AlxFont_Resize(&window.font,CharSizeX,CharSizeY);
}
void ReloadAlxFont(int CharSizeX,int CharSizeY){
    if(window.font.CharSizeX!=CharSizeX || window.font.CharSizeY!=CharSizeY){
        AlxFont_Free(&window.font);
        window.font = AlxFont_MAKE_YANIS(CharSizeX,CharSizeY);
    }
}

void Exit(){
    AlxWindow_Exit(&window);
}
States Stroke(int Stroke){
    return window.Strokes[Stroke];
}
void Render() {
    AlxWindow_Render(&window);
}
void UpdateKB(){
	AlxWindow_UpdateKB(&window);

	if (Stroke(ALX_KEY_F11).PRESSED)
	    AlxWindow_SetFullscreen(&window);
}
int Create(char* Name,int Width,int Height,int AlxFontX,int AlxFontY,void (*Setup)(AlxWindow*),void (*Update)(AlxWindow*),void (*Delete)(AlxWindow*)){
    window = AlxWindow_Null();
    AlxWindow_PreInit(&window);
    AlxWindow_Init(&window,Name,Width,Height,AlxFontX,AlxFontY,Setup,Update,Delete,NULL);
    return window.Running;
}
int CreateX(char* Name,int Width,int Height,int AlxFontX,int AlxFontY,void (*Setup)(AlxWindow*),void (*Update)(AlxWindow*),void (*Delete)(AlxWindow*),void (*Resize)(AlxWindow*)){
    window = AlxWindow_Null();
    AlxWindow_PreInit(&window);
    AlxWindow_Init(&window,Name,Width,Height,AlxFontX,AlxFontY,Setup,Update,Delete,Resize);
    return window.Running;
}
void Start(){
    AlxWindow_Start(&window);
}

#endif