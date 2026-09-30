#ifndef FLOAT_H
#define FLOAT_H

#include <math.h>

typedef float F32;
typedef double F64;

#define F32_PI      3.141592654f
#define F32_E       2.718281828f

#define F32_PI025   0.785398163f
#define F32_PI05    1.570796327f
#define F32_PI2     6.283185307f
#define F32_PISQ    9.869604401f

#define F32_SIGN    1
#define F32_EXPO    8
#define F32_MANT    23


#define F64_PI025   0.785398163
#define F64_PI05    1.570796327
#define F64_PI      3.141592654
#define F64_E       2.718281828

#define F64_PI2     6.283185307
#define F64_PISQ    9.869604401

#define F64_SIGN    1
#define F64_EXPO    11
#define F64_MANT    52

F32 F32_Map(F32 a,F32 min,F32 max,F32 newmin,F32 newmax){
	const F32 norm = (a - min) / (max - min);
    return newmin + norm * (newmax - newmin);
}
F32 F32_Clamp(F32 a,F32 min,F32 max){
	return a<min?min:(a>max?max:a);
}
F32 F32_Border(F32 a,F32 bordermin,F32 bordermax){
    F32 borderl = bordermax - bordermin;
	if(a > bordermin && a <= bordermin + borderl * 0.5f) return bordermin;
	if(a < bordermax && a >= bordermin + borderl * 0.5f) return bordermax;
    return a;
}
F32 F32_Mod(F32 a,F32 mod){
	a /= mod;
    float o = a - (int)a;
    return o;
}

F32 F32_ToRad(F32 a) {
	return (F32) (a / 180.0f * F32_PI);
}
F32 F32_ToDeg(F32 a) {
	return (F32) (a / F32_PI * 180.0f);
}
F32 F32_Abs(F32 value){
    return value>0?value:-value;
}
F32 F32_Floor(F32 value){
    return (F32)((long long)value);
}
F32 F32_Ceil(F32 value){
    return F32_Floor(value + 1.0f);
}
F32 F32_Round(F32 value){
    return F32_Floor(value + 0.5f);
}
F32 F32_Sign(F32 value){
    return value>0?1:-1;
}
char F32_Passes(F32 value,F32 add,F32 border){
    return ((value<border && value+add>=border) || (value>border && value+add<=border));
}
F32 F32_LinPer(F32 x,F32 PerLength){
    F32 y = (F32) (x / PerLength);
    y = y - (int)y;
    return y;
}
F32 F32_Sin_Sq(F32 x){
    F32 y = (F32) (x / (2*F32_PI));
    y = y - (int)y;
    y = y + 1.0f;
    y = y - (int)y;
    if(y<0.5f){
        return -16*y*y+8*y; 
    }
    return 16*y*y-16*y-8*y+8;
}
F32 F32_Sin_Cb(F32 x){
    F32 y = (F32) (x / (2*F32_PI));
    y = y - (int)y;
    y = y + 1.0f;
    y = y - (int)y;
    return 20.785f * (y-0.0f) * (y-0.5f) * (y-1.0f);
}
F32 F32_Cos_Sq(F32 x){
    return F32_Sin_Sq((F32) (x+F32_PI/2));
}
F32 F32_Cos_Cb(F32 x){
    return F32_Sin_Cb((F32) (x+F32_PI/2));
}
F32 F32_Tan_Sq(F32 x){
    F32 y = F32_Cos_Sq(x);
    if(y==0.0f) return 0.0f;
    return F32_Sin_Sq(x) / y;
}
F32 F32_Tan_Cb(F32 x){
    F32 y = F32_Cos_Cb(x);
    if(y==0.0f) return 0.0f;
    return F32_Sin_Cb(x) / y;
}
F32 F32_Sqrt(F32 a){
    return sqrtf(a);
}
F32 F32_Pow(F32 a,F32 b){
    return powf(a,b);
}
F32 F32_A_Sqrt(F32 a){
    F32 max = 1000;
    F32 num = 1.0f;
    for(int i = 0;i<max;i++){
        num = (num + (a / num)) * 0.5f;
    }
    return num;
}
F32 F32_ISqrt(F32 a){
    const F32 x2 = a * 0.5F;
    F32 y = a;

    union { float f; int i; } u;
    u.f = y;

    u.i = 0x5f3759df - (u.i >> 1);
    y = u.f;
    y = y * (1.5F - (x2 * y * y));

    return y;
}
F32 F32_Min(F32 a,F32 b){
    return a<b?a:b;
}
F32 F32_Max(F32 a,F32 b){
    return a>b?a:b;
}
F32 F32_LeakyRelu(F32 a){
    return F32_Max(0.1f * a,a);
}
F32 F32_LeakyRelu_D(F32 a){
    return a >= 0.0f ? 1.0f : 0.1f;
}
F32 F32_Relu(F32 a){
    return F32_Max(0.0f,a);
}
F32 F32_Relu_D(F32 a){
    return a >= 0.0f ? 1.0f : 0.0f;
}
F32 F32_Sigmoid(F32 a){
    return 1.0f / (1.0f + expf(-a));
}
F32 F32_Sigmoid_D(F32 a){
    return F32_Sigmoid(a) * (1 - F32_Sigmoid(a));
}
F32 F32_Tanh(F32 a){
    return tanhf(a);
}
F32 F32_Tanh_D(F32 a){
    return tanhf(a);
}
F32 F32_Krung(F32 a,F32 p){
    return F32_Sqrt(F32_Abs(a / p)) * p * F32_Sign(a);
}
F32 F32_Bino(F32 a){
    return powf(F32_E,-(a * a));
}

