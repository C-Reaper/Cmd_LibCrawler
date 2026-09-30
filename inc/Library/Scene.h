#ifndef SCENE_H
#define SCENE_H

#include "../Container/List.h"
#include "../Container/PVector.h"

#include "Pixel.h"
#include "Rect.h"
#include "Math.h"
#include "AlxTime.h"
#include "String.h"
#include "Input.h"
#include "AlxFont.h"
#include "Geometry.h"

#include "CStr_G.h"
#include "String_G.h"
#include "TString.h"


typedef unsigned int EventId;

#define EVENT_ALL               0xFFFFFFFFU
#define EVENT_NONE              0U
#define EVENT_RESIZE            1U
#define EVENT_PRESSED           2U
#define EVENT_INSIDE_RELEASED   3U
#define EVENT_OUTSIDE_RELEASED  4U
#define EVENT_DOWN              5U
#define EVENT_INSIDE            6U
#define EVENT_MOVED             7U
#define EVENT_DRAGGED           8U
#define EVENT_ENTERED           9U
#define EVENT_EXITED            10U
#define EVENT_EMTY              11U
#define EVENT_FULL              12U
#define EVENT_RESET             13U
#define EVENT_START             14U
#define EVENT_STOP              15U
#define EVENT_FOCUSED           16U
#define EVENT_UNFOCUSED         17U
#define EVENT_ROTATE            18U
#define EVENT_BORDER            19U

#define EVENT_USE(e)            (1U << (e - 1U))
#define EVENT_ISDOWN(e)         (e==EVENT_PRESSED || e==EVENT_DOWN || e==EVENT_DRAGGED)

#define EVENT_STD_LABEL         (EVENT_USE(EVENT_MOVED) | EVENT_USE(EVENT_DRAGGED) | EVENT_USE(EVENT_ENTERED) | EVENT_USE(EVENT_EXITED))
#define EVENT_STD_BUTTON        (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_INSIDE_RELEASED) | EVENT_USE(EVENT_OUTSIDE_RELEASED) | EVENT_USE(EVENT_MOVED) | EVENT_USE(EVENT_DRAGGED) | EVENT_USE(EVENT_ENTERED) | EVENT_USE(EVENT_EXITED))
#define EVENT_STD_PROGRESSBAR   (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_INSIDE_RELEASED) | EVENT_USE(EVENT_OUTSIDE_RELEASED) | EVENT_USE(EVENT_EMTY) | EVENT_USE(EVENT_FULL) | EVENT_USE(EVENT_RESET) | EVENT_USE(EVENT_START) | EVENT_USE(EVENT_STOP))
#define EVENT_STD_SCROLLBAR     (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN) | EVENT_USE(EVENT_DRAGGED))
#define EVENT_STD_SLIDER        (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN) | EVENT_USE(EVENT_DRAGGED))
#define EVENT_STD_TEXTBOX       (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN) | EVENT_USE(EVENT_FOCUSED) | EVENT_USE(EVENT_UNFOCUSED))
#define EVENT_STD_SELECTION     (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN))
#define EVENT_STD_ROTATEABLE    (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN) | EVENT_USE(EVENT_ROTATE))
#define EVENT_STD_EDITOR        (EVENT_USE(EVENT_PRESSED) | EVENT_USE(EVENT_DOWN) | EVENT_USE(EVENT_FOCUSED) | EVENT_USE(EVENT_UNFOCUSED))
#define EVENT_STD_TILINGMANAGER EVENT_ALL


typedef unsigned int Alignment;
#define ALIGN_NO_ALIGN      0b00000000000000000000000000000000U
#define ALIGN_INVERT        0b10000000000000000000000000000000U
#define ALIGN_INVISIBLE     0b10000000000000000000000000000001U
#define ALIGN_VISIBLE       0b00000000000000000000000000000001U
#define ALIGN_VERT_CENTER   0b10000000000000000000000000000010U
#define ALIGN_HORI_CENTER   0b10000000000000000000000000000100U
#define ALIGN_VERT_TOP      0b00000000000000000000000000000010U
#define ALIGN_HORI_LEFT     0b00000000000000000000000000000100U
#define ALIGN_NOBORDER      0b10000000000000000000000000001000U
#define ALIGN_BORDER        0b00000000000000000000000000001000U
#define ALIGN_NOTILING      0b10000000000000000000000000010000U
#define ALIGN_TILING        0b00000000000000000000000000010000U

char Alignment_active(Alignment a,Alignment check){
    if(check & ALIGN_INVERT)    return (a & ~(check & ~ALIGN_INVERT)) == 0;
    else                        return (a & check) != 0;
}


typedef struct Renderable {
    unsigned int Bg;
    unsigned int Fg;
    unsigned int Bc;
    Alignment Align;
    int State;
    int UsedStates;
    int Remove;
    Rect rect;
    Rect rect_out;
    struct Renderable* Parent;
    void (*Render)(unsigned int*,int,int,void*,void*);
    void (*React)(void*,void*,EventId*);
    void (*Update)(void*,void*);
    void (*Input)(void*,void*,States*,Vec2,Vec2);
    void (*Free)(void*,void*);
} Renderable;

Renderable Renderable_New(
    Renderable* Parent,
    void (*Renderable_Render)(unsigned int*,int,int,void*,void*),
    void (*Renderable_React)(void*,void*,EventId*),
    void (*Renderable_Update)(void*,void*),
    void (*Renderable_Input)(void*,void*,States*,Vec2,Vec2),
    void (*Renderable_Free)(void*,void*),
    Rect rect,
    EventId UsedStates,
    unsigned int Align,
    unsigned int Bg,
    unsigned int Fg,
    unsigned int Bc
){
    Renderable b;
    b.Bg = Bg;
    b.Fg = Fg;
    b.Bc = Bc;
    b.Align = ALIGN_VISIBLE | Align;
    b.State = EVENT_NONE;
    b.UsedStates = UsedStates;
    b.Remove = 0;
    b.rect = rect;
    b.rect_out = rect;
    b.Parent = Parent;
    b.Render = (void (*)(unsigned int*,int,int,void*,void*))Renderable_Render;
    b.React = (void (*)(void*,void*,EventId*))Renderable_React;
    b.Update = (void (*)(void*,void*))Renderable_Update;
    b.Input = (void (*)(void*,void*,States*,Vec2,Vec2))Renderable_Input;
    b.Free = (void (*)(void*,void*))Renderable_Free;
    return b;
}
Renderable Renderable_Cpy(Renderable* r){
    Renderable b;
    b.Bg = r->Bg;
    b.Fg = r->Fg;
    b.Bc = r->Bc;
    b.Align = r->Align;
    b.State = r->State;
    b.UsedStates = r->UsedStates;
    b.Remove = 0;
    b.rect = r->rect;
    b.rect_out = r->rect_out;
    b.Parent = r->Parent;
    b.Render = r->Render;
    b.React = r->React;
    b.Update = r->Update;
    b.Input = r->Input;
    b.Free = r->Free;
    return b;
}
Rect Renderable_GetRect_R(Rect r,Rect parent){
    if(r.p.x >= -1.0f && r.p.x <= 1.0f) r.p.x = r.p.x * parent.d.x;
    if(r.p.y >= -1.0f && r.p.y <= 1.0f) r.p.y = r.p.y * parent.d.y;
    if(r.d.x <= 1.0f)                   r.d.x = r.d.x * parent.d.x;
    if(r.d.y <= 1.0f)                   r.d.y = r.d.y * parent.d.y;

    r.p.x += parent.p.x;
    r.p.y += parent.p.y;
    return r;
}
Rect Renderable_GetRect(Rect r,void* parent){
    const Renderable* pr = (const Renderable*)parent;
    if(pr){
        return Renderable_GetRect_R(r,pr->rect_out);
    }else{
        return r;
    }
}
void Renderable_Set(Renderable* b,Alignment a){
    if(a & ALIGN_INVERT)    b->Align = b->Align & ~(a & ~ALIGN_INVERT);
    else                    b->Align = b->Align | a;
}
void Renderable_Update(Renderable* b,void* parent){
    b->rect_out = Renderable_GetRect(b->rect,parent);
}
void Renderable_Free(void* parent,Renderable* b){
    if(b->Free) b->Free(parent,b);

    b->Bg = BLACK;
    b->Fg = BLACK;
    b->Bc = BLACK;
    b->Align = ALIGN_NO_ALIGN;
    b->State = EVENT_NONE;
    b->UsedStates = EVENT_NONE;
    b->Remove = 0;
    b->rect = (Rect){ { 0.0f,0.0f },{ 0.0f,0.0f } };
    b->Parent = NULL;
    b->Render = NULL;
    b->React = NULL;
    b->Update = NULL;
    b->Input = NULL;
    b->Free = NULL;
}
void Renderable_Print_N(Renderable* b){
    printf("%p %p %p %p %p %p | ",b->Parent,b->Render,b->React,b->Update,b->Input,b->Free);
    printf("%f %f %f %f | ",b->rect.p.x,b->rect.p.y,b->rect.d.x,b->rect.d.y);
    printf("%d %x %x %x %x",b->UsedStates,b->Align,b->Bg,b->Fg,b->Bc);
}
void Renderable_Print(Renderable* b){
    printf("--- Renderable ---\n");
    printf("%p %p %p %p %p %p\n",b->Parent,b->Render,b->React,b->Update,b->Input,b->Free);
    printf("%f %f %f %f\n",b->rect.p.x,b->rect.p.y,b->rect.d.x,b->rect.d.y);
    printf("%d %08x %08x %08x %08x\n",b->UsedStates,b->Align,b->Bg,b->Fg,b->Bc);
    printf("------------------\n");
}




typedef struct WindowComponentEvent {
    EventId eid;
    int ButtonId;
    Vec2 pos;
    Vec2 posBefore;
} WindowComponentEvent;
typedef struct WindowComponent {
    Renderable renderable;
    String text;
    Vec2 PixelSize;
} WindowComponent;

void WindowComponent_Free(void* parent,WindowComponent* b){
    String_Free(&b->text);
}
WindowComponent WindowComponent_New(Renderable* Parent,char* text,Vec2 PixelSize,Rect rect,unsigned int Bg){
    WindowComponent b;
    b.text = String_Make(text);
    b.PixelSize = PixelSize;
    b.renderable = Renderable_New(
        Parent,
        NULL,
        NULL,
        NULL,
        NULL,
        (void(*)(void*,void*))WindowComponent_Free,
        rect,
        EVENT_STD_LABEL,
        ALIGN_INVISIBLE | ALIGN_TILING,
        Bg,
        WHITE,
        BLACK
    );
    return b;
}
WindowComponent WindowComponent_NewStd(Renderable* Parent,char* text,Vec2 PixelSize,Rect rect,unsigned int Bg){
    WindowComponent b = WindowComponent_New(
        Parent,
        text,
        PixelSize,
        rect,
        Bg
    );
    return b;
}



typedef struct LabelEvent {
    EventId eid;
    int ButtonId;
    Vec2 pos;
    Vec2 posBefore;
} LabelEvent;
typedef struct Label {
    Renderable renderable;
    String text;
    AlxFont font;
} Label;

