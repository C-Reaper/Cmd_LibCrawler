#ifndef LINE_H
#define LINE_H

#include "Intrinsics.h"
#include "Point.h"
#include "Math.h"

typedef struct Line{
    Vec2 s;
    Vec2 e;
} Line;

Line Line_New(Vec2 s,Vec2 e){
    Line l;
    l.s = s;
    l.e = e;
    return l;
}
Line Line_NewSep(Vec2 s,Vec2 e){
    const Vec2 dir = Vec2_Sub(e,s);
    const Vec2 mp = Vec2_Add(s,Vec2_Mulf(dir,0.5f));
    const Vec2 ld = Vec2_Perp(dir);
    
    Line l;
    l.s = Vec2_Sub(mp,ld);
    l.e = Vec2_Add(mp,ld);
    return l;
}

Vec2 Line_MT(Line l){
    Vec2 d = Vec2_Sub(l.e,l.s);
    float m = INFINITY;
    if(d.x!=0.0f) m = d.y / d.x;
    float t = -m * l.s.x + l.s.y;
    return (Vec2){ m,t };
}

float Line_Y(Line l,float x){
    const Vec2 mt = Line_MT(l);
    return mt.x * x + mt.y;
}
float Line_X(Line l,float y){
    const Vec2 mt = Line_MT(l);
    return (y - mt.y) / mt.x;
}

Vec2 Line_MP(Line l){
    const Vec2 dir = Vec2_Sub(l.e,l.s);
    const Vec2 mp = Vec2_Add(l.s,Vec2_Mulf(dir,0.5f));
    return mp;
}
char Line_Contains(Line l,Vec2 p){
    //const Vec2 dir0 = Vec2_Sub(l.e,l.s);
    //const Vec2 dir1 = Vec2_Sub(p,l.s);
    //const float dp = Vec2_Dot(dir0,dir1);
    //const float dist = Vec2_Mag(dir0) * Vec2_Mag(dir1);
    //return F32_Abs(dp - dist) < 0.001f;

    const float x = Line_X(l,p.y);
    const float dist = F32_Abs(x - p.x);
    return dist < 0.0001f;
}
Line Line_NewSepBorder(Vec2 s,Vec2 e,Vec2 bp,Vec2 bd){
    const Vec2 dir = Vec2_Sub(e,s);
    const Vec2 mp = Vec2_Add(s,Vec2_Mulf(dir,0.5f));
    const Vec2 ld = Vec2_Perp(dir);
    
    Line l;
    l.s = Vec2_Sub(mp,ld);
    l.e = Vec2_Add(mp,ld);

    // TOP & LEFT | BOTTOM & RIGHT

    Line ret;

    const float plx0 = Line_X(l,bp.y);
    const float plx1 = Line_X(l,bp.y + bd.y);
    const float ply0 = Line_Y(l,bp.x);
    const float ply1 = Line_Y(l,bp.x + bd.x);
    char first = 0;

    if(plx0 >= bp.x && plx0 < bp.x + bd.x){
        ret.s = (Vec2){ plx0,bp.y };
    }else{
        if(ply0 >= bp.y && ply0 < bp.y + bd.y){
            ret.s = (Vec2){ bp.x,ply0 };
            first = 2;
        }else{
            ret.s = (Vec2){ bp.x + bd.x,ply1 };
            first = 1;
        }
    }

    if(plx1 >= bp.x && plx1 < bp.x + bd.x){
        ret.e = (Vec2){ plx1,bp.y + bd.y };
    }else{
        if(first == 0){
            if(ply0 >= bp.y && ply0 < bp.y + bd.y){
                ret.e = (Vec2){ bp.x,ply0 };
            }else{
                ret.e = (Vec2){ bp.x + bd.x,ply1 };
            }
        }else if(first == 1)
            ret.e = (Vec2){ bp.x,ply0 };
        else
            ret.e = (Vec2){ bp.x + bd.x,ply1 };
    }
    return ret;
}
Vec2 Line_Clamp(Line l,Vec2 p,Vec2 min,Vec2 max){
    if(p.x<min.x){
        p.x = min.x;
        p.y = Line_Y(l,p.x);
    }
    if(p.y<min.y){
        p.y = min.y;
        p.x = Line_X(l,p.y);
    }
    if(p.x>max.x){
        p.x = max.x;
        p.y = Line_Y(l,p.x);
    }
    if(p.y>max.y){
        p.y = max.y;
        p.x = Line_X(l,p.y);
    }
    return p;
}
Vec2 Line_Intersect(Line l,Line other){
    // x = (t1 - t2) / (m2 - m1)
    const Vec2 mt0 = Line_MT(l);
    const Vec2 mt1 = Line_MT(other);
    const float x = (mt0.y - mt1.y) / (mt1.x - mt0.x);
    return (Vec2){ x,Line_Y(l,x) };
}
int Line_IsValidPoint(Vec2 p) {
    if (isnan(p.x) || isnan(p.y) || isinf(p.x) || isinf(p.y)) {
        return 0;
    }

    const float MAX_COORD = 1e7f;
    if (fabs(p.x) > MAX_COORD || fabs(p.y) > MAX_COORD) {
        return 0;
    }
    return 1;
}

