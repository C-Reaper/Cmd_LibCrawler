#ifndef LIB3D_CUBE_H
#define LIB3D_CUBE_H

#include "../Container/Vector.h"

#include "Float.h"
#include "Lib3D_Mathlib.h"


#define LIB3D_CUBE_SIDE_SOUTH	0
#define LIB3D_CUBE_SIDE_EAST	1
#define LIB3D_CUBE_SIDE_NORTH	2
#define LIB3D_CUBE_SIDE_WEST	3
#define LIB3D_CUBE_SIDE_TOP		4
#define LIB3D_CUBE_SIDE_BOTTOM	5

#define LIB3D_CUBE_SIDE_000		0
#define LIB3D_CUBE_SIDE_001		1
#define LIB3D_CUBE_SIDE_100		2
#define LIB3D_CUBE_SIDE_101		3
#define LIB3D_CUBE_SIDE_002		4
#define LIB3D_CUBE_SIDE_102		5
#define LIB3D_CUBE_SIDE_200		6
#define LIB3D_CUBE_SIDE_201		7
#define LIB3D_CUBE_SIDE_202		8
#define LIB3D_CUBE_SIDE_010		9
#define LIB3D_CUBE_SIDE_011		10
#define LIB3D_CUBE_SIDE_110		11
#define LIB3D_CUBE_SIDE_111		12
#define LIB3D_CUBE_SIDE_012		13
#define LIB3D_CUBE_SIDE_112		14
#define LIB3D_CUBE_SIDE_210		15
#define LIB3D_CUBE_SIDE_211		16
#define LIB3D_CUBE_SIDE_212		17
#define LIB3D_CUBE_SIDE_020		18
#define LIB3D_CUBE_SIDE_021		19
#define LIB3D_CUBE_SIDE_120		20
#define LIB3D_CUBE_SIDE_121		21
#define LIB3D_CUBE_SIDE_022		22
#define LIB3D_CUBE_SIDE_122		23
#define LIB3D_CUBE_SIDE_220		24
#define LIB3D_CUBE_SIDE_221		25
#define LIB3D_CUBE_SIDE_222		26

#define LIB3D_CUBE_S0_SOUTH		(1U << 0)
#define LIB3D_CUBE_S1_SOUTH		(1U << 1)
#define LIB3D_CUBE_S0_EAST		(1U << 2)
#define LIB3D_CUBE_S1_EAST		(1U << 3)
#define LIB3D_CUBE_S0_NORTH		(1U << 4)
#define LIB3D_CUBE_S1_NORTH		(1U << 5)
#define LIB3D_CUBE_S0_WEST		(1U << 6)
#define LIB3D_CUBE_S1_WEST		(1U << 7)
#define LIB3D_CUBE_S0_TOP		(1U << 8)
#define LIB3D_CUBE_S1_TOP		(1U << 9)
#define LIB3D_CUBE_S0_BOTTOM	(1U << 10)
#define LIB3D_CUBE_S1_BOTTOM	(1U << 11)

