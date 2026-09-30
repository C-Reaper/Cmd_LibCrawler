#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../Container/Vector.h"

#include "Math.h"
#include "Line.h"
#include "Ray.h"
#include "Triangle.h"
#include "Poly.h"
#include "Rect.h"
#include "Circle.h"
#include "ConvexShape.h"

//  |           |           |           |           |           |           |           |           |
//  |           |  POINT    |  RAY      |  LINE     |  CIRCLE   |  RECT     |  TRIANGLE |  POLYGON  |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  POINT    |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  RAY      |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  LINE     |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  CIRCLE   |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  RECT     |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  TRIANGLE |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
//  |           |           |           |           |           |           |           |           |
//  |  POLYGON  |           |           |           |           |           |           |           |
//  |           |           |           |           |           |           |           |           |
//  |-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|


Vec2 Nearest_Rect_Circle(Rect r,Circle c){
    Vec2 N = {
        F32_Clamp(c.p.x,r.p.x,r.p.x + r.d.x),
        F32_Clamp(c.p.y,r.p.y,r.p.y + r.d.y),
    };
    return N;
}

char Overlap_Ray_Point(Ray r,Point p){
    float y = r.m * p.x + r.t;
    return p.y==y;
}
char Overlap_Line_Point(Line l,Point p){
    Vec2 left1 = l.s.x<l.e.x?l.s:l.e;
    Vec2 right1 = l.s.x<l.e.x?l.e:l.s;
    return Overlap_Ray_Point(Ray_ByLine(l),p) && p.x>=left1.x && p.x <=right1.x;
}
char Overlap_Circle_Point(Circle c,Vec2 p){
    float d = Vec2_Mag(Vec2_Sub(p,c.p));
    return d < c.r;
}
char Overlap_Circle_Circle(Circle c1,Circle c2){
    float d = Vec2_Mag(Vec2_Sub(c2.p,c1.p));
    return d < (c1.r + c2.r);
}
char Overlap_Triangle_Point(Triangle c,Vec2 p){
   	float d1 = Point_Sign(p,c.p1,c.p2);
    float d2 = Point_Sign(p,c.p2,c.p3);
    float d3 = Point_Sign(p,c.p3,c.p1);

    char bNeg = (d1 < 0.0f) || (d2 < 0.0f) || (d3 < 0.0f);
    char bPos = (d1 > 0.0f) || (d2 > 0.0f) || (d3 > 0.0f);

    return !(bNeg && bPos);
}
char Overlap_Triangle_Triangle(Triangle c1,Triangle c2){
    return Overlap_Triangle_Point(c1,c2.p1) || Overlap_Triangle_Point(c1,c2.p2) || Overlap_Triangle_Point(c1,c2.p3) ||
		   Overlap_Triangle_Point(c2,c1.p1) || Overlap_Triangle_Point(c2,c1.p2) || Overlap_Triangle_Point(c2,c1.p3);
}
char Overlap_Rect_Point(Rect r,Vec2 p){
	return p.x>=r.p.x && p.x<r.p.x+r.d.x && p.y>=r.p.y && p.y<r.p.y+r.d.y;
}
char Overlap_Rect_Line(Rect r,Line l){
	return Overlap_Rect_Point(r,l.s) || Overlap_Rect_Point(r,l.e);
}
char Overlap_Rect_Rect(Rect r1,Rect r2){
	return !(r1.p.x<r2.p.x-r1.d.x || r1.p.y<r2.p.y-r1.d.y || r1.p.x>r2.p.x+r2.d.x || r1.p.y>r2.p.y+r2.d.y);
}
char Overlap_Rect_Circle(Rect r,Circle c){
    Vec2 N = {
        F32_Clamp(c.p.x,r.p.x,r.p.x + r.d.x),
        F32_Clamp(c.p.y,r.p.y,r.p.y + r.d.y),
    };
    return Vec2_Mag(Vec2_Sub(N,c.p)) < c.r;
}
char Overlap_ConvexShape_ConvexShape(ConvexShape* cs1,ConvexShape* cs2){
    return ConvexShape_OverlapSat(cs1,cs2);
}

