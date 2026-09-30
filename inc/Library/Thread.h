#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__linux__)
    #include <pthread.h>
    #include <time.h>
    #include <signal.h>
    #include <unistd.h>
#elif defined(_WIN32)
    #include <windows.h>
    #include <synchapi.h>
    #include <processthreadsapi.h>
    #include <mmsystem.h> // timeBeginPeriod/timeEndPeriod if needed
#elif defined(__EMSCRIPTEN__)
    #include <emscripten.h>
#else
    #error "Unsupported platform!"
#endif

typedef unsigned long long Duration;

#define MILLI_SECONDS   1000ULL
#define MICRO_SECONDS   1000000ULL
#define NANO_SECONDS    1000000000ULL

typedef struct Thread {
#if defined(__linux__)
    pthread_t h;
    void* attr;
#elif defined(_WIN32)
    HANDLE h;
    DWORD id;
#elif defined(__EMSCRIPTEN__)
    int dummy;
#endif
    void* (*func)(void*);
    void* arg;
    void* ret;
    char running;
} Thread;

#if defined(__linux__)

Thread Thread_New(void* attr,void* (*func)(void*),void* arg){
    Thread t;
    t.h = (pthread_t)0UL;
    t.attr = attr;
    t.func = func;
    t.arg = arg;
    t.ret = NULL;
    t.running = 0;
    return t;
}
Thread Thread_Null(){
    Thread t;
    t.h = (pthread_t)0UL;
    t.attr = NULL;
    t.func = NULL;
    t.arg = NULL;
    t.ret = NULL;
    t.running = 0;
    return t;
}
void* Thread_Starter(Thread* t){
    t->running = 1;
    
    if(t->func)
        t->ret = t->func(t->arg);
    else
        t->ret = NULL;

    t->running = 0;
    return t->ret;
}
void Thread_Start(Thread* t){
    if (pthread_create(&t->h,t->attr,(void*(*)(void*))Thread_Starter,t) != 0) {
        perror("[Threads]: Start: Error");
        exit(1);
    }
}
void Thread_Join(Thread* t){
    if(t->h){
        pthread_join(t->h,NULL);
        t->h = 0;
    }
}
void Thread_Detach(Thread* t){
    if(t->h) pthread_detach(t->h);
}
void Thread_Cancel(Thread* t){
    if(t->h){
        pthread_cancel(t->h);
        t->h = 0;
    }
}
void Thread_Term(Thread* t){
    if(t->h){
        pthread_kill(t->h,SIGTERM);
        t->h = 0;
    }
}
void Thread_Int(Thread* t){
    if(t->h) pthread_kill(t->h,SIGINT);
}
void Thread_Restart(Thread* t){
    if(t->running){
        if(t->h) Thread_Term(t);
        Thread_Start(t);
    }else{
        Thread_Start(t);
    }
}
void Thread_ChangeRestart(Thread* t,void* (*func)(void*)){
    t->func = func;
    if(t->running){
        if(t->h) Thread_Cancel(t);
        if(t->h) Thread_Join(t);
        Thread_Start(t);
    }else{
        Thread_Start(t);
    }
}
void Thread_Stop(Thread* t){
    t->running = 0;
    Thread_Cancel(t);
    Thread_Join(t);
}
void Thread_Free(Thread* t){
    Thread_Stop(t);
}

void Thread_Sleep_N(Duration nsecs){
    struct timespec req = { nsecs / NANO_SECONDS,nsecs % NANO_SECONDS };
    nanosleep(&req,NULL);
}
void Thread_Sleep_U(Duration usecs){
    Thread_Sleep_N(usecs * 1000ULL);
}
void Thread_Sleep_M(Duration msecs){
    Thread_Sleep_N(msecs * 1000000ULL);
}
void Thread_Sleep_S(Duration secs){
    Thread_Sleep_N(secs * 1000000000ULL);
}

#elif defined(_WINE) || defined(_WIN32) 

Thread Thread_New(void* attr,void* (*func)(void*),void* arg){
    Thread t;
    t.h = NULL;
    t.id = 0;
    t.func = func;
    t.arg = arg;
    t.running = 0;
    return t;
}
Thread Thread_Null(){
    return Thread_New(NULL,NULL,NULL);
}

DWORD WINAPI Thread_RunWrapper(LPVOID arg) {
    Thread* t = (Thread*)arg;
    t->func(t->arg);
    t->running = 0;
    return 0;
}
void Thread_Start(Thread* t){
    t->running = 1;
    t->h = CreateThread(NULL,0,Thread_RunWrapper,t,0,&t->id);
    if(!t->h){
        fprintf(stderr, "[Thread]: Start -> Error creating thread!\n");
        exit(1);
    }
}

