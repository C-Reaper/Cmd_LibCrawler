#ifndef ALXTIME_H
#define ALXTIME_H

#include <stdio.h>

#if defined __linux__
#include <time.h>
#elif defined _WIN64
#include <windows.h>
#elif defined __EMSCRIPTEN__
#include <time.h>
#include <emscripten/emscripten.h>
#else
#error "Plattform not supported!"
#endif

#define TIME_MILLI_SECONDS   1000ULL
#define TIME_MICRO_SECONDS   1000000ULL
#define TIME_NANO_SECONDS    1000000000ULL

#define TIME_NANOTOSEC  1000000000ULL
#define TIME_FNANOTOSEC 1.0E9

typedef struct TimeStamp {
    unsigned short Nano;
    unsigned short Micro;
    unsigned short Mill;
    unsigned short Sec;
    unsigned short Min;
    unsigned short Hour;
    unsigned short Day;
    unsigned short Month;
    unsigned short Year;
} TimeStamp;

typedef unsigned long long Timepoint;
typedef unsigned long long Duration;
typedef double FDuration;

Timepoint Time_Nano(){
	#ifdef __linux__
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC,&time);
    long seconds = time.tv_sec;
    long nanoseconds = time.tv_nsec;
    return (Timepoint)seconds * TIME_NANOTOSEC + ((Timepoint)nanoseconds);
    #endif
    #ifdef _WIN64
    LARGE_INTEGER freq,time;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time);
    return (Timepoint)time.QuadPart * TIME_NANOTOSEC / (Timepoint)freq.QuadPart;
    #endif
    #ifdef __EMSCRIPTEN__
    double ms = emscripten_get_now();
    return (Timepoint)(ms * 1000000.0);
    #endif
}
Timepoint Time_Micro(){
	return (Timepoint)(Time_Nano() / 1000ULL);
}
Timepoint Time_Mill(){
	return (Timepoint)(Time_Nano() / 1000000ULL);
}
Timepoint Time_Sec(){
	return (Timepoint)(Time_Nano() / 1000000000ULL);
}
Timepoint Time_Min(){
	return (Timepoint)(Time_Sec() / 60ULL);
}
Timepoint Time_Hour(){
	return (Timepoint)(Time_Sec() / (60ULL * 60ULL));
}
Timepoint Time_Day(){
	return (Timepoint)(Time_Sec() / (60ULL * 60ULL * 24ULL));
}
Timepoint Time_Month(){
	return (Timepoint)(Time_Sec() / (60ULL * 60ULL * 24ULL * 30ULL));
}
Timepoint Time_Year(){
	return (Timepoint)(Time_Sec() * 4ULL / (60ULL * 60ULL * 24ULL * 365 * 5ULL));
}

Timepoint Time_Real_Nano(){
	#ifdef __linux__
    struct timespec time;
    clock_gettime(CLOCK_REALTIME,&time);
    long seconds = time.tv_sec;
    long nanoseconds = time.tv_nsec;
    return (Timepoint)seconds * TIME_NANOTOSEC + ((Timepoint)nanoseconds);
    #endif
    #ifdef _WIN64
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);
    ULARGE_INTEGER time;
    time.LowPart = ft.dwLowDateTime;
    time.HighPart = ft.dwHighDateTime;
    return (Timepoint)(time.QuadPart * 100);
    #endif
    #ifdef __EMSCRIPTEN__
    double ms = emscripten_get_now();
    return (Timepoint)(ms * 1000000.0);
    #endif
}
Timepoint Time_Real_Micro(){
	return (Timepoint)(Time_Real_Nano() / 1000ULL);
}
Timepoint Time_Real_Mill(){
	return (Timepoint)(Time_Real_Nano() / 1000000ULL);
}
Timepoint Time_Real_Sec(){
	return (Timepoint)(Time_Real_Nano() / 1000000000ULL);
}
Timepoint Time_Real_Min(){
	return (Timepoint)(Time_Real_Sec() / 60ULL);
}
Timepoint Time_Real_Hour(){
	return (Timepoint)(Time_Real_Sec() / (60ULL * 60ULL));
}
Timepoint Time_Real_Day(){
	return (Timepoint)(Time_Real_Sec() / (60ULL * 60ULL * 24ULL));
}
Timepoint Time_Real_Month(){
	return (Timepoint)(Time_Real_Sec() / (60ULL * 60ULL * 24ULL * 30ULL));
}
Timepoint Time_Real_Year(){
	return (Timepoint)(Time_Real_Sec() * 4ULL / (60ULL * 60ULL * 24ULL * 365 * 5ULL));
}