char Contains_Circle_Circle(Circle c1,Circle c2){
	float d = Vec2_Mag(Vec2_Sub(c2.p,c1.p));
    return d<F32_Abs(c1.r-c2.r);
}
char Contains_Rect_Line(Rect r,Line l){
	return Overlap_Rect_Point(r,l.s) && Overlap_Rect_Point(r,l.e);
}
char Contains_Rect_Rect(Rect r1,Rect r2){
	return (r2.p.x>=r1.p.x && r2.p.x+r2.d.x<r1.p.x+r1.d.x && r2.p.y>=r1.p.y && r2.p.y+r2.d.y<r1.p.y+r1.d.y);
}
char Contains_Triangle_Rect(Triangle c,Rect r){
    Vec2 p1 = r.p;
	Vec2 p2 = Vec2_Add(r.p,(Vec2){ r.d.x,0.0f });
	Vec2 p3 = Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y });
	Vec2 p4 = Vec2_Add(r.p,r.d);
	
	if(Overlap_Triangle_Point(c,p1) &&
	   Overlap_Triangle_Point(c,p2) &&
	   Overlap_Triangle_Point(c,p3) &&
	   Overlap_Triangle_Point(c,p4)) return 1;
    return 0;
}
char Contains_Triangle_Triangle(Triangle c1,Triangle c2){
    return Overlap_Triangle_Point(c1,c2.p1) && Overlap_Triangle_Point(c1,c2.p2) && Overlap_Triangle_Point(c1,c2.p3);
}
char Contains_Triangle_Line(Triangle c,Line l){
    if(Overlap_Triangle_Point(c,l.s) && Overlap_Triangle_Point(c,l.e)) return 1;
    return 0;
}


Vec2 Intersections_Ray_Ray(Ray r1,Ray r2){
    if(r1.m==r2.m) return (Vec2){ POINT_INVALID,POINT_INVALID };

    float Ix = (r2.t - r1.t) / (r1.m - r2.m);
    Vec2 Ip = { Ix,r1.m * Ix + r1.t };
    return Ip;
}

Vec2 Intersections_Line_Ray(Line l,Ray r){
    Vec2 Out = { 0.0f,0.0f };
    if(l.e.x==l.s.x)        Out = (Vec2){ l.s.x,Ray_Y(r,l.s.x) };
    else if(l.e.y==l.s.y)   Out = (Vec2){ Ray_X(r,l.s.y),l.s.y };
    else                    Out = Intersections_Ray_Ray(Ray_ByLine(l),r);

    Vec2 left = l.s.x<l.e.x?l.s:l.e;
    Vec2 right = l.s.x<l.e.x?l.e:l.s;
    Vec2 up = l.s.y<l.e.y?l.s:l.e;
    Vec2 down = l.s.y<l.e.y?l.e:l.s;
    
    if(!(Out.x>=left.x && Out.x<=right.x && Out.y>=up.y && Out.y<=down.y)) return (Vec2){ POINT_INVALID,POINT_INVALID };
    return Out;
}
Vec2 Intersections_Line_Line(Line l1,Line l2){
    Vec2 Out = { 0.0f,0.0f };
    if(l1.e.x==l1.s.x && l2.e.x==l2.s.x) return (Vec2){ POINT_INVALID,POINT_INVALID };
    else if(l1.e.x==l1.s.x) Out = (Vec2){ l1.s.x,Ray_Y(Ray_ByLine(l2),l1.s.x) };
    else if(l2.e.x==l2.s.x) Out = (Vec2){ l2.s.x,Ray_Y(Ray_ByLine(l1),l2.s.x) };
    else if(l1.e.y==l1.s.y) Out = (Vec2){ Ray_X(Ray_ByLine(l2),l1.s.y),l1.s.y };
    else if(l2.e.y==l2.s.y) Out = (Vec2){ Ray_X(Ray_ByLine(l1),l2.s.y),l2.s.y };
    else                    Out = Intersections_Ray_Ray(Ray_ByLine(l1),Ray_ByLine(l2));

    Vec2 left1 = l1.s.x<l1.e.x?l1.s:l1.e;
    Vec2 right1 = l1.s.x<l1.e.x?l1.e:l1.s;
    Vec2 up1 = l1.s.y<l1.e.y?l1.s:l1.e;
    Vec2 down1 = l1.s.y<l1.e.y?l1.e:l1.s;
    
    Vec2 left2 = l2.s.x<l2.e.x?l2.s:l2.e;
    Vec2 right2 = l2.s.x<l2.e.x?l2.e:l2.s;
    Vec2 up2 = l2.s.y<l2.e.y?l2.s:l2.e;
    Vec2 down2 = l2.s.y<l2.e.y?l2.e:l2.s;

    if(!(Out.x>=left1.x && Out.x<=right1.x && Out.x>=left2.x && Out.x<=right2.x &&
         Out.y>=up1.y && Out.y<=down1.y && Out.y>=up2.y && Out.y<=down2.y))
        return (Vec2){ POINT_INVALID,POINT_INVALID };
    return Out;
}

