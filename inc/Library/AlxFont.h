#ifndef ALXFONT_H
#define ALXFONT_H

#include "Sprite.h"

typedef struct AlxFont{
    Sprite Atlas;
    int Rows;
    int Columns;
    int CharSizeX;
    int CharSizeY;
} AlxFont;

#define ALXFONT_PATH_BLOCKY                        "/home/codeleaded/System/Fonts/Alx_Font_Blocky.png"      //"../../Fonts/Alx_Font_Blocky.png"
#define ALXFONT_PATH_YANIS                         "/home/codeleaded/System/Fonts/Alx_Font_Yanis.png"       //"../../Fonts/Alx_Font_Yanis.png"
#define ALXFONT_PATH_YANIS_NO_SHADOW               "/home/codeleaded/System/Fonts/Alx_Font_Yanis_No.png"    //"../../Fonts/Alx_Font_Yanis_No.png"
#define ALXFONT_PATH_HIGH                          "/home/codeleaded/System/Fonts/Alx_Font_High.png"        //"../../Fonts/Alx_Font_High.png"

#define ALXFONT_BLOCKY                             Sprite_Load(ALXFONT_PATH_BLOCKY),16,16,8,8
#define ALXFONT_YANIS                              Sprite_Load(ALXFONT_PATH_YANIS),16,16,16,16
#define ALXFONT_YANIS_NO_SHADOW                    Sprite_Load(ALXFONT_PATH_YANIS_NO_SHADOW),16,16,16,16
#define ALXFONT_HIGH                               Sprite_Load(ALXFONT_PATH_HIGH),16,16,32,64

#define ALXFONT_PATHS_BLOCKY                       ALXFONT_PATH_BLOCKY,16,16,8,8
#define ALXFONT_PATHS_YANIS                        ALXFONT_PATH_YANIS,16,16,16,16
#define ALXFONT_PATHS_YANIS_NO_SHADOW              ALXFONT_PATH_YANIS_NO_SHADOW,16,16,16,16
#define ALXFONT_PATHS_HIGH                         ALXFONT_PATH_HIGH,16,16,32,64

AlxFont AlxFont_New(Sprite s,int Columns,int Rows,int CharSizeX,int CharSizeY){
    AlxFont f;
    f.Atlas = s;
    f.Rows = Rows;
    f.Columns = Columns;
    f.CharSizeX = CharSizeX;
    f.CharSizeY = CharSizeY;
    return f;
}
AlxFont AlxFont_Null(){
    AlxFont f;
    f.Atlas = Sprite_Null();
    f.Rows = 0;
    f.Columns = 0;
    f.CharSizeX = 0;
    f.CharSizeY = 0;
    return f;
}
void AlxFont_Resize(AlxFont* f,int CharSizeX,int CharSizeY){
    if(f->CharSizeX != CharSizeX || f->CharSizeY != CharSizeY){
        Sprite_Resize(&f->Atlas,f->Columns * CharSizeX,f->Rows * CharSizeY);
        f->CharSizeX = CharSizeX;
        f->CharSizeY = CharSizeY;
    }
}
void AlxFont_Reload(AlxFont* f,int CharSizeX,int CharSizeY){
    if(f->CharSizeX != CharSizeX || f->CharSizeY != CharSizeY){
        Sprite_Reload(&f->Atlas,f->Columns * CharSizeX,f->Rows * CharSizeY);
        f->CharSizeX = CharSizeX;
        f->CharSizeY = CharSizeY;
    }
}
AlxFont AlxFont_Make(Sprite s,int Columns,int Rows,int CharSizeX,int CharSizeY,int NewCharSizeX,int NewCharSizeY){
    AlxFont f = AlxFont_New(s,Columns,Rows,CharSizeX,CharSizeY);
    AlxFont_Resize(&f,NewCharSizeX,NewCharSizeY);
    return f;
}
void AlxFont_Free(AlxFont* f){
    Sprite_Free(&f->Atlas);
}

#define AlxFont_MAKE_BLOCKY(CharX,CharY)           AlxFont_Make(Sprite_Load(ALXFONT_PATH_BLOCKY),16,16,8,8,CharX,CharY)
#define AlxFont_MAKE_YANIS(CharX,CharY)            AlxFont_Make(Sprite_Load(ALXFONT_PATH_YANIS),16,16,16,16,CharX,CharY)
#define AlxFont_MAKE_YANIS_NO_SHADOW(CharX,CharY)  AlxFont_Make(Sprite_Load(ALXFONT_PATH_YANIS_NO_SHADOW),16,16,16,16,CharX,CharY)
#define AlxFont_MAKE_HIGH(CharX,CharY)             AlxFont_Make(Sprite_Load(ALXFONT_PATH_HIGH),16,16,32,64,CharX,CharY)

#endif // !ALXFONT_H