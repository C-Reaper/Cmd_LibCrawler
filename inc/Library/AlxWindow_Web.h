#ifndef ALXWINDOW_WEB_H
#define ALXWINDOW_WEB_H

#if defined(__EMSCRIPTEN__)

#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdlib.h>
#include <string.h>

#include "Pixel.h"
#include "AlxFont.h"
#include "AlxTime.h"
#include "Thread.h"
#include "Stroke.h"

typedef struct AlxWindow {
    char* Name;
    Pixel* Buffer;
    int Width;
    int Height;
    int PixelWidth;
    int PixelHeight;

    int x;
    int y;
    unsigned long long LastTime;
    double ElapsedTime;
    int MouseX;
    int MouseY;
    int MouseBeforeX;
    int MouseBeforeY;
    int Focus;
    int Running;
    void (*setup)(struct AlxWindow*);
    void (*update)(struct AlxWindow*);
    void (*delete)(struct AlxWindow*);
    void (*resize)(struct AlxWindow*);
    States Strokes[MAX_STROKES];
    AlxFont font;
    FDuration Delay;
    FDuration Repeat;
    Timepoint PressTick;
    Timepoint PressPoint;
    unsigned short LastKey;
    unsigned short LastChar;

    /* EMSCRIPTEN DOES NOT NEED DISPLAY/WINDOW */
    void* canvas;
    double DevicePixelRatio;
    float CanvasScale;
    int CanvasOffsetX;
    int CanvasOffsetY;
    int CanvasPixelWidth;
    int CanvasPixelHeight;
    int BaseCSSWidth;
    int BaseCSSHeight;
} AlxWindow;

static inline void AlxWindow_Stroke_Set(AlxWindow* w, int id, int pressed){
    if(id < 0 || id >= MAX_STROKES) return;
    w->Strokes[id].PRESSED  = pressed;
    w->Strokes[id].DOWN     = pressed;
    w->Strokes[id].RELEASED = !pressed;
}
void AlxWindow_SyncCanvasSize(AlxWindow* w){
    int cw, ch;
    emscripten_get_canvas_element_size("#canvas", &cw, &ch);

    double dpr = emscripten_get_device_pixel_ratio();

    int newW = (int)(cw * dpr) / w->PixelWidth;
    int newH = (int)(ch * dpr) / w->PixelHeight;

    if (newW <= 0) newW = 1;
    if (newH <= 0) newH = 1;

    if (newW == w->Width && newH == w->Height)
        return;

    if (w->Buffer)
        free(w->Buffer);

    w->Width = newW;
    w->Height = newH;

    w->Buffer = (Pixel*)malloc(sizeof(Pixel) * newW * newH);

    if (w->resize)
        w->resize(w);
}
EM_JS(void, js_get_canvas_size, (int* w, int* h, float* dpr), {
    var canvas = Module['canvas'];
    var dpr = window.devicePixelRatio || 1;
    HEAP32[w>>2] = canvas.clientWidth;
    HEAP32[h>>2] = canvas.clientHeight;
    HEAPF32[dpr>>2] = dpr;
});
EM_BOOL AlxWindow_ResizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;

    int cw, ch;
    emscripten_get_canvas_element_size("#canvas", &cw, &ch);

    double dpr = emscripten_get_device_pixel_ratio();

    int newW = (int)(cw * dpr) / w->PixelWidth;
    int newH = (int)(ch * dpr) / w->PixelHeight;

    if (newW <= 0) newW = 1;
    if (newH <= 0) newH = 1;

    if (w->Buffer)
        free(w->Buffer);

    w->Width = newW;
    w->Height = newH;

    w->Buffer = (Pixel*)malloc(sizeof(Pixel) * newW * newH);

    if (w->resize)
        w->resize(w);

    return EM_TRUE;
}
void AlxWindow_UpdateCanvasSize(AlxWindow* w){
    int cssW = 0;
    int cssH = 0;
    float dpr = 1.0f;
    
    js_get_canvas_size(&cssW, &cssH, &dpr);
    
    w->CanvasPixelWidth  = (int)(cssW * dpr);
    w->CanvasPixelHeight = (int)(cssH * dpr);
    w->CanvasScale = (float)w->Width  / (float)cssW;

    /*
    int cssW = 0;
    int cssH = 0;
    float dpr = 1.0f;
    
    js_get_canvas_size(&cssW, &cssH, &dpr);

    w->CanvasPixelWidth  = (int)(cssW * dpr);
    w->CanvasPixelHeight = (int)(cssH * dpr);

    // Engine aspect ratio
    float sx = (float)w->Width  / (float)w->CanvasPixelWidth;
    float sy = (float)w->Height / (float)w->CanvasPixelHeight;

    // IMPORTANT: uniform scale (fixes distortion)
    w->CanvasScale = (sx < sy) ? sx : sy;

    // Letterbox area
    int drawW = (int)(w->CanvasPixelWidth  * w->CanvasScale);
    int drawH = (int)(w->CanvasPixelHeight * w->CanvasScale);

    w->CanvasOffsetX = (w->CanvasPixelWidth  - drawW) / 2;
    w->CanvasOffsetY = (w->CanvasPixelHeight - drawH) / 2;
    */
}
EM_BOOL AlxWindow_FullscreenChange(int eventType, const EmscriptenFullscreenChangeEvent* e, void* userData){
    AlxWindow* w = (AlxWindow*)userData;

    if(e->isFullscreen){
        emscripten_set_canvas_element_size("#canvas", e->screenWidth, e->screenHeight);
    } else {
        emscripten_set_canvas_element_size("#canvas", w->BaseCSSWidth, w->BaseCSSHeight);
    }

    //EM_ASM({
    //    var canvas = Module['canvas'];
    //        
    //    if ($2) {
    //        // fullscreen
    //        canvas.style.width  = "100vw";
    //        canvas.style.height = "100vh";
    //    } else {
    //        canvas.style.width  = $0 + "px";
    //        canvas.style.height = $1 + "px";
    //    }
    //}, w->BaseCSSWidth, w->BaseCSSHeight, e->isFullscreen);

    AlxWindow_SyncCanvasSize(w);
    return EM_TRUE;
}
void AlxWindow_SetFullscreen(AlxWindow* w){
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, w, EM_TRUE, AlxWindow_FullscreenChange);
    emscripten_request_fullscreen("#canvas", EM_TRUE);
}