Vector Intersections_Circle_Ray(Circle c,Ray r){
    Vector Ips = Vector_New(sizeof(Vec2));
    
    Vec2 P = { 0.0f,Ray_Y(r,0.0f) };
    Vec2 V = Vec2_Norm((Vec2){ 1.0f,r.m });
    Vec2 U = Vec2_Sub(c.p,P);
    Vec2 U1 = Vec2_Mulf(V,Vec2_Dot(U,V));
    Vec2 U2 = Vec2_Sub(U,U1);
    float d = Vec2_Mag(U2);
    if(d<c.r){
        float m = sqrtf(c.r*c.r - d*d);
        Vec2 P1 = Vec2_Add(P,Vec2_Add(U1,Vec2_Mulf(V,m)));
        Vec2 P2 = Vec2_Add(P,Vec2_Sub(U1,Vec2_Mulf(V,m)));
        Vector_Push(&Ips,&P1);
        Vector_Push(&Ips,&P2);
    }
    return Ips;
}
Vector Intersections_Circle_Line(Circle c,Line l){
    Vector Ips = Vector_New(sizeof(Vec2));
    
    Vec2 P1;
    Vec2 P2;
    if(l.s.x==l.e.x){
        if(l.s.x<c.p.x-c.r || l.s.x>c.p.x+c.r) return Ips;
        float dx = c.p.x - l.s.x;
        float y = F32_Sqrt(c.r*c.r - dx*dx);
        P1 = (Vec2){ l.s.x,c.p.y-y };
        P2 = (Vec2){ l.s.x,c.p.y+y };

        Vec2 left = l.s.x<l.e.x?l.s:l.e;
        Vec2 right = l.s.x<l.e.x?l.e:l.s;
        Vec2 up = l.s.y<l.e.y?l.s:l.e;
        Vec2 down = l.s.y<l.e.y?l.e:l.s;
    
        if(P1.x>=left.x && P1.x<=right.x && P1.y>=up.y && P1.y<=down.y) Vector_Push(&Ips,&P1);
        if(P2.x>=left.x && P2.x<=right.x && P2.y>=up.y && P2.y<=down.y) Vector_Push(&Ips,&P2);
    }else{
        Ray r = Ray_ByLine(l);
        Vec2 P = { 0.0f,Ray_Y(r,0.0f) };
        Vec2 V = Vec2_Norm((Vec2){ 1.0f,r.m });
        Vec2 U = Vec2_Sub(c.p,P);
        Vec2 U1 = Vec2_Mulf(V,Vec2_Dot(U,V));
        Vec2 U2 = Vec2_Sub(U,U1);
        float d = Vec2_Mag(U2);
        if(d<c.r){
            float m = sqrtf(c.r*c.r - d*d);
            P1 = Vec2_Add(P,Vec2_Add(U1,Vec2_Mulf(V,m)));
            P2 = Vec2_Add(P,Vec2_Sub(U1,Vec2_Mulf(V,m)));

            Vec2 left = l.s.x<l.e.x?l.s:l.e;
            Vec2 right = l.s.x<l.e.x?l.e:l.s;
            Vec2 up = l.s.y<l.e.y?l.s:l.e;
            Vec2 down = l.s.y<l.e.y?l.e:l.s;

            if(P1.x>=left.x && P1.x<=right.x && P1.y>=up.y && P1.y<=down.y) Vector_Push(&Ips,&P1);
            if(P2.x>=left.x && P2.x<=right.x && P2.y>=up.y && P2.y<=down.y) Vector_Push(&Ips,&P2);
        }
    }
    return Ips;
}
Vector Intersections_Circle_Rect(Circle c,Rect r){
    Vector Ips = Vector_New(sizeof(Vec2));
    
    Line lines[4] = { { r.p,Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }) },
                      { r.p,Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }) },
                      { Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) },
                      { Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) }
                    }; 

    for(int i = 0;i<4;i++){
        Vector Points = Intersections_Circle_Line(c,lines[i]);
        for(int j = 0;j<Points.size;j++){
            Vec2 p = *(Vec2*)Vector_Get(&Points,j);
            if(Point_Valid(p)) Vector_Push(&Ips,&p);
        }
        Vector_Free(&Points);
    }
    return Ips;
}
Vector Intersections_Circle_Circle(Circle c1,Circle c2){
    Vector Ips = Vector_New(sizeof(Vec2));
    float d = Vec2_Mag(Vec2_Sub(c2.p,c1.p));
    if(d>c1.r+c2.r)             return Ips;
    if(d<F32_Abs(c1.r-c2.r))    return Ips;
    Vec2 D = Vec2_Norm(Vec2_Sub(c2.p,c1.p));
    float a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2.0f * d);
    Vec2 P = Vec2_Add(c1.p,Vec2_Mulf(D,a));
    float h = F32_Sqrt(c1.r * c1.r - a * a);
    Vec2 P1 = Vec2_Add(P,Vec2_Perp(Vec2_Mulf(D,h)));
    Vec2 P2 = Vec2_Add(P,Vec2_PerpA(Vec2_Mulf(D,h)));
    Vector_Push(&Ips,&P1);
    Vector_Push(&Ips,&P2);
    return Ips;
}
Vector Intersections_Circle_Triangle(Circle c,Triangle t){
    Vector Ips = Vector_New(sizeof(Vec2));
    Line lines[3] = { {t.p1,t.p2},{t.p2,t.p3},{t.p3,t.p1} }; 
    for(int i = 0;i<3;i++){
        Vector Points = Intersections_Circle_Line(c,lines[i]);
        for(int j = 0;j<Points.size;j++){
            Vec2 p = *(Vec2*)Vector_Get(&Points,j);
            if(Point_Valid(p)) Vector_Push(&Ips,&p);
        }
        Vector_Free(&Points);
    }
    return Ips;
}