void Label_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Label* b){
    const Rect rect = b->renderable.rect_out;
    
    int StrSizeX = b->font.CharSizeX * b->text.size;
    int StrSizeY = b->font.CharSizeY;
    Vec2 Pos = rect.p;

    if(Alignment_active(b->renderable.Align,ALIGN_HORI_CENTER)) Pos.x += rect.d.x / 2 - StrSizeX / 2;
    if(Alignment_active(b->renderable.Align,ALIGN_VERT_CENTER)) Pos.y += rect.d.y / 2 - StrSizeY / 2;
    unsigned int BgColor = b->renderable.Bg;
    
    Rect_Render(Target,Target_Width,Target_Height,rect,BgColor);

    if(Alignment_active(b->renderable.Align,ALIGN_BORDER))
        Rect_RenderWire(Target,Target_Width,Target_Height,rect,b->renderable.Bc,1.0f);
    
    CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&b->font,b->text.Memory,b->text.size,Pos.x,Pos.y,b->renderable.Fg);
}
void Label_Event(void* parent,Label* b,LabelEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Label_Update(void* parent,Label* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Label_Input(void* parent,Label* b,States* states,Vec2 Mouse,Vec2 MouseB){
    const Rect rect = b->renderable.rect_out;
    b->renderable.State = EVENT_NONE;

    if(!Overlap_Rect_Point(rect,Mouse) &&  Overlap_Rect_Point(rect,MouseB))
        Label_Event(parent,b,(LabelEvent){ .eid = EVENT_ENTERED });
    if( Overlap_Rect_Point(rect,Mouse) && !Overlap_Rect_Point(rect,MouseB))
        Label_Event(parent,b,(LabelEvent){ .eid = EVENT_EXITED });

    if(Overlap_Rect_Point(rect,Mouse)){
        if(Mouse.x==MouseB.x && Mouse.y==MouseB.y){
            if(states[ALX_MOUSE_L].DOWN)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_DOWN,      .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Label_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE,    .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }else{
            if(states[ALX_MOUSE_L].DOWN)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_DRAGGED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Label_Event(parent,b,(LabelEvent){ .eid = EVENT_MOVED,     .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }

        if(states[ALX_MOUSE_L].PRESSED)     Label_Event(parent,b,(LabelEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].PRESSED)     Label_Event(parent,b,(LabelEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].PRESSED)     Label_Event(parent,b,(LabelEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });

        if(states[ALX_MOUSE_L].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });
    }else{
        if(states[ALX_MOUSE_L].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].RELEASED)    Label_Event(parent,b,(LabelEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });
    }
}
void Label_Free(void* parent,Label* b){
    AlxFont_Free(&b->font);
    String_Free(&b->text);
}
Label Label_New(Renderable* Parent,char* text,void (*Label_React)(void*,Label*,LabelEvent*),AlxFont font,Vec2 AlxFontSize,Rect rect,unsigned int Align,unsigned int Bg,unsigned int Fg){
    Label b;
    b.text = String_Make(text);
    b.font = font;
    AlxFont_Resize(&b.font,(int)AlxFontSize.x,(int)AlxFontSize.y);
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Label_Render,
        (void(*)(void*,void*,EventId*))Label_React,
        (void(*)(void*,void*))Label_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Label_Input,
        (void(*)(void*,void*))Label_Free,
        rect,
        EVENT_STD_LABEL,
        Align | ALIGN_TILING,
        Bg,
        Fg,
        BLACK
    );
    return b;
}
Label Label_NewStd(Renderable* Parent,char* text,void (*Label_React)(void*,Label*,LabelEvent*),Vec2 AlxFontSize,Rect rect,unsigned int Bg,unsigned int Fg){
    Label b = Label_New(
        Parent,
        text,
        Label_React,
        AlxFont_New(ALXFONT_BLOCKY),
        AlxFontSize,
        rect,
        ALIGN_BORDER,
        Bg,
        Fg
    );
    return b;
}


typedef LabelEvent ButtonEvent;
typedef Label Button;

void Button_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Button* b){
    const Rect rect = b->renderable.rect_out;

    int StrSizeX = b->font.CharSizeX * b->text.size;
    int StrSizeY = b->font.CharSizeY;
    
    Vec2 Pos = rect.p;
    Vec2 PosText = rect.p;
    if(Alignment_active(b->renderable.Align,ALIGN_HORI_CENTER)) PosText.x += rect.d.x / 2 - StrSizeX / 2;
    if(Alignment_active(b->renderable.Align,ALIGN_VERT_CENTER)) PosText.y += rect.d.y / 2 - StrSizeY / 2;
    
    unsigned int BgColor = b->renderable.Bg;
    if(b->renderable.State==EVENT_DOWN || b->renderable.State==EVENT_DRAGGED){
        BgColor = Pixel_Mul(BgColor,Pixel_toRGBA(0.8f,0.8f,0.8f,1.0f));
        //Pos.y += rect.d.y * 0.05f;
        //PosText.y += rect.d.y * 0.05f;
    }
    
    Rect_RenderXX(Target,Target_Width,Target_Height,Pos.x,Pos.y,rect.d.x,rect.d.y,BgColor);
    if(Alignment_active(b->renderable.Align,ALIGN_BORDER))
        Rect_RenderXXWire(Target,Target_Width,Target_Height,Pos.x,Pos.y,rect.d.x,rect.d.y,b->renderable.Bc,1.0f);
    CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&b->font,b->text.Memory,b->text.size,PosText.x,PosText.y,b->renderable.Fg);
}
void Button_Event(void* parent,Button* b,ButtonEvent be){
    Label_Event(parent,(Label*)b,*(LabelEvent*)&be);
}
void Button_Update(void* parent,Button* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Button_Input(void* parent,Button* b,States* states,Vec2 Mouse,Vec2 MouseB){
    b->renderable.State = EVENT_NONE;
    Label_Input(parent,(Label*)b,states,Mouse,MouseB);
}
void Button_Free(void* parent,Button* b){
    AlxFont_Free(&b->font);
    String_Free(&b->text);
}
Button Button_New(Renderable* Parent,char* text,void (*Button_React)(void* parent,Button* b,ButtonEvent* be),AlxFont font,Vec2 AlxFontSize,Rect rect,int Align,unsigned int Bg,unsigned int Fg){
    Button b;
    b.text = String_Make(text);
    b.font = font;
    AlxFont_Resize(&b.font,(int)AlxFontSize.x,(int)AlxFontSize.y);
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Button_Render,
        (void(*)(void*,void*,EventId*))Button_React,
        (void(*)(void*,void*))Button_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Button_Input,
        (void(*)(void*,void*))Button_Free,
        rect,
        EVENT_STD_BUTTON,
        Align,
        Bg,
        Fg,
        BLACK
    );
    return b;
}
Button Button_NewStd(Renderable* Parent,char* text,void (*Button_React)(void* parent,Button* b,ButtonEvent* be),Vec2 AlxFontSize,Rect rect,unsigned int Bg,unsigned int Fg){
    return Button_New(
        Parent,
        text,
        Button_React,
        AlxFont_New(ALXFONT_BLOCKY),
        AlxFontSize,
        rect,
        ALIGN_BORDER | ALIGN_TILING,
        Bg,
        Fg
    );
}
Button Button_Cpy(Button* b){
    Button ret;
    ret.font = AlxFont_Make(
        Sprite_Cpy(&b->font.Atlas),
        b->font.Columns,
        b->font.Rows,
        b->font.CharSizeX,
        b->font.CharSizeY,
        b->font.CharSizeX,
        b->font.CharSizeY
    );
    ret.renderable = Renderable_Cpy(&b->renderable);
    ret.text = String_Cpy(&b->text);
    return ret;
}
Button Button_Null(){
    Button h;
    memset(&h,0,sizeof(Button));
    return h;
}
CStr Button_CStr(Button* h){
    String builder = String_Format("{  }");
    CStr resstr = String_CStr(&builder);
    String_Free(&builder);
    return resstr;
}


#define PROGRESSBAR_STOP        0
#define PROGRESSBAR_RUNNING     1

typedef LabelEvent ProgressBarEvent;
typedef struct ProgressBar {
    Label label;
    Timepoint LastTime;
    unsigned int BarColor;
    float Speed;
    float Progress;
    char status;
} ProgressBar;

void ProgressBar_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,ProgressBar* b){
    const Rect rect = b->label.renderable.rect_out;

    int StrSizeX = b->label.font.CharSizeX * b->label.text.size;
    int StrSizeY = b->label.font.CharSizeY;
    Vec2 Pos = rect.p;
    Vec2 PosText = rect.p;
    if(Alignment_active(b->label.renderable.Align,ALIGN_HORI_CENTER)) PosText.x += rect.d.x / 2 - StrSizeX / 2;
    if(Alignment_active(b->label.renderable.Align,ALIGN_VERT_CENTER)) PosText.y += rect.d.y / 2 - StrSizeY / 2;
    unsigned int BgColor = b->label.renderable.Bg;
    
    Rect_RenderXX(Target,Target_Width,Target_Height,Pos.x,Pos.y,rect.d.x,rect.d.y,BgColor);
    CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&b->label.font,b->label.text.Memory,b->label.text.size,PosText.x,PosText.y,b->label.renderable.Fg);

    Rect_RenderXXAlpha(Target,Target_Width,Target_Height,Pos.x,Pos.y,rect.d.x * b->Progress,rect.d.y,b->BarColor);
    if(Alignment_active(b->label.renderable.Align,ALIGN_BORDER))
        Rect_RenderXXWire(Target,Target_Width,Target_Height,Pos.x,Pos.y,rect.d.x,rect.d.y,b->label.renderable.Bc,1.0f);
}
void ProgressBar_Event(void* parent,ProgressBar* b,ProgressBarEvent be){
    Label_Event(parent,(Label*)b,*(LabelEvent*)&be);
}
void ProgressBar_Update(void* parent,ProgressBar* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->label.renderable,parent);
}
void ProgressBar_Input(void* parent,ProgressBar* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Label_Input(parent,(Label*)b,states,Mouse,MouseB);
    
    if(b->status==PROGRESSBAR_RUNNING){
        if(b->Progress==0.0f) ProgressBar_Event(parent,b,(ProgressBarEvent){ .eid = EVENT_EMTY });
        if(b->Progress==1.0f){
            b->status = PROGRESSBAR_STOP;
            ProgressBar_Event(parent,b,(ProgressBarEvent){ .eid = EVENT_FULL });
        }

        double AlxTime = (double)(Time_Nano()-b->LastTime) / (double)TIME_NANOTOSEC;
        b->LastTime = Time_Nano();
        b->Progress = F32_Min(b->Progress + b->Speed * (float)AlxTime,1.0f);
    }
}
void ProgressBar_Free(void* parent,ProgressBar* b){
    AlxFont_Free(&b->label.font);
    String_Free(&b->label.text);
}
ProgressBar ProgressBar_New(Renderable* Parent,char* text,void (*ProgressBar_React)(void* parent,ProgressBar* b,ProgressBarEvent* be),AlxFont font,Vec2 AlxFontSize,Rect rect,unsigned int Align,unsigned int Bg,unsigned int Fg,unsigned int BarColor,Timepoint LastTime,float Speed){
    ProgressBar b;
    b.label.text = String_Make(text);
    b.label.font = font;
    AlxFont_Resize(&b.label.font,(int)AlxFontSize.x,(int)AlxFontSize.y);
    b.label.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))ProgressBar_Render,
        (void(*)(void*,void*,EventId*))ProgressBar_React,
        (void(*)(void*,void*))ProgressBar_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))ProgressBar_Input,
        (void(*)(void*,void*))ProgressBar_Free,
        rect,
        EVENT_STD_PROGRESSBAR,
        Align,
        Bg,
        Fg,
        BLACK
    );

    b.LastTime = LastTime;
    b.BarColor = BarColor;
    b.Speed = Speed;
    b.Progress = 0.0f;
    b.status = PROGRESSBAR_STOP;
    return b;
}
ProgressBar ProgressBar_NewStd(Renderable* Parent,char* text,void (*ProgressBar_React)(void* parent,ProgressBar* b,ProgressBarEvent* be),Vec2 AlxFontSize,Rect rect,unsigned int Bg,unsigned int Fg,unsigned int BarColor,float Speed){
    ProgressBar b = ProgressBar_New(
        Parent,text,
        ProgressBar_React,
        AlxFont_New(ALXFONT_BLOCKY),
        AlxFontSize,
        rect,
        ALIGN_BORDER | ALIGN_TILING,
        Bg,
        Fg,
        BarColor,
        Time_Nano(),
        0.1f
    );
    return b;
}
void ProgressBar_Reset(void* parent,ProgressBar* b){
    b->status = PROGRESSBAR_RUNNING;
    b->Progress = 0.0f;
    b->LastTime = Time_Nano();

    ProgressBar_Event(parent,b,(ProgressBarEvent){ .eid = EVENT_RESET });
}
void ProgressBar_Start(void* parent,ProgressBar* b){
    b->status = PROGRESSBAR_RUNNING;
    b->Progress = 0.0f;
    b->LastTime = Time_Nano();

    ProgressBar_Event(parent,b,(ProgressBarEvent){ .eid = EVENT_START });
}
void ProgressBar_Stop(void* parent,ProgressBar* b){
    b->status = PROGRESSBAR_STOP;
    ProgressBar_Event(parent,b,(ProgressBarEvent){ .eid = EVENT_STOP });
}


typedef LabelEvent ScrollbarEvent;
typedef struct Scrollbar {
    Renderable renderable;
    float scrollpart;
    float scrolled;
} Scrollbar;

