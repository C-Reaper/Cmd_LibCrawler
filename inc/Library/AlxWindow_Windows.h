#ifndef ALXWINDOW_WINDOWS_H
#define ALXWINDOW_WINDOWS_H

#if defined _WIN32

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#include "Math.h"
#include "Intrinsics.h"
#include "AlxTime.h"
#include "Thread.h"
#include "String.h"
#include "Graphics.h"


typedef struct BITMAPINFO_RGBA {
    BITMAPINFOHEADER header;
    DWORD masks[4]; // R, G, B, A (A ignored)
} BITMAPINFO_RGBA;

typedef struct AlxWindow {
    char* Name;
    Pixel* Buffer;
    unsigned long long LastTime;
    double ElapsedTime;
    int x;
    int y;
    int Width;
    int Height;
    int PixelWidth;
    int PixelHeight;
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

    RECT WindowedRect;
    DWORD WindowedStyle;
    DWORD WindowedExStyle;
    HWND hwnd;
    HDC hdc;
    BITMAPINFO_RGBA bmi;
} AlxWindow;

HCURSOR AlxWindow_CreateInvisibleCursor(){
    static BYTE andMask[32] = { 0xFF };
    static BYTE xorMask[32] = { 0x00 };

    return CreateCursor(
        NULL,
        0,0,
        1,1,
        andMask,
        xorMask
    );
}

