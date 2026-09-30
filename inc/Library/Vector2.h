#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>
#include "Float.h"

typedef struct Vec2{
    float x;
    float y;
} Vec2;

typedef struct M2x2 {
    float m[2][2];
} M2x2;

Vec2 Vec2_Add(Vec2 v1,Vec2 v2){
    return (Vec2){ v1.x+v2.x,v1.y+v2.y };
}
Vec2 Vec2_Sub(Vec2 v1,Vec2 v2){
    return (Vec2){ v1.x-v2.x,v1.y-v2.y };
}
Vec2 Vec2_Mul(Vec2 v1,Vec2 v2){
    return (Vec2){ v1.x*v2.x,v1.y*v2.y };
}
Vec2 Vec2_Div(Vec2 v1,Vec2 v2){
    if(v2.x==0.0f||v2.y==0.0f)
        return (Vec2){ 0.0f,0.0f };
    return (Vec2){ v1.x/v2.x,v1.y/v2.y };
}

Vec2 Vec2_Addf(Vec2 v1,float s){
    return (Vec2){ v1.x+s,v1.y+s };
}
Vec2 Vec2_Subf(Vec2 v1,float s){
    return (Vec2){ v1.x-s,v1.y-s };
}
Vec2 Vec2_Mulf(Vec2 v1,float s){
    return (Vec2){ v1.x*s,v1.y*s };
}
Vec2 Vec2_Divf(Vec2 v1,float s){
    if(s==0.0f) return (Vec2){ v1.x,v1.y };
    return (Vec2){ v1.x/s,v1.y/s };
}

Vec2 Vec2_Clamp(Vec2 v,Vec2 min,Vec2 max){
    return (Vec2){
        F32_Max(F32_Min(v.x,max.x),min.x),
        F32_Max(F32_Min(v.y,max.y),min.y)
    };
}

Vec2 Vec2_Neg(Vec2 v){
    return (Vec2){ -v.x,-v.y };
}
Vec2 Vec2_Perp(Vec2 v){
    return (Vec2){ -v.y,v.x };
}
Vec2 Vec2_PerpA(Vec2 v){
    return (Vec2){ v.y,-v.x };
}
Vec2 Vec2_Rotate(Vec2 v,float a){
    const float cs = cosf(a);
    const float ss = sinf(a);
    return (Vec2){ v.x * cs - v.y * ss, v.x * ss + v.y * cs };
}
float Vec2_Dot(Vec2 v1,Vec2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}
float Vec2_Mag2(Vec2 v){
    return Vec2_Dot(v,v);
}
float Vec2_Mag(Vec2 v){
    return sqrtf(Vec2_Mag2(v));
}
Vec2 Vec2_Norm(Vec2 v){
    float h = Vec2_Mag(v);
    return Vec2_Divf(v,h);
}
Vec2 Vec2_OfAngle(float a){
    return (Vec2){ cosf(a),sinf(a) };
}
float Vec2_AngleOf(Vec2 v){
    return atan2f(v.y,v.x);
}
Vec2 Vec2_Dir(unsigned char dir){
    if(dir == 0U) return (Vec2){ 0.0f, 1.0f };
    if(dir == 1U) return (Vec2){ 1.0f, 0.0f };
    if(dir == 2U) return (Vec2){ 0.0f,-1.0f };
    if(dir == 3U) return (Vec2){-1.0f, 0.0f };
    return (Vec2){ 0.0f,0.0f };
}



Vec2 Vec2_Proj(Vec2 a,Vec2 b){
    const float dotab = Vec2_Dot(a,b);
    const float dotaa = Vec2_Dot(a,a);
    if(dotaa == 0.0f) return a;
    return Vec2_Mulf(a,dotab / dotaa);
}
Vec2 Vec2_Reflect(Vec2 a,Vec2 b){
    return Vec2_Sub(Vec2_Mulf(Vec2_Proj(a,b),2.0f),b);
}
char Vec2_Cmp(Vec2 a,Vec2 b){
    return Vec2_Mag2(Vec2_Sub(a,b)) < 0.0001f;
}
char Vec2_Closest(Vec2 t,Vec2 a,Vec2 b){
    const float d0 = Vec2_Mag2(Vec2_Sub(t,a));
    const float d1 = Vec2_Mag2(Vec2_Sub(t,b));
    return d0 < d1;
}

Vec2 Vec2_Func(Vec2 a,float (*Func)(float)){
    return (Vec2){ Func(a.x),Func(a.y) };
}


M2x2 M2x2_Null(){
    M2x2 mat = {{
        { 0.0f,0.0f }, 
        { 0.0f,0.0f } 
    }};
    return mat;
}
M2x2 M2x2_Identity(){
    M2x2 mat = {{
        { 1.0f,0.0f }, 
        { 0.0f,1.0f } 
    }};
    return mat;
}
M2x2 M2x2_RotateZ(float a){
    M2x2 mat = {{
        { cosf(a),sinf(a) }, 
        {-sinf(a),cosf(a) } 
    }};
    return mat;
}
M2x2 M2x2_Scale(float x,float y){
	M2x2 mat = {{
        { x,   0.0f }, 
        { 0.0f,y    } 
    }};
	return mat;
}
M2x2 M2x2_ShearH(float k){
	M2x2 mat = {{
        { 1.0f,0.0f }, 
        { k,   1.0f } 
    }};
    return mat;
}
M2x2 M2x2_ShearV(float k){
	M2x2 mat = {{
        { 1.0f,k    }, 
        { 0.0f,1.0f } 
    }};
    return mat;
}
M2x2 M2x2_Mirror(float k){
    M2x2 mat = {{
        {-1.0f,0.0f }, 
        { 0.0f,1.0f } 
    }};
    return mat;
}
M2x2 M2x2_M2x2_Mul(M2x2 m1, M2x2 m2){
	M2x2 mat = { 0 };
	for (int c = 0; c < 2; c++)
		for (int r = 0; r < 2; r++)
			mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c];
	return mat;
}
M2x2 M2x2_Inverse(M2x2 m){
    M2x2 mat = {{
        { m.m[1][1],-m.m[0][1] }, 
        {-m.m[1][0], m.m[0][0] } 
    }};
    return mat;
}

Vec2 M2x2_VecMul(Vec2 v,M2x2 m){
	Vec2 out = { 0.0f,0.0f };
	out.x = v.x * m.m[0][0] + v.y * m.m[1][0];
	out.y = v.x * m.m[0][1] + v.y * m.m[1][1];
	return out;
}

#endif