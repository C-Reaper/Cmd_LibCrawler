#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>

#include "CStr.h"
#include "Rect.h"
#include "Intrinsics.h"
#include "Image.h"
#include "Pixel.h"


typedef struct Sprite{
    unsigned int w;
    unsigned int h;
    char* path;
    unsigned int* img;
} Sprite;

typedef struct SubSprite{
    Sprite* sp;
    float ox;
    float oy;
    float dx;
    float dy;
} SubSprite;

Sprite Sprite_New(int w,int h){
    Sprite sp;
    sp.w = w;
    sp.h = h;
    sp.path = NULL;
    sp.img = (unsigned int*)malloc(w * h * sizeof(unsigned int));
    return sp;
}
Sprite Sprite_None(int w,int h){
    Sprite sp;
    sp.w = w;
    sp.h = h;
    sp.path = NULL;
    sp.img = (unsigned int*)calloc(w * h,sizeof(unsigned int));
    return sp;
}
Sprite Sprite_By(int w,int h,unsigned int* img){
    Sprite sp;
    sp.w = w;
    sp.h = h;
    sp.path = NULL;
    sp.img = img;
    return sp;
}
Sprite Sprite_Cpy(Sprite* src){
    Sprite sp;
    sp.w = src->w;
    sp.h = src->h;
    sp.path = CStr_Cpy(src->path);
    sp.img = (unsigned int*)malloc(src->w * src->h * sizeof(unsigned int));
    memcpy(sp.img,src->img,src->w * src->h * sizeof(unsigned int));
    return sp;
}
Sprite Sprite_Null(){
    Sprite sp;
    sp.w = 0;
    sp.h = 0;
    sp.path = NULL;
    sp.img = NULL;
    return sp;
}
Sprite Sprite_Load(char* filename){
    int w = 0;
    int h = 0;
    unsigned int* buffer = (unsigned int*)Image_Load(filename,&w,&h);
    if(!buffer){
        printf("[Sprite]: Error -> Loading %s\n",filename);
        return Sprite_Null();
    }

    #if (!defined IMAGE_STD) || defined _WIN32
    Memswap_i32(buffer,0,2,w * h);
    #endif

    Sprite s = Sprite_By(w,h,buffer);
    s.path = CStr_Cpy(filename);
    return s;
}
Sprite Sprite_Load_BGR(char* filename){
    Sprite sp = Sprite_Load(filename);
    Memswap_i32(sp.img,0,2,sp.w * sp.h);
    return sp;
}
void Sprite_Save(Sprite* sp,char* filename){
    if(!Image_Save(filename,sp->img,sp->w,sp->h)){
        printf("[Sprite]: Error -> Saving '%s'\n",filename);
    }
}
void Sprite_Free(Sprite* sp){
    if(sp->img) free(sp->img);
    sp->img = NULL;

    if(sp->path) free(sp->path);
    sp->path = NULL;

    sp->w = 0;
    sp->h = 0;
}

unsigned int Sprite_Get(Sprite* sp,float x,float y){
    if(sp->img && x>=0.0f && x<sp->w && y>=0.0f && y<sp->h){
        return sp->img[(int)y * (int)sp->w + (int)x];
    }
    return 0x00000000U;
}
void Sprite_Set(Sprite* sp,float x,float y,unsigned int p){
    if(sp->img && x>=0.0f && x<sp->w && y>=0.0f && y<sp->h){
        sp->img[(int)y * (int)sp->w + (int)x] = p;
    }
}

void Sprite_Clear(Sprite* sp,unsigned int p){
    Memset_i32(sp->img,p,sp->w * sp->h);
}

unsigned int Sprite_Sample(Sprite* sp,float x,float y){
    if(sp->img && x>=0.0f && x<1.0f && y>=0.0f && y<1.0f){
        return sp->img[(int)(y * sp->h) * (int)sp->w + (int)(x * sp->w)];
    }
    return 0x00000000U;
}
unsigned int Sprite_SampleSub(Sprite* sp,float x,float y,int subx,int suby,int subw,int subh){
    //if(x<0.0f || x>=1.0f || y<0.0f || y>=1.0f) return 0xFF000000;
    x -= (int)x;
    y -= (int)y;
    int sx = (float)subw * (x + (float)subx);
    int sy = (float)subh * (y + (float)suby);
    return *(sp->img + sy * (int)sp->w + sx);
}

