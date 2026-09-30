#ifndef LIB3D_MATH_H
#define LIB3D_MATH_H

#include <stdint.h>
#include <math.h>

#include "../Container/Vector.h"

#include "Float.h"
#include "Vector2.h"
#include "Pixel.h"
#include "Sprite.h"


typedef struct Vec2D{
	float u;
	float v;
	float w;
} Vec2D;

Vec2D Vec2D_New(float u,float v){
	return (Vec2D){ u,v,1.0f };
}
Vec2D Vec2D_Null(){
	return (Vec2D){ 0.0f,0.0f,1.0f };
}
Vec2D Vec2D_Add(Vec2D v1,Vec2D v2){
	return (Vec2D){ v1.u + v2.u,v1.v + v2.v,v1.w + v2.w };
}
Vec2D Vec2D_Sub(Vec2D v1,Vec2D v2){
	return (Vec2D){ v1.u - v2.u,v1.v - v2.v,v1.w - v2.w };
}
Vec2D Vec2D_Mul(Vec2D v1,float k){
	return (Vec2D){ v1.u * k,v1.v * k,v1.w * k };
}
Vec2D Vec2D_Div(Vec2D v1,float k){
	if(k==0.0f) return Vec2D_New( 0.0f,0.0f );
	return (Vec2D){ v1.u / k,v1.v / k,v1.w / k };
}


typedef struct Vec3D{
	float x;
	float y;
	float z;
	float w;
} Vec3D;

