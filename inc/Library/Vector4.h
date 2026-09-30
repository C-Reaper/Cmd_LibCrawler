#ifndef VECTOR3_H
#define VECTOR3_H

#ifndef STD_MATH_H
#define STD_MATH_H
#include <math.h>
#endif 

typedef struct Vec4{
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct M4x4 {
    float m[4][4];
} M4x4;

Vec4 Vec4_Add(Vec4 v1,Vec4 v2){
    return (Vec4){ v1.x+v2.x,v1.y+v2.y,v1.z+v2.z,v1.w+v2.w };
}
Vec4 Vec4_Sub(Vec4 v1,Vec4 v2){
    return (Vec4){ v1.x-v2.x,v1.y-v2.y,v1.z-v2.z,v1.w-v2.w };
}
Vec4 Vec4_Mul(Vec4 v1,Vec4 v2){
    return (Vec4){ v1.x*v2.x,v1.y*v2.y,v1.z*v2.z,v1.w*v2.w };
}
Vec4 Vec4_Div(Vec4 v1,Vec4 v2){
    if(v2.x==0.0f||v2.y==0.0f) return (Vec4){ 0.0f,0.0f,0.0f,0.0f };
    return (Vec4){ v1.x/v2.x,v1.y/v2.y,v1.z/v2.z,v1.w/v2.w };
}

Vec4 Vec4_Addf(Vec4 v1,float s){
    return (Vec4){ v1.x+s,v1.y+s,v1.z+s,v1.w+s };
}
Vec4 Vec4_Subf(Vec4 v1,float s){
    return (Vec4){ v1.x-s,v1.y-s,v1.z-s,v1.w-s };
}
Vec4 Vec4_Mulf(Vec4 v1,float s){
    return (Vec4){ v1.x*s,v1.y*s,v1.z*s,v1.w*s };
}
Vec4 Vec4_Divf(Vec4 v1,float s){
    if(s==0.0f) return (Vec4){ v1.x,v1.y,v1.z,v1.w };
    return (Vec4){ v1.x/s,v1.y/s,v1.z/s,v1.w/s };
}

Vec4 Vec4_Neg(Vec4 v){
    return (Vec4){ -v.x,-v.y,-v.z,-v.w };
}
Vec4 Vec4_Perp(Vec4 v){
    return (Vec4){ -v.y,v.x,v.z,v.w };
}
float Vec4_Dot(Vec4 v1,Vec4 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}
float Vec4_Mag2(Vec4 v){
    return Vec4_Dot(v,v);
}
float Vec4_Mag(Vec4 v){
    return sqrtf(Vec4_Mag2(v));
}
Vec4 Vec4_Norm(Vec4 v){
    float h = Vec4_Mag(v);
    return Vec4_Divf(v,h);
}
Vec4 Vec4_OfAngle(float a){
    return (Vec4){ cosf(a),sinf(a),0.0f,0.0f };
}

M4x4 M4x4_Null(){
    M4x4 mat = {{
        { 0.0f,0.0f,0.0f,0.0f }, 
        { 0.0f,0.0f,0.0f,0.0f }, 
        { 0.0f,0.0f,0.0f,0.0f }, 
        { 0.0f,0.0f,0.0f,0.0f } 
    }};
    return mat;
}
M4x4 M4x4_Identity(){
    M4x4 mat = {{ 
        { 1.0f,       0.0f,       0.0f,       0.0f }, 
        { 0.0f,       1.0f,       0.0f,       0.0f }, 
        { 0.0f,       0.0f,       1.0f,       0.0f },
        { 0.0f,       0.0f,       0.0f,       1.0f }
    }};
    return mat;
}
M4x4 M4x4_RotateX(float a){
    M4x4 mat = {{ 
        { 1.0f,       0.0f,       0.0f,       0.0f },
        { 0.0f,       cosf(a),    -sinf(a),   0.0f },
        { 0.0f,       sinf(a),    cosf(a),    0.0f },
        { 0.0f,       0.0f,       0.0f,       0.0f }
    }};
    return mat;
}
M4x4 M4x4_RotateY(float a){
    M4x4 mat = {{ 
        { cosf(a),       0.0f,        sinf(a),    0.0f },
        { 0.0f,          1.0f,        0.0f,       0.0f },
        { -sinf(a),      0.0f,        cosf(a),    0.0f },
        { 0.0f,          0.0f,        0.0f,       0.0f }
    }};
    return mat;
}
M4x4 M4x4_RotateZ(float a){
    M4x4 mat = {{ 
        { cosf(a),        -sinf(a),       0.0f,       0.0f },
        { sinf(a),        cosf(a),        0.0f,       0.0f },
        { 0.0f,           0.0f,           1.0f,       0.0f },
        { 0.0f,           0.0f,           0.0f,       0.0f }
    }};
    return mat;
}
M4x4 M4x4_Trans(float x, float y, float z){
	M4x4 mat = {{
        { 0.0f,           0.0f,           0.0f,       0.0f },
        { 0.0f,           0.0f,           0.0f,       0.0f },
        { 0.0f,           0.0f,           0.0f,       0.0f }, 
        { x,              y,              z,          1.0f }
    }};
	return mat;
}
Vec4 M4x4_VecMul(Vec4 v, M4x4 m){
	Vec4 out = { 0.0f,0.0f,0.0f };
	out.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	out.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
    out.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return out;
}
M4x4 M4x4_M4x4_Mul(M4x4 m1, M4x4 m2){
	M4x4 mat = M4x4_Null();
	for (int c = 0; c < 3; c++)
		for (int r = 0; r < 3; r++)
			mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c];
	return mat;
}

#endif