Vector Intersections_Triangle_Ray(Triangle c,Ray r){
    Vector Ips = Vector_New(sizeof(Vec2));
	Line l1 = { c.p1,c.p2 };
	Line l2 = { c.p2,c.p3 };
	Line l3 = { c.p3,c.p1 };
	Vec2 p1 = Intersections_Line_Ray(l1,r);
	Vec2 p2 = Intersections_Line_Ray(l2,r);
	Vec2 p3 = Intersections_Line_Ray(l3,r);
	if(Point_Valid(p1)) Vector_Push(&Ips,(Vec2[]){ p1 });
	if(Point_Valid(p2)) Vector_Push(&Ips,(Vec2[]){ p2 });
	if(Point_Valid(p3)) Vector_Push(&Ips,(Vec2[]){ p3 });
    return Ips;
}
Vector Intersections_Triangle_Line(Triangle c,Line l){
    Vector Ips = Vector_New(sizeof(Vec2));
	Line l1 = { c.p1,c.p2 };
	Line l2 = { c.p2,c.p3 };
	Line l3 = { c.p3,c.p1 };
	Vec2 p1 = Intersections_Line_Line(l1,l);
	Vec2 p2 = Intersections_Line_Line(l2,l);
	Vec2 p3 = Intersections_Line_Line(l3,l);
	if(Point_Valid(p1)) Vector_Push(&Ips,(Vec2[]){ p1 });
	if(Point_Valid(p2)) Vector_Push(&Ips,(Vec2[]){ p2 });
	if(Point_Valid(p3)) Vector_Push(&Ips,(Vec2[]){ p3 });
    return Ips;
}
Vector Intersections_Triangle_Rect(Triangle c,Rect r){
    Vector Ips = Vector_New(sizeof(Vec2));
	Line lines[4] = { { r.p,Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }) },
                      { r.p,Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }) },
                      { Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) },
                      { Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) }
                    }; 

	for(int i = 0;i<4;i++){
        Vector Points = Intersections_Triangle_Line(c,lines[i]);
        for(int j = 0;j<Points.size;j++){
            Vec2 p = *(Vec2*)Vector_Get(&Points,j);
            if(Point_Valid(p)) Vector_Push(&Ips,&p);
        }
        Vector_Free(&Points);
    }
    return Ips;
}
Vector Intersections_Triangle_Circle(Triangle t,Circle c){
    return Intersections_Circle_Triangle(c,t);
}
Vector Intersections_Triangle_Triangle(Triangle c1,Triangle c2){
    Vector Ips = Vector_New(sizeof(Vec2));
	Line l1[3] = {{ c1.p1,c1.p2 },
				  { c1.p2,c1.p3 },
				  { c1.p3,c1.p1 }};
	Line l2[3] = {{ c2.p1,c2.p2 },
				  { c2.p2,c2.p3 },
				  { c2.p3,c2.p1 }};
	
	for(int i = 0;i<3;i++){
		for(int j = 0;j<3;j++){
			Vec2 p = Intersections_Line_Line(l1[i],l2[j]);
			if(Point_Valid(p)) Vector_Push(&Ips,&p);
		}
	}
    return Ips;
}