void Sprite_ForAll(Sprite* src,Sprite* dst,Pixel (*Func)(Pixel)){
    if(dst->w!=src->w || dst->h!=src->h){
        Sprite_Free(dst);
        *dst = Sprite_New(src->w,src->h);
    }

    for(int i = 0;i<src->h;i++){
        for(int j = 0;j<src->w;j++){
            dst->img[i * (int)src->w + j] = Func(src->img[i * (int)src->w + j]);
        }
    }
}
Sprite Sprite_MakeForAll(Sprite* sp,Pixel (*Func)(Pixel)){
    Sprite out = Sprite_Null();
    Sprite_ForAll(sp,&out,Func);
    return out;
}

Sprite Sprite_Mirror(Sprite* sp){
    Sprite out = Sprite_New(sp->w,sp->h);
    for(int y = 0;y<sp->h;y++){
        for(int x = 0;x<sp->w;x++){
            const int mx = (int)sp->w - x - 1;
            const int my = (int)sp->h - y - 1;
            out.img[y * (int)sp->w + x] = sp->img[my * (int)sp->w + mx];
        }
    }
    return out;
}

void Sprite_AppendHSub(Sprite* sp,SubSprite ss){
    if(!sp->img){
        *sp = Sprite_New(ss.dx,ss.dy);

        for(int i = 0;i<ss.dy;i++){
            const unsigned int src = (unsigned int)ss.oy * ss.sp->w + (unsigned int)ss.ox + i * ss.sp->w;
            const unsigned int dst = i * sp->w;

            if(src >= ss.sp->w * ss.sp->h || src + ss.dx >= ss.sp->w * ss.sp->h)    break;
            if(dst >= sp->w * sp->h || dst + ss.dx >= sp->w * sp->h)                break;
            memcpy(sp->img + dst,ss.sp->img + src,ss.dx * sizeof(unsigned int));
        }
    }else{
        Sprite newsp = Sprite_New(sp->w + ss.dx,I32_Max(ss.dy,sp->h));
        memset(newsp.img,0,newsp.w * newsp.h * sizeof(unsigned int));

        for(int i = 0;i<sp->h;i++){
            const unsigned int src = i * sp->w;
            const unsigned int dst = i * newsp.w;
            memcpy(newsp.img + dst,sp->img + src,sp->w * sizeof(unsigned int));
        }
        for(int i = 0;i<ss.dy;i++){
            const unsigned int src = (unsigned int)ss.oy * ss.sp->w + (unsigned int)ss.ox + i * ss.sp->w;
            const unsigned int dst = i * newsp.w + sp->w;

            if(src >= ss.sp->w * ss.sp->h || src + sp->w >= ss.sp->w * ss.sp->h)    break;
            if(dst >= newsp.w * newsp.h || dst + ss.dx >= newsp.w * newsp.h)        break;
            memcpy(newsp.img + dst,ss.sp->img + src,ss.dx * sizeof(unsigned int));
        }

        Sprite_Free(sp);
        *sp = newsp;
    }
}

void Sprite_Resize(Sprite* sp,int NewX,int NewY){
    if(!(sp && sp->img)) return;
    unsigned int* NewBuffer = (unsigned int*)calloc(NewX * NewY,sizeof(unsigned int));
    for(int i = 0;i<NewY;i++){
        float y = ((float)i / (float)NewY) * sp->h;
        Memexpand(sp->img + (int)((int)y * sp->w),NewBuffer + (int)((int)i * NewX),sp->w,NewX);
    }
    sp->w = NewX;
    sp->h = NewY;
    free(sp->img);
    sp->img = NewBuffer;
}
void Sprite_Resize_ByW(Sprite* sp,int NewX){
    if(!(sp && sp->img)) return;

    const float a = (float)sp->w / (float)NewX;
    const int NewY = (int)((float)sp->h / a);

    unsigned int* NewBuffer = (unsigned int*)calloc(NewX * NewY,sizeof(unsigned int));
    for(int i = 0;i<NewY;i++){
        float y = ((float)i / (float)NewY) * sp->h;
        Memexpand(sp->img + (int)((int)y * sp->w),NewBuffer + (int)((int)i * NewX),sp->w,NewX);
    }

    sp->w = NewX;
    sp->h = NewY;

    free(sp->img);
    sp->img = NewBuffer;
}
void Sprite_Resize_ByH(Sprite* sp,int NewY){
    if(!(sp && sp->img)) return;

    const float a = (float)sp->h / (float)NewY;
    const int NewX = (int)((float)sp->w / a);

    unsigned int* NewBuffer = (unsigned int*)calloc(NewX * NewY,sizeof(unsigned int));
    for(int i = 0;i<NewY;i++){
        float y = ((float)i / (float)NewY) * sp->h;
        Memexpand(sp->img + (int)((int)y * sp->w),NewBuffer + (int)((int)i * NewX),sp->w,NewX);
    }

    sp->w = NewX;
    sp->h = NewY;

    free(sp->img);
    sp->img = NewBuffer;
}

