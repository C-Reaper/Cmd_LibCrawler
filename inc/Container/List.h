#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct Node {
    size_t ELEMENT_SIZE;
    void* Memory;
    struct Node* Before;
    struct Node* Next;
} Node;

Node* Node_New(size_t ELEMENT_SIZE,void* Item){
    Node* n = (Node*)malloc(sizeof(Node));
    n->ELEMENT_SIZE = ELEMENT_SIZE;
    n->Memory = malloc(ELEMENT_SIZE);
    memcpy(n->Memory, Item, ELEMENT_SIZE);
    return n;
}
void Node_Append(Node* n,Node* a) {
    a->Next = n->Next;
    n->Next->Before = a;
    n->Next = a;
    a->Before = n;
}
void Node_Delete(Node* n) {
    n->Before->Next = n->Next;
    n->Next->Before = n->Before;
}
void Node_Free(Node* n) {
    if(n){
        if(n->Memory)  free(n->Memory);
        if(n)          free(n);
    }
}


typedef struct List {
    int64_t size;
    Node* First;
} List;

List List_New() {
    List v;
    v.size = 0;
    v.First = NULL;
    //printf("init -> List.\n");
    return v;
}
int List_Size(List* v) {
    return v->size;
}
Node* List_FindNode(List* v, int Index) {
    if (Index >= 0 && Index < v->size) {
        Node* Previous = v->First;
        for(int i = 0;i<Index;i++)  Previous = Previous->Next;
        //if(Index<=v->size/2){
        //    for(int i = 0;i<Index;i++)  Previous = Previous->Next;
        //}else{
        //    for(int i = v->size;i>Index;i--)  Previous = Previous->Before;
        //}
        return Previous;
    }
    else {
        printf("Can't Get At Index: %i\n", Index);
    }
    return NULL;
}
unsigned int List_IndexOfNode(List* v, Node* n){
    Node* Previous = v->First;
    for(int i = 0;i<v->size;i++){
        if(Previous==n) return i;
        Previous = Previous->Next;
    }
    printf("Can't Get Node: %p\n",n);
    return 0;
}
void* List_Get(List* v, int Index) {
    if (Index >= 0 && Index < v->size) {
        Node* Previous = List_FindNode(v,Index);
        return Previous->Memory;
    }
    else {
        printf("Can't Get At Index: %i\n", Index);
    }
    return NULL;
}
void List_Set(List* v, size_t ELEMENT_SIZE, void* Item, int Index) {
    if (Index >= 0 && Index < v->size) {
        Node* Previous = List_FindNode(v,Index);
        Previous->ELEMENT_SIZE = ELEMENT_SIZE;
        void* Dst = Previous->Memory;
        memcpy(Dst, Item, ELEMENT_SIZE);
    }
    else {
        printf("Can't Set At Index: %i\n", Index);
    }
}
void List_Push(List* v, size_t ELEMENT_SIZE, void* Item) {
    if(Item){
        Node* n = Node_New(ELEMENT_SIZE,Item);

        if(v->size==0){
            v->First = n;
            n->Before = n;
            n->Next = n;
        }else{
            Node_Append(v->First->Before,n);
        }
        v->size++;
    }else {
        printf("Still Not Able to Add.\n");
    }
}
void List_Pop(List* v) {
    if (v->size>0) {
        Node* n = v->First;
        if(v->size==1){
            v->First = NULL;
        }
        Node_Delete(n);
        Node_Free(n);
        v->size--;
    }
    else {
        printf("Still Not Able to Pop.\n");
    }
}
void List_Add(List* v, size_t ELEMENT_SIZE, void* Item, unsigned int Index) {
    if (Index >= 0 && Index <= v->size) {
        Node* n = Node_New(ELEMENT_SIZE,Item);
        if(Index==0){
            Node_Append(v->First->Before,n);
            v->First = n;
        }else{
            Node_Append(List_FindNode(v,Index)->Before,n);
        }
        v->size++;
    }
    else {
        printf("Still Not Able to Add.\n");
    }
}
void List_PopTop(List* v) {
    if (v->size > 0) {
        Node* n = v->First->Before;
        if(v->size==1){
            v->First = NULL;
        }
        Node_Delete(n);
        Node_Free(n);
        v->size--;
    }
    else {
        printf("Can't Pop Top: \n");
    }
}
void List_Remove(List* v, unsigned int Index) {
    if (Index >= 0 && Index < v->size) {
        Node* n = List_FindNode(v,Index);
        if(Index==0){
            v->First = v->First->Next;
        }
        if(v->size==1){
            v->First = NULL;
        }
        Node_Delete(n);
        Node_Free(n);
        v->size--;
    }
    else {
        printf("Can't Remove At Index: %i\n", Index);
    }
}
void List_PushNode(List* v, Node* n) {
    if(v->size==0){
        v->First = n;
        n->Before = n;
        n->Next = n;
    }else{
        Node_Append(v->First->Before,n);
    }
    v->size++;
}
void List_AddNode(List* v, Node* n, unsigned int Index) {
    if (Index >= 0 && Index <= v->size) {
        if(Index==0){
            Node_Append(v->First->Before,n);
            v->First = n;
        }else if(Index==v->size){
            Node_Append(List_FindNode(v,Index-1),n);
        }else{
            Node_Append(List_FindNode(v,Index)->Before,n);
        }
        v->size++;
    }
    else {
        printf("Still Not Able to Add.\n");
    }
}
void List_RemoveNode(List* v, Node* n) {
    if(v->size > 0){
        if(v->size == 1){
            v->First = NULL;
        }else if(v->First == n){
            v->First = v->First->Next;
        }

        Node_Delete(n);
        Node_Free(n);
        v->size--;
    }
}
void List_Clear(List* v) {
    Node* n = v->First;
    for(int i = 0;i<v->size;i++){
        Node* Next = n->Next;
        free(n->Memory);
        free(n);
        n = Next;
    }
    v->First = NULL;
    v->size = 0;
}
void List_Print(List* v) {
    printf("--- List ---\n");
    printf("Size: %i\n", (int)v->size);

    Node* n = v->First;
    for(int i = 0;i<v->size;i++) {
        printf("%d [%lu]: -> \n",i,n->ELEMENT_SIZE);
        if(n->Memory && n->ELEMENT_SIZE>0){
            for (int j = 0;j<n->ELEMENT_SIZE;j++) {
                printf("%c ", ((char*)n->Memory)[j]);
            }
        }
        printf("\n");

        n = n->Next;
    }
    printf("------------\n");
}
void List_Free(List* v) {
    List_Clear(v);
}

void List_ForEach(List* v,void (*Func)(void*)){
    Node* First = v->First;
    for(int i = 0;i<v->size;i++){
        Func(First->Memory);
        First = First->Next;
    }
}
void List_ForEachReverse(List* v,void (*Func)(void*)){
    Node* Last = v->First->Before;
    for(int i = v->size-1;i>=0;i--){
        Func(Last->Memory);
        Last = Last->Before;
    }
}

#endif