void Scrollbar_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Scrollbar* b){
    const Rect rect = b->renderable.rect_out;

    unsigned int FgColor = b->renderable.Fg;
    if(b->renderable.State==EVENT_DOWN || b->renderable.State==EVENT_DRAGGED){
        FgColor = Pixel_Mul(FgColor,Pixel_toRGBA(0.8f,0.8f,0.8f,1.0f));
    }
    
    Rect_RenderXX(Target,Target_Width,Target_Height,rect.p.x,rect.p.y,rect.d.x,rect.d.y,b->renderable.Bg);
    if(Alignment_active(b->renderable.Align,ALIGN_BORDER))
        Rect_RenderXXWire(Target,Target_Width,Target_Height,rect.p.x,rect.p.y,rect.d.x,rect.d.y,b->renderable.Bc,1.0f);

    float bw = rect.d.x;
    float bh = rect.d.y * b->scrollpart;
    float bx = rect.p.x;
    float by = rect.p.y + (rect.d.y - bh) * b->scrolled;

    Rect_RenderXX(Target,Target_Width,Target_Height,bx,by,bw,bh,FgColor);
}
void Scrollbar_Event(void* parent,Scrollbar* b,ScrollbarEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Scrollbar_Update(void* parent,Scrollbar* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Scrollbar_Input(void* parent,Scrollbar* b,States* states,Vec2 Mouse,Vec2 MouseB){
    const Rect rect = b->renderable.rect_out;
    
    b->renderable.State = EVENT_NONE;

    if(Overlap_Rect_Point(rect,Mouse)){
        if(states[ALX_MOUSE_L].DOWN){
            b->scrolled = (Mouse.y - rect.p.y) / rect.d.y;
            Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_DOWN,      .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }
        
        if(Mouse.x!=MouseB.x || Mouse.y!=MouseB.y){
            if(states[ALX_MOUSE_L].DOWN)    Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_DRAGGED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_MOVED,     .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }

        if(states[ALX_MOUSE_L].PRESSED)     Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_PRESSED,           .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_L].RELEASED)    Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE_RELEASED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
    }else{
        if(states[ALX_MOUSE_L].RELEASED)
            Scrollbar_Event(parent,b,(LabelEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
    }
}
void Scrollbar_Free(void* parent,Scrollbar* b){

}
Scrollbar Scrollbar_New(Renderable* Parent,void (*Scrollbar_React)(void*,Scrollbar*,ScrollbarEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    Scrollbar b;
    b.scrollpart = 0.1f;
    b.scrolled = 0.0f;
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Scrollbar_Render,
        (void(*)(void*,void*,EventId*))Scrollbar_React,
        (void(*)(void*,void*))Scrollbar_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Scrollbar_Input,
        (void(*)(void*,void*))Scrollbar_Free,
        rect,
        EVENT_STD_SCROLLBAR,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        Fg,
        BLACK
    );
    return b;
}


typedef LabelEvent SliderEvent;
typedef struct Slider {
    Renderable renderable;
    float scrollpart;
    float scrolled;
} Slider;

void Slider_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Slider* b){
    const Rect rect = b->renderable.rect_out;

    unsigned int FgColor = b->renderable.Fg;
    if(b->renderable.State==EVENT_DOWN || b->renderable.State==EVENT_DRAGGED){
        FgColor = Pixel_Mul(FgColor,Pixel_toRGBA(0.8f,0.8f,0.8f,1.0f));
    }
    
    Rect_RenderXX(Target,Target_Width,Target_Height,rect.p.x,rect.p.y,rect.d.x,rect.d.y,b->renderable.Bg);
    if(Alignment_active(b->renderable.Align,ALIGN_BORDER))
        Rect_RenderXXWire(Target,Target_Width,Target_Height,rect.p.x,rect.p.y,rect.d.x,rect.d.y,b->renderable.Bc,1.0f);

    float bw = rect.d.x * b->scrollpart;
    float bh = rect.d.y;
    float bx = rect.p.x + (rect.d.x - bw) * b->scrolled;
    float by = rect.p.y;

    Rect_RenderXX(Target,Target_Width,Target_Height,bx,by,bw,bh,FgColor);
}
void Slider_Event(void* parent,Slider* b,SliderEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Slider_Update(void* parent,Slider* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Slider_Input(void* parent,Slider* b,States* states,Vec2 Mouse,Vec2 MouseB){
    const Rect rect = b->renderable.rect_out;

    b->renderable.State = EVENT_NONE;

    if(Overlap_Rect_Point(rect,Mouse)){
        if(states[ALX_MOUSE_L].DOWN){
            b->scrolled = (Mouse.x - rect.p.x) / rect.d.x;

            Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_DOWN,      .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }
        
        if(Mouse.x!=MouseB.x || Mouse.y!=MouseB.y){
            if(states[ALX_MOUSE_L].DOWN)    Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_DRAGGED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_MOVED,     .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }

        if(states[ALX_MOUSE_L].PRESSED)     Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_PRESSED,           .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_L].RELEASED)    Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_INSIDE_RELEASED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
    }else{
        if(states[ALX_MOUSE_L].RELEASED)
            Slider_Event(parent,b,(LabelEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
    }
}
void Slider_Free(void* parent,Slider* b){

}
Slider Slider_New(Renderable* Parent,void (*Slider_React)(void*,Slider*,SliderEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    Slider b;
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Slider_Render,
        (void(*)(void*,void*,EventId*))Slider_React,
        (void(*)(void*,void*))Slider_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Slider_Input,
        (void(*)(void*,void*))Slider_Free,
        rect,
        EVENT_STD_SLIDER,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        Fg,
        BLACK
    );
    b.scrollpart = 0.1f;
    b.scrolled = 0.0f;
    return b;
}


typedef LabelEvent TextboxEvent;
typedef struct Textbox {
    Renderable renderable;
    AlxFont font;
    int ScrollX;
    int ScrollY;
    Input In;
} Textbox;

void Textbox_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Textbox* tb){
    const Rect rect = tb->renderable.rect_out;

    Rect_Render(Target,Target_Width,Target_Height,rect,tb->renderable.Bg);
    if(Alignment_active(tb->renderable.Align,ALIGN_BORDER))
        Rect_RenderWire(Target,Target_Width,Target_Height,rect,tb->renderable.Bc,1.0f);

    int CurserX = Input_CurserX(&tb->In,tb->In.Curser);
    int CurserY = Input_CurserY(&tb->In,tb->In.Curser);

    const float text_width = (rect.d.x);
    const float text_height = rect.d.y;
    const float text_x = (rect.p.x);
    const float text_y = rect.p.y;

    float xEnd = text_width / (float)tb->font.CharSizeX * 0.8f;
    float yEnd = text_height / (float)tb->font.CharSizeY * 0.8f;

    if(tb->In.Enabled){
        if(tb->ScrollX + CurserX < 0 || tb->ScrollX + CurserX > xEnd){
            tb->ScrollX = F32_Clamp(tb->ScrollX,-CurserX,-CurserX + (int)xEnd);
            //tb->ScrollX = -CurserX;
            //tb->ScrollX = (int)(F32_Abs(tb->ScrollX) <= F32_Abs(text_width / tb->font.CharSizeX - 5)?0: tb->ScrollX + text_width / tb->font.CharSizeX - 5);
        }
        if(tb->ScrollY + CurserY < 0 || tb->ScrollY + CurserY > yEnd){
            tb->ScrollY = F32_Clamp(tb->ScrollY,-CurserY,-CurserY + (int)yEnd);
            if(tb->In.MaxLine==1) tb->ScrollY = 0;
            //tb->ScrollY = -CurserY;
            //tb->ScrollY = (int)(F32_Abs(tb->ScrollY) <= F32_Abs(text_height / tb->font.CharSizeY - 5)?0: tb->ScrollY + text_height / tb->font.CharSizeY - 5);
        }
    }

    int Lines = -tb->ScrollY;
    int FirstChar = String_FirstCharOfLine(&tb->In.Buffer,Lines);
    int LastChar = String_LastCharOfLine(&tb->In.Buffer,Lines);
    int MaxLine = -tb->ScrollY + (int)yEnd;

    MaxLine = I64_Max(1,MaxLine);

    for(int i = Lines;i<=MaxLine;i++){
        FirstChar -= tb->ScrollX;
        int Size = LastChar - FirstChar;
        float x = tb->ScrollX * tb->font.CharSizeX + text_x;
        float y = text_y + (tb->ScrollY + i) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);

        x = F32_Clamp(x,text_x,text_x+text_width);
        y = F32_Clamp(y,text_y,text_y+text_height);
        Size = (int)F32_Clamp(Size,0,(text_width - (x - text_x)) / tb->font.CharSizeX);

        CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&tb->font,(char*)(tb->In.Buffer.Memory + FirstChar),Size,x,y,tb->renderable.Fg);

        int TempF = String_FirstCharOfLineLast(&tb->In.Buffer,i+1,FirstChar,LastChar,i);
        int TempL = String_LastCharOfLineLast(&tb->In.Buffer,i+1,FirstChar,LastChar,i);
        FirstChar = TempF;
        LastChar = TempL;

        if(LastChar>tb->In.Buffer.size){
            break;
        }
    }

    if(tb->In.Enabled){
        Char_RenderAlxFont(Target,Target_Width,Target_Height,&tb->font,'_',text_x+(CurserX + tb->ScrollX) * tb->font.CharSizeX,text_y+(CurserY + tb->ScrollY) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR),BLACK);

        if(tb->In.Curser!=tb->In.CurserEnd && tb->In.CurserEnd>=0){
            int Up = tb->In.Curser<tb->In.CurserEnd?tb->In.Curser:tb->In.CurserEnd;
            int Down = tb->In.Curser>tb->In.CurserEnd?tb->In.Curser:tb->In.CurserEnd;

            int Before = Up;
            for(int i = Up;;i++){
                if(String_Get(&tb->In.Buffer,i)=='\n'){
                    int X = (i+1<Down?i+1:Down+1) - Before;

                    float x = text_x+(Input_CurserX(&tb->In,Before) + tb->ScrollX) * tb->font.CharSizeX;
                    float y = text_y+(Input_CurserY(&tb->In,Before) + tb->ScrollY) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
                    float w = X * tb->font.CharSizeX;
                    float h = (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
                    
                    x = F32_Clamp(x,text_x,text_x+text_width);
                    y = F32_Clamp(y,text_y,text_y+text_height);
                    w = F32_Clamp(w,0.0f,text_width - (x - text_x));
                    h = F32_Clamp(h,0.0f,text_height - (y - text_y));
                    
                    Rect_RenderXXAlpha(Target,Target_Width,Target_Height,x,y,w,h,0x99BBBBBB);
                    Before = i+1;
                }
                if(i>=Down && String_Get(&tb->In.Buffer,i)=='\n'){
                    break;
                }
            }
        }
    }
}
void Textbox_Event(void* parent,Textbox* b,TextboxEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Textbox_Update(void* parent,Textbox* tb,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&tb->renderable,parent);
}
void Textbox_Input(void* parent,Textbox* tb,States* states,Vec2 Mouse,Vec2 MouseB){
    const Rect rect = tb->renderable.rect_out;
    
    tb->renderable.State = EVENT_NONE;
    Input_Update(&tb->In,states);
    
    if(Input_Stroke(&tb->In,ALX_MOUSE_L).PRESSED){
        if(Overlap_Rect_Point(rect,Mouse)){
            tb->In.Enabled = 1;
            Textbox_Event(parent,tb,(LabelEvent){ .eid = EVENT_FOCUSED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }else{
            tb->In.Enabled = 0;
            Textbox_Event(parent,tb,(LabelEvent){ .eid = EVENT_UNFOCUSED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }
    }
    if(Input_Stroke(&tb->In,ALX_MOUSE_L).PRESSED){
        tb->In.Curser = Input_GetCurser(&tb->In,(Mouse.x - rect.p.x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - rect.p.y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    }
    if(Input_Stroke(&tb->In,ALX_MOUSE_L).DOWN){
        tb->In.CurserEnd = Input_GetCurser(&tb->In,(Mouse.x - rect.p.x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - rect.p.y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    }
    
    Input_DefaultReact(&tb->In,tb);
}
void Textbox_Free(void* parent,Textbox* b){
    AlxFont_Free(&b->font);
    Input_Free(&b->In);
}
Textbox Textbox_New(Renderable* Parent,char *text,void (*Textbox_React)(void*,Textbox*,TextboxEvent*),AlxFont font,Vec2 AlxFontSize,Rect rect,unsigned int Lines,unsigned int Bg,unsigned int Fg){
    Textbox b;
    b.font = font;
    AlxFont_Resize(&b.font,(int)AlxFontSize.x,(int)AlxFontSize.y);
    b.ScrollX = 0;
    b.ScrollY = 0;
    
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Textbox_Render,
        (void(*)(void*,void*,EventId*))Textbox_React,
        (void(*)(void*,void*))Textbox_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Textbox_Input,
        (void(*)(void*,void*))Textbox_Free,
        rect,
        EVENT_STD_TEXTBOX,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        Fg,
        BLACK
    );

    b.In = Input_New(10,Lines);
    Input_SetText(&b.In,text);
    return b;
}
Textbox Textbox_NewStd(Renderable* Parent,char *text,void (*Textbox_React)(void*,Textbox*,TextboxEvent*),Vec2 AlxFontSize,Rect rect,unsigned int Lines,unsigned int Bg,unsigned int Fg){
    Textbox b = Textbox_New(
        Parent,
        text,
        Textbox_React,
        AlxFont_New(ALXFONT_BLOCKY),
        AlxFontSize,
        rect,
        Lines,
        Bg,
        Fg
    );
    return b;
}


typedef LabelEvent SelectionEvent;
typedef struct Selection {
    Renderable renderable;
} Selection;

void Selection_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Selection* b){
    const Rect rect = b->renderable.rect_out;

    //unsigned int FgColor = b->renderable.Fg;
    //if(Alignment_active(b->renderable.Align,ALIGN_BORDER))

    Rect_Render(Target,Target_Width,Target_Height,rect,b->renderable.Bg);
}
void Selection_Event(void* parent,Selection* b,SelectionEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Selection_Update(void* parent,Selection* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Selection_Input(void* parent,Selection* b,States* states,Vec2 Mouse,Vec2 MouseB){
    b->renderable.State = EVENT_NONE;
}
void Selection_Free(void* parent,Selection* b){

}
Selection Selection_New(Renderable* Parent,void (*Selection_React)(void*,Selection*,SelectionEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    Selection b;
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Selection_Render,
        (void(*)(void*,void*,EventId*))Selection_React,
        (void(*)(void*,void*))Selection_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Selection_Input,
        (void(*)(void*,void*))Selection_Free,
        rect,
        EVENT_STD_SELECTION,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER | ALIGN_TILING,
        Bg,
        Fg,
        BLACK
    );
    return b;
}


typedef LabelEvent RotatableEvent;
typedef struct Rotatable {
    Renderable renderable;
    float angle;
} Rotatable;

Circle Rotatable_Circle(Rotatable* b){
    const Rect rect = b->renderable.rect_out;

    Vec2 p = Vec2_Add(rect.p,Vec2_Mulf(rect.d,0.5f));
    float r;
    if(rect.d.x < rect.d.y)     r = rect.d.x * 0.5f;
    else                        r = rect.d.y * 0.5f;
    return Circle_New(p,r);
}
void Rotatable_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Rotatable* b){
    const Rect rect = b->renderable.rect_out;

    unsigned int BgColor = b->renderable.Bg;
    Rect_Render(Target,Target_Width,Target_Height,rect,BgColor);

    if(Alignment_active(b->renderable.Align,ALIGN_BORDER))
        Rect_RenderWire(Target,Target_Width,Target_Height,rect,b->renderable.Bc,1.0f);


    Circle c = Rotatable_Circle(b);
    Vec2 t = Vec2_Add(c.p,Vec2_Mulf(Vec2_OfAngle(b->angle),c.r));

    Circle_Render(Target,Target_Width,Target_Height,c,b->renderable.Fg);
    Line_RenderX(Target,Target_Width,Target_Height,c.p,t,BLACK,1.0f);

    Circle oc = c;
    oc.r *= 0.7f;
    Circle_Render(Target,Target_Width,Target_Height,oc,Pixel_Mul(b->renderable.Fg,Pixel_toRGBA(0.8f,0.8f,0.8f,1.0f)));
}
void Rotatable_Event(void* parent,Rotatable* b,RotatableEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Rotatable_Update(void* parent,Rotatable* b,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&b->renderable,parent);
}
void Rotatable_Input(void* parent,Rotatable* b,States* states,Vec2 Mouse,Vec2 MouseB){
    b->renderable.State = EVENT_NONE;
    
    Circle circle = Rotatable_Circle(b);
    if(Overlap_Circle_Point(circle,Mouse)){
        if(states[ALX_MOUSE_L].PRESSED || states[ALX_MOUSE_L].DOWN){
            b->angle = Vec2_AngleOf(Vec2_Sub(Mouse,circle.p));
            Rotatable_Event(parent,b,(RotatableEvent){ .eid = EVENT_ROTATE,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }
    }
}
void Rotatable_Free(void* parent,Rotatable* b){
    
}
Rotatable Rotatable_New(Renderable* Parent,void (*Rotatable_React)(void*,Rotatable*,RotatableEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    Rotatable b;
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Rotatable_Render,
        (void(*)(void*,void*,EventId*))Rotatable_React,
        (void(*)(void*,void*))Rotatable_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Rotatable_Input,
        (void(*)(void*,void*))Rotatable_Free,
        rect,
        EVENT_STD_ROTATEABLE,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER | ALIGN_TILING,
        Bg,
        Fg,
        BLACK
    );
    return b;
}


#define EDITOR_LINENUMBERR     4.5

typedef struct Editor_Info {
    CStr name;
    CStr msg;
    unsigned int start_line;
    unsigned int start_char;
    unsigned int end_line;
    unsigned int end_char;
} Editor_Info;

Editor_Info Editor_Info_New(CStr name,CStr msg,unsigned int start_line,unsigned int start_char,unsigned int end_line,unsigned int end_char){
    Editor_Info ei;
    ei.name = CStr_Cpy(name);
    ei.msg = CStr_Cpy(msg);
    ei.start_line = start_line;
    ei.start_char = start_char;
    ei.end_line = end_line;
    ei.end_char = end_char;
    return ei;
}
Editor_Info Editor_Info_Null(){
    Editor_Info ei;
    memset(&ei,0,sizeof(ei));
    return ei;
}
void Editor_Info_Print(Editor_Info* ei){
    printf("%s: %s\n",ei->name,ei->msg);
    printf("%u(%u) - %u(%u)\n",ei->start_line,ei->start_char,ei->end_line,ei->end_char);
}
void Editor_Info_Free(Editor_Info* ei){
    CStr_Free(&ei->name);
    CStr_Free(&ei->msg);
}


typedef LabelEvent EditorEvent;
typedef struct Editor {
    Renderable renderable;
    Tex t;
    HighLight Syntax;
    AlxFont font;
    int ScrollX;
    int ScrollY;
    int ShowLines;
    Input In;
    Vector infos;
} Editor;

void Editor_Info_Clear(Editor* b){
    for(int i = 0;i<b->infos.size;i++){
        Editor_Info* ei = (Editor_Info*)Vector_Get(&b->infos,i);
        Editor_Info_Free(ei);
    }
    Vector_Clear(&b->infos);
}
void Editor_Info_Add(Editor* b,Editor_Info ei){
    Editor_Info_Print(&ei);
    Vector_Push(&b->infos,&ei);
}

void Editor_Syntax(Editor* b,char* Path){
    if(Path)    HighLight_Set(&b->Syntax,Path);
    else        HighLight_Clear(&b->Syntax);
}
void Editor_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Editor* tb){
    const Rect rect = tb->renderable.rect_out;

    Tex_SyncString(&tb->t,&tb->In.Buffer);
    HighLight_Tex_String(&tb->Syntax,&tb->t,&tb->In.Buffer,(HighLight_State[]){ HIGHLIGHT_NONE });

    Rect_Render(Target,Target_Width,Target_Height,rect,tb->renderable.Bg);
    if(Alignment_active(tb->renderable.Align,ALIGN_BORDER))
        Rect_RenderWire(Target,Target_Width,Target_Height,rect,tb->renderable.Bc,1.0f);

    int CurserX = Input_CurserX(&tb->In,tb->In.Curser);
    int CurserY = Input_CurserY(&tb->In,tb->In.Curser);

    const float LineNumber = (tb->ShowLines ? EDITOR_LINENUMBERR : 0.0f) * tb->font.CharSizeX;
    const float text_width = (rect.d.x - LineNumber);
    const float text_height = rect.d.y;
    const float text_x = (rect.p.x + LineNumber);
    const float text_y = rect.p.y;

    float xEnd = text_width / (float)tb->font.CharSizeX * 0.8f;
    float yEnd = text_height / (float)tb->font.CharSizeY * 0.8f;

    if(tb->In.Enabled){
        if(tb->ScrollX + CurserX < 0 || tb->ScrollX + CurserX > xEnd){
            tb->ScrollX = F32_Clamp(tb->ScrollX,-CurserX,-CurserX + (int)xEnd);
            //tb->ScrollX = -CurserX;
            //tb->ScrollX = (int)(F32_Abs(tb->ScrollX) <= F32_Abs(text_width / tb->font.CharSizeX - 5)?0: tb->ScrollX + text_width / tb->font.CharSizeX - 5);
        }
        if(tb->ScrollY + CurserY < 0 || tb->ScrollY + CurserY > yEnd){
            tb->ScrollY = F32_Clamp(tb->ScrollY,-CurserY,-CurserY + (int)yEnd);
            if(tb->In.MaxLine==1) tb->ScrollY = 0;
            //tb->ScrollY = -CurserY;
            //tb->ScrollY = (int)(F32_Abs(tb->ScrollY) <= F32_Abs(text_height / tb->font.CharSizeY - 5)?0: tb->ScrollY + text_height / tb->font.CharSizeY - 5);
        }
    }

    int Lines = -tb->ScrollY;
    int FirstChar = String_FirstCharOfLine(&tb->In.Buffer,Lines);
    int LastChar = String_LastCharOfLine(&tb->In.Buffer,Lines);
    int MaxLine = -tb->ScrollY + (int)yEnd;

    MaxLine = I64_Max(1,MaxLine);

    for(int i = Lines;i<=MaxLine;i++){
        FirstChar -= tb->ScrollX;
        int Size = LastChar - FirstChar;
        float x = tb->ScrollX * tb->font.CharSizeX + text_x;
        float y = text_y + (tb->ScrollY + i) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
        
        x = F32_Clamp(x,text_x,text_x+text_width);
        y = F32_Clamp(y,text_y,text_y+text_height);
        Size = (int)F32_Clamp(Size,0,(text_width - (x - text_x)) / tb->font.CharSizeX);

        void* texture = Vector_Get(&tb->t,FirstChar);
        if(texture) TCStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&tb->font,texture,(char*)(tb->In.Buffer.Memory + FirstChar),Size,x,y);
        else        CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&tb->font,(char*)(tb->In.Buffer.Memory + FirstChar),Size,x,y,tb->renderable.Fg);

        int TempF = String_FirstCharOfLineLast(&tb->In.Buffer,i+1,FirstChar,LastChar,i);
        int TempL = String_LastCharOfLineLast(&tb->In.Buffer,i+1,FirstChar,LastChar,i);
        FirstChar = TempF;
        LastChar = TempL;

        if(tb->ShowLines){
            CStr linetext = (CStr)Number_Get(i + 1);
            const int size = CStr_Size(linetext);
            const int posx = rect.p.x + ((int)EDITOR_LINENUMBERR - size) * tb->font.CharSizeX;
            const char cursorline = tb->ScrollY + i == CurserY + tb->ScrollY; 
            CStr_RenderSizeAlxFont(Target,Target_Width,Target_Height,&tb->font,(char*)linetext,size,posx,y,cursorline ? 0xAACCCCCC : 0xAA777777);
            CStr_Free(&linetext);
        }
        if(LastChar>tb->In.Buffer.size || FirstChar>=tb->In.Buffer.size){
            break;
        }
    }

    for(int i = 0;i<tb->infos.size;i++){
        Editor_Info* ei = (Editor_Info*)Vector_Get(&tb->infos,i);

        const int start_line = (int)ei->start_line - Lines;
        const int end_line   = (int)ei->end_line - Lines;
        
        if(end_line >= 0 && start_line <= MaxLine){
            const int c_start_line = I32_Clamp(start_line,0,MaxLine);
            const int c_end_line   = I32_Clamp(end_line,0,MaxLine);
            const int len = c_end_line - c_start_line;

            for(int i = 0;i<=len;i++){
                const int b_start_char = String_FirstCharOfLine(&tb->In.Buffer,ei->start_line + i);
                const int b_end_char = String_LastCharOfLine(&tb->In.Buffer,ei->start_line + i);

                int start_char = 0;
                int end_char   = b_end_char - b_start_char;

                if(i == 0)      start_char = ei->start_char;
                if(i == len)    end_char = ei->end_char;

                const int charpos = (tb->ScrollY + start_line + i - 1);
                if(charpos < 0 || charpos > MaxLine) continue;

                const float sx = text_x + F32_Max(0.0f,tb->ScrollX + start_char) * tb->font.CharSizeX;
                const float sy = text_y + charpos * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
                
                const float sx_msg = text_x + tb->font.CharSizeX;
                const float sy_msg = text_y + (charpos + 1) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);

                const float width = F32_Min(xEnd,end_char - start_char);

                Rect_RenderXXAlpha(
                    Target,
                    Target_Width,
                    Target_Height,
                    sx,
                    sy,
                    tb->font.CharSizeX * width,
                    tb->font.CharSizeY,
                    0x77FF0000
                );

                CStr out = CStr_Format("%s: %s",ei->name,ei->msg);
                Vec_CStr split = CStr_ChopWidth(out,I32_Min(20,(int)xEnd));

                for(int i = 0;i<split.size;i++){
                    if(sy_msg + (i + 1) * tb->font.CharSizeY > text_y + text_height) break;
                    
                    CStr_RenderAlxFont(
                        Target,
                        Target_Width,
                        Target_Height,
                        &tb->font,
                        *(CStr*)Vector_Get(&split,i),
                        sx_msg,
                        sy_msg + i * tb->font.CharSizeY,
                        0xFFFF0000
                    );
                }

                Vec_CStr_Free(&split);
                CStr_Free(&out);
            }
        }
    }

    if(tb->In.Enabled){
        Char_RenderAlxFont(Target,Target_Width,Target_Height,&tb->font,'_',text_x+(CurserX + tb->ScrollX) * tb->font.CharSizeX,text_y+(CurserY + tb->ScrollY) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR),BLACK);

        if(tb->In.Curser!=tb->In.CurserEnd && tb->In.CurserEnd>=0){
            const int Up = I32_Min(tb->In.Curser,tb->In.CurserEnd);
            const int Down = I32_Max(tb->In.Curser,tb->In.CurserEnd) + 1;

            int Before = Up;
            for(int i = Up;i<tb->In.Buffer.size;i++){
                if(String_Get(&tb->In.Buffer,i)=='\n' || i == tb->In.Buffer.size - 1){
                    int X = (i+1<Down?i+1:Down) - Before;

                    float x = text_x+(Input_CurserX(&tb->In,Before) + tb->ScrollX) * tb->font.CharSizeX;
                    float y = text_y+(Input_CurserY(&tb->In,Before) + tb->ScrollY) * (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
                    float w = X * tb->font.CharSizeX;
                    float h = (tb->font.CharSizeY * INPUT_GAP_FAKTOR);
                    
                    x = F32_Clamp(x,text_x,text_x+text_width);
                    y = F32_Clamp(y,text_y,text_y+text_height);
                    w = F32_Clamp(w,0.0f,text_width - (x - text_x));
                    h = F32_Clamp(h,0.0f,text_height - (y - text_y));
                    
                    Rect_RenderXXAlpha(Target,Target_Width,Target_Height,x,y,w,h,0x99BBBBBB);
                    Before = i+1;
                }
                if(i>=Down && String_Get(&tb->In.Buffer,i)=='\n'){
                    break;
                }
            }
        }
    }
}
void Editor_Event(void* parent,Editor* b,EditorEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;
        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React){
                b->renderable.React(parent,b,(EventId*)&be);
            }
        }
    }
}
void Editor_Update(void* parent,Editor* tb,States* states,Vec2 Mouse,Vec2 MouseB){
    Renderable_Update(&tb->renderable,parent);
}
void Editor_Input(void* parent,Editor* tb,States* states,Vec2 Mouse,Vec2 MouseB){
    const Rect rect = tb->renderable.rect_out;
    tb->renderable.State = EVENT_NONE;

    if(!Overlap_Rect_Point(rect,Mouse) &&  Overlap_Rect_Point(rect,MouseB))
        Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_ENTERED });
    if( Overlap_Rect_Point(rect,Mouse) && !Overlap_Rect_Point(rect,MouseB))
        Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_EXITED });

    if(Overlap_Rect_Point(rect,Mouse)){
        if(Mouse.x==MouseB.x && Mouse.y==MouseB.y){
            if(states[ALX_MOUSE_L].DOWN)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_DOWN,      .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_INSIDE,    .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }else{
            if(states[ALX_MOUSE_L].DOWN)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_DRAGGED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
            else                            Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_MOVED,     .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }

        if(states[ALX_MOUSE_L].PRESSED)     Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].PRESSED)     Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].PRESSED)     Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_PRESSED,   .ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });

        if(states[ALX_MOUSE_L].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_INSIDE_RELEASED,    .ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });
    }else{
        if(states[ALX_MOUSE_L].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_M].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_M,.pos = Mouse,.posBefore = MouseB });
        if(states[ALX_MOUSE_R].RELEASED)    Editor_Event(parent,tb,(EditorEvent){ .eid = EVENT_OUTSIDE_RELEASED,.ButtonId = ALX_MOUSE_R,.pos = Mouse,.posBefore = MouseB });
    }
    

    Input_Update(&tb->In,states);
    
    const float LineNumber = (tb->ShowLines ? EDITOR_LINENUMBERR : 0.0f) * tb->font.CharSizeX;
    //const float text_width = (rect.d.x - LineNumber);
    //const float text_height = rect.d.y;
    const float text_x = (rect.p.x + LineNumber);
    const float text_y = rect.p.y;

    if(Input_Stroke(&tb->In,ALX_MOUSE_L).PRESSED){
        if(Overlap_Rect_Point(rect,Mouse)){
            tb->In.Enabled = 1;
            Editor_Event(parent,tb,(LabelEvent){ .eid = EVENT_FOCUSED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }else{
            tb->In.Enabled = 0;
            Editor_Event(parent,tb,(LabelEvent){ .eid = EVENT_UNFOCUSED,.ButtonId = ALX_MOUSE_L,.pos = Mouse,.posBefore = MouseB });
        }
    }
    if(Input_Stroke(&tb->In,ALX_MOUSE_L).PRESSED){
        tb->In.Curser = Input_GetCurser(&tb->In,(Mouse.x - text_x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - text_y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    }
    if(Input_Stroke(&tb->In,ALX_MOUSE_L).DOWN){
        if(Mouse.x - LineNumber >= 0.0f && Mouse.y >= 0.0f)
            tb->In.CurserEnd = Input_GetCurser(&tb->In,(Mouse.x - text_x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - text_y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    }
    
    Input_DefaultReact(&tb->In,tb);
}
void Editor_Free(void* parent,Editor* b){
    Editor_Info_Clear(b);
    Vector_Free(&b->infos);

    AlxFont_Free(&b->font);
    Input_Free(&b->In);

    Vector_Free(&b->t);
    HighLight_Free(&b->Syntax);
}

Editor Editor_New(Renderable* Parent,char *text,void (*Editor_React)(void*,Editor*,EditorEvent*),AlxFont font,Vec2 AlxFontSize,Rect rect,unsigned int Lines,unsigned int Bg,unsigned int Fg){
    Editor b;
    b.font = font;
    AlxFont_Resize(&b.font,(int)AlxFontSize.x,(int)AlxFontSize.y);
    
    b.t = Vector_New(sizeof(SymbolInfo));
    b.Syntax = HighLight_New();
    
    b.ScrollX = 0;
    b.ScrollY = 0;
    b.ShowLines = 0;
    
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Editor_Render,
        (void(*)(void*,void*,EventId*))Editor_React,
        (void(*)(void*,void*))Editor_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Editor_Input,
        (void(*)(void*,void*))Editor_Free,
        rect,
        EVENT_STD_EDITOR,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER | ALIGN_TILING,
        Bg,
        Fg,
        BLACK
    );

    b.In = Input_New(10,Lines);
    Input_SetText(&b.In,text);

    b.infos = Vector_New(sizeof(Editor_Info));
    return b;
}
Editor Editor_NewStd(Renderable* Parent,char *text,void (*Editor_React)(void*,Editor*,EditorEvent*),Vec2 AlxFontSize,Rect rect,unsigned int Lines,unsigned int Bg,unsigned int Fg){
    Editor b = Editor_New(
        Parent,
        text,
        Editor_React,
        AlxFont_New(ALXFONT_BLOCKY),
        AlxFontSize,
        rect,
        Lines,
        Bg,
        Fg
    );
    return b;
}


#define TILINGMANAGER_LEN_MIN       50.0f 
#define TILINGMANAGER_LEN_BORDER    20.0f
#define TILINGMANAGER_LEN_OFFSET    5.0f

typedef uint64_t TilingManager_Index;
#define TILINGMANAGER_INDEX_INVALID 0x7FFFFFFFFFFFFFFFULL

typedef enum TilingManager_Dir {
    TILINGMANAGER_DIR_NONE = 0U,
    TILINGMANAGER_DIR_LEFT = 1U,
    TILINGMANAGER_DIR_RIGHT = 2U,
    TILINGMANAGER_DIR_TOP = 3U,
    TILINGMANAGER_DIR_BOTTOM = 4U
} TilingManager_Dir;

typedef struct TilingManagerEvent {
    EventId eid;
    unsigned int border;
    unsigned int* ids;
    unsigned int size;
} TilingManagerEvent;

typedef struct TilingManager_Branch {
    union {
        struct {
            void* data;
            uint32_t size;
        };
        struct {
            struct TilingManager_Branch* b0;
            struct TilingManager_Branch* b1;
            f32 border;
        };
    };
    uint32_t childs : 31;
    uint32_t dirxy : 1;
} TilingManager_Branch;

typedef struct TilingManager {
    Renderable renderable;
    TilingManager_Branch root;
    TilingManager_Index selected;
} TilingManager;

void TilingManager_Branch_Free(TilingManager_Branch* parent,TilingManager_Branch* b);
void TilingManager_Branch_Fold(TilingManager_Branch* parent,TilingManager_Branch* b){
    if(!parent || !b) return;

    TilingManager_Branch fold0 = *(parent->b1 == b ? parent->b0 : parent->b1);
    if(parent->b0) free(parent->b0);
    if(parent->b1) free(parent->b1);
    *parent = fold0;
}
void TilingManager_Branch_Free(TilingManager_Branch* parent,TilingManager_Branch* b){
    if(!b || b->childs == 0U || b->childs > 2U) return;
    
    if(b->childs == 1U){
        if(b->data){
            Renderable_Free(NULL,b->data);
            free(b->data);
            b->data = NULL;
        }

    }else if(b->childs == 2U){
        TilingManager_Branch_Free(b,b->b0);
        if(b->b0) free(b->b0);
        b->b0 = NULL;

        TilingManager_Branch_Free(b,b->b1);
        if(b->b1) free(b->b1);
        b->b1 = NULL;
    }

    b->childs = 0U;
}
Rect TilingManager_GetBorder(TilingManager_Branch* b,Rect rect){
    if(b->dirxy){
        const float dy = rect.d.y * b->border;
        const Vec2 dim = { rect.d.x,10.0f };
        const Vec2 offset = { 0.0f,dy - dim.y * 0.5f };
        return Rect_New(Vec2_Add(rect.p,offset),dim);
    }else{
        const float dx = rect.d.x * b->border;
        const Vec2 dim = { 10.0f,rect.d.y };
        const Vec2 offset = { dx - dim.x * 0.5f,0.0f };
        return Rect_New(Vec2_Add(rect.p,offset),dim);
    }
}
Rect TilingManager_GetSplit(TilingManager_Branch* b,Rect rect,uint32_t dir_lr_tb){
    const Vec2 split_dir = b->dirxy ? (Vec2){ 1.0f,b->border } : (Vec2){ b->border,1.0f };
    const Vec2 axis_dir = b->dirxy ? (Vec2){ 0.0f,1.0f } : (Vec2){ 1.0f,0.0f };
    const Vec2 split = Vec2_Mul(rect.d,split_dir);
    const Vec2 axis_split = Vec2_Mul(split,axis_dir);
    if(dir_lr_tb)   return Rect_New(Vec2_Add(rect.p,axis_split),Vec2_Sub(rect.d,axis_split));
    else            return Rect_New(rect.p,split);
}

void TilingManager_GetDir(Rect surface,Rect insert,uint32_t* dir_xy,uint32_t* dir_lr_tb){
    const Side side = Side_Rect_Rect(surface,insert);
    
    if(side == SIDE_LEFT || side == SIDE_RIGHT){
        *dir_xy = 0U;
        *dir_lr_tb = side == SIDE_RIGHT;
    }else if(side == SIDE_TOP || side == SIDE_BOTTOM){
        *dir_xy = 1U;
        *dir_lr_tb = side == SIDE_BOTTOM;
    }
}
uint32_t TilingManager_GetDirRel(Rect surface,Rect insert,uint32_t dir_xy){
    const Vec2 m0 = Vec2_Add(surface.p,Vec2_Mulf(surface.d,0.5f));
    const Vec2 m1 = Vec2_Add(insert.p,Vec2_Mulf(insert.d,0.5f));
    if(dir_xy)  return m1.y - m0.y >= 0.0f;
    else        return m1.x - m0.x >= 0.0f;
}

TilingManager_Index TilingManager_Branch_GetId(TilingManager_Branch* b,Rect rect,Vec2 p){
    if(!b || b->childs == 0U || b->childs > 2U) return TILINGMANAGER_INDEX_INVALID;
    
    if(b->childs == 1U){
        return 0U;
    }else if(b->childs == 2U){
        const Rect split0 = TilingManager_GetSplit(b,rect,0U);
        const Rect split1 = TilingManager_GetSplit(b,rect,1U);

        if(Rect_Point_Overlap(split0,p)){
            const TilingManager_Index ind = TilingManager_Branch_GetId(b->b0,split0,p);
            return ind == TILINGMANAGER_INDEX_INVALID ? ind : (ind << 1U) | 0U;
        }else if(Rect_Point_Overlap(split1,p)){
            const TilingManager_Index ind = TilingManager_Branch_GetId(b->b1,split1,p);
            return ind == TILINGMANAGER_INDEX_INVALID ? ind : (ind << 1U) | 1U;
        }
    }

    return TILINGMANAGER_INDEX_INVALID;
}
TilingManager_Index TilingManager_GetId(TilingManager* b,Vec2 p){
    if(b->root.childs == 0U)    return TILINGMANAGER_INDEX_INVALID;
    else                        return TilingManager_Branch_GetId(&b->root,b->renderable.rect_out,p);
}

void TilingManager_Branch_Insert(TilingManager_Branch* b,Rect rect,void* child,uint32_t size){
    if(!b || b->childs == 0U || b->childs > 2U || !child) return;
    
    Renderable* child_r = (Renderable*)child;
    if(b->childs == 1U){
        TilingManager_Branch* b0 = (TilingManager_Branch*)malloc(sizeof(TilingManager_Branch));
        TilingManager_Branch* b1 = (TilingManager_Branch*)malloc(sizeof(TilingManager_Branch));

        uint32_t dir_xy = 0U;
        uint32_t dir_lr_tb = 0U;
        TilingManager_GetDir(rect,child_r->rect,&dir_xy,&dir_lr_tb);

        if(dir_lr_tb){
            *b1 = *b;
            b0->childs = 1U;
            b0->data = malloc(size);
            memcpy(b0->data,child,size);
        }else{
            *b0 = *b;
            b1->childs = 1U;
            b1->data = malloc(size);
            memcpy(b1->data,child,size);
        }

        *b = (TilingManager_Branch){
            .border = 0.5f,
            .childs = 2U,
            .dirxy = dir_xy,
            .b0 = b0,
            .b1 = b1
        };
    }else if(b->childs == 2U){
        const uint32_t dir_lr_tb = TilingManager_GetDirRel(rect,child_r->rect,b->dirxy);
        if(dir_lr_tb)   TilingManager_Branch_Insert(b->b1,TilingManager_GetSplit(b,rect,dir_lr_tb),child,size);
        else            TilingManager_Branch_Insert(b->b0,TilingManager_GetSplit(b,rect,dir_lr_tb),child,size);
    }
}
void TilingManager_Insert(TilingManager* b,void* child,uint32_t size){
    if(b->root.childs == 0U){
        b->root.childs = 1U;
        b->root.data = malloc(size);
        memcpy(b->root.data,child,size);
    }else{
        TilingManager_Branch_Insert(&b->root,b->renderable.rect_out,child,size);
    }
}

void TilingManager_Branch_Add(TilingManager_Branch* b,TilingManager_Index rem_id,void* child,uint32_t size){
    if(!b || b->childs == 0U || b->childs > 2U) return;
    
    Renderable* child_r = (Renderable*)child;
    if(b->childs == 1U){
        Renderable* pre_child_r = (Renderable*)b->data;

        TilingManager_Branch* b0 = (TilingManager_Branch*)malloc(sizeof(TilingManager_Branch));
        TilingManager_Branch* b1 = (TilingManager_Branch*)malloc(sizeof(TilingManager_Branch));

        uint32_t dir_xy = 0U;
        uint32_t dir_lr_tb = 0U;
        TilingManager_GetDir(pre_child_r->rect,child_r->rect,&dir_xy,&dir_lr_tb);

        if(dir_lr_tb){
            *b1 = *b;
            b0->childs = 1U;
            b0->data = malloc(size);
            memcpy(b0->data,child,size);
        }else{
            *b0 = *b;
            b1->childs = 1U;
            b1->data = malloc(size);
            memcpy(b1->data,child,size);
        }

        *b = (TilingManager_Branch){
            .border = 0.5f,
            .childs = 2U,
            .dirxy = dir_xy,
            .b0 = b0,
            .b1 = b1
        };
    }else if(b->childs == 2U){
        const TilingManager_Index rem = rem_id & 0b1U;
        const TilingManager_Index next_id = rem_id >> 1U;
        if(rem) TilingManager_Branch_Add(b->b1,next_id,child,size);
        else    TilingManager_Branch_Add(b->b0,next_id,child,size);
    }
}
void TilingManager_Add(TilingManager* b,TilingManager_Index id,void* child,uint32_t size){
    if(id == TILINGMANAGER_INDEX_INVALID) return;
   
    if(b->root.childs == 0U){
        b->root.childs = 1U;
        b->root.data = malloc(size);
        memcpy(b->root.data,child,size);
    }else{
        TilingManager_Branch_Add(&b->root,id,child,size);
    }
}

void TilingManager_Branch_Remove(TilingManager_Branch* parent,TilingManager_Branch* b,TilingManager_Index rem_id){
    if(!b || b->childs == 0U || b->childs > 2U) return;
    
    if(b->childs == 1U){
        TilingManager_Branch_Free(parent,b);
        TilingManager_Branch_Fold(parent,b);
    }else if(b->childs == 2U){
        const TilingManager_Index rem = rem_id & 0b1U;
        const TilingManager_Index next_id = rem_id >> 1U;
        if(rem) TilingManager_Branch_Remove(b,b->b1,next_id);
        else    TilingManager_Branch_Remove(b,b->b0,next_id);
    }
}
void TilingManager_Remove(TilingManager* b,TilingManager_Index id){
    if(id == TILINGMANAGER_INDEX_INVALID) return;
    TilingManager_Branch_Remove(NULL,&b->root,id);
}

void TilingManager_Branch_Render(unsigned int *Target,int Target_Width,int Target_Height,TilingManager_Branch* b,Rect rect){
    if(!b || b->childs > 2U) return;
    
    if(b->childs == 0U){
        
    }else if(b->childs == 1U){
        Renderable* r = (Renderable*)b->data;
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Render) r->Render(Target,Target_Width,Target_Height,b,r);
    }else if(b->childs == 2U){
        TilingManager_Branch_Render(Target,Target_Width,Target_Height,b->b0,TilingManager_GetSplit(b,rect,0U));
        TilingManager_Branch_Render(Target,Target_Width,Target_Height,b->b1,TilingManager_GetSplit(b,rect,1U));

        const Rect border = TilingManager_GetBorder(b,rect);
        Rect_Render(Target,Target_Width,Target_Height,border,RED);
    }
}
void TilingManager_Branch_Event(TilingManager_Branch* b,Rect rect,TilingManagerEvent be){
    if(!b || b->childs != 1U) return;
    
    Renderable* r = (Renderable*)b->data;
    if(be.eid!=EVENT_NONE){
        r->State = be.eid;

        if(r->UsedStates & EVENT_USE(be.eid)){
            if(r->React) r->React(b,r,(EventId*)&be);
        }
    }
}
void TilingManager_Branch_Update(TilingManager_Branch* b,Rect rect){
    if(!b || b->childs > 2U) return;
    
    if(b->childs == 0U){
        
    }else if(b->childs == 1U){
        Renderable* r = (Renderable*)b->data;
        r->rect = rect;
        r->rect_out = rect;
    }else if(b->childs == 2U){
        TilingManager_Branch_Update(b->b0,TilingManager_GetSplit(b,rect,0U));
        TilingManager_Branch_Update(b->b1,TilingManager_GetSplit(b,rect,1U));
    }
}
void TilingManager_Branch_Input(TilingManager_Branch* b,Rect rect,TilingManager_Index* selected,TilingManager_Index path,States* states,Vec2 Mouse,Vec2 MouseB){
    if(!b || b->childs > 2U) return;
    
    if(b->childs == 0U){

    }else if(b->childs == 1U){
        Renderable* r = (Renderable*)b->data;
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Input && *selected == TILINGMANAGER_INDEX_INVALID)
                r->Input(b,r,states,Mouse,MouseB);

        //if(r->Remove)
        //    TilingManager_Remove(b);
    }else if(b->childs == 2U){
        TilingManager_Branch_Input(b->b0,TilingManager_GetSplit(b,rect,0U),selected,(path << 1ULL) | 0ULL,states,Mouse,MouseB);
        TilingManager_Branch_Input(b->b1,TilingManager_GetSplit(b,rect,1U),selected,(path << 1ULL) | 1ULL,states,Mouse,MouseB);

        if(Rect_Point_Overlap(rect,Mouse)){
            const Rect border = TilingManager_GetBorder(b,rect);
            
            if(states[ALX_MOUSE_L].PRESSED && Rect_Point_Overlap(border,Mouse)){
                *selected = path;
            }else if(states[ALX_MOUSE_L].DOWN && path == *selected){
                const Vec2 delta_m = Vec2_Sub(Mouse,rect.p);
                const Vec2 div_m = Vec2_Div(delta_m,rect.d);
                b->border = F32_Clamp(b->dirxy ? div_m.y : div_m.x,0.1f,0.9f);
            }
        }
    }
}

void TilingManager_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,TilingManager* b){
    Rect_Render(Target,Target_Width,Target_Height,b->renderable.rect_out,b->renderable.Bg);
    TilingManager_Branch_Render(Target,Target_Width,Target_Height,&b->root,b->renderable.rect_out);
}
void TilingManager_Event(void* parent,TilingManager* b,TilingManagerEvent be){
    TilingManager_Branch_Event(&b->root,b->renderable.rect_out,be);
}
void TilingManager_Update(void* parent,TilingManager* b){
    Renderable_Update(&b->renderable,parent);
    TilingManager_Branch_Update(&b->root,b->renderable.rect_out);
}
void TilingManager_Input(void* parent,TilingManager* b,States* states,Vec2 Mouse,Vec2 MouseB){
    if(states[ALX_MOUSE_L].RELEASED)
        b->selected = TILINGMANAGER_INDEX_INVALID;

    TilingManager_Branch_Input(&b->root,b->renderable.rect_out,&b->selected,TILINGMANAGER_INDEX_INVALID << 1ULL,states,Mouse,MouseB);
}
void TilingManager_Free(void* parent,TilingManager* b){
    TilingManager_Branch_Free(NULL,&b->root);
}

TilingManager TilingManager_New(Renderable* Parent,void (*TilingManager_React)(void*,TilingManager*,TilingManagerEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    TilingManager b;
    memset(&b,0,sizeof(b));
    b.root.childs = 0U;
    b.selected = TILINGMANAGER_INDEX_INVALID;
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))TilingManager_Render,
        (void(*)(void*,void*,EventId*))TilingManager_React,
        (void(*)(void*,void*))TilingManager_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))TilingManager_Input,
        (void(*)(void*,void*))TilingManager_Free,
        rect,
        EVENT_STD_TILINGMANAGER,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        Fg,
        BLACK
    );
    return b;
}
TilingManager TilingManager_Make(Renderable* Parent,void (*TilingManager_React)(void*,TilingManager*,TilingManagerEvent*),Rect rect,unsigned int Bg,unsigned int Fg,void** childs,uint32_t* sizes){
    TilingManager b = TilingManager_New(Parent,TilingManager_React,rect,Bg,Fg);

    if(Parent)  b.renderable.rect_out = Renderable_GetRect_R(rect,Parent->rect_out);
    else        b.renderable.rect_out = rect;

    for(int i = 0;childs[i] && sizes[i] > 0U;i++)
        TilingManager_Insert(&b,childs[i],sizes[i]);

    return b;
}