F32 F32_SAbs(F32 value,F32 lambda){
    return sqrtf(value * value + lambda);
}
F32 F32_SSign(F32 value,F32 lambda){
    return value / sqrtf(value * value + lambda);
}
F32 F32_SRelu(F32 value,F32 lambda){
    return (value + sqrtf(value * value + lambda)) * 0.5f;
}
F32 F32_SSat(F32 value,F32 lambda){
    const F32 valuem1 = value - 1.0f;
    return (1.0f + sqrtf(value * value + lambda) - sqrtf(valuem1 * valuem1 + lambda)) * 0.5f;
}
F32 F32_SMax(F32 value1,F32 value2,F32 lambda){
    const F32 value12 = value1 - value2;
    return (value1 + value2 + sqrtf(value12 * value12 + lambda)) * 0.5f;
}
F32 F32_SMin(F32 value1,F32 value2,F32 lambda){
    const F32 value12 = value1 - value2;
    return (value1 + value2 - sqrtf(value12 * value12 + lambda)) * 0.5f;
}

void F32_Swap(F32* a,F32* b){
    F32 c = *a;
    *a = *b;
    *b = c;
}
int F32_Cmp_G(void* a,void* b){
    F32 fa = *(F32*)a;
    F32 fb = *(F32*)b;
    return fa > fb ? 1 : (fa == fb ? 0 : -1);
}
int F32_Cmp_L(void* a,void* b){
    F32 fa = *(F32*)a;
    F32 fb = *(F32*)b;
    return fa < fb ? 1 : (fa == fb ? 0 : -1);
}

F64 F64_toRad(F64 a) {
	return (F64) (a / 180.0f * F64_PI);
}
F64 F64_toDeg(F64 a) {
	return (F64) (a / F64_PI * 180.0f);
}
F64 F64_Abs(F64 value){
    return value > 0.0? value : -value;
}
F64 F64_Floor(F64 value){
    return (F64)floorf(value);
}
F64 F64_Ceil(F64 value){
    return F64_Floor(value + 1.0);
}
F64 F64_Round(F64 value){
    return F64_Floor(value + 0.5);
}
F64 F64_Sign(F64 value){
    return value>0?1:-1;
}
char F64_Passes(F64 value,F64 add,F64 border){
    return ((value<border && value+add>border) || (value>border && value+add<border));
}
F64 F64_LinPer(F64 x,F64 PerLength){
    F64 y = (F64) (x / PerLength);
    y = y - (int)y;
    return y;
}
F64 F64_Sin_Sq(F64 x){
    F64 y = (F64) (x / (2*F64_PI));
    y = y - (int)y;
    y = y + 1.0f;
    y = y - (int)y;
    if(y<0.5f){
        return -16*y*y+8*y; 
    }
    return 16*y*y-16*y-8*y+8;
}
F64 F64_Sin_Cb(F64 x){
    F64 y = (F64) (x / (2*F64_PI));
    y = y - (int)y;
    y = y + 1.0f;
    y = y - (int)y;
    return 20.785f * (y-0.0f) * (y-0.5f) * (y-1.0f);
}
F64 F64_Cos_Sq(F64 x){
    return F64_Sin_Sq((F64) (x+F64_PI/2));
}
F64 F64_Cos_Cb(F64 x){
    return F64_Sin_Cb((F64) (x+F64_PI/2));
}
F64 F64_Tan_Sq(F64 x){
    F64 y = F64_Cos_Sq(x);
    if(y==0.0f) return 0.0f;
    return F64_Sin_Sq(x) / y;
}
F64 F64_Tan_Cb(F64 x){
    F64 y = F64_Cos_Cb(x);
    if(y==0.0f) return 0.0f;
    return F64_Sin_Cb(x) / y;
}
F64 F64_Sqrt(F64 a){
    F64 max = 1000;
    F64 num = 1.0f;
    for(int i = 0;i<max;i++){
        num = (num + (a / num)) * 0.5f;
    }
    return num;
}
F64 F64_Min(F64 a,F64 b){
    return a<b?a:b;
}
F64 F64_Max(F64 a,F64 b){
    return a>b?a:b;
}
F64 F64_Clamp(F64 a,F64 min,F64 max){
	return a<min?min:(a>max?max:a);
}

