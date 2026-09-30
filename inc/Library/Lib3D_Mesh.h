#ifndef LIB3D_MESH_H
#define LIB3D_MESH_H

#include "Files.h"
#include "ConstParser.h"
#include "Intrinsics.h"
#include "Lib3D_Mathlib.h"


typedef Vector Mesh;// Vector<Tri3D>

Mesh Mesh_New(){
	return Vector_New(sizeof(Tri3D));
}
void Mesh_Shade(Mesh* m,Vec3D dir){
	for(int i = 0;i<m->size;i++){
		Tri3D* t = (Tri3D*)Vector_Get(m,i);
		Tri3D_ShadeNorm(t,dir);
	}
}
void Mesh_Read(Mesh* m,char* Path){
	char c = Files_isFile(Path);
	if(!c){
		printf("[Mesh]: Read -> Error!\n");
		return;
	}

	Vector verts = Vector_New(sizeof(Vec3D));

	FilesSize fs;
	char* Content = Files_ReadTB(Path,&fs);
	int start = 0;
	int end = 0;
	
	while(1){
		start = end;
		end = start + CStr_Find(Content + start,'\n') + 1;
		if(start==end || end<0) break;

		char* lineIn = CStr_ChopTo(Content + start,'\n');

		//char junk;
		if(lineIn[0] == 'v'){
			Vec3D v = Vec3D_Null();
			
			Vector vec = CStr_ChopDown(lineIn,' ');
			//junk = *(char*)Vector_Get(&vec,0);
			v.x = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,1),DOUBLE_SIGNED);
			v.y = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,2),DOUBLE_SIGNED);
			v.z = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,3),DOUBLE_SIGNED);
			
			for(int i = 0;i<vec.size;i++) free(*(char**)Vector_Get(&vec,i));
			Vector_Free(&vec);

			Vector_Push(&verts,&v);
		}else if(lineIn[0] == 'f'){
			int f[3];

			Vector vec = CStr_ChopDown(lineIn,' ');
			//junk = *(char*)Vector_Get(&vec,0);
			f[0] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,1));
			f[1] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,2));
			f[2] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,3));
			for(int i = 0;i<vec.size;i++) free(*(char**)Vector_Get(&vec,i));
			Vector_Free(&vec);
			
			Tri3D t = {
				.p = {
					*(Vec3D*)Vector_Get(&verts,f[0] - 1),
					*(Vec3D*)Vector_Get(&verts,f[1] - 1),
					*(Vec3D*)Vector_Get(&verts,f[2] - 1)
				},
				.c = {
					.c = 0xFFFFFFFF,
					.l = 1.0f
				}
			};
			Tri3D_CalcNorm(&t);
			Vector_Push(m,&t);
		}
	
		CStr_Free(&lineIn);
	}
	Vector_Free(&verts);
}
void Mesh_ReadTex(Mesh* m,char* Path,char hasTexture){
	if(!Files_isFile(Path)){
		printf("[Mesh]: Read -> Error!\n");
		return;
	}

	Vector verts = Vector_New(sizeof(Vec3D));
	Vector texs = Vector_New(sizeof(Vec2D));

	FilesSize fs;
	char* Content = Files_ReadTB(Path,&fs);
	int start = 0;
	int end = 0;
	
	while(1){
		char* line = Content + end + 1;
		end = CStr_Find(line,'\n');
		if(end<0) break;

		char* lineIn = CStr_ChopTo(Content + start,'\n');

		if (line[0] == 'v'){
			if (line[1] == 't'){
				Vec2D v;
			
				Vector vec = CStr_ChopDown(lineIn,' ');
				//junk = *(char*)Vector_Get(&vec,0);
				v.u = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,1),DOUBLE_SIGNED);
				v.v = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,2),DOUBLE_SIGNED);
				
				for(int i = 0;i<vec.size;i++) free(*(char**)Vector_Get(&vec,i));
				Vector_Free(&vec);

				Vector_Push(&texs,&v);
			}else{
				Vec3D v = Vec3D_Null();
			
				Vector vec = CStr_ChopDown(lineIn,' ');
				//junk = *(char*)Vector_Get(&vec,0);
				v.x = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,1),DOUBLE_SIGNED);
				v.y = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,2),DOUBLE_SIGNED);
				v.z = (float)Double_Parse((i8*)*(char**)Vector_Get(&vec,3),DOUBLE_SIGNED);
				
				for(int i = 0;i<vec.size;i++) free(*(char**)Vector_Get(&vec,i));
				Vector_Free(&vec);

				Vector_Push(&verts,&v);
			}
		}
		if (!hasTexture){
			int f[3];

			Vector vec = CStr_ChopDown(lineIn,' ');
			//junk = *(char*)Vector_Get(&vec,0);
			f[0] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,1));
			f[1] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,2));
			f[2] = (float)Number_Parse((i8*)*(char**)Vector_Get(&vec,3));
			for(int i = 0;i<vec.size;i++) free(*(char**)Vector_Get(&vec,i));
			Vector_Free(&vec);
			
			Tri3D t = {
				.p = {
					*(Vec3D*)Vector_Get(&verts,f[0] - 1),
					*(Vec3D*)Vector_Get(&verts,f[1] - 1),
					*(Vec3D*)Vector_Get(&verts,f[2] - 1)
				},
				.c = {
					.c = 0xFFFFFFFF,
					.l = 1.0f
				}
			};
			Tri3D_CalcNorm(&t);
			Vector_Push(m,&t);
		}else{
			if (line[0] == 'f'){
				// s >> junk;
				// String tokens[6] = {String_New(),String_New(),String_New(),String_New(),String_New(),String_New()};
				// int nTokenCount = -1;
				// while (!s.eof())
				// {
				// 	char c = s.get();
				// 	if (c == ' ' || c == '/')
				// 		nTokenCount++;
				// 	else
				// 		tokens[nTokenCount].append(1, c);
				// }
				// tokens[nTokenCount].pop_back();
				// tris.push_back({ 
				// 	verts[stoi(tokens[0]) - 1],
				// 	verts[stoi(tokens[2]) - 1],
				// 	verts[stoi(tokens[4]) - 1],
				// 	texs[stoi(tokens[1]) - 1],
				// 	texs[stoi(tokens[3]) - 1],
				// 	texs[stoi(tokens[5]) - 1]
				// });
			}
		}
	}
}
void Mesh_Write(Mesh* m,char* Path){
	
}
void Mesh_Free(Mesh* m){
	Vector_Free(m);
}

#endif