/*
Line Line_NewChopBorder(const Line* l0, Vec2 a, Vec2 b,      // Sites von l0
                        const Line* l1, Vec2 c1, Vec2 c2,
                        const Line* l2, Vec2 c3, Vec2 c4) {

    Vec2 isp = Line_Intersect(*l0, *l1);
    if (!Line_IsValidPoint(isp)) return *l0;

    Vec2 c = c3;  // dritter Punkt

    // Richtung der Linie normalisieren
    Vec2 dir = Vec2_Norm(Vec2_Sub(l0->e, l0->s));

    // Testpunkte mit etwas größerem Abstand + kleine Störung vermeiden
    const float test_dist = 0.5f;   // war 0.1f → stabiler
    Vec2 test_start = Vec2_Add(isp, Vec2_Mulf(dir, -test_dist));
    Vec2 test_end   = Vec2_Add(isp, Vec2_Mulf(dir,  test_dist));

    // Abstände zu A/B vs C
    double d_ab_start = fmin(Vec2_Mag2(Vec2_Sub(test_start, a)), 
                             Vec2_Mag2(Vec2_Sub(test_start, b)));
    double d_c_start  = Vec2_Mag2(Vec2_Sub(test_start, c));

    double d_ab_end = fmin(Vec2_Mag2(Vec2_Sub(test_end, a)), 
                           Vec2_Mag2(Vec2_Sub(test_end, b)));
    double d_c_end  = Vec2_Mag2(Vec2_Sub(test_end, c));

    Line result = *l0;

    // Mit Toleranz arbeiten, um Flackern zu reduzieren
    const double epsilon = 1e-5;

    if (d_ab_start + epsilon < d_c_start) {
        result.e = isp;        // Start-Seite gut
    } 
    else if (d_ab_end + epsilon < d_c_end) {
        result.s = isp;        // End-Seite gut
    } 
    else {
        // Fallback: Mittelpunkt + kleine Hysterese
        Vec2 mid = Vec2_Mulf(Vec2_Add(a, b), 0.5f);
        double d_mid_start = Vec2_Mag2(Vec2_Sub(test_start, mid));
        if (d_mid_start < Vec2_Mag2(Vec2_Sub(test_end, mid))) {
            result.e = isp;
        } else {
            result.s = isp;
        }
    }

    return result;
}
*/

Line Line_NewChopBorder(Line* l0,Vec2 p00,Vec2 p01,Line* l1,Vec2 p10,Vec2 p11,Line* l2,Vec2 p20,Vec2 p21){
    const Vec2 isp = Line_Intersect(*l0,*l1);
    //const Line nl00 = Line_New(l0->s,isp);
    //const Line nl01 = Line_New(l0->e,isp);
    const Vec2 dir00 = Vec2_Sub(l0->s,isp);
    //const Vec2 dir01 = Vec2_Sub(l0->e,isp);
    const Vec2 p02 = Vec2_Cmp(p10,p20) || Vec2_Cmp(p10,p21) ? p10 : p11;
    const Vec2 dir02 = Vec2_Sub(p02,isp);
    const float d00 = Vec2_Dot(dir00,dir02);
    if(d00 < 0.0f) 	return Line_New(l0->s,isp);
    else			return Line_New(l0->e,isp);
}

void Line_Chop0(Line* l,Line other){
    const Vec2 ip = Line_Intersect(*l,other);
    l->s = ip;
}
void Line_Chop1(Line* l,Line other){
    const Vec2 ip = Line_Intersect(*l,other);
    l->e = ip;
}
Vec2 Line_Direction(const Line* l0){
    return Vec2_Norm(Vec2_Sub(l0->e,l0->s));
}
float Line_Proj(const Line* l, Vec2 p) {
    Vec2 dir = Line_Direction(l);
    return Vec2_Dot(Vec2_Sub(p, l->s), dir);
}
Line Line_FromPerpBisector(Vec2 p0, Vec2 p1) {
    Line l = {
        .s = { 0.0f,0.0f },
        .e = { 0.0f,0.0f }
    };

    const Vec2 mid = Vec2_Mulf(Vec2_Add(p0, p1), 0.5f);
    const Vec2 dir = Vec2_Sub(p1, p0);
    const Vec2 perp = Vec2_Perp(dir);

    if (Vec2_Mag2(dir) < 1e-12f) {
        l.s = p0;
        l.e = Vec2_Add(p0, (Vec2){0, 1000});
        return l;
    }

    //const float l_a = perp.x;
    //const float l_b = perp.y;
    //const float l_c = -(l_a * mid.x + l_b * mid.y);
    const Vec2 unit_perp = Vec2_Norm(perp);
    const float length = 10000.0f;

    l.s = Vec2_Sub(mid, Vec2_Mulf(unit_perp, length));
    l.e = Vec2_Add(mid, Vec2_Mulf(unit_perp, length));
    return l;
}

