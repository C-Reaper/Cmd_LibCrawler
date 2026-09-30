#ifndef LIBCRAWLER_H
#define LIBCRAWLER_H

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "Files.h"
#include "AlxParser.h"


typedef struct LibCrawlerFile {
	unsigned int size;
	unsigned int padd;
	char* data;
	CStr path;
} LibCrawlerFile;

LibCrawlerFile LibCrawlerFile_New(char* path){
    LibCrawlerFile lcf;
    FilesSize size;
	lcf.data = Files_ReadTB(path,&size);
	if(!lcf.data){
		printf("[LibCrawlerFile]: New -> Path '%s' doesn't exist!\n",path);
	}else{
		printf("[LibCrawler]: New -> Found '%s'\n",path);
	}

    lcf.size = size;
    lcf.path = CStr_Cpy(path);
	return lcf;
}
LibCrawlerFile LibCrawlerFile_Link(char* path){
    LibCrawlerFile lcf;
	lcf.data = NULL;
    lcf.size = 0U;
    lcf.path = CStr_Cpy(path);
	return lcf;
}
void LibCrawlerFile_LoadData(LibCrawlerFile* lcf){
    if(lcf->data) free(lcf->data);
	lcf->data = NULL;

    FilesSize size;
    lcf->data = Files_ReadTB(lcf->path,&size);
	if(!lcf->data){
		printf("[LibCrawlerFile]: LoadData -> Path '%s' doesn't exist!\n",lcf->path);
	}else{
		printf("[LibCrawler]: LoadData -> Found '%s'\n",lcf->path);
		lcf->size = size;
	}
}
void LibCrawlerFile_Free(LibCrawlerFile* lcf){
    if(lcf->data) free(lcf->data);
	lcf->data = NULL;

    CStr_Free(&lcf->path);
}


typedef struct LibCrawler {
	Vector headers;//Vector<LibCrawlerFile>
	CStr dir;
} LibCrawler;

