#ifndef LIB3D_WORLD3D_H
#define LIB3D_WORLD3D_H

#include "Triangle.h"
#include "Files.h"
#include "ConstParser.h"
#include "Intrinsics.h"
#include "Lib3D_Mathlib.h"
#include "Lib3D_Mesh.h"

#define WORLD3D_ATLAS_COLS			16
#define WORLD3D_ATLAS_ROWS			16
#define WORLD3D_TILE_WIDTH			(1.0f / (float)WORLD3D_ATLAS_COLS)
#define WORLD3D_TILE_HEIGHT			(1.0f / (float)WORLD3D_ATLAS_ROWS)

#define WORLD3D_NORMAL_NONE			0
#define WORLD3D_NORMAL_CAP			1
#define WORLD3D_NORMAL_RCAP			2

#define WORLD3D_TEX_NONE			0
#define WORLD3D_TEX_ON				1

typedef struct World3D{
	Mesh trisIn;
	Mesh trisUpdate;
	Mesh trisBuff;
	Mesh trisOut;
	M4x4D model;
	M4x4D view;
	M4x4D proj;
	float* depth;
	Vec2 output;
	unsigned int normal : 2;
	unsigned int tex : 1;
	unsigned int flags : 29;
	unsigned int lastsize;
	unsigned int menu;
	unsigned int mode;
} World3D;

World3D World3D_Make(M4x4D model,M4x4D view,M4x4D proj){
	World3D m;
	m.trisIn = Vector_New(sizeof(Tri3D));
	m.trisUpdate = Vector_New(sizeof(Tri3D));
	m.trisBuff = Vector_New(sizeof(Tri3D));
	m.trisOut = Vector_New(sizeof(Tri3D));
	m.model = model;
	m.view = view;
	m.proj = proj;
	m.normal = WORLD3D_NORMAL_NONE;
	m.tex = WORLD3D_TEX_NONE;
	m.depth = NULL;
	m.flags = 0;
	m.lastsize = 0U;
	return m;
}
World3D World3D_New(){
	return World3D_Make(
		Matrix_MakeIdentity(),
		Matrix_MakeIdentity(),
		Matrix_MakeIdentity()
	);
}

void World3D_Set_Mode(World3D* m,int mode){
	m->mode = mode % 2;
}
void World3D_Set_Model(World3D* m,M4x4D model){
	m->model = model;
}
void World3D_Set_View(World3D* m,M4x4D view){
	m->view = view;
}
void World3D_Set_Proj(World3D* m,M4x4D proj){
	m->proj = proj;
}

