#ifndef STRING_G_H
#define STRING_G_H

#include "String.h"
#include "CStr_G.h"
#include "ConstParser.h"

void String_RenderAlxFont(unsigned int* Target,int Target_Width,int Target_Height,AlxFont* f,String* str,float x,float y,unsigned int c){
    char* cstr = (char*)String_CStr(str);
    CStr_RenderAlxFont(Target,Target_Width,Target_Height,f,cstr,x,y,c);
    if(cstr) free(cstr);
}
void String_RenderSizeAlxFont(unsigned int* Target,int Target_Width,int Target_Height,AlxFont* f,String* str,int Size,float x,float y,unsigned int c){
    CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,f,(char*)str->Memory,str->size,x,y,c);
}

#endif