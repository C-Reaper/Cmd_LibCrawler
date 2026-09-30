#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_STARTSIZE    5

typedef struct Vector {
    int size;
    int SIZE;
    int ELEMENT_SIZE;
    int PADDING;
    void* Memory;
} Vector;

Vector Vector_New(int ELEMENT_SIZE) {
    Vector v;
    v.size = 0;
    v.ELEMENT_SIZE = ELEMENT_SIZE;
    v.SIZE = VECTOR_STARTSIZE;
    v.Memory = (void*)malloc(v.SIZE * v.ELEMENT_SIZE);
    return v;
}
Vector Vector_Make(int ELEMENT_SIZE,int SIZE) {
    Vector v;
    v.size = 0;
    v.ELEMENT_SIZE = ELEMENT_SIZE;
    v.SIZE = SIZE;
    v.Memory = (void*)malloc(v.SIZE * v.ELEMENT_SIZE);
    return v;
}
Vector Vector_Cpy(Vector* v){
    Vector out;
    out.size = v->size;
    out.SIZE = v->SIZE;
    out.ELEMENT_SIZE = v->ELEMENT_SIZE;
    out.Memory = malloc(v->SIZE * v->ELEMENT_SIZE);
    memcpy(out.Memory,v->Memory,v->size * v->ELEMENT_SIZE);
    return out;
}
Vector Vector_MBuild(int ELEMENT_SIZE,int SIZE,void* Memory){
    Vector out;
    out.size = SIZE;
    out.SIZE = SIZE;
    out.ELEMENT_SIZE = ELEMENT_SIZE;
    out.Memory = Memory;
    return out;
}
Vector Vector_Null(){
    Vector out;
    out.size = -1;
    out.SIZE = -1;
    out.ELEMENT_SIZE = -1;
    out.Memory = NULL;
    return out;
}
int Vector_Size(Vector* v){
    return v->size;
}
void* Vector_Get(Vector* v, unsigned int Index) {
    if(!v->Memory) return NULL;
    if (Index < v->size) {
        return (char*)v->Memory + Index * v->ELEMENT_SIZE;
    }
    return NULL;
}
void Vector_Set(Vector* v, void* Item, unsigned int Index) {
    if (Index < v->size) {
        void* Dst = (char*)v->Memory + Index * v->ELEMENT_SIZE;
        memcpy(Dst,Item,v->ELEMENT_SIZE);
    }
    else {
        printf("[Vector]: Set -> Can't Set At Index: %u\n", Index);
    }
}
void* Vector_Find(Vector* v, void* Item, int (*Cmp)(const void*,const void*)) {
    for(int i = 0;i<v->size;i++){
        void* s = Vector_Get(v,i);
        if(Cmp(s,Item)) return s;
    }
    return NULL;
}
int Vector_Contains(Vector* v, void* Item, int (*Cmp)(const void*,const void*)) {
    return Vector_Find(v,Item,Cmp) != NULL;
}
void Vector_Expand(Vector* v) {
    if (v->size >= v->SIZE) {
        //printf("Expand Vector.\n");
        int NewSize = v->SIZE * 2;
        char* NewMemory = (char*)malloc(NewSize * v->ELEMENT_SIZE);
        memcpy(NewMemory, v->Memory, v->size * v->ELEMENT_SIZE);
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void Vector_Compress(Vector* v) {
    if (v->size <= (v->SIZE / 2)) {
        int NewSize = v->SIZE / 2;
        NewSize = NewSize<VECTOR_STARTSIZE ? VECTOR_STARTSIZE:NewSize;
        char* NewMemory = (char*)malloc(NewSize * v->ELEMENT_SIZE);
        memcpy(NewMemory,v->Memory,NewSize * v->ELEMENT_SIZE);
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }
}
void Vector_ExpandTo(Vector* v,int ExpandSize) {
    if (ExpandSize >= v->size){
        int NewSize = ExpandSize;
        char* NewMemory = (char*)malloc(NewSize * v->ELEMENT_SIZE);
        memcpy(NewMemory, v->Memory, v->size * v->ELEMENT_SIZE);
        if (v->Memory) free(v->Memory);
        v->Memory = NewMemory;
        v->SIZE = NewSize;
    }else{
        printf("[Vector]: ExpandTo -> Could't Expand Vector to: %d\n",ExpandSize);
    }
}
void Vector_ExpandBy(Vector* v,int ExpandSize) {
    int TargetSize = v->SIZE + ExpandSize;
    while(v->SIZE < TargetSize) v->SIZE *= 2;

    char* NewMemory = (char*)malloc(v->SIZE * v->ELEMENT_SIZE);
    memcpy(NewMemory,v->Memory,v->size * v->ELEMENT_SIZE);
    if (v->Memory) free(v->Memory);
    v->Memory = NewMemory;
}
void Vector_Move(Vector* v, unsigned int Index, int Count) {
    if(Index < v->size){
        if(Count>0){
            int targetSize = v->size + Count;
            int NewSize = v->SIZE;
            while(NewSize<targetSize) NewSize *= 2;

            char* NewMemory = (char*)malloc(NewSize * v->ELEMENT_SIZE);
            if(Index>0)         memcpy(NewMemory,v->Memory,Index);
            if(Index<v->size)   memcpy(NewMemory + Index + Count,v->Memory + Index,v->size - Index);

            free(v->Memory);
            v->Memory = NewMemory;
            v->SIZE = NewSize;
            v->size = v->size + Count;
        }else{
            int targetSize = v->size + Count;
            int NewSize = v->SIZE;
            while(NewSize>targetSize) NewSize /= 2;
            NewSize *= 2;

            char* NewMemory = (char*)malloc(NewSize * v->ELEMENT_SIZE);
            if(Index + Count>0) memcpy(NewMemory,v->Memory,Index + Count);
            if(Index<v->size)   memcpy(NewMemory + Index + Count,v->Memory + Index,v->size - Index);

            free(v->Memory);
            v->Memory = NewMemory;
            v->SIZE = NewSize;
            v->size = v->size + Count;
        }
    }else {
        printf("[Vector]: Move -> not able to move.\n");
    }
}
void Vector_Push(Vector* v, void* Item) {
    Vector_Expand(v);
    if (v->size < v->SIZE)  Vector_Set(v, Item, v->size++);
    else                    printf("[Vector]: not able to push\n");
}
void Vector_PushCount(Vector* v, void* Items, int Count) {
    Vector_ExpandBy(v,Count);
    if (v->size < v->SIZE){
        // memcpy((char*)v->Memory + v->size * v->ELEMENT_SIZE,Items,Count * v->ELEMENT_SIZE);
        // v->size += Count;

        for(int i = 0;i<Count;i++){
            Vector_Push(v,(char*)Items + i * v->ELEMENT_SIZE);
        }
    }
    else printf("[Vector]: not able to push count\n");
}
void Vector_Add(Vector* v, void* Item, unsigned int Index) {
    Vector_Expand(v);
    if (v->size < v->SIZE && Index < v->size) {
        void* Src = ((char*)v->Memory + Index * v->ELEMENT_SIZE);
        void* Dst = ((char*)v->Memory + (Index + 1) * v->ELEMENT_SIZE);
        memmove(Dst,Src,(int)((v->size - Index) * v->ELEMENT_SIZE));
        Vector_Set(v, Item, Index);
        v->size++;
    }else if(v->size < v->SIZE && Index == v->size){
        v->size++;
        Vector_Set(v, Item, Index);
    }else {
        printf("[Vector]: not able to add\n");
    }
}
void Vector_AddCount(Vector* v, void* Items, int Count, unsigned int Index) {
    Vector_ExpandBy(v,Count);
    if (v->size < v->SIZE){
        // Vector_Move(v,Index,Count);
        // memcpy((char*)v->Memory + Index * v->ELEMENT_SIZE, Items, Count * v->ELEMENT_SIZE);
        // v->size += Count;
        
        for(int i = 0;i<Count;i++){
            Vector_Add(v,(char*)Items + i * v->ELEMENT_SIZE,Index);
        }
    }
    else printf("[Vector]: not able to AddCount\n");
}
void Vector_PopTop(Vector* v) {
    if (v->size > 0) {
        v->size--;
        Vector_Compress(v);
    }else {
        printf("[Vector]: not able to PopTop\n");
    }
}
void Vector_PopTopCount(Vector* v,int Count) {
    if (v->size - Count >= 0) {
        v->size -= Count;
        Vector_Compress(v);
    }else {
        printf("[Vector]: not able to PopTopCount\n");
    }
}
void Vector_Remove(Vector* v, unsigned int Index) {
    if (Index < v->size-1 && v->size > 0) {
        v->size--;
        void* Src = &(((char*)v->Memory)[(Index + 1) * v->ELEMENT_SIZE]);
        void* Dst = &(((char*)v->Memory)[Index * v->ELEMENT_SIZE]);
        memmove(Dst, Src, (int)((v->size - Index) * v->ELEMENT_SIZE));
        Vector_Compress(v);
    }else if(Index == v->size-1){
        Vector_PopTop(v);
    }else {
        printf("[Vector]: not able to Remove At Index: %d\n", Index);
    }
}
void Vector_RemoveCount(Vector* v, unsigned int Index,int Count) {
    if (Index < v->size-1) {
        // if(Index + Count<v->size)
        //     Vector_Move(v,Index + Count,-Count);
        
        // v->size -= Count;

        for(int i = 0;i<Count;i++){
            Vector_Remove(v,Index);
        }
    }else if(Index == v->size-1){
        Vector_PopTop(v);
    }else {
        printf("[Vector]: not able to RemoveCount At Index: %d\n", Index);
    }
}
void Vector_Swap(Vector* v,unsigned int i1,unsigned int i2) {
    if(i1>=0 && i1<v->size && i2>=0 && i2<v->size){
        void* swap = malloc(v->ELEMENT_SIZE);
        memcpy(swap,Vector_Get(v,i1),v->ELEMENT_SIZE);
        Vector_Set(v,Vector_Get(v,i2),i1);
        Vector_Set(v,swap,i2);
        free(swap);
    }else {
        printf("[Vector]: not able to Swap: %d & %d\n",i1,i2);
    }
}
void Vector_Reset(Vector* v) {
    if(!v) return;
    if(v->Memory) free(v->Memory);
    v->Memory = malloc(v->SIZE * v->ELEMENT_SIZE);
}
void Vector_Clear(Vector* v) {
    if(!v) return;
    if(v->size==0) return;
    if(v->SIZE<=10){
        v->size = 0;
        return;
    }
    if(v->Memory) free(v->Memory);
    v->size = 0;
    v->Memory = malloc(VECTOR_STARTSIZE * v->ELEMENT_SIZE);
    v->SIZE = VECTOR_STARTSIZE;
}
void Vector_Free(Vector* v) {
    if(!v) return;
    if(v->Memory) free(v->Memory);
    v->Memory = NULL;
    v->ELEMENT_SIZE = (int)0ULL;
    v->size = 0U;
}
void Vector_Print(Vector* v) {
    if(!v) return;
    printf("--- Vector ---\n");
    printf("Size: %d\n", (int)v->size);
    printf("ESize: %d\n", (int)v->ELEMENT_SIZE);
    for (int i = 0; i < v->size; i++) {
        printf("Element %d: ", i);
        for (int j = 0; j < v->ELEMENT_SIZE; j++) {
            printf("%x", ((char*)v->Memory)[i * v->ELEMENT_SIZE + j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}
#define VECTOR_END  Vector_Null()

void Vector2D_Free(Vector* v) {
    for(int i = 0;i<v->size;i++){
        Vector* vec = (Vector*)Vector_Get(v,i);
        Vector_Free(vec);
    }
    Vector_Free(v);
}

void Vector_BubbleSortStep(Vector* v,int Start,int Size,int Step,int (*Cmp)(void*,void*)){
    for(int i = Start;i<Start+Size;i+=Step){
        for(int j = Start;j<Start+Size;j+=Step){
            void* Element1 = Vector_Get(v,i);
            void* Element2 = Vector_Get(v,j);
            if(Cmp(Element1,Element2)>0){
                Vector_Swap(v,i,j); //Vector_Swap(Element1,Element2,v->ELEMENT_SIZE);
            }
        }
    }
}
void Vector_BubbleSort(Vector* v,int (*Cmp)(void*,void*)){
    for(int i = 0;i<v->size;i++){
        int Swaps = 0;
        for(int j = 0;j<v->size;j++){
            void* Element1 = Vector_Get(v,i);
            void* Element2 = Vector_Get(v,j);
            if(Cmp(Element1,Element2)>0){
                Vector_Swap(v,i,j); //Vector_Swap(Element1,Element2,v->ELEMENT_SIZE);
                Swaps++;
            }
        }
        if(Swaps==0) return;
    }
}
int Vector_Count(Vector* v,int Start,int Size,int Step,void* Data,int (*Cmp)(void*,void*)){
    int Count = 0;
    for(int j = Start;j<Start+Size;j+=Step){
        void* Element = Vector_Get(v,j);
        if(Cmp(Data,Element)==0){
            Count++;
        }
    }
    return Count;
}

#endif