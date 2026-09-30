#ifndef POLY_H
#define POLY_H

#include "Intrinsics.h"
#include "Math.h"
#include "../Container/Vector.h"
#include "Triangle.h"

typedef struct Poly{
    Vector p;
} Poly;

Poly Poly_New(Vec2* p,int Count){
	Vector v = Vector_New(sizeof(Vec2));
	Vector_PushCount(&v,p,Count);
	return (Poly){ v };
}

void Poly_Free(Poly* v){
	Vector_Free(&v->p);
}

void Poly_Sort(Poly* t){
	for(int i = 0;i<t->p.size-1;i++){
		Vec2 Check = *(Vec2*)Vector_Get(&t->p,i);
		int Smallest = i+1;
		for(int j = i+2;j<t->p.size;j++){
			Vec2 point = *(Vec2*)Vector_Get(&t->p,j);
			if(Vec2_Mag2(Vec2_Sub(Check,point)) < Vec2_Mag2(Vec2_Sub(Check,*(Vec2*)Vector_Get(&t->p,Smallest)))){
				Vector_Swap(&t->p,Smallest,j);
			}
		}
	}
}

void Poly_Render(unsigned int* Target,int Target_Width,int Target_Height,Poly* t,unsigned int c){
	Poly_Sort(t);
	
	Vec2 p1 = *(Vec2*)Vector_Get(&t->p,0);
	for(int i = 0;i<t->p.size-1;i++){
		Vec2 p2 = *(Vec2*)Vector_Get(&t->p,i+1);
		Vec2 p3 = *(Vec2*)Vector_Get(&t->p,(i+1) % (t->p.size-1) + 1);
		Triangle_Render(Target,Target_Width,Target_Height,Triangle_New(p1,p2,p3),c);
	}
}

void Poly_RenderX(unsigned int* Target,int Target_Width,int Target_Height,Vec2* p,int Count,unsigned int c){
	Poly poly = Poly_New(p,Count);
	Poly_Render(Target,Target_Width,Target_Height,&poly,c);
	Poly_Free(&poly);
}

void Poly_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,Poly* t,unsigned int c,F32 StrokeSize){
	Poly_Sort(t);
	
	Vec2 p1 = *(Vec2*)Vector_Get(&t->p,0);
	for(int i = 0;i<t->p.size-1;i++){
		Vec2 p2 = *(Vec2*)Vector_Get(&t->p,i+1);
		Vec2 p3 = *(Vec2*)Vector_Get(&t->p,(i+1) % (t->p.size-1) + 1);
		Triangle_RenderWire(Target,Target_Width,Target_Height,Triangle_New(p1,p2,p3),c,StrokeSize);
	}
}

void Poly_RenderXWire(unsigned int* Target,int Target_Width,int Target_Height,Vec2* p,int Count,unsigned int c,F32 StrokeSize){
	Poly poly = Poly_New(p,Count);
	Poly_RenderWire(Target,Target_Width,Target_Height,&poly,c,StrokeSize);
	Poly_Free(&poly);
}

#endif // !POLY_H