Vec3D Lib3D_Cube_NS(int s){
	switch (s){
	case LIB3D_CUBE_SIDE_SOUTH: 	return (Vec3D){ 0.0f, 0.0f,-1.0f,1.0f };
	case LIB3D_CUBE_SIDE_EAST: 		return (Vec3D){ 1.0f, 0.0f, 0.0f,1.0f };
	case LIB3D_CUBE_SIDE_NORTH: 	return (Vec3D){ 0.0f, 0.0f, 1.0f,1.0f };
	case LIB3D_CUBE_SIDE_WEST: 		return (Vec3D){-1.0f, 0.0f, 0.0f,1.0f };
	case LIB3D_CUBE_SIDE_TOP: 		return (Vec3D){ 0.0f, 1.0f, 0.0f,1.0f };
	case LIB3D_CUBE_SIDE_BOTTOM: 	return (Vec3D){ 0.0f,-1.0f, 0.0f,1.0f };
	}
	return (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
}
int Lib3D_Cube_SO(int s){
	switch (s){
	case LIB3D_CUBE_SIDE_SOUTH: 	return LIB3D_CUBE_SIDE_NORTH;
	case LIB3D_CUBE_SIDE_EAST: 		return LIB3D_CUBE_SIDE_WEST;
	case LIB3D_CUBE_SIDE_NORTH: 	return LIB3D_CUBE_SIDE_SOUTH;
	case LIB3D_CUBE_SIDE_WEST: 		return LIB3D_CUBE_SIDE_EAST;
	case LIB3D_CUBE_SIDE_TOP: 		return LIB3D_CUBE_SIDE_BOTTOM;
	case LIB3D_CUBE_SIDE_BOTTOM:	return LIB3D_CUBE_SIDE_TOP;
	}
	return LIB3D_CUBE_SIDE_SOUTH;
}
Vec3D Lib3D_Cube_AS(int s){
	switch (s){
		case LIB3D_CUBE_SIDE_000: return (Vec3D){-1.0f,-1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_001: return (Vec3D){-1.0f,-1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_100: return (Vec3D){ 0.0f,-1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_101: return (Vec3D){ 0.0f,-1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_002: return (Vec3D){-1.0f,-1.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_102: return (Vec3D){ 0.0f,-1.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_200: return (Vec3D){ 1.0f,-1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_201: return (Vec3D){ 1.0f,-1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_202: return (Vec3D){ 1.0f,-1.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_010: return (Vec3D){-1.0f, 0.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_011: return (Vec3D){-1.0f, 0.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_110: return (Vec3D){ 0.0f, 0.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_111: return (Vec3D){ 0.0f, 0.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_012: return (Vec3D){-1.0f, 0.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_112: return (Vec3D){ 0.0f, 0.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_210: return (Vec3D){ 1.0f, 0.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_211: return (Vec3D){ 1.0f, 0.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_212: return (Vec3D){ 1.0f, 0.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_020: return (Vec3D){-1.0f, 1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_021: return (Vec3D){-1.0f, 1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_120: return (Vec3D){ 0.0f, 1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_121: return (Vec3D){ 0.0f, 1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_022: return (Vec3D){-1.0f, 1.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_122: return (Vec3D){ 0.0f, 1.0f, 1.0f,1.0f };
		case LIB3D_CUBE_SIDE_220: return (Vec3D){ 1.0f, 1.0f,-1.0f,1.0f };
		case LIB3D_CUBE_SIDE_221: return (Vec3D){ 1.0f, 1.0f, 0.0f,1.0f };
		case LIB3D_CUBE_SIDE_222: return (Vec3D){ 1.0f, 1.0f, 1.0f,1.0f };
	}
	return (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
}

Tri3D Lib3D_Tri(Vec3D p,Vec3D d,unsigned int tri,unsigned int c1,unsigned int c2){
	Tri3D tris[12] = {
		// SOUTH
		{ { { 0.0f,0.0f,0.0f,1.0f },{ 0.0f,1.0f,0.0f,1.0f },{ 1.0f,1.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f,0.0f,1.0f },{ 1.0f,0.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
		// EAST                                                      
		{ { { 1.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 1.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
		// NORTH                                                     
		{ { { 1.0f,0.0f,1.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f },{ 0.0f,1.0f,1.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 1.0f,0.0f,1.0f,1.0f },{ 0.0f,1.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
		// WEST                                                      
		{ { { 0.0f,0.0f,1.0f,1.0f },{ 0.0f,1.0f,1.0f,1.0f },{ 0.0f,1.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 0.0f,0.0f,1.0f,1.0f },{ 0.0f,1.0f,0.0f,1.0f },{ 0.0f,0.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
		// TOP                                                       
		{ { { 0.0f,1.0f,0.0f,1.0f },{ 0.0f,1.0f,1.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 0.0f,1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f },{ 1.0f,1.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
		// BOTTOM                                                    
		{ { { 1.0f,0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f,1.0f },{ 0.0f,0.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f },{ 1.0f,0.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c1},1.0f} },
		{ { { 1.0f,0.0f,1.0f,1.0f },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,0.0f,0.0f,1.0f } },{ { 0.0f,1.0f,1.0f },{ 1.0f,0.0f,1.0f },{ 1.0f,1.0f,1.0f } },{ 0.0f,0.0f,0.0f,1.0f },{{c2},1.0f} },
	};
	Tri3D_Mul(tris + tri,d);
	Tri3D_Offset(tris + tri,p);
	return tris[tri];
}
void Lib3D_Sides(Vector* trisOut,short sides,Vec3D p,Vec3D d,unsigned int c1,unsigned int c2){
	for(int i = 0;i < 12;i++){
		if(sides & (1U << i)) continue;

		Tri3D tri = Lib3D_Tri(p,d,i,c1,c2);
		Tri3D_CalcNorm(&tri);
		Vector_Push(trisOut,&tri);
	}
}
void Lib3D_Cube(Vector* trisOut,Vec3D p,Vec3D d,unsigned int c1,unsigned int c2){
	Lib3D_Sides(trisOut,0,p,d,c1,c2);
}
void Lib3D_Plane(Vector* trisOut,Vec3D p,Vec3D d,unsigned int plane,unsigned int c1,unsigned int c2){
	const unsigned int tri1 = 1 << ((plane << 1) + 0);
	const unsigned int tri2 = 1 << ((plane << 1) + 1);
	Lib3D_Sides(trisOut,~(tri1 | tri2),p,d,c1,c2);
}
Vector Lib3D_Cube_Make(Vec3D p,Vec3D d,unsigned int c1,unsigned int c2){
	Vector trisOut = Vector_New(sizeof(Tri3D));
	Lib3D_Cube(&trisOut,p,d,c1,c2);
	return trisOut;
}

#endif //!3D_CUBE_H