/*
typedef struct TilingManagerEvent {
    EventId eid;
    unsigned int border;
    unsigned int* ids;
    unsigned int size;
} TilingManagerEvent;

typedef struct TilingManager {
    Renderable renderable;
    List childs;
    Vector borders;
    unsigned int selected;
    unsigned int selected_neg_id;
    unsigned int selected_pos_id;
} TilingManager;

typedef struct TilingManager_Border {
    Rect rect;
    unsigned int neg_id;
    unsigned int pos_id;
} TilingManager_Border;

void* TilingManager_Add(TilingManager* b,void* child,unsigned int size);
int TilingManager_Cmp_Id(const void* e1,const void* e2);

void TilingManager_SetRectOut(TilingManager* b,Renderable* r,Rect rect){
    const Rect parent = b->renderable.rect_out;

    r->rect_out = rect;

    if(parent.d.x != 0.0f && parent.d.y != 0.0f){
        r->rect.p.x = (rect.p.x - parent.p.x) / parent.d.x;
        r->rect.p.y = (rect.p.y - parent.p.y) / parent.d.y;
        r->rect.d.x = rect.d.x / parent.d.x;
        r->rect.d.y = rect.d.y / parent.d.y;
    }else{
        r->rect.p = (Vec2){ 0.0f,0.0f };
        r->rect.d = (Vec2){ 0.0f,0.0f };
    }
}
void TilingManager_SetRectRelative(TilingManager* b,Renderable* r,Rect rect){
    const Rect parent = b->renderable.rect_out;

    r->rect = rect;
    r->rect_out = Renderable_GetRect_R(rect,parent);
}
void TilingManager_ApplyChildRect(TilingManager* b,Renderable* r){
    r->rect_out = Renderable_GetRect_R(r->rect,b->renderable.rect_out);
}
unsigned int TilingManager_FindBorder(TilingManager* b,Vec2 Mouse){
    for(unsigned int i = 0U;i<b->borders.size;i++){
        TilingManager_Border* const tar_b = (TilingManager_Border*)Vector_Get(&b->borders,i);
        const char tar_axis = tar_b->rect.d.x <= TILINGMANAGER_LEN_BORDER;
        const float left = tar_b->rect.p.x - (tar_axis ? tar_b->rect.d.x * 0.5f : 0.0f);
        const float top = tar_b->rect.p.y - (tar_axis ? 0.0f : tar_b->rect.d.y * 0.5f);
        const float right = left + tar_b->rect.d.x;
        const float bottom = top + tar_b->rect.d.y;

        if(Mouse.x >= left - TILINGMANAGER_LEN_OFFSET &&
           Mouse.x <= right + TILINGMANAGER_LEN_OFFSET &&
           Mouse.y >= top - TILINGMANAGER_LEN_OFFSET &&
           Mouse.y <= bottom + TILINGMANAGER_LEN_OFFSET)
            return i;
    }

    return TILINGMANAGER_INDEX_INVALID;
}
void TilingManager_AddBorder(TilingManager* b,unsigned int j,unsigned int i,Renderable* const rj,Renderable* const ri){
    const Rect a = rj->rect_out;
    const Rect b_rect = ri->rect_out;
    const float eps = 1.0f;

    const float a_left = a.p.x;
    const float a_right = a.p.x + a.d.x;
    const float a_top = a.p.y;
    const float a_bottom = a.p.y + a.d.y;

    const float b_left = b_rect.p.x;
    const float b_right = b_rect.p.x + b_rect.d.x;
    const float b_top = b_rect.p.y;
    const float b_bottom = b_rect.p.y + b_rect.d.y;

    const float overlap_x = F32_Min(a_right,b_right) - F32_Max(a_left,b_left);
    const float overlap_y = F32_Min(a_bottom,b_bottom) - F32_Max(a_top,b_top);

    const float gap_ab_x = b_left - a_right;
    const float gap_ba_x = a_left - b_right;
    const float gap_ab_y = b_top - a_bottom;
    const float gap_ba_y = a_top - b_bottom;

    const char vertical_sep =
        (((gap_ab_x >= -eps && gap_ab_x <= TILINGMANAGER_LEN_BORDER + eps) ||
          (gap_ba_x >= -eps && gap_ba_x <= TILINGMANAGER_LEN_BORDER + eps)) &&
         overlap_y > 0.0f);

    const char horizontal_sep =
        (((gap_ab_y >= -eps && gap_ab_y <= TILINGMANAGER_LEN_BORDER + eps) ||
          (gap_ba_y >= -eps && gap_ba_y <= TILINGMANAGER_LEN_BORDER + eps)) &&
         overlap_x > 0.0f);

    if(!vertical_sep && !horizontal_sep)
        return;

    TilingManager_Border tmb = { .rect = (Rect){ {0.0f,0.0f},{0.0f,0.0f} }, .neg_id = i, .pos_id = j };

    if(vertical_sep){
        const char a_before_b = gap_ab_x >= -eps && gap_ab_x <= TILINGMANAGER_LEN_BORDER + eps;
        const float x = a_before_b ? (a_right + b_left) * 0.5f : (b_right + a_left) * 0.5f;
        const float overlap_top = F32_Max(a_top,b_top);
        const float overlap_bottom = F32_Min(a_bottom,b_bottom);
        const float width = TILINGMANAGER_LEN_BORDER;
        const float height = overlap_bottom - overlap_top - TILINGMANAGER_LEN_OFFSET * 2.0f;

        if(height <= 0.0f)
            return;

        tmb.rect = Rect_New(
            (Vec2){ .x = x, .y = overlap_top + TILINGMANAGER_LEN_OFFSET },
            (Vec2){ .x = width, .y = height }
        );

        tmb.neg_id = a_before_b ? j : i;
        tmb.pos_id = a_before_b ? i : j;
    }else if(horizontal_sep){
        const char a_before_b = gap_ab_y >= -eps && gap_ab_y <= TILINGMANAGER_LEN_BORDER + eps;
        const float y = a_before_b ? (a_bottom + b_top) * 0.5f : (b_bottom + a_top) * 0.5f;
        const float overlap_left = F32_Max(a_left,b_left);
        const float overlap_right = F32_Min(a_right,b_right);
        const float width = overlap_right - overlap_left - TILINGMANAGER_LEN_OFFSET * 2.0f;
        const float height = TILINGMANAGER_LEN_BORDER;

        if(width <= 0.0f)
            return;

        tmb.rect = Rect_New(
            (Vec2){ .x = overlap_left + TILINGMANAGER_LEN_OFFSET, .y = y },
            (Vec2){ .x = width, .y = height }
        );

        tmb.neg_id = a_before_b ? j : i;
        tmb.pos_id = a_before_b ? i : j;
    }

    Vector_Push(&b->borders,&tmb);
}
void TilingManager_UpdateBorders(TilingManager* b){
    Vector_Clear(&b->borders);

    if(b->childs.size < 2)
        return;

    Node* ni = b->childs.First;
    for(unsigned int i = 0U;i<b->childs.size;i++){
        Renderable* const ri = (Renderable*)ni->Memory;

        Node* nj = ni->Next;
        for(unsigned int j = i + 1U;j<b->childs.size && nj;j++){
            Renderable* const rj = (Renderable*)nj->Memory;
            TilingManager_AddBorder(b,j,i,rj,ri);
            nj = nj->Next;
        }

        ni = ni->Next;
    }
}
unsigned int TilingManager_FindSelectedBorder(TilingManager* b){
    for(unsigned int i = 0U;i<b->borders.size;i++){
        TilingManager_Border* const border =
            (TilingManager_Border*)Vector_Get(&b->borders,i);

        if(border->neg_id == b->selected_neg_id &&
           border->pos_id == b->selected_pos_id)
            return i;
    }

    return TILINGMANAGER_INDEX_INVALID;
}
Vector TilingManager_GetAligned(TilingManager* b,unsigned int border_id){
    if(border_id >= b->borders.size)
        return Vector_New(sizeof(unsigned int));

    Vector aligned_b = Vector_New(sizeof(unsigned int));
    Vector pending = Vector_New(sizeof(unsigned int));
    Vector visited = Vector_New(sizeof(unsigned int));

    Vector_Push(&pending,&border_id);
    Vector_Push(&visited,&border_id);

    for(unsigned int cursor = 0U;cursor < pending.size;cursor++){
        unsigned int current_id =
            *(unsigned int*)Vector_Get(&pending,cursor);
        TilingManager_Border* const current =
            (TilingManager_Border*)Vector_Get(&b->borders,current_id);
        const char current_axis = current->rect.d.x <= TILINGMANAGER_LEN_BORDER;

        Vector_Push(&aligned_b,&current_id);

        for(unsigned int candidate_id = 0U;
            candidate_id < b->borders.size;
            candidate_id++){
            if(Vector_Contains(&visited,&candidate_id,TilingManager_Cmp_Id))
                continue;

            TilingManager_Border* const candidate =
                (TilingManager_Border*)Vector_Get(&b->borders,candidate_id);
            const char candidate_axis =
                candidate->rect.d.x <= TILINGMANAGER_LEN_BORDER;

            if(candidate_axis != current_axis)
                continue;

            const char shares_negative_side =
                candidate->neg_id == current->neg_id;
            const char shares_positive_side =
                candidate->pos_id == current->pos_id;

            if(shares_negative_side || shares_positive_side){
                Vector_Push(&visited,&candidate_id);
                Vector_Push(&pending,&candidate_id);
            }
        }
    }

    Vector_Free(&pending);
    Vector_Free(&visited);

    return aligned_b;
}
int TilingManager_Cmp_Id(const void* e1,const void* e2){
    const unsigned int id1 = *(unsigned int*)e1;
    const unsigned int id2 = *(unsigned int*)e2;
    return id1 == id2;
}
Vector TilingManager_GetInfluenced_A(TilingManager* b,Vector* aligned_b,unsigned int border_id){
    Vector comps = Vector_New(sizeof(unsigned int));

    if(border_id >= b->borders.size)
        return comps;

    for(unsigned int i = 0U;i<aligned_b->size;i++){
        const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

        if(!Vector_Contains(&comps,&tmb->neg_id,TilingManager_Cmp_Id))
            Vector_Push(&comps,&tmb->neg_id);
        if(!Vector_Contains(&comps,&tmb->pos_id,TilingManager_Cmp_Id))
            Vector_Push(&comps,&tmb->pos_id);
    }

    return comps;
}
Vector TilingManager_GetInfluenced(TilingManager* b,unsigned int border_id){
    Vector aligned_b = TilingManager_GetAligned(b,border_id);
    Vector comps = TilingManager_GetInfluenced_A(b,&aligned_b,border_id);
    Vector_Free(&aligned_b);
    return comps;
}
void TilingManager_CompCorrection(TilingManager* b,TilingManager_Border* tmb,Renderable* const neg_rend,Renderable* const pos_rend){
    if(tmb->rect.d.x <= TILINGMANAGER_LEN_BORDER){
        neg_rend->rect_out.d.x = F32_Max(
            TILINGMANAGER_LEN_BORDER,
            tmb->rect.p.x - neg_rend->rect_out.p.x - tmb->rect.d.x * 0.5f
        );

        const float p_pre = pos_rend->rect_out.p.x;
        pos_rend->rect_out.p.x = tmb->rect.p.x + tmb->rect.d.x * 0.5f;
        pos_rend->rect_out.d.x = F32_Max(
            TILINGMANAGER_LEN_BORDER,
            pos_rend->rect_out.d.x - (pos_rend->rect_out.p.x - p_pre)
        );
    }else{
        neg_rend->rect_out.d.y = F32_Max(
            TILINGMANAGER_LEN_BORDER,
            tmb->rect.p.y - neg_rend->rect_out.p.y - tmb->rect.d.y * 0.5f
        );

        const float p_pre = pos_rend->rect_out.p.y;
        pos_rend->rect_out.p.y = tmb->rect.p.y + tmb->rect.d.y * 0.5f;
        pos_rend->rect_out.d.y = F32_Max(
            TILINGMANAGER_LEN_BORDER,
            pos_rend->rect_out.d.y - (pos_rend->rect_out.p.y - p_pre)
        );
    }
    TilingManager_SetRectOut(b,neg_rend,neg_rend->rect_out);
    TilingManager_SetRectOut(b,pos_rend,pos_rend->rect_out);
}
void TilingManager_BorderCorrection(TilingManager* b,Vector* aligned_b){
    char axis = 0;
    float border_pos = 0.0f;

    for(unsigned int i = 0U;i<aligned_b->size;i++){
        const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

        Renderable* const neg_rend = (Renderable*)List_Get(&b->childs,tmb->neg_id);
        Renderable* const pos_rend = (Renderable*)List_Get(&b->childs,tmb->pos_id);

        if(tmb->rect.d.x <= TILINGMANAGER_LEN_BORDER){
            if(tmb->rect.p.x - neg_rend->rect_out.p.x <
                (TILINGMANAGER_LEN_BORDER * 0.5f) + TILINGMANAGER_LEN_MIN){
                border_pos =
                    neg_rend->rect_out.p.x +
                    TILINGMANAGER_LEN_MIN +
                    (TILINGMANAGER_LEN_BORDER * 0.5f);
                axis = 0;
            }

            if(tmb->rect.p.x + (TILINGMANAGER_LEN_BORDER * 0.5f) + TILINGMANAGER_LEN_MIN >
                pos_rend->rect_out.p.x + pos_rend->rect_out.d.x){
                border_pos =
                    (pos_rend->rect_out.p.x + pos_rend->rect_out.d.x) -
                    (TILINGMANAGER_LEN_BORDER * 0.5f + TILINGMANAGER_LEN_MIN);
                axis = 0;
            }
        }else{
            if(tmb->rect.p.y - neg_rend->rect_out.p.y <
                (TILINGMANAGER_LEN_BORDER * 0.5f) + TILINGMANAGER_LEN_MIN){
                border_pos =
                    neg_rend->rect_out.p.y +
                    TILINGMANAGER_LEN_MIN +
                    (TILINGMANAGER_LEN_BORDER * 0.5f);
                axis = 1;
            }

            if(tmb->rect.p.y + (TILINGMANAGER_LEN_BORDER * 0.5f) + TILINGMANAGER_LEN_MIN >
                pos_rend->rect_out.p.y + pos_rend->rect_out.d.y){
                border_pos =
                    (pos_rend->rect_out.p.y + pos_rend->rect_out.d.y) -
                    (TILINGMANAGER_LEN_BORDER * 0.5f + TILINGMANAGER_LEN_MIN);
                axis = 1;
            }
        }
    }

    if(border_pos != 0.0f){
        for(unsigned int i = 0U;i<aligned_b->size;i++){
            const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
            TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

            tmb->rect.p.x = axis == 0 ? border_pos : tmb->rect.p.x;
            tmb->rect.p.y = axis == 1 ? border_pos : tmb->rect.p.y;
        }
    }
}
void TilingManager_MoveBorder_A(TilingManager* b,Vector* aligned_b,unsigned int border_id,Vec2 target){
    if(border_id >= b->borders.size)
        return;

    for(unsigned int i = 0U;i<aligned_b->size;i++){
        const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

        if(tmb->rect.d.x <= TILINGMANAGER_LEN_BORDER)
            tmb->rect.p.x = target.x;
        else
            tmb->rect.p.y = target.y;
    }

    TilingManager_BorderCorrection(b,aligned_b);

    for(unsigned int i = 0U;i<aligned_b->size;i++){
        const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

        Renderable* const neg_rend = (Renderable*)List_Get(&b->childs,tmb->neg_id);
        Renderable* const pos_rend = (Renderable*)List_Get(&b->childs,tmb->pos_id);

        TilingManager_CompCorrection(b,tmb,neg_rend,pos_rend);
    }
}
void TilingManager_MoveBorder(TilingManager* b,unsigned int border_id,Vec2 target){
    Vector aligned_b = TilingManager_GetAligned(b,border_id);
    TilingManager_MoveBorder_A(b,&aligned_b,border_id,target);
    Vector_Free(&aligned_b);
}
void TilingManager_MoveInfluenced_A(TilingManager* b,Vector* aligned_b){
    for(unsigned int i = 0U;i<aligned_b->size;i++){
        const unsigned int id_b = *(unsigned int*)Vector_Get(aligned_b,i);
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,id_b);

        Renderable* const neg_rend = (Renderable*)List_Get(&b->childs,tmb->neg_id);
        Renderable* const pos_rend = (Renderable*)List_Get(&b->childs,tmb->pos_id);

        TilingManager_CompCorrection(b,tmb,neg_rend,pos_rend);
    }
}
int TilingManager_FindSplitComponent(TilingManager* b){
    if(b->childs.size == 0)
        return -1;

    if(b->selected != TILINGMANAGER_INDEX_INVALID && b->selected < b->borders.size){
        TilingManager_Border* border =
            (TilingManager_Border*)Vector_Get(&b->borders,b->selected);

        if(border->neg_id < b->childs.size)
            return (int)border->neg_id;
    }

    int best = 0;
    float best_area = -1.0f;

    Node* n = b->childs.First;
    for(unsigned int i = 0U;i<b->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        const float area = r->rect_out.d.x * r->rect_out.d.y;

        if(area > best_area){
            best_area = area;
            best = (int)i;
        }

        n = n->Next;
    }

    return best;
}
static void TilingManager_SplitArea(Rect area,char vertical,Rect* first,Rect* second){
    *first = area;
    *second = area;

    if(vertical){
        const float half = area.d.x * 0.5f;
        first->d.x = half;
        second->p.x += half;
        second->d.x = half;
    }else{
        const float half = area.d.y * 0.5f;
        first->d.y = half;
        second->p.y += half;
        second->d.y = half;
    }
}
static void TilingManager_AssignChildRect(TilingManager* b,Renderable* child,Rect rect){
    TilingManager_SetRectOut(b,child,rect);
}
void* TilingManager_Insert(TilingManager* b,void* child,unsigned int size){
    if(!b || !child || size == 0U)
        return NULL;

    const Rect area = b->renderable.rect_out;
    if(area.d.x <= 0.0f || area.d.y <= 0.0f)
        return NULL;

    int split_id = TilingManager_FindSplitComponent(b);
    if(b->childs.size > 0 && split_id < 0)
        return NULL;

    Renderable* split = NULL;
    Rect split_rect = area;

    if(split_id >= 0)
        split = (Renderable*)List_Get(&b->childs,(unsigned int)split_id);

    if(split){
        split_rect = split->rect_out;
        const char vertical = split_rect.d.x >= split_rect.d.y;

        if(vertical && split_rect.d.x < TILINGMANAGER_LEN_MIN * 2.0f)
            return NULL;
        if(!vertical && split_rect.d.y < TILINGMANAGER_LEN_MIN * 2.0f)
            return NULL;
    }

    List_Push(&b->childs,size,child);

    Renderable* added = (Renderable*)List_Get(&b->childs,b->childs.size - 1U);
    if(!added)
        return NULL;

    added->Parent = &b->renderable;
    added->Remove = 0;

    if(!split){
        TilingManager_AssignChildRect(b,added,area);
    }else{
        const char vertical = split_rect.d.x >= split_rect.d.y;
        Rect first = split_rect;
        Rect second = split_rect;

        TilingManager_SplitArea(split_rect,vertical,&first,&second);
        TilingManager_SetRectOut(b,split,first);
        TilingManager_AssignChildRect(b,added,second);
    }

    TilingManager_UpdateBorders(b);
    return added;
}
void* TilingManager_Add(TilingManager* b,void* child,unsigned int size){
    return TilingManager_Insert(b,child,size);
}
void TilingManager_Remove(TilingManager* b,int i){
    Node* n = List_FindNode(&b->childs,i);

    if(n){
        Renderable* rend = (Renderable*)n->Memory;
        Renderable_Free(b,rend);

        Node_Delete(n);
        Node_Free(n);
        b->childs.size--;

        b->selected = TILINGMANAGER_INDEX_INVALID;
        b->selected_neg_id = TILINGMANAGER_INDEX_INVALID;
        b->selected_pos_id = TILINGMANAGER_INDEX_INVALID;

        if(b->childs.size == 0){
            Vector_Clear(&b->borders);
            return;
        }

        Node* first_node = b->childs.First;
        Renderable* first = (Renderable*)first_node->Memory;
        TilingManager_SetRectOut(b,first,b->renderable.rect_out);

        for(unsigned int j = 1U;j<b->childs.size;j++){
            Renderable* r = (Renderable*)List_Get(&b->childs,j);
            if(!r) continue;

            int split_id = TilingManager_FindSplitComponent(b);
            if(split_id < 0) break;

            Renderable* split = (Renderable*)List_Get(&b->childs,(unsigned int)split_id);
            Rect sr = split->rect_out;

            if(sr.d.x >= sr.d.y && sr.d.x >= TILINGMANAGER_LEN_MIN * 2.0f){
                const float half = sr.d.x * 0.5f;
                Rect a = sr;
                Rect c = sr;
                a.d.x = half;
                c.p.x += half;
                c.d.x = half;
                TilingManager_SetRectOut(b,split,a);
                TilingManager_SetRectOut(b,r,c);
            }else if(sr.d.y >= TILINGMANAGER_LEN_MIN * 2.0f){
                const float half = sr.d.y * 0.5f;
                Rect a = sr;
                Rect c = sr;
                a.d.y = half;
                c.p.y += half;
                c.d.y = half;
                TilingManager_SetRectOut(b,split,a);
                TilingManager_SetRectOut(b,r,c);
            }
        }

        TilingManager_UpdateBorders(b);
    }
}
void TilingManager_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,TilingManager* b){
    const Rect rect = b->renderable.rect_out;
    Rect_Render(Target,Target_Width,Target_Height,rect,b->renderable.Bg);

    Node* n = b->childs.First;
    for(int i = 0;i<b->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;

        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Render) r->Render(Target,Target_Width,Target_Height,b,n->Memory);

        n = n->Next;
    }

    for(int i = 0;i<b->borders.size;i++){
        TilingManager_Border* const tmb = (TilingManager_Border*)Vector_Get(&b->borders,i);
        const char axis = tmb->rect.d.x <= TILINGMANAGER_LEN_BORDER;

        Rect_RenderXX(
            Target,
            Target_Width,
            Target_Height,
            tmb->rect.p.x - (axis ? tmb->rect.d.x * 0.5f : 0.0f),
            tmb->rect.p.y - (!axis ? tmb->rect.d.y * 0.5f : 0.0f),
            tmb->rect.d.x,
            tmb->rect.d.y,
            b->renderable.Fg
        );
    }
}
void TilingManager_Event(void* parent,TilingManager* b,TilingManagerEvent be){
    if(be.eid!=EVENT_NONE){
        b->renderable.State = be.eid;

        if(b->renderable.UsedStates & EVENT_USE(be.eid)){
            if(b->renderable.React)
                b->renderable.React(parent,b,(EventId*)&be);
        }
    }
}
void TilingManager_Update(void* parent,TilingManager* b){
    Renderable_Update(&b->renderable,parent);

    Node* n = b->childs.First;
    for(int i = 0;i<b->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;

        if(Alignment_active(r->Align,ALIGN_VISIBLE)){
            if(r->Update)
                r->Update(b,n->Memory);
        }

        TilingManager_ApplyChildRect(b,r);

        n = n->Next;
    }

    TilingManager_UpdateBorders(b);
}
void TilingManager_Input(void* parent,TilingManager* b,States* states,Vec2 Mouse,Vec2 MouseB){
    if(states[ALX_MOUSE_L].DOWN && b->selected != TILINGMANAGER_INDEX_INVALID){
        b->selected = TilingManager_FindSelectedBorder(b);
        
        if(b->selected == TILINGMANAGER_INDEX_INVALID){
            b->selected_neg_id = TILINGMANAGER_INDEX_INVALID;
            b->selected_pos_id = TILINGMANAGER_INDEX_INVALID;
            goto TilingManager_Input_Components;
        }

        Vector aligned_b = TilingManager_GetAligned(b,b->selected);
        Vector comps = TilingManager_GetInfluenced_A(b,&aligned_b,b->selected);

        TilingManager_MoveBorder_A(b,&aligned_b,b->selected,Mouse);

        TilingManager_Event(
            parent,
            b,
            (TilingManagerEvent){
                .eid = EVENT_BORDER,
                .border = b->selected,
                .ids = (unsigned int*)comps.Memory,
                .size = comps.size
            }
        );

        Vector_Free(&comps);
        Vector_Free(&aligned_b);

        TilingManager_UpdateBorders(b);
    }else if(states[ALX_MOUSE_L].PRESSED && Overlap_Rect_Point(b->renderable.rect_out,Mouse)){
        b->selected = TilingManager_FindBorder(b,Mouse);
        
        if(b->selected != TILINGMANAGER_INDEX_INVALID){
            TilingManager_Border* const border = (TilingManager_Border*)Vector_Get(&b->borders,b->selected);
            b->selected_neg_id = border->neg_id;
            b->selected_pos_id = border->pos_id;
        }
    }else if(states[ALX_MOUSE_L].RELEASED){
        b->selected = TILINGMANAGER_INDEX_INVALID;
        b->selected_neg_id = TILINGMANAGER_INDEX_INVALID;
        b->selected_pos_id = TILINGMANAGER_INDEX_INVALID;
    }

TilingManager_Input_Components:
    Node* n = b->childs.First;
    for(int i = 0;i<b->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;

        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Input) r->Input(b,n->Memory,states,Mouse,MouseB);

        Node* const rem = n;
        n = n->Next;

        if(r->Remove && rem){
            TilingManager_Remove(b,i);
            i--;
        }
    }
}
void TilingManager_Free(void* parent,TilingManager* b){
    Node* n = b->childs.First;

    for(int i = 0;i<b->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        Renderable_Free(b,r);
        n = n->Next;
    }

    Vector_Free(&b->borders);
    List_Free(&b->childs);
}
TilingManager TilingManager_New(Renderable* Parent,void (*TilingManager_React)(void*,TilingManager*,TilingManagerEvent*),Rect rect,unsigned int Bg,unsigned int Fg){
    TilingManager b;
    b.borders = Vector_New(sizeof(TilingManager_Border));
    b.childs = List_New();
    b.selected = TILINGMANAGER_INDEX_INVALID;
    b.selected_neg_id = TILINGMANAGER_INDEX_INVALID;
    b.selected_pos_id = TILINGMANAGER_INDEX_INVALID;

    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))TilingManager_Render,
        (void(*)(void*,void*,EventId*))TilingManager_React,
        (void(*)(void*,void*))TilingManager_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))TilingManager_Input,
        (void(*)(void*,void*))TilingManager_Free,
        rect,
        EVENT_STD_TILINGMANAGER,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        Fg,
        BLACK
    );

    return b;
}
TilingManager TilingManager_Make(Renderable* Parent,void (*TilingManager_React)(void*,TilingManager*,TilingManagerEvent*),Rect rect,unsigned int Bg,unsigned int Fg,void** childs,unsigned int* sizes){
    TilingManager b = TilingManager_New(Parent,TilingManager_React,rect,Bg,Fg);

    if(Parent)
        b.renderable.rect_out = Renderable_GetRect_R(rect,Parent->rect_out);
    else
        b.renderable.rect_out = rect;

    for(int i = 0;childs[i] && sizes[i] > 0U;i++)
        TilingManager_Add(&b,childs[i],sizes[i]);

    return b;
}
*/