int World3D_Compare(const void* e1,const void* e2) {
	Tri3D t1 = *(Tri3D*)e1;
	Tri3D t2 = *(Tri3D*)e2;
	float z1 = (t1.p[0].z+t1.p[1].z+t1.p[2].z)/3;
    float z2 = (t2.p[0].z+t2.p[1].z+t2.p[2].z)/3;
    return z1 == z2 ? 0 : (z1 < z2 ? 1 : -1);
}
Vec3D World3D_Pos_Process(World3D* m,Vec3D pos,Vec2 output){
	pos = Matrix_MultiplyVector(m->model,pos);
	pos = Matrix_MultiplyVector(m->view,pos);
	pos = Matrix_MultiplyVector(m->proj,pos);
	pos = Vec3D_Div(pos,pos.w);
	
	pos.x *= -1.0f;
	pos.y *= -1.0f;
	
	const Vec3D vOffsetView = Vec3D_New( 1,1,0 );
	pos = Vec3D_Add(pos,vOffsetView);
	pos.x *= 0.5f * output.x;
	pos.y *= 0.5f * output.y;
	return pos;
}
void World3D_Tri3D_Process(World3D* m,Tri3D* t,Vec3D camera,Vec2 output){
	Tri3D tri = *t;
	
	tri.p[0] = Matrix_MultiplyVector(m->model,tri.p[0]);
	tri.p[1] = Matrix_MultiplyVector(m->model,tri.p[1]);
	tri.p[2] = Matrix_MultiplyVector(m->model,tri.p[2]);
	Tri3D_CalcNorm(&tri);
	//Tri3D_ShadeNorm(&tri,(Vec3D){ 0.2f,0.1f,-0.6f });
	
	Vec3D vCameraRay = Vec3D_Sub(tri.p[0],camera);
	float dp = Vec3D_DotProduct(tri.n,vCameraRay);

	char c = (m->normal == WORLD3D_NORMAL_NONE) ||
			 (m->normal == WORLD3D_NORMAL_CAP && dp < 0.0f) ||
			 (m->normal == WORLD3D_NORMAL_RCAP && dp > 0.0f);

	if(c){
		tri.p[0] = Matrix_MultiplyVector(m->view,tri.p[0]);
		tri.p[1] = Matrix_MultiplyVector(m->view,tri.p[1]);
		tri.p[2] = Matrix_MultiplyVector(m->view,tri.p[2]);
		
		int nClippedTri3Ds = 0;
		Tri3D clipped[2];
		nClippedTri3Ds = Tri3D_ClipAgainstPlane(Vec3D_New(0.0f,0.0f,0.01f),Vec3D_New(0.0f,0.0f,1.0f),&tri,&clipped[0],&clipped[1]);
		
		for (int n = 0; n < nClippedTri3Ds; n++){
			clipped[n].p[0] = Matrix_MultiplyVector(m->proj, clipped[n].p[0]);
			clipped[n].p[1] = Matrix_MultiplyVector(m->proj, clipped[n].p[1]);
			clipped[n].p[2] = Matrix_MultiplyVector(m->proj, clipped[n].p[2]);
			
			clipped[n].p[0] = Vec3D_Div(clipped[n].p[0], clipped[n].p[0].w);
			clipped[n].p[1] = Vec3D_Div(clipped[n].p[1], clipped[n].p[1].w);
			clipped[n].p[2] = Vec3D_Div(clipped[n].p[2], clipped[n].p[2].w);
			
			clipped[n].p[0].x *= -1.0f;
			clipped[n].p[1].x *= -1.0f;
			clipped[n].p[2].x *= -1.0f;
			clipped[n].p[0].y *= -1.0f;
			clipped[n].p[1].y *= -1.0f;
			clipped[n].p[2].y *= -1.0f;

			Vec3D vOffsetView = Vec3D_New( 1,1,0 );
			clipped[n].p[0] = Vec3D_Add(clipped[n].p[0], vOffsetView);
			clipped[n].p[1] = Vec3D_Add(clipped[n].p[1], vOffsetView);
			clipped[n].p[2] = Vec3D_Add(clipped[n].p[2], vOffsetView);
			clipped[n].p[0].x *= 0.5f * output.x;
			clipped[n].p[0].y *= 0.5f * output.y;
			clipped[n].p[1].x *= 0.5f * output.x;
			clipped[n].p[1].y *= 0.5f * output.y;
			clipped[n].p[2].x *= 0.5f * output.x;
			clipped[n].p[2].y *= 0.5f * output.y;
			Vector_Push(&m->trisBuff,&clipped[n]);
		}			
	}
}
void World3D_Tri3D_ProcessTex(World3D* m,Tri3D* t,Vec3D camera,Vec2 output){
	Tri3D tri = *t;
	
	tri.p[0] = Matrix_MultiplyVector(m->model,tri.p[0]);
	tri.p[1] = Matrix_MultiplyVector(m->model,tri.p[1]);
	tri.p[2] = Matrix_MultiplyVector(m->model,tri.p[2]);
	Tri3D_CalcNorm(&tri);
	//Tri3D_ShadeNorm(&tri,(Vec3D){ 0.2f,0.1f,-0.6f });
	
	Vec3D vCameraRay = Vec3D_Sub(tri.p[0],camera);
	float dp = Vec3D_DotProduct(tri.n,vCameraRay);

	char c = (m->normal == WORLD3D_NORMAL_NONE) ||
			 (m->normal == WORLD3D_NORMAL_CAP && dp < 0.0f) ||
			 (m->normal == WORLD3D_NORMAL_RCAP && dp > 0.0f);

	if(c){
		tri.p[0] = Matrix_MultiplyVector(m->view,tri.p[0]);
		tri.p[1] = Matrix_MultiplyVector(m->view,tri.p[1]);
		tri.p[2] = Matrix_MultiplyVector(m->view,tri.p[2]);
		
		int nClippedTri3Ds = 0;
		Tri3D clipped[2];
		nClippedTri3Ds = Tri3D_ClipAgainstPlane(Vec3D_New(0.0f,0.0f,0.01f),Vec3D_New(0.0f,0.0f,1.0f),&tri,&clipped[0],&clipped[1]);
		
		for (int n = 0; n < nClippedTri3Ds; n++){
			clipped[n].p[0] = Matrix_MultiplyVector(m->proj, clipped[n].p[0]);
			clipped[n].p[1] = Matrix_MultiplyVector(m->proj, clipped[n].p[1]);
			clipped[n].p[2] = Matrix_MultiplyVector(m->proj, clipped[n].p[2]);
			
			clipped[n].t[0] = Vec2D_Div(clipped[n].t[0], clipped[n].p[0].w);
			clipped[n].t[1] = Vec2D_Div(clipped[n].t[1], clipped[n].p[1].w);
			clipped[n].t[2] = Vec2D_Div(clipped[n].t[2], clipped[n].p[2].w);

			clipped[n].p[0] = Vec3D_Div(clipped[n].p[0], clipped[n].p[0].w);
			clipped[n].p[1] = Vec3D_Div(clipped[n].p[1], clipped[n].p[1].w);
			clipped[n].p[2] = Vec3D_Div(clipped[n].p[2], clipped[n].p[2].w);
			
			clipped[n].p[0].x *= -1.0f;
			clipped[n].p[1].x *= -1.0f;
			clipped[n].p[2].x *= -1.0f;
			clipped[n].p[0].y *= -1.0f;
			clipped[n].p[1].y *= -1.0f;
			clipped[n].p[2].y *= -1.0f;

			Vec3D vOffsetView = Vec3D_New( 1,1,0 );
			clipped[n].p[0] = Vec3D_Add(clipped[n].p[0], vOffsetView);
			clipped[n].p[1] = Vec3D_Add(clipped[n].p[1], vOffsetView);
			clipped[n].p[2] = Vec3D_Add(clipped[n].p[2], vOffsetView);
			clipped[n].p[0].x *= 0.5f * output.x;
			clipped[n].p[0].y *= 0.5f * output.y;
			clipped[n].p[1].x *= 0.5f * output.x;
			clipped[n].p[1].y *= 0.5f * output.y;
			clipped[n].p[2].x *= 0.5f * output.x;
			clipped[n].p[2].y *= 0.5f * output.y;
			Vector_Push(&m->trisBuff,&clipped[n]);
		}			
	}
}
void World3D_Updated(World3D* m,Vec3D p,Vec2 output){
	void (*process)(World3D*,Tri3D*,Vec3D,Vec2) = m->tex ? World3D_Tri3D_ProcessTex : World3D_Tri3D_Process;

	for(int i = 0;i<m->trisUpdate.size;i++){
		Tri3D* t = (Tri3D*)Vector_Get(&m->trisUpdate,i);
		process(m,t,p,output);
	}
}
void World3D_Clipped(World3D* m,Tri3D* t,Vec2 output){
	Tri3D clipped[2];
	Vector list = Vector_New(sizeof(Tri3D));
	Vector_Push(&list,t);
	
	int nNewTri3Ds = 1;
	for (int p = 0; p < 4; p++){
		int nTrisToAdd = 0;
		while (nNewTri3Ds > 0){
			Tri3D test = *(Tri3D*)Vector_Get(&list,0);
			Vector_Remove(&list,0);
			nNewTri3Ds--;
			
			switch (p){
				case 0:	nTrisToAdd = Tri3D_ClipAgainstPlane(Vec3D_New( 0.0f, 0.0f, 0.0f ), 		  Vec3D_New( 0.0f, 1.0f, 0.0f ), &test,&clipped[0],&clipped[1]); break;
				case 1:	nTrisToAdd = Tri3D_ClipAgainstPlane(Vec3D_New( 0.0f, output.y - 1, 0.0f ),Vec3D_New( 0.0f, -1.0f, 0.0f ),&test,&clipped[0],&clipped[1]); break;
				case 2:	nTrisToAdd = Tri3D_ClipAgainstPlane(Vec3D_New( 0.0f, 0.0f, 0.0f ), 		  Vec3D_New( 1.0f, 0.0f, 0.0f ), &test,&clipped[0],&clipped[1]); break;
				case 3:	nTrisToAdd = Tri3D_ClipAgainstPlane(Vec3D_New( output.x - 1, 0.0f, 0.0f ),Vec3D_New( -1.0f, 0.0f, 0.0f ),&test,&clipped[0],&clipped[1]); break;
			}
			for (int w = 0; w < nTrisToAdd; w++)
				Vector_Push(&list,&clipped[w]);
		}
		nNewTri3Ds = list.size;
	}
	for(int i = 0;i<list.size;i++)
		Vector_Push(&m->trisOut,Vector_Get(&list,i));

	Vector_Free(&list);
}
void World3D_Clip(World3D* m,Vec2 output){
	if(m->tex && (m->output.x != output.x || m->output.y != output.y)){
		if(m->depth) free(m->depth);
		m->depth = (float*)malloc(sizeof(float) * (int)(output.x * output.y));
	}

	//Memset_i32((unsigned int*)m->depth,*(unsigned int*)(float[]){ 1.0E35 },(int)(output.x * output.y));
	if(m->tex)	memset(m->depth,0,sizeof(float) * (int)(output.x * output.y));
	else 		qsort(m->trisBuff.Memory,m->trisBuff.size,m->trisBuff.ELEMENT_SIZE,World3D_Compare);

	for(int i = 0;i<m->trisBuff.size;i++){
		Tri3D* triToRaster = (Tri3D*)Vector_Get(&m->trisBuff,i);
		World3D_Clipped(m,triToRaster,output);
	}
}
void World3D_Update(World3D* m,Vec3D p,Vec2 output){
	Vector_Clear(&m->trisBuff);
	Vector_Clear(&m->trisOut);

	void (*process)(World3D*,Tri3D*,Vec3D,Vec2) = m->tex ? World3D_Tri3D_ProcessTex : World3D_Tri3D_Process;

	for(int i = 0;i<m->trisIn.size;i++){
		Tri3D* t = (Tri3D*)Vector_Get(&m->trisIn,i);
		process(m,t,p,output);
	}
	
	World3D_Updated(m,p,output);
	World3D_Clip(m,output);
	Vector_Clear(&m->trisUpdate);
}

