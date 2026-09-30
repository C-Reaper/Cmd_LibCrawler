#ifndef ALXWINDOW_LINUX_H
#define ALXWINDOW_LINUX_H

#ifdef __linux__

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xrender.h>

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
    int Fullscreen;
    
    int screen;
    Display* display;
    Window alxwindow;

    //Visual* visual;
    XSetWindowAttributes attr;
    XVisualInfo* vinfo;
    GC gc;
    Atom wmDeleteMessage;
} AlxWindow;

void AlxWindow_Mouse_Set(AlxWindow* w,Vec2 p){
    XWarpPointer(w->display,w->alxwindow,w->alxwindow,0,0,0,0,p.x * w->PixelWidth,p.y * w->PixelHeight);
    XFlush(w->display);

    w->MouseX = p.x;
    w->MouseY = p.y;
    w->MouseBeforeX = p.x;
    w->MouseBeforeY = p.y;
}
void AlxWindow_Mouse_SetInvisible(AlxWindow* w){
    Pixmap emptyPixmap = XCreatePixmap(w->display,w->alxwindow,1,1,1);

    XColor dummyColor;
    dummyColor.red = 0;
    dummyColor.green = 0;
    dummyColor.blue = 0;
    dummyColor.flags = DoRed | DoGreen | DoBlue;

    Cursor invisibleCursor = XCreatePixmapCursor(w->display,emptyPixmap,emptyPixmap,&dummyColor,&dummyColor,0,0);

    //Cursor invisibleCursor = XCreateFontCursor(w->display,0);
    XDefineCursor(w->display,w->alxwindow,invisibleCursor);

    XFreePixmap(w->display,emptyPixmap);
    XFreeCursor(w->display,invisibleCursor);
}
void AlxWindow_Mouse_SetVisible(AlxWindow* w){
    Cursor visibleCursor = XCreateFontCursor(w->display,XC_arrow);
    XDefineCursor(w->display,w->alxwindow,visibleCursor);
    //XFreeCursor(w->display,visibleCursor);
}
void AlxWindow_SetFullscreen(AlxWindow* w){
    Atom wm_state   = XInternAtom(w->display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(w->display, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.type = ClientMessage;
    xev.xclient.window = w->alxwindow;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;

    // TOGGLE / FORCE
    xev.xclient.data.l[0] = 2; // _NET_WM_STATE_TOGGLE
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(w->display,
               DefaultRootWindow(w->display),
               False,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);

    XFlush(w->display);

    w->Fullscreen = !w->Fullscreen;
}

void AlxWindow_Exit(AlxWindow* w){
    w->Running = 0;
    if(w->delete) w->delete(w);

    XSync(w->display,0);

    if(w->Buffer) free(w->Buffer);
    w->Buffer = NULL;
    AlxFont_Free(&w->font);

    XFreeGC(w->display,w->gc);
    XDestroyWindow(w->display,w->alxwindow);
    XCloseDisplay(w->display);
}
void AlxWindow_OnlyExit(AlxWindow* w){
    AlxWindow_Exit(w);
    
    if(w->vinfo) free(w->vinfo);
    w->vinfo = NULL;
}
void AlxWindow_Render(AlxWindow* w) {
    if(w->PixelWidth<=1 && w->PixelHeight<=1){
        XImage *image = XCreateImage(
            w->display,w->vinfo->visual,
            w->vinfo->depth,ZPixmap,0,(char*)w->Buffer,
            w->Width,w->Height,32,0
        );
        XPutImage(w->display,w->alxwindow,w->gc,image,0,0,0,0,w->Width,w->Height);
        XFree(image);
    }else{
        Pixel* output = (Pixel*)malloc(sizeof(Pixel) * w->Width * w->Height * w->PixelWidth * w->PixelHeight);
        // for(int i = 0;i<w->Height * w->PixelHeight;i++){
        //     Memexpand_IntShift(
        //         w->Buffer + i / w->PixelHeight * w->Width,
        //         output + i * w->Width * w->PixelWidth,
        //         w->Width * w->PixelWidth,
        //         I64_Log2(w->PixelWidth)
        //     );
        // }

        for(int i = 0;i<w->PixelHeight * w->Height;i++){
            Memexpand(
                w->Buffer + (i / w->PixelHeight) * w->Width,
                output + i * w->Width * w->PixelWidth,
                w->Width,
                w->Width * w->PixelWidth
            );
        }

        //Graphics_Scale(w->Buffer,output,(Vic2){ w->Width,w->Height },w->PixelWidth);
        
        XImage *image = XCreateImage(
            w->display,w->vinfo->visual,
            w->vinfo->depth,ZPixmap,0,(char*)output,
            w->Width * w->PixelWidth,w->Height * w->PixelHeight,32,0
        );
        XPutImage(w->display,w->alxwindow,w->gc,image,0,0,0,0,w->Width * w->PixelWidth,w->Height * w->PixelHeight);
        XFree(image);
        free(output);

        /*unsigned int win_width = w->Width * w->PixelWidth;
        unsigned int win_height = w->Height * w->PixelHeight;

        XRenderPictFormat *fmt = XRenderFindStandardFormat(w->display,PictStandardARGB32);
       
        Pixmap pix = XCreatePixmap(w->display,w->alxwindow,w->Width,w->Height,32);
        Pixmap scaled_pix = XCreatePixmap(w->display,w->alxwindow,w->Width * w->PixelWidth,w->Height * w->PixelHeight,32);

        Picture src_pic = XRenderCreatePicture(w->display,pix,fmt,0,NULL);
        Picture dst_pic = XRenderCreatePicture(w->display,scaled_pix,fmt,0,NULL);

        XTransform xform = {
            {
                { 65536 * w->PixelWidth,   0,                        0 },
                { 0,                       65536 * w->PixelHeight,   0 },
                { 0,                       0,                        65536 }
            }
        };
        XRenderSetPictureTransform(w->display, src_pic, &xform);
        XRenderComposite(w->display,PictOpSrc,src_pic,None,dst_pic,0,0,0,0,0,0, win_width, win_height);
        XCopyArea(w->display,scaled_pix,w->alxwindow,w->gc,0,0,win_width,win_height,0,0);

        XFreePixmap(w->display, pix);
        XFreePixmap(w->display, scaled_pix);
        XRenderFreePicture(w->display, src_pic);
        XRenderFreePicture(w->display, dst_pic);*/

        //XImage *image = XCreateImage(
        //    w->display,w->vinfo.visual,
        //    w->vinfo.depth,ZPixmap,0,(char*)output,
        //    w->Width * w->PixelWidth,w->Height * w->PixelHeight,32,0
        //);
        //XPutImage(w->display,w->alxwindow,w->gc,image,0,0,0,0,w->Width * w->PixelWidth,w->Height * w->PixelHeight);
        //XFree(image);
        //free(output);
    }
    
    //XImage *image = XGetImage(w->display,w->alxwindow,0,0,w->Width,w->Height,AllPlanes,ZPixmap);
    //XPutImage(w->display,w->alxwindow,w->gc,w->image,0,0,0,0,w->Width,w->Height);
    XFlush(w->display);
}
void AlxWindow_UpdateKB(AlxWindow* w){
    w->MouseBeforeX = w->MouseX;
    w->MouseBeforeY = w->MouseY;

	w->LastKey = 0;
    w->LastChar = 0;

    int repeat = 0;
    if(w->PressPoint > 0U){
        if(Time_Elapsed(w->PressPoint,Time_Nano()) > w->Delay && Time_Elapsed(w->PressTick,Time_Nano()) > w->Repeat){
            repeat = 1;
            w->PressTick = Time_Nano();
        }
    }else{
        w->PressTick = 0U;
    }

    for (int i = 0; i < MAX_STROKES; i++){
        w->Strokes[i].REPEAT = repeat;
        w->Strokes[i].PRESSED = 0;
		w->Strokes[i].RELEASED = 0;
    }

    XEvent event;
    while (XPending(w->display) > 0) {
        XNextEvent(w->display,&event);
        if (event.type == Expose) {
            AlxWindow_Render(w);
        }else if (event.type == KeyPress) {
            if(!w->Strokes[event.xkey.keycode].DOWN){
                w->PressPoint = Time_Nano();
                w->LastKey = event.xkey.keycode;
                w->LastChar = XLookupKeysym(&event.xkey,0);

                w->Strokes[event.xkey.keycode].PRESSED = 1;
                w->Strokes[event.xkey.keycode].DOWN = 1;
            }
        }else if (event.type == KeyRelease) {
		    if(w->Strokes[event.xkey.keycode].DOWN){
                w->PressPoint = 0U;

                w->Strokes[event.xkey.keycode].RELEASED = 1;
                w->Strokes[event.xkey.keycode].DOWN = 0;
            }
        }else if (event.type == ButtonPress) {
            if(!w->Strokes[event.xkey.keycode].DOWN){
                w->PressPoint = Time_Nano();

                w->LastKey = event.xbutton.button;
                w->Strokes[event.xbutton.button].PRESSED = 1;
                w->Strokes[event.xbutton.button].DOWN = 1;
            }
        }else if (event.type == ButtonRelease) {
            if(w->Strokes[event.xkey.keycode].DOWN){
                w->PressPoint = 0U;
                
                w->Strokes[event.xbutton.button].RELEASED = 1;
                w->Strokes[event.xbutton.button].DOWN = 0;
            }
        }else if (event.type == MotionNotify) {
            w->MouseX = event.xmotion.x / w->PixelWidth;
            w->MouseY = event.xmotion.y / w->PixelHeight;
        }else if (event.type == ConfigureNotify) {
            XConfigureEvent xce = event.xconfigure;

            //XWindowAttributes wa;
            //XGetWindowAttributes(w->display,w->alxwindow,&wa);
            //if (wa.width != w->Width || wa.height != w->Height) {
            //    w->Width = wa.width;
            //    w->Height = wa.height;
            //
            //    Pixel* SwapPixel = w->Buffer;
            //    w->Buffer = (Pixel*)malloc(xce.width * xce.height * sizeof(Pixel));
            //    free(SwapPixel);
            //}

            unsigned int width = xce.width / w->PixelWidth;
            unsigned int height = xce.height / w->PixelHeight;

            Pixel* SwapPixel = w->Buffer;
            w->Buffer = (Pixel*)malloc(width * height * sizeof(Pixel));
            
            //const int minwidth = I32_Min(w->Width,width);
            //const int minheight = I32_Min(w->Height,height);
            //for(int i = 0;i<minheight;i++){
            //    memcpy(w->Buffer + i * width,SwapPixel + i * w->Width,minwidth);
            //}

            free(SwapPixel);

            //XImage* SwapImage = w->image;
            //w->image = XCreateImage(w->display,w->vinfo.visual,32,ZPixmap,0,(char*)(w->Buffer),xce.width,xce.height,32,0);
            //w->image->data = (char*)w->Buffer;
            //w->image->width = xce.width;
            //w->image->height = xce.height;
            //
            //free(SwapPixel);
            //XDestroyImage(SwapImage);

            w->x = xce.x;
            w->y = xce.y;
            w->Width = width;
            w->Height = height;

            if(w->resize) w->resize(w);

            AlxWindow_Render(w);

            //XResizeWindow(w->display,w->alxwindow,w->Width,w->Height);
            //printf("W=%d vs %d, H=%d vs %d\n",xce.width,w->Width,xce.height,w->Height);
        }else if (event.type == DestroyNotify) {
            w->Running = 0;
            return;
        }else if (event.type == ClientMessage) {
            if (event.xclient.data.l[0] == w->wmDeleteMessage) {
                w->Running = 0;
                return;
            }
        }else if (event.type == FocusIn) {
            w->Focus = 1;
        }else if (event.type == FocusOut) {
            w->Focus = 0;
        }
    }
}

AlxWindow AlxWindow_Null(){
    AlxWindow w;
    memset(&w,0,sizeof(w));
    return w;
}
void AlxWindow_PreInit(AlxWindow* w){
    w->display = XOpenDisplay(NULL);
    if (w->display == NULL) {
        printf("[AlxWindow]: Can't open Display!");
        return;
    }
    w->screen = DefaultScreen(w->display);

    w->vinfo = (XVisualInfo*)malloc(sizeof(XVisualInfo));
    if (!XMatchVisualInfo(w->display,w->screen,32,TrueColor,w->vinfo)) {
        printf("[AlxWindow]: Make -> Error: 32-Bit TrueColor Visual not avalible!\n");
        return;
    }
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
    w->PressPoint = 0U;
    w->PressTick = 0U;
    w->Delay = 1.0;
    w->Repeat = 0.1;

    w->Width = Width;
    w->Height = Height;
    w->PixelWidth = AlxFontX;
    w->PixelHeight = AlxFontY;

    w->setup = Setup;
    w->update = Update;
    w->delete = Delete;
    w->resize = Resize;

    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(w->display,RootWindow(w->display,w->screen),w->vinfo->visual,AllocNone);
    attr.background_pixel = WhitePixel(w->display,w->screen);
    attr.border_pixel = BlackPixel(w->display,w->screen);

    Window root = RootWindow(w->display,w->screen);
    Window alxwindow = XCreateWindow(w->display,root,0,0,Width * w->PixelWidth,Height * w->PixelHeight,0,w->vinfo->depth, InputOutput,w->vinfo->visual,CWColormap | CWBorderPixel | CWBackPixel,&attr);
    
    XStoreName(w->display,alxwindow,Name);
    XSelectInput(w->display,alxwindow,ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | FocusChangeMask);
    XMapWindow(w->display,alxwindow);

    w->wmDeleteMessage = XInternAtom(w->display,"WM_DELETE_WINDOW",False);
    XSetWMProtocols(w->display,alxwindow,&w->wmDeleteMessage,1);
    
    w->gc = XCreateGC(w->display,alxwindow,0,NULL);
    w->attr = attr;

    w->alxwindow = alxwindow;
    w->Buffer = (Pixel*)malloc(w->Width * w->Height * sizeof(Pixel));
    
    //w->visual = DefaultVisual(display,screen);
    //w->gc = DefaultGC(display,screen);
    
    w->LastTime = 0;
    w->ElapsedTime = 0.0;
    w->Running = 1;
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
    w->Focus = 1;
    w->font = AlxFont_New(ALXFONT_YANIS);
    AlxFont_Resize(&w->font,40,40);
    
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
    sprintf(Buffer,"Alx - %s - %4.1f",w->Name,Fps);
    XStoreName(w->display,w->alxwindow,Buffer);

    if (w->Strokes[ALX_KEY_F11].PRESSED)
	    AlxWindow_SetFullscreen(w);
}
void AlxWindow_GameLoop(AlxWindow* w,void (*Render)(AlxWindow*)){
    while (w->Running) {
        AlxWindow_GameUpdate(w);
        
        if(w->update) w->update(w);
        
        Render(w);
        
        if(!w->Focus)   Thread_Sleep_M(40);
        else            Thread_Sleep_M(2);
    }
}
void AlxWindow_Start(AlxWindow* w){
    AlxWindow_GameInit(w);
    AlxWindow_GameLoop(w,AlxWindow_Render);
    AlxWindow_OnlyExit(w);
}
void AlxWindow_Free(AlxWindow* w){
    AlxWindow_Exit(w);
}

#else
#error "Platform is not supported!"
#endif

#endif 