Vector Intersections_Rect_Ray(Rect r,Ray ray){
	Line l1 = { r.p,Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }) };
    Line l2 = { r.p,Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }) };
    Line l3 = { Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) };
    Line l4 = { Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) };
    
    Vector Ips = Vector_New(sizeof(Vec2));
    Vec2 p = Intersections_Line_Ray(l1,ray);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Ray(l2,ray);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Ray(l3,ray);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Ray(l4,ray);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    return Ips;
}
Vector Intersections_Rect_Line(Rect r,Line l){
	Line l1 = { r.p,Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }) };
    Line l2 = { r.p,Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }) };
    Line l3 = { Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) };
    Line l4 = { Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) };
    
    Vector Ips = Vector_New(sizeof(Vec2));
    Vec2 p = Intersections_Line_Line(l1,l);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Line(l2,l);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Line(l3,l);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    p = Intersections_Line_Line(l4,l);
    if(Point_Valid(p)) Vector_Push(&Ips,&p);
    return Ips;
}
Vector Intersections_Rect_Rect(Rect r1,Rect r2){
	Line r1Lines[4] = {
        { r1.p,Vec2_Add(r1.p,(Vec2){ r1.d.x,0.0f }) },
        { r1.p,Vec2_Add(r1.p,(Vec2){ 0.0f,r1.d.y }) },
        { Vec2_Add(r1.p,(Vec2){ r1.d.x,0.0f }),Vec2_Add(r1.p,(Vec2){ r1.d.x,r1.d.y }) },
        { Vec2_Add(r1.p,(Vec2){ 0.0f,r1.d.y }),Vec2_Add(r1.p,(Vec2){ r1.d.x,r1.d.y }) }
    };
    Line r2Lines[4] = {
        { r2.p,Vec2_Add(r2.p,(Vec2){ r2.d.x,0.0f }) },
        { r2.p,Vec2_Add(r2.p,(Vec2){ 0.0f,r2.d.y }) },
        { Vec2_Add(r2.p,(Vec2){ r2.d.x,0.0f }),Vec2_Add(r2.p,(Vec2){ r2.d.x,r2.d.y }) },
        { Vec2_Add(r2.p,(Vec2){ 0.0f,r2.d.y }),Vec2_Add(r2.p,(Vec2){ r2.d.x,r2.d.y }) }
    };
    
    Vector Ips = Vector_New(sizeof(Vec2));
    for(int i = 0;i<4;i++){
        for(int j = 0;j<4;j++){
            Vec2 p = Intersections_Line_Line(r1Lines[i],r2Lines[j]);
            if(Point_Valid(p)) Vector_Push(&Ips,&p);
        }
    }
    return Ips;
}
Vector Intersections_Rect_Circle(Rect r,Circle c){
	return Intersections_Circle_Rect(c,r);
}
Vector Intersections_Rect_Triangle(Rect r,Triangle t){
	return Intersections_Triangle_Rect(t,r);
}


