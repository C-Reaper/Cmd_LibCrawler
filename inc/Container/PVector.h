#ifndef PVECTOR_H
#define PVECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PVECTOR_STARTSIZE    5

typedef struct PVector {
    int size;
    int SIZE;
    void** Memory;
} PVector;

PVector PVector_New() {
    PVector v;
    v.size = 0;
    v.SIZE = PVECTOR_STARTSIZE;
    v.Memory = calloc(v.SIZE,sizeof(void*));
    return v;
}
PVector PVector_Make(size_t SIZE) {
    PVector v;
    v.size = 0;
    v.SIZE = SIZE;
    v.Memory = calloc(v.SIZE,sizeof(void*));
    return v;
}
PVector PVector_Cpy(PVector* v){
    PVector out;
    out.size = v->size;
    out.SIZE = v->SIZE;
    out.Memory = calloc(v->SIZE,sizeof(void*));
    memcpy(out.Memory,v->Memory,v->size * sizeof(void*));
    return out;
}
PVector PVector_Null(){
    PVector out;
    out.size = -1;
    out.SIZE = -1;
    out.Memory = NULL;
    return out;
}
void* PVector_Get(PVector* v, int Index) {
    if (Index >= 0 && Index < v->size) {
        void* Dst = (char*)v->Memory + Index * sizeof(void*);
        return *(void**)Dst;
    }
    return NULL;
}
void PVector_Set(PVector* v,void* Item,int size,unsigned int Index) {
    if(Index >= 0 && Index < v->size){
        void* Dst = (char*)v->Memory + Index * sizeof(void*);
        void* Ptr = *(void**)Dst;
        if(Ptr) free(Ptr);
        *(void**)Dst = malloc(size);
        if(Item)    memcpy(*(void**)Dst,Item,size);
        else        *(void**)Dst = NULL;
    }else{
        printf("[PVector]: Set: can't At Index: %i\n", Index);
    }
}
void PVector_PSet(PVector* v,void* pItem,unsigned int Index) {
    if(Index >= 0 && Index < v->size){
        void* Dst = (char*)v->Memory + Index * sizeof(void*);
        void* Ptr = *(void**)Dst;
        if(Ptr) free(Ptr);
        *(void**)Dst = pItem;
    }else{
        printf("Can't Set At Index: %i\n", Index);
    }
}
void PVector_Expand(PVector* v) {
    if (v->size >= v->SIZE) {
        int NewSize = v->SIZE * 2;
        void** NewMemory = (void**)calloc(NewSize,sizeof(void*));
        memcpy(NewMemory,v->Memory,v->size * sizeof(void*));
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void PVector_Compress(PVector* v) {
    int NewSize = v->SIZE / 2;
    if (v->size <= NewSize && NewSize>=PVECTOR_STARTSIZE) {
        void** NewMemory = (void**)calloc(NewSize,sizeof(void*));
        memcpy(NewMemory,v->Memory,NewSize * sizeof(void*));
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void PVector_ExpandTo(PVector* v,size_t ExpandSize) {
    if (ExpandSize >= v->size){
        int NewSize = ExpandSize;
        void** NewMemory = (void**)calloc(NewSize,sizeof(void*));
        memcpy(NewMemory, v->Memory, v->size * sizeof(void*));
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }else{
        printf("[PVector]: ExpandTo: %llu from %d\n",(long long unsigned int)ExpandSize,v->size);
    }
}
void PVector_ExpandBy(PVector* v,size_t ExpandSize) {
    int TargetSize = v->SIZE + ExpandSize;
    while(v->SIZE < TargetSize) v->SIZE *= 2;

    void** NewMemory = (void**)calloc(v->SIZE,sizeof(void*));
    memcpy(NewMemory,v->Memory,v->size * sizeof(void*));
    if (v->Memory) free(v->Memory);
    v->Memory = NewMemory;
}

void PVector_Push(PVector* v,void* Item,int size) {
    PVector_Expand(v);
    if (v->size < v->SIZE)  PVector_Set(v,Item,size,v->size++);
    else                    printf("[PVector]: Not able to Push: s:%d [S:%d]\n",v->size,v->SIZE);
}
void PVector_Add(PVector* v,void* Item,int size,unsigned int Index) {
    PVector_Expand(v);
    if (v->size < v->SIZE && Index >= 0 && Index < v->size) {
        void* Src = ((char*)v->Memory + Index * sizeof(void*));
        void* Dst = ((char*)v->Memory + (Index + 1) * sizeof(void*));
        memmove(Dst,Src,(size_t)((v->size - Index) * sizeof(void*)));
        PVector_Set(v,Item,size,Index);
        v->size++;
    }else if(v->size < v->SIZE && Index == v->size){
        v->size++;
        PVector_Set(v,Item,size,Index);
    }else {
        printf("[PVector]: Not able to Add\n");
    }
}
void PVector_PopTop(PVector* v) {
    if(v->size > 0){
        v->size--;
        void* Dst = ((char*)v->Memory + v->size * sizeof(void*));
        void* Ptr = *(void**)Dst;
        if(Ptr) free(Ptr);
        *(void**)Dst = NULL;
        PVector_Compress(v);
    }else {
        printf("[PVector]: Not able to PopTop: %d\n",v->size);
    }
}
void PVector_Remove(PVector* v,unsigned int Index) {
    if(Index >= 0 && Index < v->size-1){
        v->size--;
        void* Src = (char*)v->Memory + (Index+1) * sizeof(void*);
        void* Dst = (char*)v->Memory + Index * sizeof(void*);
        void* Ptr = *(void**)Dst;
        if(Ptr) free(Ptr);
        
        memmove(Dst,Src,(size_t)((v->size - Index) * sizeof(void*)));
        
        void* End = (char*)v->Memory + v->size * sizeof(void*);
        *(void**)End = NULL;
        PVector_Compress(v);
    }else if(Index >= 0 && Index == v->size-1){
        PVector_PopTop(v);
    }else {
        printf("[PVector]: Not able to Remove At Index: %i\n",Index);
    }
}

void PVector_PPush(PVector* v,void* pItem) {
    PVector_Expand(v);
    if (v->size < v->SIZE)  PVector_PSet(v,pItem,v->size++);
    else                    printf("[PVector]: Not able to PPush: s:%d [S:%d]\n",v->size,v->SIZE);
}
void PVector_PAdd(PVector* v,void* pItem,unsigned int Index) {
    PVector_Expand(v);
    if (v->size < v->SIZE && Index >= 0 && Index < v->size) {
        void* Src = ((char*)v->Memory + Index * sizeof(void*));
        void* Dst = ((char*)v->Memory + (Index + 1) * sizeof(void*));
        memmove(Dst,Src,(size_t)((v->size - Index) * sizeof(void*)));
        PVector_PSet(v,pItem,Index);
        v->size++;
    }else if(v->size < v->SIZE && Index == v->size){
        v->size++;
        PVector_PSet(v,pItem,Index);
    }else {
        printf("[PVector]: Not able to PAdd\n");
    }
}
void PVector_PPopTop(PVector* v) {
    if(v->size > 0){
        v->size--;
        void* Dst = ((char*)v->Memory + v->size * sizeof(void*));
        *(void**)Dst = NULL;
        PVector_Compress(v);
    }else {
        printf("[PVector]: Not able to PPopTop: %d\n",v->size);
    }
}
void PVector_PRemove(PVector* v,unsigned int Index) {
    if(Index >= 0 && Index < v->size-1){
        v->size--;
        void* Src = (char*)v->Memory + (Index+1) * sizeof(void*);
        void* Dst = (char*)v->Memory + Index * sizeof(void*);
        
        memmove(Dst,Src,(size_t)((v->size - Index) * sizeof(void*)));
        void* End = (char*)v->Memory + v->size * sizeof(void*);
        *(void**)End = NULL;
        PVector_Compress(v);
    }else if(Index >= 0 && Index == v->size-1){
        PVector_PPopTop(v);
    }else {
        printf("[PVector]: Not able to PRemove At Index: %i\n",Index);
    }
}

void PVector_Clear(PVector* v) {
    if(v->size==0) return;
    if(v->SIZE<=10){
        v->size = 0;
        return;
    }

    for(int i = 0;i<v->size;i++){
        void* Ptr = PVector_Get(v,i);
        if(Ptr) free(Ptr);
    }
    if(v->Memory) free(v->Memory);
    v->size = 0;
    v->Memory = calloc(PVECTOR_STARTSIZE,sizeof(void*));
    v->SIZE = PVECTOR_STARTSIZE;
}
void PVector_Free(PVector* v) {
    for(int i = 0;i<v->size;i++){
        void* Ptr = PVector_Get(v,i);
        if(Ptr) free(Ptr);
    }
    if(v->Memory) free(v->Memory);
    v->Memory = NULL;
    v->size = 0;
    v->SIZE = 0;
}
void PVector_Print(PVector* v) {
    printf("--- PVector ---\n");
    printf("S:%i\n", (int)v->SIZE);
    printf("ES:%i\n", (int)sizeof(void*));
    for (int i = 0; i < v->size; i++) {
        printf("Element %i: ", i);
        for (int j = 0; j < sizeof(void*); j++) {
            printf("%c ", ((char*)v->Memory)[i * sizeof(void*) + j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}
void PVector_Print_A(PVector* v) {
    printf("--- PVector ---\n");
    printf("S:%d, s:%d\n",(int)v->SIZE,(int)v->size);
    printf("ES:%d\n", (int)sizeof(void*));
    if(v->Memory){
        for (int i = 0;i<v->SIZE;i++) {
            void* Dst = (char*)v->Memory + i * sizeof(void*);
            printf("A:%p,",Dst);
            if(Dst) printf("P:%p\n",*(void**)Dst);
        }
    }else{
        printf("Emty Block\n");
    }
    printf("--------------\n");
}
#define PVECTOR_END  PVector_Null()

#endif