void World3D_Render(unsigned int* Target,int Width,int Height,World3D* m){
	for(int i = 0;i<m->trisOut.size;i++){
		Tri3D* t = (Tri3D*)Vector_Get(&m->trisOut,i);

		if(m->mode==0)
			Triangle_RenderX(
				Target,Width,Height,
				((Vec2){ t->p[0].x, t->p[0].y }),
				((Vec2){ t->p[1].x, t->p[1].y }),
				((Vec2){ t->p[2].x, t->p[2].y }),
				Pixel_Mulf(t->c.c,t->c.l)
			);
		else if(m->mode==1)
			Triangle_RenderXWire(
				Target,Width,Height,
				((Vec2){ t->p[0].x, t->p[0].y }),
				((Vec2){ t->p[1].x, t->p[1].y }),
				((Vec2){ t->p[2].x, t->p[2].y }),
				t->c.c,1.0f
			);	
		else if(m->mode==2){
			//Tri3D_RenderTexX(
			//	Target,Width,Height,m->depth,
			//	((Vic2){ t->p[0].x,t->p[0].y }),t->t[0].u,t->t[0].v,t->t[0].w,
			//	((Vic2){ t->p[1].x,t->p[1].y }),t->t[1].u,t->t[1].v,t->t[1].w,
			//	((Vic2){ t->p[2].x,t->p[2].y }),t->t[2].u,t->t[2].v,t->t[2].w,
			//	//((Vec2){ t->p[0].x,t->p[0].y }),t->t[0],
			//	//((Vec2){ t->p[1].x,t->p[1].y }),t->t[1],
			//	//((Vec2){ t->p[2].x,t->p[2].y }),t->t[2],
			//	Voxel_World_BlockId(&m->map,Block_Side(t->c.f1,t->c.f2)),
			//	t->c.l
			//);
		}
	}
}
void World3D_Free(World3D* m){
	if(m->depth) free(m->depth);
	m->depth = NULL;

	Vector_Free(&m->trisIn);
	Vector_Free(&m->trisUpdate);
	Vector_Free(&m->trisBuff);
	Vector_Free(&m->trisOut);
}

#endif // !LIB3D_WORLD3D_HImage€)