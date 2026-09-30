#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Library/String.h"


#define DATASTREAM_STARTSIZE    20

typedef struct DataStream {
    int size;
    int SIZE;
    void* Memory;
} DataStream;

DataStream DataStream_New() {
    DataStream v;
    v.size = 0;
    v.SIZE = DATASTREAM_STARTSIZE;
    v.Memory = malloc(v.SIZE);
    return v;
}
DataStream DataStream_Make(int SIZE) {
    DataStream v;
    v.size = 0;
    v.SIZE = SIZE;
    v.Memory = malloc(v.SIZE);
    return v;
}
DataStream DataStream_By(char* data,int size) {
    DataStream v;
    v.size = size;
    v.SIZE = size;
    v.Memory = data;
    return v;
}
DataStream DataStream_Cpy(DataStream* v){
    DataStream out;
    out.size = v->size;
    out.SIZE = v->SIZE;
    out.Memory = malloc(v->SIZE);
    memcpy(out.Memory,v->Memory,v->size);
    return out;
}
DataStream DataStream_Null(){
    DataStream out;
    out.size = -1;
    out.SIZE = -1;
    out.Memory = NULL;
    return out;
}
void* DataStream_Get(DataStream* v,int index) {
    return (char*)v->Memory + index;
}

void DataStream_Expand(DataStream* v) {
    if (v->size >= v->SIZE) {
        int NewSize = v->SIZE * 2;
        char* NewMemory = (char*)malloc(NewSize);
        memcpy(NewMemory,v->Memory,v->size);
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void DataStream_Compress(DataStream* v) {
    if (v->size <= (v->SIZE / 2)) {
        int NewSize = v->SIZE / 2;
        NewSize = NewSize<DATASTREAM_STARTSIZE ? DATASTREAM_STARTSIZE:NewSize;
        char* NewMemory = (char*)malloc(NewSize);
        memcpy(NewMemory,v->Memory,NewSize);
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void DataStream_ExpandBy(DataStream* v,int ExpandSize) {
    v->SIZE += ExpandSize;
    char* NewMemory = (char*)malloc(v->SIZE);
    memcpy(NewMemory,v->Memory,v->size);
    if (v->Memory) free(v->Memory);
    v->Memory = NewMemory;
}
void DataStream_ResizeTo(DataStream* v,int ExpandSize) {
    if(ExpandSize > 0){
        int NewSize = ExpandSize;
        char* NewMemory = (char*)malloc(NewSize);
        memcpy(NewMemory,v->Memory,(v->size < ExpandSize ? v->size : ExpandSize));
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }else if(ExpandSize < 0){
        printf("[DataStream]: ResizeTo -> not able to resize to %d from %d!\n",ExpandSize,v->SIZE);
    }
}
void DataStream_Move(DataStream* v,int Index,int Count) {
    if (Index + Count >= 0 && Index + Count < v->size) {
        void* Src = ((char*)v->Memory + Index);
        void* Dst = ((char*)v->Memory + (Index + Count));
        memmove(Dst,Src,v->size - Index);

        DataStream_ResizeTo(v,v->size + Count);
    }else {
        printf("[DataStream]: Move -> not able to move %d with count %d!\n",Index,Count);
    }
}

void DataStream_PushCount(DataStream* v,void* Items,int Count) {
    DataStream_ExpandBy(v,Count);
    if (v->size < v->SIZE){
        memcpy((char*)v->Memory + v->size,Items,Count);
        v->size += Count;
    }
    else printf("[DataStream]: PushCount -> Not able to!\n");
}
void DataStream_AddCount(DataStream* v, void* Items,int Count,int Index) {
    DataStream_ExpandBy(v,Count);
    if (v->size < v->SIZE){
        DataStream_Move(v,Index,Count);
        memcpy(v->Memory + Index, Items, Count);
        v->size += Count;
    }
    else printf("[DataStream]: AddCount -> Not able to!\n");
}
void DataStream_PopTopCount(DataStream* v,int Count) {
    if (v->size - Count >= 0) {
        v->size -= Count;
        DataStream_Compress(v);
    }else {
        printf("[DataStream]: Not able to PopTopCount\n");
    }
}
void DataStream_RemoveCount(DataStream* v,int Index,int Count) {
    if (Index >= 0 && Index + Count <= v->size) {
        DataStream_Move(v,Index + Count,-Count);
        v->size -= Count;
    }else {
        printf("[DataStream]: RemoveC -> Not able to at Index: %d (C:%d)!\n",Index,Count);
    }
}
void DataStream_ReadCount(DataStream* v,void* target,int Index,int Count) {
    if (Index >= 0 && Index + Count <= v->size) {
        char* data = (char*)v->Memory + Index;
        memcpy(target,data,Count);
        DataStream_RemoveCount(v,Index,Count);
    }else {
        printf("[DataStream]: RemoveC -> Not able to at Index: %d (C:%d)!\n",Index,Count);
    }
}

void DataStream_Printf(DataStream* v,char* format,...) {
    va_list args;
    va_start(args,format);
    String app = String_FormatA(format,args);
    va_end(args);

    DataStream_PushCount(v,app.Memory,app.size);
    String_Free(&app);
}
void DataStream_Print_N(DataStream* v,char* ncstr) {
    int lastcut = 0;
    const int len = CStr_Size(ncstr);
    
    for(int i = 0;i<len;i++){
        if(ncstr[i] == '\n'){
            if(i > lastcut)
                DataStream_PushCount(v,ncstr + lastcut,i - lastcut);

            DataStream_PushCount(v,"\\n",sizeof(char) * 2);
            lastcut = i + 1;
        }else if(i + 1 == len){
            DataStream_PushCount(v,ncstr + lastcut,len - lastcut);
        }
    }
}


void DataStream_Clear(DataStream* v) {
    if(v->size==0) return;
    if(v->SIZE<=10){
        v->size = 0;
        return;
    }
    if(v->Memory) free(v->Memory);
    v->size = 0;
    v->Memory = malloc(DATASTREAM_STARTSIZE);
    v->SIZE = DATASTREAM_STARTSIZE;
}
void DataStream_Free(DataStream* v) {
    if (v->Memory) free(v->Memory);
    v->Memory = NULL;
    v->size = 0U;
}
void DataStream_Print(DataStream* v) {
    printf("--- DataStream ---\n");
    printf("SIZE: %d\n", (int)v->SIZE);
    printf("Size: %d\n", (int)v->size);
    printf("------------------\n");
}
#define DATASTREAM_END  DataStream_Null()

#endif