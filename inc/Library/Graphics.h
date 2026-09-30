#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "Pixel.h"
#include "Rect.h"
#include "Circle.h"
#include "Triangle.h"
#include "Line.h"
#include "Math.h"
#include "Intrinsics.h"
#include "AlxTime.h"
#include "String.h"
#include "Input.h"
#include "AlxFont.h"
#include "TextBox.h"
#include "Scene.h"

#include "../Container/List.h"

void Graphics_Scale(Pixel* src,Pixel* dst,Vic2 l,int scale_factor) {
    Vic2 dstl = Vic2_Mulf(l,scale_factor);
    for (int y_dst = 0; y_dst < l.y; y_dst++) {
        for (int x_dst = 0; x_dst < l.x; x_dst += 8) {
            int y_src = (y_dst * scale_factor);
            int x_src = (x_dst * scale_factor);

            __m256i pixels = _mm256_set_epi32(
                src[y_src * l.x + (x_src + 7)],
                src[y_src * l.x + (x_src + 6)],
                src[y_src * l.x + (x_src + 5)],
                src[y_src * l.x + (x_src + 4)],
                src[y_src * l.x + (x_src + 3)],
                src[y_src * l.x + (x_src + 2)],
                src[y_src * l.x + (x_src + 1)],
                src[y_src * l.x + (x_src + 0)]
            );

            const unsigned int* pixelbuffer = (void*)&pixels;
            
            for (int i = 0; i < 8; i++) {
                if (x_dst + i < dstl.x && y_dst < dstl.y) {
                    dst[y_dst * dstl.x + (x_dst + i)] = pixelbuffer[i];
                }
            }
        }
    }
}
void Graphics_Scale2(Pixel* src,Pixel* dst,Vic2 l,int Scale) {
    for(int i = 0;i<l.y * Scale;i++){
        Memexpand_IntShift(
            src + i / Scale * l.x,
            dst + i * l.x * Scale,
            l.x * Scale,
            I64_Log2(Scale)
        );
    }
}

void Graphics_GrayClear(Pixel* Target,int Target_Width,int Target_Height,Pixel c){
    memset((unsigned int*)(Target),c,Target_Width * Target_Height * sizeof(unsigned int));
}
void Graphics_Clear(Pixel* Target,int Target_Width,int Target_Height,Pixel c){
    Memset_i32((unsigned int*)(Target),c,Target_Width * Target_Height);
}