/* ---------------- INPUT CALLBACKS ---------------- */

static EM_BOOL KeyDownCB(int eventType, const EmscriptenKeyboardEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;
    int key = (int)e->keyCode;
    AlxWindow_Stroke_Set(w, key, 1);
    w->LastKey = key;
    //printf("KeyDownCB: %d\n",key);
    return 1;
}
static EM_BOOL KeyUpCB(int eventType, const EmscriptenKeyboardEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;
    int key = (int)e->keyCode;
    AlxWindow_Stroke_Set(w, key, 0);
    //printf("KeyUpCB: %d\n",key);
    return 1;
}

static EM_BOOL MouseDownCB(int eventType, const EmscriptenMouseEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;
    AlxWindow_Stroke_Set(w, e->button + 1, 1);
    //printf("MouseDownCB: %d\n",e->button);
    return 1;
}
static EM_BOOL MouseUpCB(int eventType, const EmscriptenMouseEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;
    AlxWindow_Stroke_Set(w, e->button + 1, 0);
    //printf("MouseUpCB: %d\n",e->button);
    return 1;
}
static EM_BOOL MouseMoveCB(int eventType, const EmscriptenMouseEvent *e, void *userData){
    AlxWindow* w = (AlxWindow*)userData;

    int cssW = 0;
    int cssH = 0;
    float dpr = 1.0f;
    js_get_canvas_size(&cssW, &cssH, &dpr);

    if(cssW <= 0 || cssH <= 0) return 1;

    double sx = (double)w->Width  / (double)cssW;
    double sy = (double)w->Height / (double)cssH;

    w->MouseX = (int)(e->targetX * sx);
    w->MouseY = (int)(e->targetY * sy);

    return 1;
}

/* ---------------- CORE WINDOW FUNCTIONS ---------------- */