/*
typedef PVector Scene;

Scene Scene_New(){
    Scene b = PVector_New();
    return b;
}
void Scene_Add(Scene* scene,void* r,unsigned int size){// Renderable
    PVector_Push(scene,r,size);
}
void* Scene_Add_R(Scene* scene,void* r,unsigned int size){// Renderable
    PVector_Push(scene,r,size);
    return PVector_Get(scene,scene->size-1);
}
void Scene_Update(Scene* scene,States* states,Vec2 Mouse,Vec2 MouseB){
    for(int i = 0;i<scene->size;i++){
        Renderable* r = (Renderable*)PVector_Get(scene,i);
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Update) r->Update(r,states,Mouse,MouseB);
    }
}
void Scene_Render(unsigned int *Target,int Target_Width,int Target_Height,Scene* scene){
    for(int i = 0;i<scene->size;i++){
        Renderable* r = (Renderable*)PVector_Get(scene,i);

        Renderable_Print(r);

        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Render) r->Render(Target,Target_Width,Target_Height,scene);
    }
}
char Scene_Contains(Scene* scene,void* t){
    for(int i = 0;i<scene->size;i++){
        Renderable* r = (Renderable*)PVector_Get(scene,i);
        if(r==t && t!=NULL) return 1;
    }
    return 0;
}
void Scene_Free(Scene* scene){
    for(int i = 0;i<scene->size;i++){
        Renderable* r = (Renderable*)PVector_Get(scene,i);
        Renderable_Free(r);
    }
    PVector_Free(scene);
}
*/