#define Graphics_RenderPoint(Target,Target_Width,Target_Height,x,y,c)                                           Point_RenderX(Target,Target_Width,Target_Height,x,y,c)
#define Graphics_RenderPointAlpha(Target,Target_Width,Target_Height,x,y,c)                                      Point_RenderXAlpha(Target,Target_Width,Target_Height,x,y,c)
#define Graphics_RenderLine(Target,Target_Width,Target_Height,s,e,c,STOKE)                                      Line_RenderX(Target,Target_Width,Target_Height,s,e,c,STOKE)
#define Graphics_RenderRectWire(Target,Target_Width,Target_Height,x,y,w,h,c,STOKE)                              Rect_RenderXXWire(Target,Target_Width,Target_Height,x,y,w,h,c,STOKE)
#define Graphics_RenderRect(Target,Target_Width,Target_Height,x,y,w,h,c)                                        Rect_RenderXX(Target,Target_Width,Target_Height,x,y,w,h,c)
#define Graphics_RenderRectAlpha(Target,Target_Width,Target_Height,x,y,w,h,c)                                   Rect_RenderXXAlpha(Target,Target_Width,Target_Height,x,y,w,h,c)
#define Graphics_RenderTriangle(Target,Target_Width,Target_Height,p1,p2,p3,c)                                   Triangle_RenderX(Target,Target_Width,Target_Height,p1,p2,p3,c)
#define Graphics_RenderTriangleWire(Target,Target_Width,Target_Height,p1,p2,p3,c,STOKE)                         Triangle_RenderXWire(Target,Target_Width,Target_Height,p1,p2,p3,c,STOKE)
#define Graphics_RenderCircle(Target,Target_Width,Target_Height,p,r,c)                                          Circle_RenderX(Target,Target_Width,Target_Height,p,r,c)
#define Graphics_RenderCircleWire(Target,Target_Width,Target_Height,p,r,c,STOKE)                                Circle_RenderXWire(Target,Target_Width,Target_Height,p,r,c,STOKE)
#define Graphics_RenderSprite(Target,Target_Width,Target_Height,sp,x,y)                                         Sprite_Render(Target,Target_Width,Target_Height,sp,x,y)
#define Graphics_RenderSpriteAlpha(Target,Target_Width,Target_Height,sp,x,y)                                    Sprite_RenderAlpha(Target,Target_Width,Target_Height,sp,x,y)
#define Graphics_RenderSpriteAlphaTint(Target,Target_Width,Target_Height,sp,x,y,Tint)                           Sprite_RenderAlphaTint(Target,Target_Width,Target_Height,sp,x,y,Tint)
#define Graphics_RenderSubSprite(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy)                          Sprite_RenderSub(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy)
#define Graphics_RenderSubSpriteAlpha(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy)                     Sprite_RenderSubAlpha(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy)
#define Graphics_RenderSubSpriteAlphaTint(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy,Tint)            Sprite_RenderSubAlphaTint(Target,Target_Width,Target_Height,sp,x,y,ox,oy,dx,dy,Tint)
#define Graphics_RenderSpriteResize(Target,Target_Width,Target_Height,sp,x,y,w,h)                               Sprite_RenderResize(Target,Target_Width,Target_Height,sp,x,y,w,h)
#define Graphics_RenderSpriteResizeAlpha(Target,Target_Width,Target_Height,sp,x,y,w,h)                          Sprite_RenderResizeAlpha(Target,Target_Width,Target_Height,sp,x,y,w,h)
#define Graphics_RenderSpriteResizeAlphaTint(Target,Target_Width,Target_Height,sp,x,y,w,h,Tint)                 Sprite_RenderResizeAlphaTint(Target,Target_Width,Target_Height,sp,x,y,w,h,Tint)
#define Graphics_RenderSubSpriteResize(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy)                Sprite_RenderSubResize(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy)
#define Graphics_RenderSubSpriteResizeAlpha(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy)           Sprite_RenderSubResizeAlpha(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy)
#define Graphics_RenderSubSpriteResizeAlphaTint(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy,Tint)  Sprite_RenderSubResizeAlphaTint(Target,Target_Width,Target_Height,sp,x,y,w,h,ox,oy,dx,dy,Tint)
#define Graphics_RenderCharAlxFont(Target,Target_Width,Target_Height,f,ch,x,y,Color)                            Char_RenderAlxFont(Target,Target_Width,Target_Height,f,ch,x,y,Color)
#define Graphics_RenderCStrAlxFont(Target,Target_Width,Target_Height,f,cstr,x,y,Color)                          CStr_RenderAlxFont(Target,Target_Width,Target_Height,f,cstr,x,y,Color)
#define Graphics_RenderCStrSizeAlxFont(Target,Target_Width,Target_Height,f,cstr,Size,x,y,Color)                 CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,f,cstr,Size,x,y,Color)
#define Graphics_RenderCStrAlxFontTex(Target,Target_Width,Target_Height,f,t,cstr,x,y)                           CStr_RenderAlxFontTex(Target,Target_Width,Target_Height,f,t,cstr,x,y)
#define Graphics_RenderCStrSizeAlxFontTex(Target,Target_Width,Target_Height,f,t,cstr,Size,x,y)                  CStr_RenderSizeAlxFontTex(Target,Target_Width,Target_Height,f,t,cstr,Size,x,y)

#define Graphics_RenderTextBox(Target,Target_Width,Target_Height,tb)                                            TextBox_Render(Target,Target_Width,Target_Height,tb)
#define Graphics_RenderLabel(Target,Target_Width,Target_Height,l)                                               Label_Render(Target,Target_Width,Target_Height,l) 
#define Graphics_RenderButton(Target,Target_Width,Target_Height,b)                                              Button_Render(Target,Target_Width,Target_Height,b) 
#define Graphics_RenderProgressBar(Target,Target_Width,Target_Height,p)                                         ProgressBar_Render(Target,Target_Width,Target_Height,p) 
#define Graphics_RenderScene(Target,Target_Width,Target_Height,s)                                               Scene_Render(Target,Target_Width,Target_Height,s) 

#endif // GRAPHICS_H