void AlxWindow_Mouse_Set(AlxWindow* w, Vec2 p){
    EM_ASM({
        var x = $0;
        var y = $1;

        // Canvas-relative mouse move is NOT allowed in browsers directly.
        // We simulate it via pointer lock API if active.

        if(document.pointerLockElement === Module['canvas']){
            // relative movement mode (FPS style)
            // cannot warp absolute cursor position
        } else {
            // fallback: no real warp possible in browser
        }

    }, (int)p.x, (int)p.y);

    w->MouseX = p.x;
    w->MouseY = p.y;
    w->MouseBeforeX = p.x;
    w->MouseBeforeY = p.y;
}
void AlxWindow_Mouse_SetInvisible(AlxWindow* w){
    EM_ASM({
        var canvas = Module['canvas'];

        canvas.style.cursor = 'none';
    });
}
void AlxWindow_Mouse_SetVisible(AlxWindow* w){
    EM_ASM({
        var canvas = Module['canvas'];
        canvas.style.cursor = 'default';
    });
}

void AlxWindow_Exit(AlxWindow* w){
    w->Running = 0;
    if(w->delete) w->delete(w);

    if(w->Buffer) free(w->Buffer);
    w->Buffer = NULL;
    AlxFont_Free(&w->font);
}
void AlxWindow_OnlyExit(AlxWindow* w){
    AlxWindow_Exit(w);
}

void AlxWindow_Render(AlxWindow* w){
    if(!w || !w->Buffer) return;
    
    /*
    int cw, ch;
    emscripten_get_canvas_element_size("#canvas", &cw, &ch);

    if (cw != w->Width || ch != w->Height)
    {
        // fallback safety resize trigger
        AlxWindow_ResizeCallback(0, NULL, w);
    }
    */

    int width  = w->Width;
    int height = w->Height;

    EM_ASM({
        var width  = $0;
        var height = $1;
        var ptr    = $2;

        var canvas = Module['canvas'];
        if(canvas.width !== width) canvas.width = width;
        if(canvas.height !== height) canvas.height = height;

        var ctx = canvas.getContext('2d');

        var imageData = ctx.createImageData(width, height);

        // Pixel* Buffer -> Uint8ClampedArray (RGBA erwartet)
        var src = HEAPU8.subarray(ptr, ptr + width * height * 4);

        imageData.data.set(src);

        ctx.putImageData(imageData, 0, 0);
    }, width, height, (int)w->Buffer);
}
void AlxWindow_UpdateKB(AlxWindow* w){
    //AlxWindow_SyncCanvasSize(w);
    AlxWindow_UpdateCanvasSize(w);

    w->LastKey = 0;
    w->LastChar = 0;

    int repeat = 0;

    if(w->PressPoint > 0U){
        if(Time_Elapsed(w->PressPoint,Time_Nano()) > w->Delay &&
           Time_Elapsed(w->PressTick,Time_Nano()) > w->Repeat){
            repeat = 1;
            w->PressTick = Time_Nano();
        }
    }else{
        w->PressTick = 0U;
    }

    for(int i=0;i<MAX_STROKES;i++){
        w->Strokes[i].REPEAT = repeat;
        
        if(w->Strokes[i].PRESSED){
            if(!w->Strokes[i].PRESSED_MARK){
                w->Strokes[i].PRESSED_MARK = 1;
            }else{
                w->Strokes[i].PRESSED = 0;
                w->Strokes[i].PRESSED_MARK = 0;
            }
        }else{
            w->Strokes[i].PRESSED_MARK = 0;
        }

        if(w->Strokes[i].RELEASED){
            if(!w->Strokes[i].RELEASED_MARK){
                w->Strokes[i].RELEASED_MARK = 1;
            }else{
                w->Strokes[i].RELEASED = 0;
                w->Strokes[i].RELEASED_MARK = 0;
            }
        }else{
            w->Strokes[i].RELEASED_MARK = 0;
        }
    }
}