void Line_Render(unsigned int* Target,int Target_Width,int Target_Height,Line l,unsigned int c,F32 StrokeSize){
	if(l.s.x < 0.0f && l.e.x < 0.0f)                        return;
    if(l.s.x >= Target_Width && l.e.x >= Target_Width)      return;
    if(l.s.y < 0.0f && l.e.y < 0.0f)                        return;
    if(l.s.y >= Target_Height && l.e.y >= Target_Height)    return;
    
    Vec2 d = Vec2_Norm(Vec2_Sub(l.e,l.s));

    F32 m = 1.0f;
    if(d.x!=0.0f){
        m = d.y / d.x;
        F32 t = -m * l.s.x + l.s.y;
        if(l.s.y<1.0f || l.s.y>=Target_Height-1.0f){
            l.s.y = l.s.y<1.0f?1.0f:(l.s.y>Target_Height-1.0f?Target_Height-1.0f:l.s.y);
            l.s.x = (l.s.y - t) / m;
        }
        if(l.e.y<1.0f || l.e.y>=Target_Height-1.0f){
            l.e.y = l.e.y<1.0f?1.0f:(l.e.y>Target_Height-1.0f?Target_Height-1.0f:l.e.y);
            l.e.x = (l.e.y - t) / m;
        }
        if(l.s.x<1.0f || l.s.x>=Target_Width-1.0f){
            l.s.x = l.s.x<1.0f?1.0f:(l.s.x>Target_Width-1.0f?Target_Width-1.0f:l.s.x);
            l.s.y = m * l.s.x + t;
        }
        if(l.e.x<1.0f || l.e.x>=Target_Width-1.0f){
            l.e.x = l.e.x<1.0f?1.0f:(l.e.x>Target_Width-1.0f?Target_Width-1.0f:l.e.x);
            l.e.y = m * l.e.x + t;
        }
    }else{
        if(l.s.y<1.0f || l.s.y>=Target_Height-1.0f){
            l.s.y = l.s.y<1.0f?1.0f:(l.s.y>Target_Height-1.0f?Target_Height-1.0f:l.s.y);
        }
        if(l.e.y<1.0f || l.e.y>=Target_Height-1.0f){
            l.e.y = l.e.y<1.0f?1.0f:(l.e.y>Target_Height-1.0f?Target_Height-1.0f:l.e.y);
        }
    }

    F32 w = 0.0f;
    F32 L = Vec2_Mag(Vec2_Sub(l.e,l.s));

    if(isnan(L) || isinf(L)) return;

    while(w<L){
        l.s = Vec2_Add(l.s,d);
        w += 1.0f;

        float y = l.s.y;
        if(StrokeSize == 1.0f) {
            if(l.s.x<1.0f||l.s.x>=Target_Width-1||y<1.0f||y>=Target_Height-1)
                continue;
            
			if(l.s.x>=0.0f && l.s.x<Target_Width && y>=0.0f && y<Target_Height)
                Target[(int)y * Target_Width + (int)l.s.x] = c;
        }else{
            for(float i = -StrokeSize;i<StrokeSize;i+=1.0f){
                float x = l.s.x + Vec2_Perp(d).x*i;
                y = l.s.y + Vec2_Perp(d).y*i;
                if(x<1.0f||x>=Target_Width-1||y<1.0f||y>=Target_Height-1)
                    continue;
                
                if(l.s.x>=0.0f && l.s.x<Target_Width && y>=0.0f && y<Target_Height)
                    Target[(int)y * Target_Width + (int)l.s.x] = c;
            }
        }
    }
}
void Line_RenderX(unsigned int* Target,int Target_Width,int Target_Height,Vec2 s,Vec2 e,unsigned int c,F32 StrokeSize){
	Line_Render(Target,Target_Width,Target_Height,Line_New(s,e),c,StrokeSize);
}

#endif // !LINE_Hystem/!