typedef struct Scene {
    Renderable renderable;
    List childs;
} Scene;

void Scene_Add(Scene* s,void* r,unsigned int size){// Renderable
    List_Push(&s->childs,size,r);
}
void* Scene_Add_R(Scene* s,void* r,unsigned int size){// Renderable
    List_Push(&s->childs,size,r);
    return List_Get(&s->childs,s->childs.size - 1);
}
void Scene_RemoveI(Scene* s,int i){
    Node* n = List_FindNode(&s->childs,i);
    
    if(n){
        Renderable* rend = (Renderable*)n->Memory;
        Renderable_Free(s,rend);
        
        Node_Delete(n);
        Node_Free(n);
        s->childs.size--;
        return;
    }
}
void Scene_Remove(Scene* s,void* r){
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        if(n->Memory == r){
            Renderable* rend = (Renderable*)r;
            if(rend->Free) rend->Free(s,rend);

            Node_Delete(n);
            Node_Free(n);
            s->childs.size--;
            return;
        }
        n = n->Next;
    }
}
void Scene_RemoveQueue(Scene* s,void* r){
    Renderable* rend = (Renderable*)r;
    if(rend) rend->Remove = 1;
}

void Scene_Comp_Render(unsigned int *Target,int Target_Width,int Target_Height,void* parent,Scene* s){
    s->renderable.rect_out = Renderable_GetRect_R(s->renderable.rect,Rect_New((Vec2){ 0.0f,0.0f },(Vec2){ Target_Width,Target_Height }));
    const Rect rect = s->renderable.rect_out;

    Rect_Render(Target,Target_Width,Target_Height,rect,s->renderable.Bg);
    
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Render) r->Render(Target,Target_Width,Target_Height,s,n->Memory);
        
        n = n->Next;
    }
}
void Scene_Comp_Update(void* parent,Scene* s){
    //Renderable_Update(&s->renderable,parent);

    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Update) r->Update(s,n->Memory);
        
        Node* const rem = n;
        n = n->Next;

        if(r->Remove && rem){
            Scene_RemoveI(s,i);
            i--;
        }
    }
}
void Scene_Comp_Input(void* parent,Scene* s,States* states,Vec2 Mouse,Vec2 MouseB){
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        
        if(Alignment_active(r->Align,ALIGN_VISIBLE))
            if(r->Input) r->Input(s,n->Memory,states,Mouse,MouseB);
        
        Node* const rem = n;
        n = n->Next;

        if(r->Remove && rem){
            Scene_RemoveI(s,i);
            i--;
        }
    }
}
void Scene_Comp_Free(void* parent,Scene* s){
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        Renderable_Free(s,r);
        n = n->Next;
    }
    List_Free(&s->childs);
}