AlxWindow AlxWindow_Null(){
    AlxWindow w;
    memset(&w,0,sizeof(w));
    return w;
}
void AlxWindow_PreInit(AlxWindow* w){
    w->Focus = 1;
    w->Running = 1;
}
void AlxWindow_Init(
    AlxWindow* w,
    char* Name,
    int Width,
    int Height,
    int AlxFontX,
    int AlxFontY,
    void (*Setup)(AlxWindow*),
    void (*Update)(AlxWindow*),
    void (*Delete)(AlxWindow*),
    void (*Resize)(AlxWindow*)
){
    w->Name = CStr_Cpy(Name);

    w->Width = Width;
    w->Height = Height;
    w->PixelWidth = AlxFontX;
    w->PixelHeight = AlxFontY;

    w->setup = Setup;
    w->update = Update;
    w->delete = Delete;
    w->resize = Resize;

    w->Buffer = (Pixel*)malloc(w->Width * w->Height * sizeof(Pixel));

    w->BaseCSSWidth  = Width;
    w->BaseCSSHeight = Height;

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, w, 1, KeyDownCB);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, w, 1, KeyUpCB);

    emscripten_set_mousedown_callback("#canvas", w, 1, MouseDownCB);
    emscripten_set_mouseup_callback("#canvas", w, 1, MouseUpCB);
    emscripten_set_mousemove_callback("#canvas", w, 1, MouseMoveCB);
    
    emscripten_set_resize_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW,
        w,
        EM_TRUE,
        AlxWindow_ResizeCallback
    );
    w->DevicePixelRatio = emscripten_get_device_pixel_ratio();

    //emscripten_request_fullscreen("#canvas", EM_TRUE);
}
AlxWindow AlxWindow_Make(
    char* Name,
    int Width,
    int Height,
    int AlxFontX,
    int AlxFontY,
    void (*Setup)(AlxWindow*),
    void (*Update)(AlxWindow*),
    void (*Delete)(AlxWindow*),
    void (*Resize)(AlxWindow*)
){
    AlxWindow w = AlxWindow_Null();
    AlxWindow_PreInit(&w);
    AlxWindow_Init(&w,Name,Width,Height,AlxFontX,AlxFontY,Setup,Update,Delete,Resize);
    return w;
}
AlxWindow AlxWindow_New(
    char* Name,
    int Width,
    int Height,
    int AlxFontX,
    int AlxFontY,
    void (*Setup)(AlxWindow*),
    void (*Update)(AlxWindow*),
    void (*Delete)(AlxWindow*)
){
    AlxWindow w = AlxWindow_Make(Name,Width,Height,AlxFontX,AlxFontY,Setup,Update,Delete,NULL);
    return w;
}

void AlxWindow_GameInit(AlxWindow* w){
    if(w->setup) w->setup(w);
    w->LastTime = Time_Nano();
}
void AlxWindow_GameUpdate(AlxWindow* w){
    w->ElapsedTime = (double)(Time_Nano() - w->LastTime) / 1E9;
    w->LastTime = Time_Nano();
    const double Fps = 1.0 / w->ElapsedTime;

    w->MouseBeforeX = w->MouseX;
    w->MouseBeforeY = w->MouseY;
    AlxWindow_UpdateKB(w);

    char Buffer[128];
    sprintf(Buffer, "Alx - %s - %4.1f", w->Name, Fps);
    emscripten_set_window_title(Buffer);

    if (w->Strokes[ALX_KEY_F11].PRESSED)
	    AlxWindow_SetFullscreen(w);
}
bool AlxWindow_GameLoop_Wrapper(double time, void* userData){
    AlxWindow* w = (AlxWindow*)userData;

    AlxWindow_GameUpdate(w);

    if(w->update)
        w->update(w);

    AlxWindow_Render(w);

    if(!w->Focus) Thread_Sleep_M(40);
    else Thread_Sleep_M(2);

    return w->Running; // wichtig: true = weiterlaufen
}
void AlxWindow_GameLoop(void* arg){
    AlxWindow* w = (AlxWindow*)arg;

    if(!w->Running) return;

    AlxWindow_GameUpdate(w);

    if(w->update) w->update(w);

    emscripten_request_animation_frame_loop(AlxWindow_GameLoop_Wrapper, w);
}
void AlxWindow_Start(AlxWindow* w){
    AlxWindow_GameInit(w);
    emscripten_request_animation_frame_loop(AlxWindow_GameLoop_Wrapper, w);
}
void AlxWindow_Free(AlxWindow* w){
    AlxWindow_Exit(w);
}

#else
#error "Platform is not supported!"
#endif

#endif