LibCrawler LibCrawler_New(char* dir){
	Files_Mkdir(dir);

    char path[256];
	sprintf(path,"%s/Library",dir);
	Files_Mkdir(path);

	sprintf(path,"%s/Container",dir);
	Files_Mkdir(path);

	LibCrawler pm;
    pm.headers = Vector_New(sizeof(LibCrawlerFile));
    pm.dir = CStr_Cpy(dir);
	return pm;
}
char LibCrawler_Contains(LibCrawler* pm,char* path){
	for(int i = 0;i<pm->headers.size;i++){
		LibCrawlerFile* lcf = (LibCrawlerFile*)Vector_Get(&pm->headers,i);
		if(CStr_Cmp(lcf->path,path))
			return 1;
	}
	return 0;
}
void LibCrawler_Find(LibCrawler* pm,LibCrawlerFile* lcf){
    Parser p = Parser_New();
    Parser_Parse_CStr(&p,lcf->data,lcf->path);
    Parser_TF_Num(&p);
    Parser_TF_Esc(&p);
    Parser_TF_Std(&p);
	//Parser_Print(&p);

	char changed = 0;
	Vec_CStr new_fc = CStr_ChopDown(lcf->data,'\n');

	for(int i = 0;i + 2 < p.size;i++){
		Token* hash = (Token*)Vector_Get(&p,i);
		Token* include = (Token*)Vector_Get(&p,i + 1);
		Token* cstr = (Token*)Vector_Get(&p,i + 2);
		
		if(hash->tt == TOKEN_HASH_POUND_SIGN && include->tt == TOKEN_STRING && cstr->tt == TOKEN_CONSTSTRING_DOUBLE){
			if(CStr_Cmp(include->str,"include")){
				char* cpath = NULL;
				
				if(cstr->str[0] == '/'){
					cpath = CStr_Cpy(cstr->str);
					printf("Replacing Abs Path: '%s' into (l. %u,%u)\n",cstr->str,cstr->line,cstr->ch);

					if(cstr->line > 0U && cstr->ch > 0U){
						const unsigned int line = cstr->line - 1U;
						const unsigned int ch = cstr->ch - 1U;

						CStr new_subname = Files_NameStep(cstr->str,1);
						CStr new_name = CStr_Format("../%s/%s",basename(pm->dir),new_subname);

						CStr* line_pcstr = (CStr*)Vector_Get(&new_fc,line);
						printf("  Content: '%s'\n\n",*line_pcstr);

						if(new_subname && new_name && line < new_fc.size){
							printf("  >> %s to %s\n",new_subname,new_name);
							
							const unsigned int end = CStr_Find(*line_pcstr + ch + 1U,'\"');
							
							String line_buffer = String_Make(*line_pcstr);
							String_RemoveCount(&line_buffer,ch + 1U,end);
							String_Add(&line_buffer,new_name,ch + 1U);
							String_AppendChar(&line_buffer,'\0');

							CStr buffer_out = (CStr)line_buffer.Memory;
							CStr_Set(line_pcstr,buffer_out);
							//printf("  Set '%s' to '%s'\n",*line_pcstr,buffer_out);
							String_Free(&line_buffer);

							changed = 1;
						}

						CStr_Free(&new_name);
						CStr_Free(&new_subname);
					}
				}else{
					char* ppath = Files_Path(lcf->path);
					cpath = Files_FromPath(ppath,cstr->str);
					CStr_Free(&ppath);
				}
				
				if(!LibCrawler_Contains(pm,cpath)){
					LibCrawlerFile lcf_found = LibCrawlerFile_New(cpath);
					Vector_Push(&pm->headers,&lcf_found);
					LibCrawler_Find(pm,&lcf_found);
				}

				CStr_Free(&cpath);
			}
		}
	}

	if(changed){
		String output = Vec_CStr_AddUp_S(&new_fc,'\n');
		printf(">>>>{ Write:\n'%*s'\n>>>>{ to %s\n",output.size,(char*)output.Memory,lcf->path);
		Files_WriteT(lcf->path,output.Memory,output.size);
		String_Free(&output);
	}
	Vec_CStr_Free(&new_fc);

	Parser_Free(&p);
}
void LibCrawler_Build(LibCrawler* pm){
    for(int i = 0;i<pm->headers.size;i++){
		LibCrawlerFile* lcf = (LibCrawlerFile*)Vector_Get(&pm->headers,i);
		CStr name = Files_DirNameStepBack(pm->dir,lcf->path,1);
		printf("[LibCrawler]: Build -> '%s' to '%s'\n",lcf->path,name);
		Files_WriteT(name,lcf->data,lcf->size);
		CStr_Free(&name);
	}
}
void LibCrawler_EntryFind(LibCrawler* pm,char* rpath){
	LibCrawlerFile lcf_found = LibCrawlerFile_New(rpath);
	Vector_Push(&pm->headers,&lcf_found);
	LibCrawler_Find(pm,&lcf_found);
}
void LibCrawler_FindAdd(LibCrawler* pm,LibCrawlerFile* lcf){
    Parser p = Parser_New();
    Parser_Parse_CStr(&p,lcf->data,lcf->path);
    Parser_TF_Num(&p);
    Parser_TF_Esc(&p);
    Parser_TF_Std(&p);

	//Parser_Print(&p);

	for(int i = 0;i + 2<p.size;i++){
		Token* hash = (Token*)Vector_Get(&p,i);
		Token* include = (Token*)Vector_Get(&p,i + 1);
		Token* cstr = (Token*)Vector_Get(&p,i + 2);
		
		if(hash->tt == TOKEN_HASH_POUND_SIGN && include->tt == TOKEN_STRING && cstr->tt == TOKEN_CONSTSTRING_DOUBLE){
			if(CStr_Cmp(include->str,"include")){
				char* cpath = NULL;
				if(cstr->str[0] == '/'){
					cpath = CStr_Cpy(cstr->str);
				}else{
					char* ppath = Files_Path(lcf->path);
					cpath = Files_FromPath(ppath,cstr->str);
					CStr_Free(&ppath);
				}
				
				if(!LibCrawler_Contains(pm,cpath)){
					LibCrawlerFile lcf_found = LibCrawlerFile_Link(cpath);
					Vector_Push(&pm->headers,&lcf_found);
				}

				CStr_Free(&cpath);
			}
		}
	}

	Parser_Free(&p);
}
void LibCrawler_Clear(LibCrawler* pm){
    for(int i = 0;i<pm->headers.size;i++){
		LibCrawlerFile* lcf = (LibCrawlerFile*)Vector_Get(&pm->headers,i);
		LibCrawlerFile_Free(lcf);
	}
	Vector_Clear(&pm->headers);
}
void LibCrawler_Free(LibCrawler* pm){
    for(int i = 0;i<pm->headers.size;i++){
		LibCrawlerFile* lcf = (LibCrawlerFile*)Vector_Get(&pm->headers,i);
		LibCrawlerFile_Free(lcf);
	}
	Vector_Free(&pm->headers);
	
	CStr_Free(&pm->dir);
}

#endif //!LIBCRAWLER_H