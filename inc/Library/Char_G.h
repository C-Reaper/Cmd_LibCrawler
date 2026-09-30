#ifndef CHAR_G_H
#define CHAR_G_H

#include "Char.h"
#include "Sprite.h"
#include "AlxFont.h"

void Char_RenderAlxFont(unsigned int* Target,int Target_Width,int Target_Height,AlxFont* f,char ch,float x,float y,unsigned int c){
    float ox = (float)(ch % f->Columns)  * f->CharSizeX;
    float oy = (float)(ch / f->Rows)     * f->CharSizeY;
    Sprite_RenderSubAlphaTint(Target,Target_Width,Target_Height,&f->Atlas,x,y,ox,oy,f->CharSizeX,f->CharSizeY,c);
}

#endif // !CHAR_G_H