F64 F64_SAbs(F64 value,F64 lambda){
    return sqrt(value * value + lambda);
}
F64 F64_SSign(F64 value,F64 lambda){
    return value / sqrt(value * value + lambda);
}
F64 F64_SRelu(F64 value,F64 lambda){
    return (value + sqrt(value * value + lambda)) * 0.5;
}
F64 F64_SSat(F64 value,F64 lambda){
    const F64 valuem1 = value - 1.0;
    return (1.0 + sqrt(value * value + lambda) - sqrt(valuem1 * valuem1 + lambda)) * 0.5;
}
F64 F64_SMax(F64 value1,F64 value2,F64 lambda){
    const F64 value12 = value1 - value2;
    return (value1 + value2 + sqrt(value12 * value12 + lambda)) * 0.5;
}
F64 F64_SMin(F64 value1,F64 value2,F64 lambda){
    const F64 value12 = value1 - value2;
    return (value1 + value2 - sqrt(value12 * value12 + lambda)) * 0.5;
}

F64 SinTable[] = {
0.000000,
0.062791,
0.125333,
0.187381,
0.248690,
0.309017,
0.368125,
0.425779,
0.481754,
0.535827,
0.587785,
0.637424,
0.684547,
0.728969,
0.770513,
0.809017,
0.844328,
0.876307,
0.904827,
0.929776,
0.951057,
0.968583,
0.982287,
0.992115,
0.998027,
1.000000,
0.998027,
0.992115,
0.982287,
0.968583,
0.951057,
0.929776,
0.904827,
0.876307,
0.844328,
0.809017,
0.770513,
0.728969,
0.684547,
0.637424,
0.587785,
0.535827,
0.481754,
0.425779,
0.368125,
0.309017,
0.248690,
0.187381,
0.125333,
0.062791,
-0.000000,
-0.062791,
-0.125333,
-0.187381,
-0.248690,
-0.309017,
-0.368125,
-0.425779,
-0.481754,
-0.535827,
-0.587785,
-0.637424,
-0.684547,
-0.728969,
-0.770513,
-0.809017,
-0.844328,
-0.876307,
-0.904827,
-0.929776,
-0.951057,
-0.968583,
-0.982287,
-0.992115,
-0.998027,
-1.000000,
-0.998027,
-0.992115,
-0.982287,
-0.968583,
-0.951057,
-0.929776,
-0.904827,
-0.876307,
-0.844328,
-0.809017,
-0.770513,
-0.728969,
-0.684547,
-0.637424,
-0.587785,
-0.535827,
-0.481754,
-0.425779,
-0.368125,
-0.309017,
-0.248690,
-0.187381,
-0.125333,
-0.062791,
0.000000,
};

F64 F64_Sin_It(F64 x){
    F64 y = (F64) (x / (2 * F64_PI));
    y = y - (int)y;
    y = y + 1.0f;
    y = y - (int)y;
    
    int TableLength = (sizeof(SinTable) / sizeof(F64));
    double TablePlace = (y * TableLength);
    int Index = (int)TablePlace;
    TablePlace -= Index;
    F64 v1 = SinTable[Index % TableLength];
    F64 v2 = SinTable[(Index+1) % TableLength];
    F64 It = v1 + (v2 - v1) * TablePlace;
    return It;
}
F64 F64_Pow10(F64 a){
    F64 n = 1.0;
    const long long absv = (long long)F64_Abs(a);
    const F64 value = a > 0 ? 10.0 : 0.1;
    for(long long i = 0;i<absv;i++)
        n *= value;
    return n;
}

int F64_Cmp_G(void* a,void* b){
    F64 fa = *(F64*)a;
    F64 fb = *(F64*)b;
    return fa > fb ? 1 : (fa == fb ? 0 : -1);
}
int F64_Cmp_L(void* a,void* b){
    F64 fa = *(F64*)a;
    F64 fb = *(F64*)b;
    return fa < fb ? 1 : (fa == fb ? 0 : -1);
}

#endif