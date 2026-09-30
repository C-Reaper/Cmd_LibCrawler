#ifndef VDCTOR2_H
#define VDCTOR2_H

#include <math.h>
#include "Float.h"

typedef struct Vdc2{
    double x;
    double y;
} Vdc2;

typedef struct M2x2D {
    double m[2][2];
} M2x2D;

Vdc2 Vdc2_Add(Vdc2 v1,Vdc2 v2){
    return (Vdc2){ v1.x+v2.x,v1.y+v2.y };
}
Vdc2 Vdc2_Sub(Vdc2 v1,Vdc2 v2){
    return (Vdc2){ v1.x-v2.x,v1.y-v2.y };
}
Vdc2 Vdc2_Mul(Vdc2 v1,Vdc2 v2){
    return (Vdc2){ v1.x*v2.x,v1.y*v2.y };
}
Vdc2 Vdc2_Div(Vdc2 v1,Vdc2 v2){
    if(v2.x==0.0||v2.y==0.0)
        return (Vdc2){ 0.0,0.0 };
    return (Vdc2){ v1.x/v2.x,v1.y/v2.y };
}

Vdc2 Vdc2_Addf(Vdc2 v1,double s){
    return (Vdc2){ v1.x+s,v1.y+s };
}
Vdc2 Vdc2_Subf(Vdc2 v1,double s){
    return (Vdc2){ v1.x-s,v1.y-s };
}
Vdc2 Vdc2_Mulf(Vdc2 v1,double s){
    return (Vdc2){ v1.x*s,v1.y*s };
}
Vdc2 Vdc2_Divf(Vdc2 v1,double s){
    if(s==0.0) return (Vdc2){ v1.x,v1.y };
    return (Vdc2){ v1.x/s,v1.y/s };
}

Vdc2 Vdc2_Clamp(Vdc2 v,Vdc2 min,Vdc2 max){
    return (Vdc2){
        F32_Max(F32_Min(v.x,max.x),min.x),
        F32_Max(F32_Min(v.y,max.y),min.y)
    };
}

Vdc2 Vdc2_Neg(Vdc2 v){
    return (Vdc2){ -v.x,-v.y };
}
Vdc2 Vdc2_Perp(Vdc2 v){
    return (Vdc2){ -v.y,v.x };
}
Vdc2 Vdc2_PerpA(Vdc2 v){
    return (Vdc2){ v.y,-v.x };
}
double Vdc2_Dot(Vdc2 v1,Vdc2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}
double Vdc2_Mag2(Vdc2 v){
    return Vdc2_Dot(v,v);
}
double Vdc2_Mag(Vdc2 v){
    return sqrtf(Vdc2_Mag2(v));
}
Vdc2 Vdc2_Norm(Vdc2 v){
    double h = Vdc2_Mag(v);
    return Vdc2_Divf(v,h);
}
Vdc2 Vdc2_OfAngle(double a){
    return (Vdc2){ cosf(a),sinf(a) };
}
double Vdc2_AngleOf(Vdc2 v){
    return atan2f(v.y,v.x);
}

Vdc2 Vdc2_Proj(Vdc2 a,Vdc2 b){
    const double dotab = Vdc2_Dot(a,b);
    const double dotaa = Vdc2_Dot(a,a);
    if(dotaa == 0.0) return a;
    return Vdc2_Mulf(a,dotab / dotaa);
}
Vdc2 Vdc2_Reflect(Vdc2 a,Vdc2 b){
    return Vdc2_Sub(Vdc2_Mulf(Vdc2_Proj(a,b),2.0),b);
}

Vdc2 Vdc2_Func(Vdc2 a,double (*Func)(double)){
    return (Vdc2){ Func(a.x),Func(a.y) };
}


M2x2D M2x2D_Null(){
    M2x2D mat = {{
        { 0.0,0.0 }, 
        { 0.0,0.0 } 
    }};
    return mat;
}
M2x2D M2x2D_Identity(){
    M2x2D mat = {{
        { 1.0,0.0 }, 
        { 0.0,1.0 } 
    }};
    return mat;
}
M2x2D M2x2D_RotateZ(double a){
    M2x2D mat = {{
        { cosf(a),sinf(a) }, 
        {-sinf(a),cosf(a) } 
    }};
    return mat;
}
M2x2D M2x2D_Scale(double x,double y){
	M2x2D mat = {{
        { x,   0.0 }, 
        { 0.0,y    } 
    }};
	return mat;
}
M2x2D M2x2D_ShearH(double k){
	M2x2D mat = {{
        { 1.0,0.0 }, 
        { k,   1.0 } 
    }};
    return mat;
}
M2x2D M2x2D_ShearV(double k){
	M2x2D mat = {{
        { 1.0,k    }, 
        { 0.0,1.0 } 
    }};
    return mat;
}
M2x2D M2x2D_Mirror(double k){
    M2x2D mat = {{
        {-1.0,0.0 }, 
        { 0.0,1.0 } 
    }};
    return mat;
}
M2x2D M2x2D_M2x2D_Mul(M2x2D m1, M2x2D m2){
	M2x2D mat = { 0 };
	for (int c = 0; c < 2; c++)
		for (int r = 0; r < 2; r++)
			mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c];
	return mat;
}
M2x2D M2x2D_Inverse(M2x2D m){
    M2x2D mat = {{
        { m.m[1][1],-m.m[0][1] }, 
        {-m.m[1][0], m.m[0][0] } 
    }};
    return mat;
}

Vdc2 M2x2D_VecMul(Vdc2 v,M2x2D m){
	Vdc2 out = { 0.0,0.0 };
	out.x = v.x * m.m[0][0] + v.y * m.m[1][0];
	out.y = v.x * m.m[0][1] + v.y * m.m[1][1];
	return out;
}

#endif