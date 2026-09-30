#ifndef INTEGER_H
#define INTEGER_H

typedef char bol;
#define BOL_FALSE   0
#define BOL_TRUE    1

typedef char I8;
#define I8_MIN         -128
#define I8_MAX          127

typedef unsigned char U8;
#define U8_MIN          0U
#define U8_MAX          255U

typedef short I16;
#define I16_MIN        -32768
#define I16_MAX         32767

typedef unsigned short U16;
#define U16_MIN         0U
#define U16_MAX         65535U

typedef int I32;
#define I32_MIN        -2147483648
#define I32_MAX         2147483647

typedef unsigned int U32;
#define U32_MIN         0U
#define U32_MAX         4294967295U

typedef long I64;
#define I64_MIN         (I64)0x8000000000000000LL
#define I64_MAX         (I64)0x7FFFFFFFFFFFFFFFLL
#define I64_PARSEERROR  (I64)0xFFFFFFFFFFFFFFFFLL

typedef unsigned long U64;
#define U64_MIN         (U64)0x0000000000000000ULL
#define U64_MAX         (U64)0xFFFFFFFFFFFFFFFFULL
#define U64_PARSEERROR  (U64)0xFFFFFFFFFFFFFFFFULL

I32 I32_Sqrt(I64 a){
    I32 max = 32;
    I64  num = 1;
    for(I32 i = 0;i<max;i++){
        I64  z = num;
        num = (num + (a / num)) >> 1;
        if(num==z) break;
    }
    return num;
}
I32 I32_Min(I32 a,I32 b){
    return a<b?a:b;
}
I32 I32_Max(I32 a,I32 b){
    return a>b?a:b;
}
I32 I32_Clamp(I32 a,I32 min,I32 max){
    return I32_Max(min,I32_Min(max,a));
}
I32 I32_Sign(I32 a){
    return a<0?-1:(a>0?1:0);
}
I32 I32_Abs(I32 a){
    return a * I32_Sign(a);
}
I32 I32_Map(I32 a,I32 min,I32 max,I32 newmin,I32 newmax){
    I32 d = max - min;
    I32 newd = newmax - newmin;
    return ((a - min) * newd) / d;
}
bol I32_Odd(I32 a){
    return a % 2 == 0;
}
bol I32_Odda(I32 a){
    return a % 2 == 1;
}
bol I32_Prime(I32 a){
    I32 maxsqrt = I32_Sqrt(a);
    for(int i = 2;i<maxsqrt;i++){
        if(a % i == 0) return 0;
    }
    return 1;
}
void I32_Swap(I32* a,I32* b){
    I32 c = *a;
    *a = *b;
    *b = c;
}

U32 U32_Sqrt(I64 a){
    U32 max = 32;
    I64  num = 1;
    for(U32 i = 0;i<max;i++){
        U64 z = num;
        num = (num + (a / num)) >> 1;
        if(num==z) break;
    }
    return num;
}
U32 U32_Min(U32 a,U32 b){
    return a<b?a:b;
}
U32 U32_Max(U32 a,U32 b){
    return a>b?a:b;
}
U32 U32_Clamp(U32 a,U32 min,U32 max){
    return U32_Max(min,U32_Min(max,a));
}
U32 U32_Digits2(U32 n) {
    U32 count = 0;
    do {
        count++;
        n >>= 1;
    } while (n);
    return count;
}

I64 I64_Sqrt(I64 a){
    I64 max = 32;
    I64  num = 1;
    for(I64 i = 0;i<max;i++){
        I64  z = num;
        num = (num + (a / num)) >> 1;
        if(num==z) break;
    }
    return num;
}
I64 I64_Min(I64 a,I64 b){
    return a<b?a:b;
}
I64 I64_Max(I64 a,I64 b){
    return a>b?a:b;
}
I64 I64_Clamp(I64 a,I64 min,I64 max){
    return I64_Max(min,I64_Min(max,a));
}
char I64_Passes(I64 value,I64 add,I64 border){
    return ((value<border && value+add>=border) || (value>border && value+add<=border));
}
I64 I64_Sign(I64 a){
    return a<0?-1:(a>0?1:0);
}
I64 I64_Abs(I64 a){
    return a * I64_Sign(a);
}
I64 I64_Map(I64 a,I64 min,I64 max,I64 newmin,I64 newmax){
    I64 d = max - min;
    I64 newd = newmax - newmin;
    return ((a - min) * newd) / d;
}
I64 I64_Log2(I64 a){
    I64 i = -1;
    for(;a>0;i++) a >>= 1;
    return i;
}
I64 I64_Log10(I64 a){
    I64 i = -1;
    for(;a>0;i++) a /= 10;
    return i;
}
I64 I64_Log(I64 a,I64 b){
    I64 i = -1;
    for(;a>0;i++) a /= b;
    return i;
}

bol I64_Odd(I64 a){
    return a % 2 == 0;
}
bol I64_Odda(I64 a){
    return a % 2 == 1;
}
bol I64_Prime(I64 a){
    I64 maxsqrt = I64_Sqrt(a);
    for(int i = 2;i<maxsqrt;i++){
        if(a % i == 0) return 0;
    }
    return 1;
}

U64 U64_Pow10(U64 a){
    U64 n = 1UL;
    for(int i = 0;i<a;i++)
        n *= 10UL;
    return n;
}
U64 U64_Pow2(U64 a){
    U64 n = 1UL << a;
    return n;
}

#endif