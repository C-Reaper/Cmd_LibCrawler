#ifndef COMPLEX2_H
#define COMPLEX2_H

#include <math.h>

typedef struct Complex2{
    double r;
    double i;
} Complex2;

Complex2 Complex2_Add(Complex2 v1,Complex2 v2){
    return (Complex2){ v1.r+v2.r,v1.i+v2.i };
}
Complex2 Complex2_Sub(Complex2 v1,Complex2 v2){
    return (Complex2){ v1.r-v2.r,v1.i-v2.i };
}
Complex2 Complex2_Mul(Complex2 v1,Complex2 v2){
    return (Complex2){ v1.r*v2.r - v1.i*v2.i,v1.i*v2.r + v1.r*v2.i };
}
Complex2 Complex2_Div(Complex2 v1,Complex2 v2){
    if(v2.r==0.0 && v2.i==0.0) return (Complex2){ 0.0,0.0 };
    double z = v2.r*v2.r + v2.i*v2.i;
    Complex2 c = Complex2_Mul(v1,v2);
    return (Complex2){ c.r/z,c.i/z };
}

Complex2 Complex2_Addf(Complex2 v1,double s){
    return (Complex2){ v1.r+s,v1.i+s };
}
Complex2 Complex2_Subf(Complex2 v1,double s){
    return (Complex2){ v1.r-s,v1.i-s };
}
Complex2 Complex2_Mulf(Complex2 v1,double s){
    return (Complex2){ v1.r*s,v1.i*s };
}
Complex2 Complex2_Divf(Complex2 v1,double s){
    if(s==0.0f) return (Complex2){ v1.r,v1.i };
    return (Complex2){ v1.r/s,v1.i/s };
}

Complex2 Complex2_Neg(Complex2 v){
    return (Complex2){ -v.r,-v.i };
}
Complex2 Complex2_Perp(Complex2 v){
    return (Complex2){ -v.i,v.r };
}
Complex2 Complex2_PerpA(Complex2 v){
    return (Complex2){ v.i,-v.r };
}
double Complex2_Dot(Complex2 v1,Complex2 v2){
    return v1.r*v2.r + v1.i*v2.i;
}
double Complex2_Mag2(Complex2 v){
    return Complex2_Dot(v,v);
}
double Complex2_Mag(Complex2 v){
    return sqrt(Complex2_Mag2(v));
}
Complex2 Complex2_Norm(Complex2 v){
    double h = Complex2_Mag(v);
    return Complex2_Divf(v,h);
}
Complex2 Complex2_OfAngle(double a){
    return (Complex2){ cos(a),sin(a) };
}
double Complex2_AngleOf(Complex2 v){
    return atan2(v.i,v.r);
}

#endif