#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

typedef struct Vec3{
    float x;
    float y;
    float z;
} Vec3;

typedef struct M3x3 {
    float m[3][3];
} M3x3;

Vec3 Vec3_Add(Vec3 v1,Vec3 v2){
    return (Vec3){ v1.x+v2.x,v1.y+v2.y,v1.z+v2.z };
}
Vec3 Vec3_Sub(Vec3 v1,Vec3 v2){
    return (Vec3){ v1.x-v2.x,v1.y-v2.y,v1.z-v2.z };
}
Vec3 Vec3_Mul(Vec3 v1,Vec3 v2){
    return (Vec3){ v1.x*v2.x,v1.y*v2.y,v1.z*v2.z };
}
Vec3 Vec3_Div(Vec3 v1,Vec3 v2){
    if(v2.x==0.0f||v2.y==0.0f) return (Vec3){ 0.0f,0.0f,0.0f };
    return (Vec3){ v1.x/v2.x,v1.y/v2.y,v1.z/v2.z };
}

Vec3 Vec3_Addf(Vec3 v1,float s){
    return (Vec3){ v1.x+s,v1.y+s,v1.z+s };
}
Vec3 Vec3_Subf(Vec3 v1,float s){
    return (Vec3){ v1.x-s,v1.y-s,v1.z-s };
}
Vec3 Vec3_Mulf(Vec3 v1,float s){
    return (Vec3){ v1.x*s,v1.y*s,v1.z*s };
}
Vec3 Vec3_Divf(Vec3 v1,float s){
    if(s==0.0f) return (Vec3){ v1.x,v1.y,v1.z };
    return (Vec3){ v1.x/s,v1.y/s,v1.z/s };
}

Vec3 Vec3_Neg(Vec3 v){
    return (Vec3){ -v.x,-v.y,-v.z };
}
Vec3 Vec3_Perp(Vec3 v){
    return (Vec3){ -v.y,v.x,v.z };
}
float Vec3_Dot(Vec3 v1,Vec3 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
float Vec3_Mag2(Vec3 v){
    return Vec3_Dot(v,v);
}
float Vec3_Mag(Vec3 v){
    return sqrtf(Vec3_Mag2(v));
}
Vec3 Vec3_Norm(Vec3 v){
    float h = Vec3_Mag(v);
    return Vec3_Divf(v,h);
}
Vec3 Vec3_OfAngle(float a){
    return (Vec3){ cosf(a),sinf(a),0.0f };
}
Vec3 Vec3_Floor(Vec3 v){
    return (Vec3){ floorf(v.x),floorf(v.y),floorf(v.z) };
}

M3x3 M3x3_Null(){
    M3x3 mat = {{ 
        { 0.0f,0.0f,0.0f }, 
        { 0.0f,0.0f,0.0f }, 
        { 0.0f,0.0f,0.0f } 
    }};
    return mat;
}
M3x3 M3x3_FloatMul(float s,M3x3 m){
	M3x3 mat = {{ 
        { m.m[0][0] * s,m.m[0][1] * s,m.m[0][2] * s },
        { m.m[1][0] * s,m.m[1][1] * s,m.m[1][2] * s },
        { m.m[2][0] * s,m.m[2][1] * s,m.m[2][2] * s }
    }};
	return mat;
}
Vec3 M3x3_VecMul(Vec3 v,M3x3 m){
	Vec3 out = { 0.0f,0.0f,0.0f };
	out.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	out.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
    out.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return out;
}
M3x3 M3x3_M3x3_Mul(M3x3 m1,M3x3 m2){
	M3x3 mat = M3x3_Null();
	for (int c = 0; c < 3; c++)
		for (int r = 0; r < 3; r++)
			mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c];
	return mat;
}