void Thread_Join(Thread* t) {
    if(t && t->h){
        WaitForSingleObject(t->h, INFINITE);
        CloseHandle(t->h);
        t->h = NULL;
    }
}
void Thread_Detach(Thread* t) {
    if(t && t->h){
        CloseHandle(t->h);
        t->h = NULL;
    }
}
void Thread_Cancel(Thread* t) {
    if(t && t->h){
        TerminateThread(t->h, 0);
        CloseHandle(t->h);
        t->h = NULL;
        t->running = 0;
    }
}
void Thread_Term(Thread* t) {
    if(t && t->h){
        TerminateThread(t->h, 1);
        CloseHandle(t->h);
        t->h = NULL;
        t->running = 0;
    }
}
void Thread_Int(Thread* t){
    
}
void Thread_Restart(Thread* t){
    if(t->running){
        if(t->h) Thread_Term(t);
        Thread_Start(t);
    }else{
        Thread_Start(t);
    }
}
void Thread_ChangeRestart(Thread* t,void* (*func)(void*)){
    t->func = func;
    if(t->running){
        if(t->h) Thread_Cancel(t);
        if(t->h) Thread_Join(t);
        Thread_Start(t);
    }else{
        Thread_Start(t);
    }
}
void Thread_Stop(Thread* t){
    t->running = 0;
    if(t->h) Thread_Cancel(t);
    if(t->h) Thread_Join(t);
}
void Thread_Free(Thread* t){
    Thread_Stop(t);
}

void Thread_Sleep_N(Duration nsecs){
    if(nsecs == 0) return;

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!timer) return;

    LARGE_INTEGER li;
    // Convert nanoseconds to 100-ns units (1 unit = 100 ns)
    // li.QuadPart must be negative (relative time)
    // avoid overflow: nsecs / 100 fits in 64-bit for reasonable values
    li.QuadPart = -(LONGLONG)(nsecs / 100ULL);
    if(li.QuadPart == 0) li.QuadPart = -1; // at least 1 100-ns unit

    if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)){
        CloseHandle(timer);
        return;
    }
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
void Thread_Sleep_U(Duration usecs){
    Thread_Sleep_N(usecs * 1000ULL);
}
void Thread_Sleep_M(Duration msecs){
    Sleep(msecs);
}
void Thread_Sleep_S(Duration secs){
    Sleep(secs * 1000ULL);
}

#elif defined(__EMSCRIPTEN__)

Thread Thread_New(void* attr,void* (*func)(void*),void* arg){
    Thread t;
    t.func = func;
    t.arg = arg;
    t.ret = NULL;
    t.running = 0;
    return t;
}

Thread Thread_Null(){
    Thread t;
    t.func = NULL;
    t.arg = NULL;
    t.ret = NULL;
    t.running = 0;
    return t;
}

void Thread_Start(Thread* t){
    if(!t->func){
        printf("[Thread]: No function provided (Emscripten stub)\n");
        return;
    }

    t->running = 1;

    // No real OS threads in normal WASM -> simulate
    printf("[Thread]: Running synchronously (Emscripten stub)\n");

    t->ret = t->func(t->arg);

    t->running = 0;
}

void Thread_Join(Thread* t){
    printf("[Thread]: Join not supported in Emscripten (sync mode)\n");
}

void Thread_Detach(Thread* t){
    printf("[Thread]: Detach not supported in Emscripten\n");
}

void Thread_Cancel(Thread* t){
    t->running = 0;
    printf("[Thread]: Cancel simulated in Emscripten\n");
}

void Thread_Term(Thread* t){
    t->running = 0;
    printf("[Thread]: Term simulated in Emscripten\n");
}

void Thread_Int(Thread* t){
    printf("[Thread]: Interrupt not supported in Emscripten\n");
}

void Thread_Restart(Thread* t){
    Thread_Start(t);
}

void Thread_ChangeRestart(Thread* t,void* (*func)(void*)){
    t->func = func;
    Thread_Start(t);
}

void Thread_Stop(Thread* t){
    t->running = 0;
}

void Thread_Free(Thread* t){
    Thread_Stop(t);
}

/* Sleep via busy wait */
void Thread_Sleep_N(Duration nsecs){
    double start = emscripten_get_now();
    double target = start + (double)nsecs / 1000000.0;

    while(emscripten_get_now() < target){}
}
void Thread_Sleep_U(Duration usecs){
    Thread_Sleep_N(usecs * 1000ULL);
}
void Thread_Sleep_M(Duration msecs){
    double start = emscripten_get_now();
    double target = start + (double)msecs;

    while(emscripten_get_now() < target){}
}
void Thread_Sleep_S(Duration secs){
    Thread_Sleep_M(secs * 1000ULL);
}

#elif defined(__APPLE__)
    #error "Apple not supported!"
#else
    #error "Unsupported platform!"
#endif

#endif //!THREAD_H‹fS`