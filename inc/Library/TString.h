#ifndef TSTRING_H
#define TSTRING_H

#include "../Container/Vector.h"
#include "../Container/Pair.h"

#include "String.h"
#include "Tex.h"
#include "Char_G.h"
#include "AlxFont.h"

typedef struct TString {
    String s;
    Tex si;
} TString;

TString TString_New(){
    TString ts;
    ts.s = String_New();
    ts.si = Vector_New(sizeof(SymbolInfo));
    return ts;
}
void TString_Sync(TString* ts){
    Tex_SyncString(&ts->si,&ts->s);
}
void TString_HighLight(TString* ts,HighLight* ps){
    HighLight_Tex_String(ps,&ts->si,&ts->s,(HighLight_State[]){ HIGHLIGHT_NONE });
}
void TString_Free(TString* ts){
    String_Free(&ts->s);
    Vector_Free(&ts->si);
}

void TCStr_RenderAlxFont(unsigned int* Target,int Target_Width,int Target_Height,AlxFont* f,void* t,char* cstr,float x,float y){
    SymbolInfo* tex = (SymbolInfo*)t;
    int Size = CStr_Size((char*)cstr);
    for(int i = 0;i<Size;i++){
        Char_RenderAlxFont(Target,Target_Width,Target_Height,f,cstr[i],x+i*f->CharSizeX,y,tex[i].c);
    }
}
void TCStr_RenderSizeAlxFont(unsigned int* Target,int Target_Width,int Target_Height,AlxFont* f,void* t,char* cstr,int Size,float x,float y){
    SymbolInfo* tex = (SymbolInfo*)t;
    for(int i = 0;i<Size;i++){
        Char_RenderAlxFont(Target,Target_Width,Target_Height,f,cstr[i],x+i*f->CharSizeX,y,tex[i].c);
    }
}

#endif