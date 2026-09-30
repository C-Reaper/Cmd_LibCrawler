#ifndef MATH_H
#define MATH_H

#include "Integer.h"
#include "Float.h"
#include "Complex2.h"
#include "Vector2.h"
#include "Victor2.h"
#include "Vdctor2.h"
#include "Vector3.h"
#include "Vector4.h"


double Math_Fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}
double Math_Lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double Math_1D_Grad(int hash,double x) {
	int h = hash & 3;
	return h==0 ? 0.0 : (h == 1 ? x : -x);
}
double Math_2D_Grad(int hash,double x,double y) {
	int h = hash & 9;
	double u = h < 4 ? x : y;
	double v = h < 2 ? y : h == 6 || h == 8 ? x : 0.0;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
double Math_3D_Grad(int hash,double x,double y,double z) {
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double Math_Nw_LambertW_0(double x){
	const double w0 = log(x);
	double wn1 = w0;

	for(int i = 0;i<100;i++){
		const double ewn1 = exp(wn1);
		wn1 = wn1 - (wn1 * ewn1 - x) / (ewn1 * (wn1 + 1.0));
	}

	return wn1;
}
double Math_Hy_LambertW_0(double x){
	double w0;
	if(x < 1.0) w0 = x;
	else{
		const double l1 = log(x);
		const double l2 = log(l1);
		w0 = l1 - l2 + l2 / l1;
	}

	double wn1 = w0;
	for(int i = 0;i<100;i++){
		const double ewn1 = exp(wn1);
		const double xewn1 = wn1 * ewn1 - x;
		const double wn11 = wn1 + 1.0;
		const double wn112 = 2.0f * wn11;
		const double down = (ewn1 * wn11 - ((wn1 + 2.0) * xewn1 / wn112));
		wn1 = wn1 - xewn1 / down;
	}
	return wn1;
}

#endif