void Sprite_Reload(Sprite* sp,int NewX,int NewY){
    if(sp->w!=NewX || sp->h!=NewY){
        if(sp->path){
            Sprite newsp = Sprite_Load(sp->path);
            Sprite_Resize(&newsp,NewX,NewY);
            Sprite_Free(sp);
            *sp = newsp;
        }else{
            Sprite_Resize(sp,NewX,NewY);
        }
    }
}
void Sprite_Reload_BGR(Sprite* sp,int NewX,int NewY){
    if(sp->path){
        Sprite newsp = Sprite_Load_BGR(sp->path);
        Sprite_Resize(&newsp,NewX,NewY);
        Sprite_Free(sp);
        *sp = newsp;
    }else{
        Sprite_Resize(sp,NewX,NewY);
    }
}

void Sprite_Render(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y){
    Rect r;
    if(!Rect_Clip(&r,x,y,sp->w,sp->h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0.0f; py<r.d.y ;py+=1.0f){
        Memcpy_i32(sp->img+(int)((int)((r.p.y - y) + py) * sp->w + (r.p.x - x)),Target + (int)((int)(r.p.y + py) * Target_Width + (int)r.p.x),(int)r.d.x);
    }
}
void Sprite_RenderAlpha(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y){
    Rect r;
    if(!Rect_Clip(&r,x,y,sp->w,sp->h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0; py<r.d.y ;py+=1.0f){
        Memcpy_Alpha(sp->img+(unsigned int)((int)(py + (r.p.y - y)) * (int)sp->w + (r.p.x - x)),(unsigned int*)(Target + (int)(r.p.y + py) * Target_Width + (int)r.p.x),(int)r.d.x);
    }
}
void Sprite_RenderAlphaTint(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,unsigned int Tint){
    Rect r;
    if(!Rect_Clip(&r,x,y,sp->w,sp->h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0; py<r.d.y ;py+=1.0f){
        Memcpy_Alpha_Tint(sp->img+(unsigned int)((int)(py + (r.p.y - y)) * (int)sp->w + (r.p.x - x)),(unsigned int*)(Target + (int)(r.p.y + py)*Target_Width + (int)r.p.x),(int)r.d.x,Tint);
    }
}
void Sprite_RenderResize(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h){
    Rect r;
    if(!Rect_Clip(&r,x,y,w,h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0.0f; py<r.d.y ;py+=1.0f){
        float ax = sp->w / w;
        float ay = sp->h / h;
        Memexpand(sp->img+(int)((int)((r.p.y - y + py) * ay) * sp->w + (r.p.x - x) * ax),Target + (int)((int)(r.p.y + py) * Target_Width + (int)r.p.x),(int)(r.d.x * ax),(int)r.d.x);
    }
}
void Sprite_RenderResizeAlpha(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h){
    Rect r;
    if(!Rect_Clip(&r,x,y,sp->w,sp->h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0; py<r.d.y ;py+=1.0f){
        float ax = sp->w / r.d.x;
        float ay = sp->h / r.d.y;
        Memexpand_Alpha(sp->img+(int)((int)((r.p.y - y + py) * ay) * sp->w + (r.p.x - x) * ax),Target + (int)((int)(r.p.y + py) * Target_Width + (int)r.p.x),(int)(r.d.x * ax),(int)r.d.x);
    }
}
void Sprite_RenderResizeAlphaTint(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h,unsigned int Tint){
    Rect r;
    if(!Rect_Clip(&r,x,y,sp->w,sp->h,0.0f,0.0f,Target_Width,Target_Height))
        return;
    for(float py = 0; py<r.d.y ;py+=1.0f){
        float ax = sp->w / r.d.x;
        float ay = sp->h / r.d.y;
        Memexpand_Alpha_Tint(sp->img+(int)((int)((r.p.y - y + py) * ay) * sp->w + (r.p.x - x) * ax),Target + (int)((int)(r.p.y + py) * Target_Width + (int)r.p.x),(int)(r.d.x * ax),(int)r.d.x,Tint);
    }
}

void Sprite_RenderSub(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float ox,float oy,float dx,float dy){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_i32(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x
        );
    }
}
void Sprite_RenderSubAlpha(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float ox,float oy,float dx,float dy){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_Alpha(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x
        );
    }
}
void Sprite_RenderSubAlphaTint(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float ox,float oy,float dx,float dy,unsigned int Tint){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_Alpha_Tint(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x,
            Tint
        );
    }
}
void Sprite_RenderSubResize(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h,float ox,float oy,float dx,float dy){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_i32(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x
        );
    }
}
void Sprite_RenderSubResizeAlpha(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h,float ox,float oy,float dx,float dy){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_Alpha(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x
        );
    }
}
void Sprite_RenderSubResizeAlphaTint(unsigned int* Target,int Target_Width,int Target_Height,Sprite* sp,float x,float y,float w,float h,float ox,float oy,float dx,float dy,unsigned int Tint){
    Rect r;
    if(!Rect_Clip(&r,ox,oy,dx,dy,0.0f,0.0f,sp->w,sp->h))
        return;

    Rect sub;
    if(!Rect_Clip(&sub,x,y,r.d.x,r.d.y,0.0f,0.0f,Target_Width,Target_Height))
        return;

    const int shiftx = (int)sub.p.x == 0 ? (r.d.x - sub.d.x) : 0;
    const int shifty = (int)sub.p.y == 0 ? (r.d.y - sub.d.y) : 0;
    for(float py = 0.0f; py<sub.d.y ;py+=1.0f){
        Memcpy_Alpha_Tint(
            sp->img + ((int)(r.p.y + py + shifty) * sp->w + (int)r.p.x + shiftx),
            Target + ((int)(sub.p.y + py) * Target_Width + (int)sub.p.x),
            (int)sub.d.x,
            Tint
        );
    }
}