Vec3D Vec3D_New(float x,float y,float z){
	return (Vec3D){ x,y,z,1.0f };
}
Vec3D Vec3D_Null(){
	return (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
}
Vec3D Vec3D_Neg(Vec3D v){
	return Vec3D_New( -v.x,-v.y,-v.z );
}
Vec3D Vec3D_Add(Vec3D v1,Vec3D v2){
	return Vec3D_New( v1.x + v2.x,v1.y + v2.y,v1.z + v2.z );
}
Vec3D Vec3D_Sub(Vec3D v1,Vec3D v2){
	return Vec3D_New( v1.x - v2.x,v1.y - v2.y,v1.z - v2.z );
}
Vec3D Vec3D_Mul(Vec3D v1,float k){
	return Vec3D_New( v1.x * k,v1.y * k,v1.z * k );
}
Vec3D Vec3D_Div(Vec3D v1,float k){
	if(k==0.0f) return Vec3D_New( 0.0f,0.0f,0.0f );
	return Vec3D_New( v1.x / k,v1.y / k,v1.z / k );
}
float Vec3D_DotProduct(Vec3D v1,Vec3D v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Vec3D_Length(Vec3D v){
	return sqrtf(Vec3D_DotProduct(v,v));
}
Vec3D Vec3D_Normalise(Vec3D v){
	float l = Vec3D_Length(v);
	return Vec3D_New( v.x / l,v.y / l,v.z / l );
}
Vec3D Vec3D_Perp(Vec3D v){
	return Vec3D_New( -v.z,v.y,v.x );
}
Vec3D Vec3D_Round(Vec3D v){
	return Vec3D_New( roundf(v.x),roundf(v.y),roundf(v.z) );
}
Vec3D Vec3D_CrossProduct(Vec3D v1,Vec3D v2){
	Vec3D v = Vec3D_New(0.0f,0.0f,0.0f);
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
Vec3D Vec3D_IntersectPlane(Vec3D plane_p,Vec3D* plane_n,Vec3D lineStart,Vec3D lineEnd,float* t){
	*plane_n = Vec3D_Normalise(*plane_n);
	float plane_d = -Vec3D_DotProduct(*plane_n,plane_p);
	float ad = Vec3D_DotProduct(lineStart,*plane_n);
	float bd = Vec3D_DotProduct(lineEnd,*plane_n);
	*t = (-plane_d - ad) / (bd - ad);
	Vec3D lineStartToEnd = Vec3D_Sub(lineEnd,lineStart);
	Vec3D lineToIntersect = Vec3D_Mul(lineStartToEnd,*t);
	return Vec3D_Add(lineStart,lineToIntersect);
}
float Vec3D_Dist(Vec3D plane_p,Vec3D plane_n,Vec3D p){
	//Vec3D n = Vec3D_Normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vec3D_DotProduct(plane_n,plane_p));
}


typedef struct Shade{
	union {
		unsigned int c;
		struct {
			unsigned short f1;
			unsigned short f2;
		};
	};
	float l;
} Shade;

typedef struct Tri3D{
	Vec3D p[3];
	Vec2D t[3];
	Vec3D n;
	Shade c;
} Tri3D;

void Tri3D_RenderTexX(
	unsigned int* Target,int Target_Width,int Target_Height,
	float* Depth,
	Vic2 p1,float u1,float v1,float w1,
	Vic2 p2,float u2,float v2,float w2,
	Vic2 p3,float u3,float v3,float w3,
    SubSprite tex,float shade
){
	if (p2.y < p1.y){
		I32_Swap(&p1.y,&p2.y);
		I32_Swap(&p1.x,&p2.x);
		F32_Swap(&u1,&u2);
		F32_Swap(&v1,&v2);
		F32_Swap(&w1,&w2);
	}
	if (p3.y < p1.y){
		I32_Swap(&p1.y,&p3.y);
		I32_Swap(&p1.x,&p3.x);
		F32_Swap(&u1,&u3);
		F32_Swap(&v1,&v3);
		F32_Swap(&w1,&w3);
	}
	if (p3.y < p2.y){
		I32_Swap(&p2.y,&p3.y);
		I32_Swap(&p2.x,&p3.x);
		F32_Swap(&u2,&u3);
		F32_Swap(&v2,&v3);
		F32_Swap(&w2,&w3);
	}

	int dy1 = p2.y - p1.y;
	int dx1 = p2.x - p1.x;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;
	int dy2 = p3.y - p1.y;
	int dx2 = p3.x - p1.x;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;
	float tex_u,tex_v,tex_w;
	float dax_step = 0,dbx_step = 0,
		du1_step = 0,dv1_step = 0,
		du2_step = 0,dv2_step = 0,
		dw1_step=0,dw2_step=0;
	
	if (dy1) dax_step = dx1 / fabsf((float)dy1);
	if (dy2) dbx_step = dx2 / fabsf((float)dy2);
	if (dy1) du1_step = du1 / fabsf((float)dy1);
	if (dy1) dv1_step = dv1 / fabsf((float)dy1);
	if (dy1) dw1_step = dw1 / fabsf((float)dy1);
	if (dy2) du2_step = du2 / fabsf((float)dy2);
	if (dy2) dv2_step = dv2 / fabsf((float)dy2);
	if (dy2) dw2_step = dw2 / fabsf((float)dy2);
	
	if (dy1){
		for (int i = p1.y; i <= p2.y; i++){
			int ax = p1.x + (float)(i - p1.y) * dax_step;
			int bx = p1.x + (float)(i - p1.y) * dbx_step;
			float tex_su = u1 + (float)(i - p1.y) * du1_step;
			float tex_sv = v1 + (float)(i - p1.y) * dv1_step;
			float tex_sw = w1 + (float)(i - p1.y) * dw1_step;
			float tex_eu = u1 + (float)(i - p1.y) * du2_step;
			float tex_ev = v1 + (float)(i - p1.y) * dv2_step;
			float tex_ew = w1 + (float)(i - p1.y) * dw2_step;
			
			if (ax > bx){
				I32_Swap(&ax,&bx);
				F32_Swap(&tex_su,&tex_eu);
				F32_Swap(&tex_sv,&tex_ev);
				F32_Swap(&tex_sw,&tex_ew);
			}
			
			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;
			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;
			for (int j = ax; j < bx; j++){
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > Depth[i * Target_Width + j]){
					const Pixel sample = SubSprite_Sample(&tex,tex_u / tex_w,tex_v / tex_w);
					const Pixel shaded = Pixel_Mulf(sample,shade);
					Point_Render(Target,Target_Width,Target_Height,(Vec2){ j,i },shaded);
					Depth[i * Target_Width + j] = tex_w;
				}
				t += tstep;
			}
		}
	}

	dy1 = p3.y - p2.y;
	dx1 = p3.x - p2.x;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;
	if (dy1) dax_step = dx1 / fabsf((float)dy1);
	if (dy2) dbx_step = dx2 / fabsf((float)dy2);
	du1_step = 0,dv1_step = 0;
	if (dy1) du1_step = du1 / fabsf((float)dy1);
	if (dy1) dv1_step = dv1 / fabsf((float)dy1);
	if (dy1) dw1_step = dw1 / fabsf((float)dy1);
	
	if(dy1){
		for(int i = p2.y; i <= p3.y; i++){
			int ax = p2.x + (float)(i - p2.y) * dax_step;
			int bx = p1.x + (float)(i - p1.y) * dbx_step;
			float tex_su = u2 + (float)(i - p2.y) * du1_step;
			float tex_sv = v2 + (float)(i - p2.y) * dv1_step;
			float tex_sw = w2 + (float)(i - p2.y) * dw1_step;
			float tex_eu = u1 + (float)(i - p1.y) * du2_step;
			float tex_ev = v1 + (float)(i - p1.y) * dv2_step;
			float tex_ew = w1 + (float)(i - p1.y) * dw2_step;
			
			if(ax > bx){
				I32_Swap(&ax,&bx);
				F32_Swap(&tex_su,&tex_eu);
				F32_Swap(&tex_sv,&tex_ev);
				F32_Swap(&tex_sw,&tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;
			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;
			for(int j = ax; j < bx; j++){
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > Depth[i * Target_Width + j]){
					const Pixel sample = SubSprite_Sample(&tex,tex_u / tex_w,tex_v / tex_w);
					const Pixel shaded = Pixel_Mulf(sample,shade);
					Point_Render(Target,Target_Width,Target_Height,(Vec2){ j,i },shaded);
					Depth[i * Target_Width + j] = tex_w;
				}
				t += tstep;
			}
		}	
	}		
}

void Tri3D_RenderTexX_Opt(
    unsigned int* Target,int W,int H,
    float* Depth,
    Vic2 p1,float u1,float v1,float w1,
    Vic2 p2,float u2,float v2,float w2,
    Vic2 p3,float u3,float v3,float w3,
    SubSprite tex,float shade
){
    /* --- Sort by Y --- */
    #define SWAP(a,b,t) { t=a; a=b; b=t; }

    if (p2.y < p1.y){
        int ti; float tf;
        SWAP(p1.x,p2.x,ti); SWAP(p1.y,p2.y,ti);
        SWAP(u1,u2,tf); SWAP(v1,v2,tf); SWAP(w1,w2,tf);
    }
    if (p3.y < p1.y){
        int ti; float tf;
        SWAP(p1.x,p3.x,ti); SWAP(p1.y,p3.y,ti);
        SWAP(u1,u3,tf); SWAP(v1,v3,tf); SWAP(w1,w3,tf);
    }
    if (p3.y < p2.y){
        int ti; float tf;
        SWAP(p2.x,p3.x,ti); SWAP(p2.y,p3.y,ti);
        SWAP(u2,u3,tf); SWAP(v2,v3,tf); SWAP(w2,w3,tf);
    }

    /* --- Prepare perspective correct values --- */
    float u1w = u1 * w1,v1w = v1 * w1;
    float u2w = u2 * w2,v2w = v2 * w2;
    float u3w = u3 * w3,v3w = v3 * w3;

    /* --- Long edge (p1 -> p3) --- */
    int dy_long = p3.y - p1.y;
    float inv_dy_long = dy_long ? (1.0f / dy_long) : 0.0f;

    float dx_long = (float)(p3.x - p1.x) * inv_dy_long;
    float duw_long = (u3w - u1w) * inv_dy_long;
    float dvw_long = (v3w - v1w) * inv_dy_long;
    float dw_long  = (w3  - w1 ) * inv_dy_long;

    /* --- Upper & lower parts --- */
    for (int part = 0; part < 2; part++){
        int y_start = part == 0 ? p1.y : p2.y;
        int y_end   = part == 0 ? p2.y : p3.y;

        int dy = part == 0 ? (p2.y - p1.y) : (p3.y - p2.y);
        if (dy <= 0) continue;

        float inv_dy = 1.0f / dy;

        float xs = part == 0 ? p1.x : p2.x;
        float dxs = part == 0
            ? (float)(p2.x - p1.x) * inv_dy
            : (float)(p3.x - p2.x) * inv_dy;

        float uws = part == 0 ? u1w : u2w;
        float vws = part == 0 ? v1w : v2w;
        float ws  = part == 0 ? w1  : w2;

        float duws = part == 0 ? (u2w - u1w) * inv_dy : (u3w - u2w) * inv_dy;
        float dvws = part == 0 ? (v2w - v1w) * inv_dy : (v3w - v2w) * inv_dy;
        float dws  = part == 0 ? (w2  - w1 ) * inv_dy : (w3  - w2 ) * inv_dy;

        float xl = p1.x + (y_start - p1.y) * dx_long;
        float uwl = u1w + (y_start - p1.y) * duw_long;
        float vwl = v1w + (y_start - p1.y) * dvw_long;
        float wl  = w1  + (y_start - p1.y) * dw_long;

        for (int y = y_start; y < y_end; y++){
            int x_start = (int)xs;
            int x_end   = (int)xl;

            float uws_l = uws;
            float vws_l = vws;
            float ws_l  = ws;

            float uws_r = uwl;
            float vws_r = vwl;
            float ws_r  = wl;

            if (x_start > x_end){
                int ti; float tf;
                SWAP(x_start,x_end,ti);
                SWAP(uws_l,uws_r,tf);
                SWAP(vws_l,vws_r,tf);
                SWAP(ws_l ,ws_r ,tf);
            }

            int span = x_end - x_start;
            if (span > 0){
                float inv_span = 1.0f / span;

                float duw = (uws_r - uws_l) * inv_span;
                float dvw = (vws_r - vws_l) * inv_span;
                float dw  = (ws_r  - ws_l ) * inv_span;

                float uw = uws_l;
                float vw = vws_l;
                float w  = ws_l;

                unsigned int* tgt = Target + y * W + x_start;
                float* depth = Depth + y * W + x_start;

                for (int x = x_start; x < x_end; x++){
                    if (w > *depth){
                        float inv_w = 1.0f / w;
                        Pixel p = SubSprite_Sample(&tex,uw * inv_w,vw * inv_w);
                        *tgt = Pixel_Mulf(p,shade);
                        *depth = w;
                    }
                    uw += duw; vw += dvw; w += dw;
                    tgt++; depth++;
                }
            }

            xs += dxs;
            uws += duws; vws += dvws; ws += dws;

            xl += dx_long;
            uwl += duw_long; vwl += dvw_long; wl += dw_long;
        }
    }
}


#define FIXED_TO(x)   ((int32_t)((x) * 256.0f))
#define FIXED_FROM(x) ((float)(x) / 256.0f)

static inline void Tri3D_SwapVerts(Vic2* a, Vic2* b,
                             float* ua, float* va, float* wa,
                             float* ub, float* vb, float* wb)
{
    Vic2 t = *a; *a = *b; *b = t;
    float tf = *ua; *ua = *ub; *ub = tf;
    tf = *va; *va = *vb; *vb = tf;
    tf = *wa; *wa = *wb; *wb = tf;
}

static void Tri3D_RenderHalf_Fast(
    uint32_t* Target, int TW, float* Depth, SubSprite* sp_tex, float shade,
    Vic2 p1, Vic2 p2, Vic2 p3,
    float u1, float v1, float w1,
    float u2, float v2, float w2,
    float u3, float v3, float w3,
    int upper)
{
    int yStart = upper ? p1.y : p2.y;
    int yEnd   = upper ? p2.y : p3.y;
    if (yStart >= yEnd) return;

    float dax_step = upper ? (p2.x - p1.x) / (float)(p2.y - p1.y) : (p3.x - p2.x) / (float)(p3.y - p2.y);
    float dbx_step = (p3.x - p1.x) / (float)(p3.y - p1.y);

    float du1_step = upper ? (u2-u1)/(float)(p2.y-p1.y) : (u3-u2)/(float)(p3.y-p2.y);
    float dv1_step = upper ? (v2-v1)/(float)(p2.y-p1.y) : (v3-v2)/(float)(p3.y-p2.y);
    float dw1_step = upper ? (w2-w1)/(float)(p2.y-p1.y) : (w3-w2)/(float)(p3.y-p2.y);

    float du2_step = (u3-u1)/(float)(p3.y-p1.y);
    float dv2_step = (v3-v1)/(float)(p3.y-p1.y);
    float dw2_step = (w3-w1)/(float)(p3.y-p1.y);

    const __m256 onev = _mm256_set1_ps(1.0f);

    for (int y = yStart; y <= yEnd; ++y)
    {
        float ty        = (float)(y - (upper ? p1.y : p2.y));
        float ty_global = (float)(y - p1.y);

        int ax = (int)((upper ? p1.x : p2.x) + ty * dax_step + 0.5f);
        int bx = (int)(p1.x + ty_global * dbx_step + 0.5f);

        float tex_su = (upper ? u1 : u2) + ty * du1_step;
        float tex_sv = (upper ? v1 : v2) + ty * dv1_step;
        float tex_sw = (upper ? w1 : w2) + ty * dw1_step;

        float tex_eu = u1 + ty_global * du2_step;
        float tex_ev = v1 + ty_global * dv2_step;
        float tex_ew = w1 + ty_global * dw2_step;

        // === WICHTIGER FIX ===
        if (ax > bx)
        {
            int tmp = ax; ax = bx; bx = tmp;
            // UVW ebenfalls tauschen!
            F32_Swap(&tex_su, &tex_eu);
            F32_Swap(&tex_sv, &tex_ev);
            F32_Swap(&tex_sw, &tex_ew);
        }

        if (ax >= bx) continue;

        float len = (float)(bx - ax);
        if (len <= 0.0f) continue;

        float du = (tex_eu - tex_su) / len;
        float dv = (tex_ev - tex_sv) / len;
        float dw = (tex_ew - tex_sw) / len;

        float cur_u = tex_su;
        float cur_v = tex_sv;
        float cur_w = tex_sw;

        uint32_t* color_row = Target + y * TW;
        float*    depth_row = Depth + y * TW;

        int x = ax;

        // AVX2 Block
        for (; x <= bx - 8; x += 8)
        {
            __attribute__((aligned(32))) float w_vals[8];
            __attribute__((aligned(32))) float u_vals[8];
            __attribute__((aligned(32))) float v_vals[8];

            for (int k = 0; k < 8; ++k)
            {
                w_vals[k] = cur_w + dw * k;
                u_vals[k] = cur_u + du * k;
                v_vals[k] = cur_v + dv * k;
            }

            __m256 wv = _mm256_load_ps(w_vals);
            __m256 depthv = _mm256_loadu_ps(depth_row + x);
            __m256 mask = _mm256_cmp_ps(wv, depthv, _CMP_GT_OQ);

            int movemask = _mm256_movemask_ps(mask);
            if (movemask == 0)
            {
                cur_u += du * 8;
                cur_v += dv * 8;
                cur_w += dw * 8;
                continue;
            }

            __m256 invw = _mm256_div_ps(onev, wv);
            __m256 tu = _mm256_mul_ps(_mm256_load_ps(u_vals), invw);
            __m256 tv = _mm256_mul_ps(_mm256_load_ps(v_vals), invw);

            _mm256_store_ps(u_vals, tu);
            _mm256_store_ps(v_vals, tv);

            __attribute__((aligned(32))) uint32_t colors[8] = {0};

            for (int k = 0; k < 8; ++k)
            {
                if ((movemask & (1 << k)) != 0)
                {
                    Pixel sample = SubSprite_Sample(sp_tex, u_vals[k], v_vals[k]);
                    colors[k] = Pixel_Mulf(sample, shade);
                }
            }

            __m256i colorv = _mm256_load_si256((__m256i*)colors);
            _mm256_maskstore_epi32((int*)(color_row + x), _mm256_castps_si256(mask), colorv);
            _mm256_maskstore_ps(depth_row + x, _mm256_castps_si256(mask), wv);

            cur_u += du * 8;
            cur_v += dv * 8;
            cur_w += dw * 8;
        }

        // Scalar Rest
        for (; x < bx; ++x)
        {
            if (cur_w > depth_row[x])
            {
                float fu = cur_u / cur_w;
                float fv = cur_v / cur_w;
                Pixel sample = SubSprite_Sample(sp_tex, fu, fv);
                uint32_t shaded = Pixel_Mulf(sample, shade);
                color_row[x] = shaded;
                depth_row[x] = cur_w;
            }
            cur_u += du;
            cur_v += dv;
            cur_w += dw;
        }
    }
}

void Tri3D_RenderTex_Fast(
    uint32_t* Target, int Target_Width, int Target_Height,
    float* Depth,
    Vic2 p1, float u1, float v1, float w1,
    Vic2 p2, float u2, float v2, float w2,
    Vic2 p3, float u3, float v3, float w3,
    SubSprite sp_tex, float shade)
{
    if (p2.y < p1.y) Tri3D_SwapVerts(&p1, &p2, &u1,&v1,&w1, &u2,&v2,&w2);
    if (p3.y < p1.y) Tri3D_SwapVerts(&p1, &p3, &u1,&v1,&w1, &u3,&v3,&w3);
    if (p3.y < p2.y) Tri3D_SwapVerts(&p2, &p3, &u2,&v2,&w2, &u3,&v3,&w3);

    Tri3D_RenderHalf_Fast(Target, Target_Width, Depth, &sp_tex, shade, p1,p2,p3, u1,v1,w1, u2,v2,w2, u3,v3,w3, 1);
    Tri3D_RenderHalf_Fast(Target, Target_Width, Depth, &sp_tex, shade, p1,p2,p3, u1,v1,w1, u2,v2,w2, u3,v3,w3, 0);
}

void Tri3D_RenderTex_Old(
	unsigned int* Target,int Target_Width,int Target_Height,
	float* Depth,
	Vic2 p1,float u1,float v1,float w1,
    Vic2 p2,float u2,float v2,float w2,
    Vic2 p3,float u3,float v3,float w3,
    SubSprite sp_tex,float shade
){
	if (p2.y < p1.y){
		I32_Swap(&p1.y,&p2.y);
		I32_Swap(&p1.x,&p2.x);
		F32_Swap(&u1,&u2);
		F32_Swap(&v1,&v2);
		F32_Swap(&w1,&w2);
	}
	if (p3.y < p1.y){
		I32_Swap(&p1.y,&p3.y);
		I32_Swap(&p1.x,&p3.x);
		F32_Swap(&u1,&u3);
		F32_Swap(&v1,&v3);
		F32_Swap(&w1,&w3);
	}
	if (p3.y < p2.y){
		I32_Swap(&p2.y,&p3.y);
		I32_Swap(&p2.x,&p3.x);
		F32_Swap(&u2,&u3);
		F32_Swap(&v2,&v3);
		F32_Swap(&w2,&w3);
	}

	int dy1 = p2.y - p1.y;
	int dx1 = p2.x - p1.x;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;
	int dy2 = p3.y - p1.y;
	int dx2 = p3.x - p1.x;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;
	float tex_u,tex_v,tex_w;
	float dax_step = 0,dbx_step = 0,
		du1_step = 0,dv1_step = 0,
		du2_step = 0,dv2_step = 0,
		dw1_step=0,dw2_step=0;
	
	if (dy1) dax_step = dx1 / fabsf((float)dy1);
	if (dy2) dbx_step = dx2 / fabsf((float)dy2);
	if (dy1) du1_step = du1 / fabsf((float)dy1);
	if (dy1) dv1_step = dv1 / fabsf((float)dy1);
	if (dy1) dw1_step = dw1 / fabsf((float)dy1);
	if (dy2) du2_step = du2 / fabsf((float)dy2);
	if (dy2) dv2_step = dv2 / fabsf((float)dy2);
	if (dy2) dw2_step = dw2 / fabsf((float)dy2);
	
	if (dy1){
		for (int i = p1.y; i <= p2.y; i++){
			int ax = p1.x + (float)(i - p1.y) * dax_step;
			int bx = p1.x + (float)(i - p1.y) * dbx_step;
			float tex_su = u1 + (float)(i - p1.y) * du1_step;
			float tex_sv = v1 + (float)(i - p1.y) * dv1_step;
			float tex_sw = w1 + (float)(i - p1.y) * dw1_step;
			float tex_eu = u1 + (float)(i - p1.y) * du2_step;
			float tex_ev = v1 + (float)(i - p1.y) * dv2_step;
			float tex_ew = w1 + (float)(i - p1.y) * dw2_step;
			
			if (ax > bx){
				I32_Swap(&ax,&bx);
				F32_Swap(&tex_su,&tex_eu);
				F32_Swap(&tex_sv,&tex_ev);
				F32_Swap(&tex_sw,&tex_ew);
			}
			
			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;
			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;
			for (int j = ax; j < bx; j++){
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > Depth[i * Target_Width + j]){
					const Pixel sample = SubSprite_Sample(&sp_tex,tex_u / tex_w,tex_v / tex_w);
					const Pixel shaded = Pixel_Mulf(sample,shade);
					Point_Render(Target,Target_Width,Target_Height,(Vec2){ j,i },shaded);
					Depth[i * Target_Width + j] = tex_w;
				}
				t += tstep;
			}
		}
	}

	dy1 = p3.y - p2.y;
	dx1 = p3.x - p2.x;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;
	if (dy1) dax_step = dx1 / fabsf((float)dy1);
	if (dy2) dbx_step = dx2 / fabsf((float)dy2);
	du1_step = 0,dv1_step = 0;
	if (dy1) du1_step = du1 / fabsf((float)dy1);
	if (dy1) dv1_step = dv1 / fabsf((float)dy1);
	if (dy1) dw1_step = dw1 / fabsf((float)dy1);
	
	if(dy1){
		for(int i = p2.y; i <= p3.y; i++){
			int ax = p2.x + (float)(i - p2.y) * dax_step;
			int bx = p1.x + (float)(i - p1.y) * dbx_step;
			float tex_su = u2 + (float)(i - p2.y) * du1_step;
			float tex_sv = v2 + (float)(i - p2.y) * dv1_step;
			float tex_sw = w2 + (float)(i - p2.y) * dw1_step;
			float tex_eu = u1 + (float)(i - p1.y) * du2_step;
			float tex_ev = v1 + (float)(i - p1.y) * dv2_step;
			float tex_ew = w1 + (float)(i - p1.y) * dw2_step;
			
			if(ax > bx){
				I32_Swap(&ax,&bx);
				F32_Swap(&tex_su,&tex_eu);
				F32_Swap(&tex_sv,&tex_ev);
				F32_Swap(&tex_sw,&tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;
			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;
			for(int j = ax; j < bx; j++){
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > Depth[i * Target_Width + j]){
					const Pixel sample = SubSprite_Sample(&sp_tex,tex_u / tex_w,tex_v / tex_w);
					const Pixel shaded = Pixel_Mulf(sample,shade);
					Point_Render(Target,Target_Width,Target_Height,(Vec2){ j,i },shaded);
					Depth[i * Target_Width + j] = tex_w;
				}
				t += tstep;
			}
		}	
	}
}

void Tri3D_CalcNorm(Tri3D* t){
	Vec3D line1 = Vec3D_Sub(t->p[1],t->p[0]);
	Vec3D line2 = Vec3D_Sub(t->p[2],t->p[0]);
	Vec3D normal = Vec3D_CrossProduct(line1,line2);

	t->n = Vec3D_Normalise(normal);
}
void Tri3D_ShadeNorm(Tri3D* t,Vec3D dirLight){
	t->c.l = F32_Min(F32_Max(0.2f,Vec3D_DotProduct(t->n,dirLight)),1.0f);
}
void Tri3D_Scale(Tri3D* t,float s){
	t->p[0] = Vec3D_Mul(t->p[0],s);
	t->p[1] = Vec3D_Mul(t->p[1],s);
	t->p[2] = Vec3D_Mul(t->p[2],s);
}
void Tri3D_Mul(Tri3D* t,Vec3D scale){
	t->p[0] = (Vec3D){ t->p[0].x * scale.x,t->p[0].y * scale.y,t->p[0].z * scale.z };
	t->p[1] = (Vec3D){ t->p[1].x * scale.x,t->p[1].y * scale.y,t->p[1].z * scale.z };
	t->p[2] = (Vec3D){ t->p[2].x * scale.x,t->p[2].y * scale.y,t->p[2].z * scale.z };
}
void Tri3D_Offset(Tri3D* t,Vec3D offset){
	t->p[0] = Vec3D_Add(t->p[0],offset);
	t->p[1] = Vec3D_Add(t->p[1],offset);
	t->p[2] = Vec3D_Add(t->p[2],offset);
}
int Tri3D_ClipAgainstPlane(Vec3D plane_p,Vec3D plane_n,Tri3D* const in_tri,Tri3D* out_tri1,Tri3D* out_tri2){
	plane_n = Vec3D_Normalise(plane_n);
	
	Vec3D* inside_points[3];  int nInsidePointCount = 0;
	Vec3D* outside_points[3]; int nOutsidePointCount = 0;
	Vec2D* inside_tex[3]; int nInsideTexCount = 0;
	Vec2D* outside_tex[3]; int nOutsideTexCount = 0;
	
    float d0 = Vec3D_Dist(plane_p,plane_n,in_tri->p[0]);
	float d1 = Vec3D_Dist(plane_p,plane_n,in_tri->p[1]);
	float d2 = Vec3D_Dist(plane_p,plane_n,in_tri->p[2]);
	if (d0 >= 0) { 
		inside_points[nInsidePointCount++] = &in_tri->p[0];
		inside_tex[nInsideTexCount++] = &in_tri->t[0];
	}else {
		outside_points[nOutsidePointCount++] = &in_tri->p[0];
		outside_tex[nOutsideTexCount++] = &in_tri->t[0];
	}

	if (d1 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri->p[1];
		inside_tex[nInsideTexCount++] = &in_tri->t[1];
	}else {
		outside_points[nOutsidePointCount++] = &in_tri->p[1];
		outside_tex[nOutsideTexCount++] = &in_tri->t[1];
	}

	if (d2 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri->p[2];
		inside_tex[nInsideTexCount++] = &in_tri->t[2];
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri->p[2];
		outside_tex[nOutsideTexCount++] = &in_tri->t[2];
	}
	
	if (nInsidePointCount == 0){
		return 0;
	}
	if (nInsidePointCount == 3){
		*out_tri1 = *in_tri;
		return 1;
	}
	if (nInsidePointCount == 1 && nOutsidePointCount == 2){
		out_tri1->c =  in_tri->c;
		
        out_tri1->p[0] = *inside_points[0];
		out_tri1->t[0] = *inside_tex[0];
		
		float t;
		out_tri1->p[1] = Vec3D_IntersectPlane(plane_p,&plane_n,*inside_points[0],*outside_points[0],&t);
		out_tri1->t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1->t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1->t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;
		out_tri1->p[2] = Vec3D_IntersectPlane(plane_p,&plane_n,*inside_points[0],*outside_points[1],&t);
		out_tri1->t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1->t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1->t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;
		return 1;
	}
	if (nInsidePointCount == 2 && nOutsidePointCount == 1){
		out_tri1->c =  in_tri->c;
		out_tri2->c =  in_tri->c;
		
        out_tri1->p[0] = *inside_points[0];
		out_tri1->p[1] = *inside_points[1];
		out_tri1->t[0] = *inside_tex[0];
		out_tri1->t[1] = *inside_tex[1];
		
		float t;
		out_tri1->p[2] = Vec3D_IntersectPlane(plane_p,&plane_n,*inside_points[0],*outside_points[0],&t);
		out_tri1->t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1->t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1->t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;
		
        out_tri2->p[0] = *inside_points[1];
		out_tri2->t[0] = *inside_tex[1];
		out_tri2->p[1] = out_tri1->p[2];
		out_tri2->t[1] = out_tri1->t[2];
		out_tri2->p[2] = Vec3D_IntersectPlane(plane_p,&plane_n,*inside_points[1],*outside_points[0],&t);
		out_tri2->t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
		out_tri2->t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
		out_tri2->t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
		return 2;
	}
	return 0;
}
int Tri3D_Compare(const void* e1,const void* e2) {
	Tri3D t1 = *(Tri3D*)e1;
	Tri3D t2 = *(Tri3D*)e2;
	float z1 = (t1.p[0].z+t1.p[1].z+t1.p[2].z)/3;
    float z2 = (t2.p[0].z+t2.p[1].z+t2.p[2].z)/3;
    return z1 == z2 ? 0 : (z1 < z2 ? 1 : -1);
}


typedef struct M4x4D{
	float m[4][4];
} M4x4D;

M4x4D Matrix_Null(){
	return (M4x4D){{
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f }
	}};
}
Vec3D Matrix_MultiplyVector(M4x4D m,Vec3D i){
	Vec3D v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}
M4x4D Matrix_MultiplyMatrix(M4x4D m1,M4x4D m2){
	M4x4D matrix = Matrix_Null();
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}
M4x4D Matrix_PointAt(Vec3D pos,Vec3D target,Vec3D up){
	Vec3D newForward = Vec3D_Sub(target,pos);
	newForward = Vec3D_Normalise(newForward);
	
	Vec3D a = Vec3D_Mul(newForward,Vec3D_DotProduct(up,newForward));
	Vec3D newUp = Vec3D_Sub(up,a);
	newUp = Vec3D_Normalise(newUp);

	Vec3D newRight = Vec3D_CrossProduct(newUp,newForward);

	return (M4x4D){{
		{ newRight.x,	newRight.y,		newRight.z,		0.0f },
		{ newUp.x,		newUp.y,		newUp.z,		0.0f },
		{ newForward.x,	newForward.y,	newForward.z,	0.0f },
		{ pos.x,		pos.y,			pos.z,			1.0f }
	}};
}
M4x4D Matrix_QuickInverse(M4x4D m){
	M4x4D matrix = Matrix_Null();
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
M4x4D Matrix_MakeView(Vec3D pos,Vec3D target,Vec3D up){
	Vec3D newForward = Vec3D_Normalise(Vec3D_Sub(target,pos));
	Vec3D a = Vec3D_Mul(newForward,Vec3D_DotProduct(up,newForward));
	Vec3D newUp = Vec3D_Normalise(Vec3D_Sub(up,a));
	Vec3D newRight = Vec3D_CrossProduct(newUp,newForward);

	return (M4x4D){{
		{ newRight.x,						newUp.x,						newForward.x,						0.0f },
		{ newRight.y,						newUp.y,						newForward.y,						0.0f },
		{ newRight.z,						newUp.z,						newForward.z,						0.0f },
		{ -Vec3D_DotProduct(newRight,pos),	-Vec3D_DotProduct(newUp,pos),	-Vec3D_DotProduct(newForward,pos),	1.0f }
	}};
}

M4x4D Matrix_MakeIdentity(){
	return (M4x4D){{
		{ 1.0f,0.0f,0.0f,0.0f },
		{ 0.0f,1.0f,0.0f,0.0f },
		{ 0.0f,0.0f,1.0f,0.0f },
		{ 0.0f,0.0f,0.0f,1.0f }
	}};
}
M4x4D Matrix_MakeRotationX(float fAngleRad){
	return (M4x4D){{
		{ 1.0f,0.0f,			0.0f,			0.0f },
		{ 0.0f,cosf(fAngleRad),	sinf(fAngleRad),0.0f },
		{ 0.0f,-sinf(fAngleRad),cosf(fAngleRad),0.0f },
		{ 0.0f,0.0f,			0.0f,			1.0f }
	}};
}
M4x4D Matrix_MakeRotationY(float fAngleRad){
	return (M4x4D){{
		{ cosf(fAngleRad),	0.0f,sinf(fAngleRad),	0.0f },
		{ 0.0f,				1.0f,0.0f,				0.0f },
		{ -sinf(fAngleRad),	0.0f,cosf(fAngleRad),	0.0f },
		{ 0.0f,				0.0f,0.0f,				1.0f }
	}};
}
M4x4D Matrix_MakeRotationZ(float fAngleRad){
	return (M4x4D){{
		{ cosf(fAngleRad),	sinf(fAngleRad),0.0f,0.0f },
		{ -sinf(fAngleRad),	cosf(fAngleRad),0.0f,0.0f },
		{ 0.0f,				0.0f,			1.0f,0.0f },
		{ 0.0f,				0.0f,			0.0f,1.0f }
	}};
}
M4x4D Matrix_MakeTranslation(float x,float y,float z){
	return (M4x4D){{
		{ 1.0f,	0.0f,	0.0f,	0.0f },
		{ 0.0f,	1.0f,	0.0f,	0.0f },
		{ 0.0f,	0.0f,	1.0f,	0.0f },
		{ x,	y,		z,		1.0f }
	}};
}
M4x4D Matrix_MakeProjection(float fFovDegrees,float fAspectRatio,float fNear,float fFar){
	const float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	return (M4x4D){{
		{ fAspectRatio * fFovRad,	0.0f,	0.0f,								0.0f },
		{ 0.0f,						fFovRad,0.0f,								0.0f },
		{ 0.0f,						0.0f,	fFar / (fFar - fNear),				1.0f },
		{ 0.0f,						0.0f,	(-fFar * fNear) / (fFar - fNear),	0.0f }
	}};
}
M4x4D Matrix_MakeWorld(Vec3D origin,Vec3D angle){
	const M4x4D matRotX = Matrix_MakeRotationX(angle.x);
	const M4x4D matRotY = Matrix_MakeRotationY(angle.y);
	const M4x4D matRotZ = Matrix_MakeRotationZ(angle.z);
	const M4x4D matTrans = Matrix_MakeTranslation(origin.x,origin.y,origin.z);
	const M4x4D matWorld1 = Matrix_MultiplyMatrix(matTrans,matRotY);
	const M4x4D matWorld2 = Matrix_MultiplyMatrix(matRotX,matRotZ);
	const M4x4D matWorld = Matrix_MultiplyMatrix(matWorld1,matWorld2);
	return matWorld;
}
M4x4D Matrix_MakeWorldR(Vec3D origin,Vec3D angle){
	const M4x4D matRotX = Matrix_MakeRotationX(angle.x);
	const M4x4D matRotY = Matrix_MakeRotationY(angle.y);
	const M4x4D matRotZ = Matrix_MakeRotationZ(angle.z);
	const M4x4D matTrans = Matrix_MakeTranslation(origin.x,origin.y,origin.z);
	const M4x4D matWorld1 = Matrix_MultiplyMatrix(matRotY,matRotZ);
	const M4x4D matWorld2 = Matrix_MultiplyMatrix(matRotX,matTrans);
	const M4x4D matWorld = Matrix_MultiplyMatrix(matWorld1,matWorld2);
	return matWorld;
}
M4x4D Matrix_MakePerspektive(Vec3D pos,Vec3D up,Vec3D a){
	const M4x4D matCameraRotX = Matrix_MakeRotationX(a.x);
	const M4x4D matCameraRotY = Matrix_MakeRotationY(a.y);
	const M4x4D matCameraRotZ = Matrix_MakeRotationZ(a.z);
	
	Vec3D lookdir = Vec3D_New(0.0f,0.0f,1.0f);
	lookdir = Matrix_MultiplyVector(matCameraRotX,lookdir);
	lookdir = Matrix_MultiplyVector(matCameraRotY,lookdir);
	lookdir = Matrix_MultiplyVector(matCameraRotZ,lookdir);
	
	const Vec3D target = Vec3D_Add(pos,lookdir);
	//const M4x4D matCamera = Matrix_PointAt(pos,target,up);
	//const M4x4D matView = Matrix_QuickInverse(matCamera);
	const M4x4D matView = Matrix_MakeView(pos,target,up);
	return matView;
}


typedef struct Camera {
	Vec3D p;
	Vec3D up;
	Vec3D ld;
	Vec3D fd;
	Vec3D sd;
	Vec3D a;
	float fov;
} Camera;

Camera Camera_New(){
	Camera c;
	c.p = (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
	c.up = (Vec3D){ 0.0f,1.0f,0.0f,1.0f };
	c.ld = (Vec3D){ 0.0f,0.0f,1.0f,1.0f };
	c.fd = (Vec3D){ 0.0f,0.0f,1.0f,1.0f };
	c.sd = (Vec3D){ 1.0f,0.0f,0.0f,1.0f };
	c.a = (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
	c.fov = 90.0f;
	return c;
}
Camera Camera_Make(Vec3D p,Vec3D a,float fov){
	Camera c = Camera_New();
	c.p = p;
	c.a = a;
	c.fov = fov;
	return c;
}
void Camera_Update(Camera* c){
	if(c->a.x < -3.14159f * 0.5f + 0.01f) c->a.x = -3.14159f * 0.5f + 0.01f;
	if(c->a.x >  3.14159f * 0.5f - 0.01f) c->a.x =  3.14159f * 0.5f - 0.01f;

	const M4x4D matCameraRotY = Matrix_MakeRotationY(c->a.y);
	const M4x4D matCameraRotX = Matrix_MakeRotationX(c->a.x);
	const Vec3D lookdir = Matrix_MultiplyVector(matCameraRotY,Matrix_MultiplyVector(matCameraRotX,Vec3D_New(0.0f,0.0f,1.0f)));
	const Vec3D facedir = Matrix_MultiplyVector(matCameraRotY,Vec3D_New(0.0f,0.0f,1.0f));
	const Vec3D sidedir = Matrix_MultiplyVector(matCameraRotY,Vec3D_New(1.0f,0.0f,0.0f));
	
	c->ld = lookdir;
	c->fd = facedir;
	c->sd = sidedir;
}
void Camera_Focus(Camera* c,Vec2 before,Vec2 now,Vec2 screen){
	if(now.x != before.x || now.y != before.y){
		Vec2 d = Vec2_Sub(now,before);
		Vec2 a = Vec2_Mulf(Vec2_Div(d,(Vec2){ screen.x,screen.y }),4.0f * 3.141592654f);
		c->a.y += a.x;
		c->a.x += a.y;
	}
}
void Camera_Focus_S(Camera* c,Vec2 before,Vec2 now,Vec2 screen,float speed){
	if(now.x != before.x || now.y != before.y){
		Vec2 d = Vec2_Sub(now,before);
		Vec2 a = Vec2_Mulf(Vec2_Div(d,(Vec2){ screen.x,screen.y }),speed * 3.141592654f);
		c->a.y += a.x;
		c->a.x += a.y;
	}
}


typedef struct Rect3D {
	Vec3D p;
	Vec3D d;
} Rect3D;

char Rect3D_Overlap(Rect3D r1,Rect3D r2){
	return !(r1.p.x<r2.p.x-r1.d.x || r1.p.y<r2.p.y-r1.d.y || r1.p.z<r2.p.z-r1.d.z || r1.p.x>r2.p.x+r2.d.x || r1.p.y>r2.p.y+r2.d.y || r1.p.z>r2.p.z+r2.d.z);
}
void Rect3D_Static(Rect3D* r1,Rect3D r2,void* Data,void (**Funcs)(void*)){
	if(Rect3D_Overlap(*r1,r2)){
        Vec3D m1 = Vec3D_Add(r1->p,Vec3D_Mul(r1->d,0.5f));
        Vec3D m2 = Vec3D_Add(r2.p,Vec3D_Mul(r2.d,0.5f));

        Vec3D l = Vec3D_Add(r1->d,r2.d);
        Vec3D d = Vec3D_Sub(m2,m1);
        d = Vec3D_New(d.x / l.x,d.y / l.y,d.z / l.z);

        if(F32_Abs(d.x)>F32_Abs(d.y)){
            if(F32_Abs(d.x)>F32_Abs(d.z)){
				if(d.x>0.0f){
                	m1.x = m2.x - l.x * 0.5f;
                	r1->p.x = m1.x - r1->d.x * 0.5f;

                    if(Funcs[0]) Funcs[0](Data);
            	}else{
            	    m1.x = m2.x + l.x * 0.5f;
            	    r1->p.x = m1.x - r1->d.x * 0.5f;

                    if(Funcs[1]) Funcs[1](Data);
            	}
			}else{
				if(d.z>0.0f){
                	m1.z = m2.z - l.z * 0.5f;
                	r1->p.z = m1.z - r1->d.z * 0.5f;

                    if(Funcs[2]) Funcs[2](Data);
            	}else{
            	    m1.z = m2.z + l.z * 0.5f;
            	    r1->p.z = m1.z - r1->d.z * 0.5f;

                    if(Funcs[3]) Funcs[3](Data);
            	}
			}
        }else{
            if(F32_Abs(d.y)>F32_Abs(d.z)){
				if(d.y>0.0f){
                	m1.y = m2.y - l.y * 0.5f;
                	r1->p.y = m1.y - r1->d.y * 0.5f;

                    if(Funcs[4]) Funcs[4](Data);
            	}else{
            	    m1.y = m2.y + l.y * 0.5f;
            	    r1->p.y = m1.y - r1->d.y * 0.5f;

                    if(Funcs[5]) Funcs[5](Data);
            	}
			}else{
				if(d.z>0.0f){
                	m1.z = m2.z - l.z * 0.5f;
                	r1->p.z = m1.z - r1->d.z * 0.5f;

                    if(Funcs[2]) Funcs[2](Data);
            	}else{
            	    m1.z = m2.z + l.z * 0.5f;
            	    r1->p.z = m1.z - r1->d.z * 0.5f;

                    if(Funcs[3]) Funcs[3](Data);
            	}
			}
        }
    }
}

#endif0t