char Overlap_Rect_Ray(Rect r,Ray l){
	Vector v = Intersections_Rect_Ray(r,l);
    int Size = v.size;
    Vector_Free(&v);
    return Size>0;
}
char Overlap_Triangle_Line(Triangle c,Line l){
    Line l1 = { c.p1,c.p2 };
	Line l2 = { c.p2,c.p3 };
	Line l3 = { c.p3,c.p1 };
	Vec2 p1 = Intersections_Line_Line(l1,l1);
	Vec2 p2 = Intersections_Line_Line(l2,l1);
	Vec2 p3 = Intersections_Line_Line(l3,l1);
	if(Point_Valid(p1)) return 1;
	if(Point_Valid(p2)) return 1;
	if(Point_Valid(p3)) return 1;
    return 0;
}
char Overlap_Triangle_Rect(Triangle c,Rect r){
    Line l1[3] = {{ c.p1,c.p2 },
				  { c.p2,c.p3 },
				  { c.p3,c.p1 }};
	
	Line l2[4] = { { r.p,Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }) },
                   { r.p,Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }) },
                   { Vec2_Add(r.p,(Vec2){ r.d.x,0.0f }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) },
                   { Vec2_Add(r.p,(Vec2){ 0.0f,r.d.y }),Vec2_Add(r.p,(Vec2){ r.d.x,r.d.y }) }
                 }; 
	
	for(int i = 0;i<3;i++){
        for(int j = 0;j<4;j++){
            Vec2 p = Intersections_Line_Line(l1[i],l2[j]);
            if(Point_Valid(p)) return 1;
        }
    }
    return 0;
}
char Overlap_Triangle_Ray(Triangle c,Ray r){
    Line l1 = { c.p1,c.p2 };
	Line l2 = { c.p2,c.p3 };
	Line l3 = { c.p3,c.p1 };
	Vec2 p1 = Intersections_Line_Ray(l1,r);
	Vec2 p2 = Intersections_Line_Ray(l2,r);
	Vec2 p3 = Intersections_Line_Ray(l3,r);
	if(Point_Valid(p1)) return 1;
	if(Point_Valid(p2)) return 1;
	if(Point_Valid(p3)) return 1;
    return 0;
}
char Overlap_Rect_Rect_V(Rect r1,Rect r2,Vec2 np){
    const Vec2 m1 = Vec2_Add(r1.p,Vec2_Mulf(r1.d,0.5f));
    const Vec2 m2 = Vec2_Add(r2.p,Vec2_Mulf(r2.d,0.5f));
    const Rect ex = Rect_New(m1,Vec2_Add(r1.d,r2.d));
    const Line l = Line_New(m2,Vec2_Add(np,Vec2_Mulf(r2.d,0.5f)));
    Vector Ips = Intersections_Rect_Line(ex,l);
    const char b = Ips.size>0;
    Vector_Free(&Ips);
    return b;
}