void Scene_Adapt(Scene* s,int Target_Width,int Target_Height){
    s->renderable.rect_out = Renderable_GetRect_R(
        s->renderable.rect,
        Rect_New(
            (Vec2){ 0.0f,0.0f },
            (Vec2){ Target_Width,Target_Height }
        )
    );
}
void Scene_Update(Scene* s){
    Scene_Comp_Update(s->renderable.Parent,s);
}
void Scene_Input(Scene* s,States* states,Vec2 Mouse,Vec2 MouseB){
    Scene_Comp_Input(s->renderable.Parent,s,states,Mouse,MouseB);
}
void Scene_Render(unsigned int *Target,int Target_Width,int Target_Height,Scene* s){
    Scene_Comp_Render(Target,Target_Width,Target_Height,s->renderable.Parent,s);
}
void Scene_Free(Scene* s){
    Scene_Comp_Free(s->renderable.Parent,s);
}
char Scene_Contains(Scene* s,void* t){
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        if(r==t && t!=NULL) return 1;
        
        n = n->Next;
    }
    return 0;
}
void Scene_Print(Scene* s){
    printf("--- Scene ---\n");
    
    Node* n = s->childs.First;
    for(int i = 0;i<s->childs.size;i++){
        Renderable* r = (Renderable*)n->Memory;
        if(r){
            printf("%d: %p -> ",i,r);
            Renderable_Print_N(r);
            printf("\n");
        }
        n = n->Next;
    }
    
    printf("-------------\n");
}
Scene Scene_New(Renderable* Parent,Rect rect,unsigned int Bg){
    Scene b;
    b.childs = List_New();
    b.renderable = Renderable_New(
        Parent,
        (void(*)(unsigned int*,int,int,void*,void*))Scene_Comp_Render,
        (void(*)(void*,void*,EventId*))NULL,
        (void(*)(void*,void*))Scene_Comp_Update,
        (void(*)(void*,void*,States*,Vec2,Vec2))Scene_Comp_Input,
        (void(*)(void*,void*))Scene_Comp_Free,
        rect,
        EVENT_ALL,
        ALIGN_HORI_LEFT | ALIGN_VERT_TOP | ALIGN_BORDER,
        Bg,
        WHITE,
        BLACK
    );
    return b;
}

#endif // !SCENE_H