void AlxWindow_Mouse_Set(AlxWindow* w,Vec2 p){
    POINT pt;
    pt.x = (LONG)p.x * w->PixelWidth;
    pt.y = (LONG)p.y * w->PixelHeight;

    ClientToScreen(w->hwnd,&pt);
    SetCursorPos(pt.x,pt.y);
    
    w->MouseX = p.x;
    w->MouseY = p.y;
    w->MouseBeforeX = p.x;
    w->MouseBeforeY = p.y;
}
void AlxWindow_Mouse_SetInvisible(AlxWindow* w){
    static HCURSOR invisible = NULL;

    if (!invisible)
        invisible = AlxWindow_CreateInvisibleCursor();

    SetClassLongPtr(
        w->hwnd,
        GCLP_HCURSOR,
        (LONG_PTR)invisible
    );

    SetCursor(invisible);
}
void AlxWindow_Mouse_SetVisible(AlxWindow* w){
    HCURSOR cursor = LoadCursor(NULL,IDC_ARROW);

    SetClassLongPtr(
        w->hwnd,
        GCLP_HCURSOR,
        (LONG_PTR)cursor
    );

    SetCursor(cursor);
}
void AlxWindow_SetFullscreen(AlxWindow* w){
    if(!w->Fullscreen){
        w->WindowedStyle   = GetWindowLong(w->hwnd, GWL_STYLE);
        w->WindowedExStyle = GetWindowLong(w->hwnd, GWL_EXSTYLE);
        GetWindowRect(w->hwnd, &w->WindowedRect);

        HMONITOR monitor = MonitorFromWindow(w->hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(monitor, &mi);

        SetWindowLong(w->hwnd, GWL_STYLE,
            w->WindowedStyle & ~(WS_OVERLAPPEDWINDOW));

        SetWindowLong(w->hwnd, GWL_EXSTYLE,
            w->WindowedExStyle);

        SetWindowPos(w->hwnd, HWND_TOP,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        SetWindowPos(w->hwnd, HWND_TOP,
            mi.rcMonitor.left,
            mi.rcMonitor.top,
            mi.rcMonitor.right  - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_NOOWNERZORDER);

        w->Fullscreen = 1;
    } else {
        SetWindowLong(w->hwnd, GWL_STYLE, w->WindowedStyle);
        SetWindowLong(w->hwnd, GWL_EXSTYLE, w->WindowedExStyle);

        SetWindowPos(w->hwnd, NULL,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        SetWindowPos(w->hwnd, NULL,
            w->WindowedRect.left,
            w->WindowedRect.top,
            w->WindowedRect.right  - w->WindowedRect.left,
            w->WindowedRect.bottom - w->WindowedRect.top,
            SWP_NOOWNERZORDER);

        w->Fullscreen = 0;
    }
}

void AlxWindow_Exit(AlxWindow* w){
    w->Running = 0;
    if(w->delete) w->delete(w);
    
    if(w->Buffer) free(w->Buffer);
    w->Buffer = NULL;
    
    AlxFont_Free(&w->font);
    DestroyWindow(w->hwnd);
}
void AlxWindow_OnlyExit(AlxWindow* w){
    AlxWindow_Exit(w);
}
void AlxWindow_Render(AlxWindow* w) {
    StretchDIBits(
        w->hdc, 
        0, 0, w->Width * w->PixelWidth, w->Height * w->PixelHeight, 
        0, 0, w->Width, w->Height,
        w->Buffer, 
        (BITMAPINFO*)&w->bmi, 
        DIB_RGB_COLORS, 
        SRCCOPY
    );
    UpdateWindow(w->hwnd);
}
void AlxWindow_UpdateKB(AlxWindow* w){
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) w->Running = 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    char Buffer[256];
    GetKeyboardState(Buffer);
    wchar_t Unicode[2];
    
    if(w->Focus){
        w->LastKey = 0;
        w->LastChar = 0;
        for (int i = 0; i < MAX_STROKES; i++)
	    {
	    	w->Strokes[i].PRESSED = 0;
	    	w->Strokes[i].RELEASED = 0;
	    	if (GetAsyncKeyState(i) & 0x8000){
                w->LastKey = i;
                ToAscii(i,MapVirtualKey(i,MAPVK_VK_TO_VSC),Buffer,&w->LastChar,0);
                w->PressPoint = Time_Nano();
                
                w->Strokes[i].PRESSED = !w->Strokes[i].DOWN;
	    		w->Strokes[i].DOWN = 1;
	    	}else{
                w->PressPoint = 0U;

	    		w->Strokes[i].RELEASED = w->Strokes[i].DOWN;
	    		w->Strokes[i].DOWN = 0;
	    	}
	    }
    }
}

LRESULT CALLBACK AlxWindow_StdWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NCCREATE:{
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            AlxWindow* w = (AlxWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)w);
            //return DefWindowProc(hwnd, msg, wParam, lParam);
            return TRUE;
        }break;

        case WM_MOUSEMOVE:{
            AlxWindow* w = (AlxWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->MouseX = (int)LOWORD(lParam) / (w->PixelWidth==0 ? 1 : (int)w->PixelWidth);
            w->MouseY = (int)HIWORD(lParam) / (w->PixelHeight==0 ? 1 : (int)w->PixelHeight);
            return 0;
        }break;

        case WM_SIZE:{
            AlxWindow* w = (AlxWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            int Width  = (int)LOWORD(lParam);
            int Height = (int)HIWORD(lParam);
                    
            w->Width  = Width / w->PixelWidth;
            w->Height = Height / w->PixelHeight;

            const int size = w->Width * w->Height * sizeof(Pixel);
            Pixel* oldBuffer = w->Buffer;
            Pixel* newBuffer = (Pixel*)malloc(size);
            if(!newBuffer){
                return 0;
            }

            memset(newBuffer,0,size);
            free(oldBuffer);

            w->Buffer = newBuffer;

            ZeroMemory(&w->bmi,sizeof(BITMAPINFO_RGBA));
            w->bmi.header.biSize      = sizeof(BITMAPINFOHEADER);
            w->bmi.header.biWidth     = w->Width;
            w->bmi.header.biHeight    = -w->Height;
            w->bmi.header.biPlanes    = 1;
            w->bmi.header.biBitCount  = 32;
            w->bmi.header.biCompression = BI_BITFIELDS;

            w->bmi.masks[0] = 0x00FF0000; // Red
            w->bmi.masks[1] = 0x0000FF00; // Green
            w->bmi.masks[2] = 0x000000FF; // Blue
            w->bmi.masks[3] = 0xFF000000; // Alpha

            if (w->resize)
                w->resize(w);
            return 0;
        }break;

        //case WM_SYSCOMMAND:
        //    switch (wParam & 0xFFF0) {
        //        case SC_MINIMIZE:
        //            // Zusätzliche Aktionen vor dem Minimieren könnten hier durchgeführt werden
        //            ShowWindow(hwnd, SW_SHOWMINIMIZED);
        //            return 0; // Rückgabe 0, um die Standardbehandlung nicht auszuführen
        //
        //        case SC_RESTORE:
        //            // Behandlung für das Wiederherstellen des Fensters (nach dem Minimieren oder Maximieren)
        //            ShowWindow(hwnd, SW_RESTORE);
        //            return 0; // Rückgabe 0, um die Standardbehandlung nicht auszuführen
        //    }
        //break;

        case WM_SETFOCUS:{
            AlxWindow* w = (AlxWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->Focus = 1;
            return 0;
        }break;

        case WM_KILLFOCUS:{
            AlxWindow* w = (AlxWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->Focus = 0;
            return 0;
        }break;
        
        case WM_CLOSE:{
            AlxWindow* w = (AlxWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            //if (MessageBox(hwnd, "Are you sure?", "Quit", MB_OKCANCEL) == IDOK){
            //    delete();
            //    DestroyWindow(hwnd);
            //}
            w->Running = 0;
            return 0;
        }break;
        
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

AlxWindow AlxWindow_Null(){
    AlxWindow w;
    memset(&w,0,sizeof(w));
    return w;
}
void AlxWindow_PreInit(AlxWindow* w){
    WNDCLASSEX wc;
    ZeroMemory(&wc,sizeof(wc));

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = AlxWindow_StdWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    //wc.hInstance     = GetStdHandle(STD_OUTPUT_HANDLE);
    wc.hCursor       = LoadCursor(NULL, IDC_HAND);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "AlxWindow";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "AlxWindow Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
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

    w->hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "AlxWindow",
        "Buffer Example",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        //WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX     /     WS_OVERLAPPEDWINDOW
        //WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX     /     WS_OVERLAPPEDWINDOW
        CW_USEDEFAULT, CW_USEDEFAULT, w->Width * w->PixelWidth, w->Height * w->PixelHeight,
        NULL,NULL,
        GetModuleHandle(NULL),
        //GetStdHandle(STD_OUTPUT_HANDLE),
        w
    );

    ZeroMemory(&w->bmi,sizeof(BITMAPINFO_RGBA));
    w->bmi.header.biSize      = sizeof(BITMAPINFOHEADER);
    w->bmi.header.biWidth     = w->Width;
    w->bmi.header.biHeight    = -w->Height;
    w->bmi.header.biPlanes    = 1;
    w->bmi.header.biBitCount  = 32;
    w->bmi.header.biCompression = BI_BITFIELDS;

    w->bmi.masks[0] = 0x00FF0000; // Red
    w->bmi.masks[1] = 0x0000FF00; // Green
    w->bmi.masks[2] = 0x000000FF; // Blue
    w->bmi.masks[3] = 0xFF000000; // Alpha

    //HICON hIconSmall = LoadIcon(
    //    GetModuleHandle(NULL),
    //    MAKEINTRESOURCE(IDI_EXCLAMATION)
    //);
    //SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);
    
    //SetWindowPos(
    //    w->hwnd, NULL, 0,0,//w->x, w->y,
    //    w->Width * w->PixelWidth,
    //    w->Height * w->PixelHeight,
    //    SWP_NOMOVE | SWP_NOZORDER
    //);

    w->hdc = GetDC(w->hwnd);
    ShowWindow(w->hwnd,SW_SHOW);
    UpdateWindow(w->hwnd);
    
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
    SetWindowLongPtr(w->hwnd,GWLP_USERDATA,(LONG_PTR)w);

    w->Focus = 1;
    w->font = AlxFont_New(ALXFONT_YANIS);
    AlxFont_Resize(&w->font,40,40);
    
    if(w->setup) w->setup(w);
    
    w->LastTime = Time_Nano();
}
void AlxWindow_GameUpdate(AlxWindow* w){
    w->ElapsedTime = (double)(Time_Nano() - w->LastTime) / 1E9;
    w->LastTime = Time_Nano();
    double Fps = 1.0;
    
    if(w->ElapsedTime != 0.0)
        Fps /= w->ElapsedTime;

    w->MouseBeforeX = w->MouseX;
    w->MouseBeforeY = w->MouseY;
    AlxWindow_UpdateKB(w);

    char Buffer[100];
    sprintf(Buffer,"Alx - %s - %4.1f",w->Name,Fps);
    SetWindowText(w->hwnd,Buffer);

	if (w->Strokes[ALX_KEY_F11].PRESSED)
	    AlxWindow_SetFullscreen(w);
}
void AlxWindow_GameLoop(AlxWindow* w,void (*Render)(AlxWindow*)){
    timeBeginPeriod(1);
    while (w->Running) {
        AlxWindow_GameUpdate(w);
        
        if(w->update) w->update(w);
        
        Render(w);
        
        if(!w->Focus)   Thread_Sleep_M(40);
        else            Thread_Sleep_M(2);
    }
    timeEndPeriod(1);
    // needed because windows has no "good" sleep function!!!
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
#error "Platform is not Windows!"
#endif

#endif//ALXWINDOW_WINDOWS_H