Vec2 Nearest_Ray_Point(Ray r,Point p){
    Ray r1 = r;

    Vec2 d = Vec2_Perp(Vec2_Norm((Vec2){ 1.0f,r.m }));
    float m = d.y / d.x;
    float t = -m * p.x + p.y;
    Ray r2 = Ray_New(m,t);
    
    return Intersections_Ray_Ray(r1,r2);
}
Vec2 Nearest_Line_Point(Line l,Point p){
    Ray r = Ray_ByLine(l);
    Vec2 n = Nearest_Ray_Point(r,p);

    Vec2 left = l.s.x<l.e.x?l.s:l.e;
    Vec2 right = l.s.x<l.e.x?l.e:l.s;
    
    if(n.x<left.x)  n = left;
    if(n.x>right.x) n = right;
    return n;
}


typedef unsigned char Side;
#define SIDE_NONE       0U
#define SIDE_TOP        1U
#define SIDE_BOTTOM     2U
#define SIDE_LEFT       3U
#define SIDE_RIGHT      4U

Side Side_Rect_Point(Rect r,Vec2 p){
    Vec2 m = Vec2_Add(r.p,Vec2_Mulf(r.d,0.5f));
    Vec2 d = Vec2_Div(Vec2_Sub(m,p),r.d);

    if(F32_Abs(d.x)>F32_Abs(d.y)){
        if(d.x>0.0f)    return SIDE_LEFT;
        else            return SIDE_RIGHT;
    }else{
        if(d.y>0.0f)    return SIDE_TOP;
        else            return SIDE_BOTTOM;
    }
}
Vec2 Side_Dir(Side s){
    switch(s){
        case SIDE_TOP:      return (Vec2){  0.0f,-1.0f };
        case SIDE_BOTTOM:   return (Vec2){  0.0f, 1.0f };
        case SIDE_LEFT:     return (Vec2){ -1.0f, 0.0f };
        case SIDE_RIGHT:    return (Vec2){  1.0f, 0.0f };
    }
    return (Vec2){ 0.0f, 0.0f };
}
Side Side_Rect_Rect(Rect r1,Rect r2){
    if(Overlap_Rect_Rect(r1,r2)){
        Vec2 m1 = Vec2_Add(r1.p,Vec2_Mulf(r1.d,0.5f));
        Vec2 m2 = Vec2_Add(r2.p, Vec2_Mulf(r2.d, 0.5f));

        Vec2 l = Vec2_Add(r1.d,r2.d);
        Vec2 d = Vec2_Sub(m2,m1);
        d = Vec2_Div(d,l);

        if(F32_Abs(d.x)>F32_Abs(d.y)){
            if(d.x>0.0f)    return SIDE_LEFT;
            else            return SIDE_RIGHT;
        }else{
            if(d.y>0.0f)    return SIDE_TOP;
            else            return SIDE_BOTTOM;
        }
    }
    return SIDE_NONE;
}