SubSprite SubSprite_New(Sprite* sp,float ox,float oy,float dx,float dy){
    SubSprite ret;
    ret.sp = sp;
    ret.ox = ox;
    ret.oy = oy;
    ret.dx = dx;
    ret.dy = dy;
    return ret;
}
SubSprite SubSprite_Null(){
    SubSprite ret;
    ret.sp = NULL;
    ret.ox = 0.0f;
    ret.oy = 0.0f;
    ret.dx = 0.0f;
    ret.dy = 0.0f;
    return ret;
}

unsigned int SubSprite_Sample(SubSprite* sp,float x,float y){
    if(!sp->sp) return 0x00000000U;

    const int sx = (int)(x * sp->dx) + sp->ox;
    const int sy = (int)(y * sp->dy) + sp->oy;

    if(sx>=0 && sx<sp->sp->w && sy>=0 && sy<sp->sp->h){
        return sp->sp->img[sy * (int)sp->sp->w + sx];
    }
    return 0x00000000U;
}



typedef Vector Vec_Sprite;

Vec_Sprite Vec_Sprite_New(){
    return Vector_New(sizeof(Sprite));
}
Vec_Sprite Vec_Sprite_Make(Sprite* sps){
    Vec_Sprite vs = Vec_Sprite_New();

    for(int i = 0;sps[i].img;i++){
        Vector_Push(&vs,sps + i);
    }

    return vs;
}
void Vec_Sprite_Reload(Vec_Sprite* vs,int NewX,int NewY){
    for(int i = 0;i<vs->size;i++){
        Sprite* s = (Sprite*)Vector_Get(vs,i);
        Sprite_Reload(s,NewX,NewY);
    }
}
void Vec_Sprite_Free(Vec_Sprite* vs){
    for(int i = 0;i<vs->size;i++){
        Sprite* s = (Sprite*)Vector_Get(vs,i);
        Sprite_Free(s);
    }
    Vector_Free(vs);
}

#endif // !SPRITE_H
