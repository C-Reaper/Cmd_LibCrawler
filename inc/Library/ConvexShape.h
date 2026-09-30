#ifndef CONVEXSHAPE_H
#define CONVEXSHAPE_H

#include "Float.h"
#include "Vector2.h"
#include "Line.h"

#include "../Container/Vector.h"

typedef struct ConvexShape {
    Vector points;
    Vector targets;
    Vec2 middle;
} ConvexShape;

ConvexShape ConvexShape_New(Vec2* points,int count,Vec2 middle){
    ConvexShape cs;
    cs.points = Vector_New(sizeof(Vec2));
    cs.targets = Vector_New(sizeof(Vec2));
    
    for(int i = 0;i<count;i++)
        Vector_Push(&cs.points,points + i);

    cs.middle = middle;
    return cs;
}
char ConvexShape_Contains(ConvexShape* cs,Vec2 point) {
	float prevcross = 0.0f;
	
	for(int i = 0;i<cs->targets.size;i++) {
		const Vec2 p = *(Vec2*)Vector_Get(&cs->targets,i);
		const Vec2 next = *(Vec2*)Vector_Get(&cs->targets,(i + 1) % cs->targets.size);
		
		const Vec2 a = Vec2_Sub(next,p);
		const Vec2 b = Vec2_Sub(point,p);
		const float cross = a.x * b.y - a.y * b.x;

		if(prevcross==0.0f || (prevcross > 0) == (cross > 0))
			prevcross = cross;
		else
			return 0;
	}
	return 1;
}
char ConvexShape_OverlapSat(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
        
		for(int a = 0;a<poly1->targets.size;a++) {
			int b = (a+1) % poly1->targets.size;

            Vec2 p1 = *(Vec2*)Vector_Get(&poly1->targets,b);
            Vec2 p2 = *(Vec2*)Vector_Get(&poly1->targets,a);

            Vec2 axisProj = Vec2_Perp(Vec2_Sub(p1,p2));
			
			float min_r1 = INFINITY,max_r1 = -INFINITY;
			for(int p = 0;p<poly1->targets.size;p++) {
                Vec2 q1 = *(Vec2*)Vector_Get(&poly1->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r1 = F32_Min(min_r1, q);
				max_r1 = F32_Max(max_r1, q);
			}
			
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for(int p = 0;p<poly2->targets.size;p++) {
				Vec2 q1 = *(Vec2*)Vector_Get(&poly2->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r2 = F32_Min(min_r2, q);
				max_r2 = F32_Max(max_r2, q);
			}
			
			if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return 0;
		}
	}
	return 1;
}
char ConvexShape_StaticSat(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	float overlap = INFINITY;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		for(int a = 0;a<poly1->targets.size;a++) {
			int b = (a+1) % poly1->targets.size;

			Vec2 p1 = *(Vec2*)Vector_Get(&poly1->targets,b);
            Vec2 p2 = *(Vec2*)Vector_Get(&poly1->targets,a);

            Vec2 axisProj = Vec2_Perp(Vec2_Sub(p1,p2));
			
			float min_r1 = INFINITY, max_r1 = -INFINITY;
			for(int p = 0;p<poly1->targets.size;p++) {
                Vec2 q1 = *(Vec2*)Vector_Get(&poly1->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r1 = F32_Min(min_r1, q);
				max_r1 = F32_Max(max_r1, q);
			}
			
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for(int p = 0;p<poly2->targets.size;p++) {
				Vec2 q1 = *(Vec2*)Vector_Get(&poly2->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r2 = F32_Min(min_r2, q);
				max_r2 = F32_Max(max_r2, q);
			}
			
			overlap = F32_Min(F32_Min(max_r1,max_r2) - F32_Max(min_r1,min_r2),overlap);
			
			if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return 0;
		}
	}
	Vec2 d = Vec2_Norm(Vec2_Sub(cs2->middle,cs1->middle));
	cs1->middle = Vec2_Sub(cs1->middle,Vec2_Mulf(d,overlap));
	return 0;
}
char ConvexShape_OverlapDia(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		for(int p = 0;p<poly1->points.size;p++) {
			Vec2 line_r1s = poly1->middle;
			Vec2 line_r1e = *(Vec2*)Vector_Get(&poly1->targets,p);
			
			for(int q = 0;q<poly2->points.size;q++) {
				Vec2 line_r2s = *(Vec2*)Vector_Get(&poly2->targets,q);
				Vec2 line_r2e = *(Vec2*)Vector_Get(&poly2->targets,(q+1) % poly2->points.size);
				
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;
				
				if(t1>=0.0f&&t1<1.0f&&t2>=0.0f&&t2<1.0f) {
					return 1;
				}
			}
		}
	}
	return 0;
}
char ConvexShape_StaticDia(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		
		for(int p = 0;p<poly1->points.size;p++) {
			Vec2 line_r1s = poly1->middle;
			Vec2 line_r1e = *(Vec2*)Vector_Get(&poly1->targets,p);
			
			Vec2 displace = { 0.0f,0.0f };
			
			for(int q = 0;q<poly2->points.size;q++) {
				Vec2 line_r2s = *(Vec2*)Vector_Get(&poly2->targets,q);
				Vec2 line_r2e = *(Vec2*)Vector_Get(&poly2->targets,(q+1) % poly2->points.size);
				
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;
				
				if(t1>=0.0f&&t1<1.0f&&t2>=0.0f&&t2<1.0f) {
					displace.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
					displace.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
			}
			
			cs1->middle.x += displace.x * (shape==0?-1:1);
			cs1->middle.y += displace.y * (shape==0?-1:1);
		}
	}
	return 0;
}

void ConvexShape_Update(ConvexShape* cs){
    Vector_Clear(&cs->targets);

    for(int i = 0;i<cs->points.size;i++){
		Vec2 p = *(Vec2*)Vector_Get(&cs->points,i);
		Vec2 t = Vec2_Add(cs->middle,p);
        Vector_Push(&cs->targets,&t);
	}
}
void ConvexShape_Render(unsigned int* Target,int Target_Width,int Target_Height,ConvexShape* cs,unsigned int color){
    Vec2 prev = *(Vec2*)Vector_Get(&cs->targets,cs->targets.size - 1);
    for(int i = 0;i<cs->targets.size;i++){
        Vec2 p = *(Vec2*)Vector_Get(&cs->targets,i);
        Line_RenderX(Target,Target_Width,Target_Height,prev,p,color,1.0f);
        prev = p;
    }
}
void ConvexShape_RenderWire(unsigned int* Target,int Target_Width,int Target_Height,ConvexShape* cs,unsigned int color,float s){
    Vec2 prev = *(Vec2*)Vector_Get(&cs->targets,cs->targets.size - 1);
    for(int i = 0;i<cs->targets.size;i++){
        Vec2 p = *(Vec2*)Vector_Get(&cs->targets,i);
        Line_RenderX(Target,Target_Width,Target_Height,prev,p,color,s);
        prev = p;
    }
}
void ConvexShape_Free(ConvexShape* cs){
    Vector_Free(&cs->points);
    Vector_Free(&cs->targets);
}

#endif