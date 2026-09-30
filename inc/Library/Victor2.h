#ifndef VIC2_H
#define VIC2_H

#ifndef STD_MATH_H
#define STD_MATH_H
#include <math.h>
#endif 

typedef struct Vic2{
    int x;
    int y;
} Vic2;

Vic2 Vic2_Add(Vic2 v1,Vic2 v2){
    return (Vic2){ v1.x+v2.x,v1.y+v2.y };
}
Vic2 Vic2_Sub(Vic2 v1,Vic2 v2){
    return (Vic2){ v1.x-v2.x,v1.y-v2.y };
}
Vic2 Vic2_Mul(Vic2 v1,Vic2 v2){
    return (Vic2){ v1.x*v2.x,v1.y*v2.y };
}
Vic2 Vic2_Div(Vic2 v1,Vic2 v2){
    if(v2.x==0 || v2.y==0) return (Vic2){ 0,0 };
    return (Vic2){ v1.x/v2.x,v1.y/v2.y };
}

Vic2 Vic2_Addf(Vic2 v1,int s){
    return (Vic2){ v1.x+s,v1.y+s };
}
Vic2 Vic2_Subf(Vic2 v1,int s){
    return (Vic2){ v1.x-s,v1.y-s };
}
Vic2 Vic2_Mulf(Vic2 v1,int s){
    return (Vic2){ v1.x*s,v1.y*s };
}
Vic2 Vic2_Divf(Vic2 v1,int s){
    if(s==0.0f) return (Vic2){ v1.x,v1.y };
    return (Vic2){ v1.x/s,v1.y/s };
}

Vic2 Vic2_Neg(Vic2 v){
    return (Vic2){ -v.x,-v.y };
}
Vic2 Vic2_Perp(Vic2 v){
    return (Vic2){ -v.y,v.x };
}
Vic2 Vic2_PerpA(Vic2 v){
    return (Vic2){ v.y,-v.x };
}
int Vic2_Dot(Vic2 v1,Vic2 v2){
    return v1.x*v2.x + v1.y*v2.y;
}
int Vic2_Mag2(Vic2 v){
    return Vic2_Dot(v,v);
}
int Vic2_Mag(Vic2 v){
    return sqrtf(Vic2_Mag2(v));
}
Vic2 Vic2_Norm(Vic2 v){
    int h = Vic2_Mag(v);
    return Vic2_Divf(v,h);
}
Vic2 Vic2_OfAngle(int a){
    return (Vic2){ (int)cos(a),(int)sin(a) };
}



typedef struct Vuc2{
    unsigned int x;
    unsigned int y;
} Vuc2;

Vuc2 Vuc2_Add(Vuc2 v1,Vuc2 v2){
    return (Vuc2){ v1.x+v2.x,v1.y+v2.y };
}
Vuc2 Vuc2_Sub(Vuc2 v1,Vuc2 v2){
    return (Vuc2){ v1.x-v2.x,v1.y-v2.y };
}
Vuc2 Vuc2_Mul(Vuc2 v1,Vuc2 v2){
    return (Vuc2){ v1.x*v2.x,v1.y*v2.y };
}
Vuc2 Vuc2_Div(Vuc2 v1,Vuc2 v2){
    if(v2.x==0 || v2.y==0) return (Vuc2){ 0,0 };
    return (Vuc2){ v1.x/v2.x,v1.y/v2.y };
}

Vuc2 Vuc2_Addf(Vuc2 v1,unsigned int s){
    return (Vuc2){ v1.x+s,v1.y+s };
}
Vuc2 Vuc2_Subf(Vuc2 v1,unsigned int s){
    return (Vuc2){ v1.x-s,v1.y-s };
}
Vuc2 Vuc2_Mulf(Vuc2 v1,unsigned int s){
    return (Vuc2){ v1.x*s,v1.y*s };
}
Vuc2 Vuc2_Divf(Vuc2 v1,unsigned int s){
    if(s==0.0f) return (Vuc2){ v1.x,v1.y };
    return (Vuc2){ v1.x/s,v1.y/s };
}

Vuc2 Vuc2_Neg(Vuc2 v){
    return (Vuc2){ -v.x,-v.y };
}
Vuc2 Vuc2_Perp(Vuc2 v){
    return (Vuc2){ -v.y,v.x };
}
Vuc2 Vuc2_PerpA(Vuc2 v){
    return (Vuc2){ v.y,-v.x };
}
unsigned int Vuc2_Dot(Vuc2 v1,Vuc2 v2){
    return v1.x*v2.x + v1.y*v2.y;
}
unsigned int Vuc2_Mag2(Vuc2 v){
    return Vuc2_Dot(v,v);
}
unsigned int Vuc2_Mag(Vuc2 v){
    return sqrtf(Vuc2_Mag2(v));
}
Vuc2 Vuc2_Norm(Vuc2 v){
    unsigned int h = Vuc2_Mag(v);
    return Vuc2_Divf(v,h);
}
Vuc2 Vuc2_OfAngle(unsigned int a){
    return (Vuc2){ (unsigned int)cos(a),(unsigned int)sin(a) };
}

#endif