void Resolve_Circle_Point(Circle* c,Vec2 p){
    Vec2 d = Vec2_Sub(c->p,p);
	float h = Vec2_Mag(d);
    float Overlap = c->r - h;
    c->p = Vec2_Sub(c->p,Vec2_Mulf(Vec2_Norm(d),Overlap));
}  
void Resolve_Circle_Circle(Circle* c1,Circle* c2){
    Vec2 d = Vec2_Sub(c1->p,c2->p);
	float h = Vec2_Mag(d);
	float Overlap = 0.5f * (h - c1->r - c2->r );
	c1->p = Vec2_Sub(c1->p,Vec2_Divf(Vec2_Mulf(d,Overlap),h));
	c2->p = Vec2_Add(c2->p,Vec2_Divf(Vec2_Mulf(d,Overlap),h));
}
void Resolve_Triangle_Triangle(Triangle* c1,Triangle* c2){
    
}
Side Resolve_Rect_Rect(Rect* r1,Rect r2){
    if(Overlap_Rect_Rect(*r1,r2)){
        Vec2 m1 = Vec2_Add(r1->p,Vec2_Mulf(r1->d,0.5f));
        Vec2 m2 = Vec2_Add(r2.p, Vec2_Mulf(r2.d, 0.5f));

        Vec2 l = Vec2_Add(r1->d,r2.d);
        Vec2 d = Vec2_Sub(m2,m1);
        d = Vec2_Div(d,l);

        if(F32_Abs(d.x)>F32_Abs(d.y)){
            if(d.x>0.0f){
                m1.x = m2.x - l.x * 0.5f;
                r1->p.x = m1.x - r1->d.x * 0.5f;
                return SIDE_LEFT;
            }else{
                m1.x = m2.x + l.x * 0.5f;
                r1->p.x = m1.x - r1->d.x * 0.5f;
                return SIDE_RIGHT;
            }
        }else{
            if(d.y>0.0f){
                m1.y = m2.y - l.y * 0.5f;
                r1->p.y = m1.y - r1->d.y * 0.5f;
                return SIDE_TOP;
            }else{
                m1.y = m2.y + l.y * 0.5f;
                r1->p.y = m1.y - r1->d.y * 0.5f;
                return SIDE_BOTTOM;
            }
        }
    }
    return SIDE_NONE;
}
void Resolve_Rect_Rect_Side(Rect* r1,Rect r2,Side s){
    const Vec2 m2 = Vec2_Add(r2.p, Vec2_Mulf(r2.d, 0.5f));
    const Vec2 l = Vec2_Add(r1->d,r2.d);

    if(s==SIDE_LEFT)        r1->p.x = (m2.x - l.x * 0.5f) - r1->d.x * 0.5f;
    else if(s==SIDE_RIGHT)  r1->p.x = (m2.x + l.x * 0.5f) - r1->d.x * 0.5f;
    else if(s==SIDE_TOP)    r1->p.y = (m2.y - l.y * 0.5f) - r1->d.y * 0.5f;  
    else if(s==SIDE_BOTTOM) r1->p.y = (m2.y + l.y * 0.5f) - r1->d.y * 0.5f;
}
void Resolve_Circle_Rect(Rect r,Circle* c,Vec2 v){
    if(Overlap_Rect_Circle(r,*c)){
        const Vec2 N = Nearest_Rect_Circle(r,*c);
        const float Overlap = c->r - Vec2_Mag(Vec2_Sub(N,c->p));
        
        const Vec2 NRay = Vec2_Sub(N,c->p);
        
        c->p = Vec2_Sub(c->p,Vec2_Mulf(Vec2_Norm(NRay),Overlap));
    }
}
void Resolve_ConvexShape_ConvexShape(ConvexShape* cs1,ConvexShape* cs2){
    ConvexShape_StaticSat(cs1,cs2);
}

void Resolve_Rect_Rect_V(Rect r1,Rect* r2,Vec2 np){
    const Vec2 m1 = Vec2_Add(r1.p,Vec2_Mulf(r1.d,0.5f));
    const Vec2 m2 = Vec2_Add(r2->p,Vec2_Mulf(r2->d,0.5f));
    const Rect ex = Rect_New(m1,Vec2_Add(r1.d,r2->d));
    
    const Line l = Line_New(m2,np);
    Vector Ips = Intersections_Rect_Line(ex,l);
    Vec2* closest = NULL;

    for(int i = 0;i<Ips.size;i++){
        Vec2* point = Vector_Get(&Ips,i);
        
        if(closest){
            if(Vec2_Mag2(Vec2_Sub(*point,m2))<Vec2_Mag2(Vec2_Sub(*closest,m2))){
                closest = point;
            }
        }else
            closest = point;
    }

    if(closest){
        //const Side s = Side_Rect_Point(ex,*closest);
        //const Vec2 v = Side_Dir(s);
        r2->p = Vec2_Add(Vec2_Sub(m1,Vec2_Mulf(r2->d,0.5f)),Vec2_Mulf(ex.d,0.5f));
    }

    Vector_Free(&Ips);
}

#endif // !GEOMETRY_Hd_glP