M3x3 M3x3_Identity(){
    M3x3 mat = {{ 
        { 1.0f,       0.0f,       0.0f }, 
        { 0.0f,       1.0f,       0.0f }, 
        { 0.0f,       0.0f,       1.0f } 
    }};
    return mat;
}
M3x3 M3x3_RotateX(float a){
    M3x3 mat = {{ 
        { 1.0f,       0.0f,       0.0f      }, 
        { 0.0f,       cosf(a),    -sinf(a)  }, 
        { 0.0f,       sinf(a),    cosf(a)   }
    }};
    return mat;
}
M3x3 M3x3_RotateY(float a){
    M3x3 mat = {{ 
        { cosf(a),       0.0f,        sinf(a) }, 
        { 0.0f,          1.0f,        0.0f    }, 
        { -sinf(a),      0.0f,        cosf(a) }
    }};
    return mat;
}
M3x3 M3x3_RotateZ(float a){
    M3x3 mat = {{ 
        { cosf(a),        -sinf(a),       0.0f },
        { sinf(a),        cosf(a),        0.0f }, 
        { 0.0f,           0.0f,           1.0f }
    }};
    return mat;
}
M3x3 M3x3_Trans(float x,float y,float z){
	M3x3 mat = {{ 
        { 1.0f,           0.0f,           0.0f },
        { 0.0f,           1.0f,           0.0f }, 
        { x,              y,              z    }
    }};
	return mat;
}
M3x3 M3x3_Scale(float x,float y,float z){
	M3x3 mat = {{
        { x,          0.0f,       0.0f },
        { 0.0f,       y,          0.0f }, 
        { 0.0f,       0.0f,       z    }
    }};
	return mat;
}
float M3x3_Det(M3x3 m){
	return 
        m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2])
       -m.m[1][0] * (m.m[0][1] * m.m[2][2] - m.m[2][1] * m.m[0][2])
       +m.m[2][0] * (m.m[0][1] * m.m[1][2] - m.m[1][1] * m.m[0][2]);
}
M3x3 M3x3_Adj(M3x3 m){
	M3x3 mat = {{
        { m.m[0][0],m.m[1][0],m.m[2][0] },
        { m.m[0][1],m.m[1][1],m.m[2][1] },
        { m.m[0][2],m.m[1][2],m.m[2][2] }
    }};
	return mat;
}
M3x3 M3x3_Inverse(M3x3 matIn){
    float det = matIn.m[0][0] * (matIn.m[1][1] * matIn.m[2][2] - matIn.m[1][2] * matIn.m[2][1]) -
			    matIn.m[1][0] * (matIn.m[0][1] * matIn.m[2][2] - matIn.m[2][1] * matIn.m[0][2]) +
			    matIn.m[2][0] * (matIn.m[0][1] * matIn.m[1][2] - matIn.m[1][1] * matIn.m[0][2]);

	float idet = 1.0f / det;
    M3x3 matOut;
	matOut.m[0][0] = (matIn.m[1][1] * matIn.m[2][2] - matIn.m[1][2] * matIn.m[2][1]) * idet;
	matOut.m[1][0] = (matIn.m[2][0] * matIn.m[1][2] - matIn.m[1][0] * matIn.m[2][2]) * idet;
	matOut.m[2][0] = (matIn.m[1][0] * matIn.m[2][1] - matIn.m[2][0] * matIn.m[1][1]) * idet;
	matOut.m[0][1] = (matIn.m[2][1] * matIn.m[0][2] - matIn.m[0][1] * matIn.m[2][2]) * idet;
	matOut.m[1][1] = (matIn.m[0][0] * matIn.m[2][2] - matIn.m[2][0] * matIn.m[0][2]) * idet;
	matOut.m[2][1] = (matIn.m[0][1] * matIn.m[2][0] - matIn.m[0][0] * matIn.m[2][1]) * idet;
	matOut.m[0][2] = (matIn.m[0][1] * matIn.m[1][2] - matIn.m[0][2] * matIn.m[1][1]) * idet;
	matOut.m[1][2] = (matIn.m[0][2] * matIn.m[1][0] - matIn.m[0][0] * matIn.m[1][2]) * idet;
	matOut.m[2][2] = (matIn.m[0][0] * matIn.m[1][1] - matIn.m[0][1] * matIn.m[1][0]) * idet;
    return matOut;
	//return M3x3_FloatMul(1.0f / M3x3_Det(matIn),M3x3_Adj(matIn));
}

#endif