double Time_DSec(){
    return (double)Time_Nano() / TIME_FNANOTOSEC;
}
float Time_FSec(){
	return (float)Time_Nano() / TIME_FNANOTOSEC;
}

double Time_Real_DSec(){
    return (double)Time_Real_Nano() / TIME_FNANOTOSEC;
}
float Time_Real_FSec(){
	return (float)Time_Real_Nano() / TIME_FNANOTOSEC;
}

Duration Time_ElapsedD(Timepoint Start,Timepoint End){
    return (Duration)(End - Start);
}
FDuration Time_Elapsed(Timepoint Start,Timepoint End){
    return (FDuration)(End - Start) / TIME_FNANOTOSEC;
}

FDuration Time_Real_Elapsed(Timepoint Start,Timepoint End){
    return (FDuration)(End-Start) / TIME_FNANOTOSEC;
}
Timepoint Time_Real_SecToNano(double Secs){
	return (Timepoint)(Secs * (double)1.0E9);
}
double Time_Real_NanoToSec(Timepoint Nanos){
	return (double)Nanos / (double)1.0E9;
}

unsigned char MONTH_DAYS[] = {
    31U,
    28U,
    31U,
    30U,
    31U,
    30U,
    31U,
    31U,
    30U,
    31U,
    30U,
    31U,
};

char Time_IsLeap(unsigned int year) {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}
TimeStamp Time_Get(Timepoint Nano){
    TimeStamp t;
    t.Nano = Nano - (Nano / 1000) * 1000;
    Nano /= 1000;
    t.Micro = Nano - (Nano / 1000) * 1000;
    Nano /= 1000;
    t.Mill = Nano - (Nano / 1000) * 1000;
    Nano /= 1000;
    t.Sec = Nano - (Nano / 60) * 60;
    Nano /= 60;
    t.Min = Nano - (Nano / 60) * 60;
    Nano /= 60;
    t.Hour = Nano - (Nano / 24) * 24;
    Nano /= 24;
    t.Day = Nano;
    t.Year = 1970;
    
    char leap = Time_IsLeap(t.Year);
    while((t.Day >= 365U + leap)){
        t.Day -= 365U + leap;
        t.Year++;
        leap = Time_IsLeap(t.Year);
    }
    for(int i = 0;i<12;i++){
        unsigned char days = MONTH_DAYS[i];
        if(i==1 && leap) days++;

        if(t.Day < days) break;

        t.Day -= days;
        t.Month++;
    }

    t.Hour++;
    t.Month++;
    t.Day++;
    
    t.Hour++;
    
    if(t.Hour >= 24){
        t.Hour -= 24;
        t.Day++;
    }
    return t;
}
void Time_Str(char* Buffer,Timepoint Nano){
    TimeStamp t = Time_Get(Nano);
    sprintf(Buffer,"%2d.%2d.%2d [%2d:%2d:%2d]",t.Day,t.Month,t.Year,t.Hour,t.Min,t.Sec);
}
void Time_Sleep(double d){
    #ifdef __EMSCRIPTEN__
    printf("[Time]: Sleep -> Busy wait in Emscripten (not ideal)\n");
    #endif
    Timepoint Start = Time_Nano();
    while(Time_Elapsed(Start,Time_Nano())<d){}
}

#endif