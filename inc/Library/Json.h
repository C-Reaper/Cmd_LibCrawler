#ifndef JSON_H
#define JSON_H

#include "../Container/Tree.h"
#include "../Container/DataStream.h"

#include "AlxParser.h"
#include "Files.h"

#define JSON_STATE_NONE			0U
#define JSON_STATE_ERROR		1U
#define JSON_STATE_NAME			2U
#define JSON_STATE_COLON		3U
#define JSON_STATE_VALUE		4U
#define JSON_STATE_OBJECT		5U
#define JSON_STATE_LIST			6U
#define JSON_STATE_COMMA		7U

typedef int Json_State;

typedef struct Json_Pair {
	CStr tag;
	CStr value;
} Json_Pair;

Json_Pair Json_Pair_New(CStr tag,CStr value){
	Json_Pair yp;
	yp.tag = CStr_Cpy(tag);
	yp.value = CStr_Cpy(value);
	return yp;
}
Json_Pair Json_Pair_Move(CStr tag,CStr value){
	Json_Pair yp;
	yp.tag = tag;
	yp.value = value;
	return yp;
}
void Json_Pair_Free(Json_Pair* yp){
	CStr_Free(&yp->tag);
	CStr_Free(&yp->value);
}



#define JSON_ACCESS			"."
#define JSON_TOKEN_ACCESS	TOKEN_PERIOD
#define JSON_TOKEN_DATE		(TOKEN_START + 0)
#define JSON_TYPE			"json"

typedef struct Json {
	Tree t;
} Json;

char Json_IsLabel(Token* t){
	return t->tt == TOKEN_STRING || t->tt == TOKEN_CONSTSTRING_SINGLE || t->tt == TOKEN_CONSTSTRING_DOUBLE;
}
char Json_IsDate(TokenMap* tm,unsigned int start,unsigned int end){
	if(end - start == 5){
		Token* a0 = (Token*)Vector_Get(tm,start + 0);
		Token* c0 = (Token*)Vector_Get(tm,start + 1);
		Token* a1 = (Token*)Vector_Get(tm,start + 2);
		Token* c1 = (Token*)Vector_Get(tm,start + 3);
		Token* a2 = (Token*)Vector_Get(tm,start + 4);

		return (
			a0->tt == TOKEN_NUMBER &&
			a1->tt == TOKEN_NUMBER &&
			a2->tt == TOKEN_NUMBER
		) && (
			(c0->tt == TOKEN_COLON && c1->tt == TOKEN_COLON) ||
			(c0->tt == TOKEN_MINUS_SIGN && c1->tt == TOKEN_MINUS_SIGN)
		);
	}
	return 0;
}
CStr Json_GetDate(TokenMap* tm,unsigned int start,unsigned int end){
	String Buffer = String_New();

	for(;start < end;start++){
		Token* t = (Token*)Vector_Get(tm,start);
		Token_Append(t,&Buffer);
	}

	String_AppendChar(&Buffer,'\0');
	return Buffer.Memory;
}

char Json_IntoList(TokenMap* tm,Branch* b,unsigned int start,unsigned int end){
	while(start < end){
		TT_Iter it = TokenMap_FindS(tm,TOKEN_COMMA,start);
		if(it < 0) it = end;

		Token* t = (Token*)Vector_Get(tm,start);
		if(it - start == 1U && (Json_IsLabel(t) || t->tt == TOKEN_NUMBER || t->tt == TOKEN_FLOAT)){
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_Move(NULL,Token_CStr(t)) },sizeof(Json_Pair));
			Branch_Add(b,newb);
		}else if(Json_IsDate(tm,start,it)){
			CStr date = Json_GetDate(tm,start,it);
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_Move(NULL,date) },sizeof(Json_Pair));
			Branch_Add(b,newb);
		}else{
			return 0;
		}

		start = it + 1U;
	}
	return 1;
}
Branch* Json_LastBranch(Json* yl,unsigned int depth){
	Branch* b = yl->t.Root;
	
	for(;depth > 0U && b->Childs.size > 0;depth--){
		b = *(Branch**)Vector_Get(&b->Childs,b->Childs.size - 1);
	}
	
	if(depth > 0U){
		printf("[Json]: LastBranch -> Depth not reached: %u\n",depth);
	}
	return b;
}
Branch* Json_LastParent(Json* yl,unsigned int depth){
	Branch* b = yl->t.Root;
	
	for(;depth > 1U && b->Childs.size > 0;depth--){
		b = *(Branch**)Vector_Get(&b->Childs,b->Childs.size - 1);
	}
	
	if(depth > 1U){
		printf("[Json]: LastParent -> Depth not reached: %u\n",depth);
	}
	return b;
}

Boolean Json_IsObject(Json* yl,Branch* b){
	if(b->Childs.size <= 0) return False;
	
	Branch* cb = *(Branch**)Vector_Get(&b->Childs,0);
	if(!cb->Memory) return False;

	Json_Pair* jp = (Json_Pair*)cb->Memory;
	return jp->tag != NULL;
}
Boolean Json_IsList(Json* yl,Branch* b){
	if(b->Childs.size <= 0) return False;
	
	Branch* cb = *(Branch**)Vector_Get(&b->Childs,0);
	if(!cb->Memory) return False;

	Json_Pair* jp = (Json_Pair*)cb->Memory;
	return jp->tag == NULL;
}

Json_State Json_Build_None(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_CURLY_BRACES_L:{
			Branch* b = Json_LastBranch(yl,*depth);
			if(!b->Memory){
				Branch_Set(b,(Json_Pair[]){ Json_Pair_Move(NULL,NULL) },sizeof(Json_Pair));
			}
			(*depth)++;
			return JSON_STATE_OBJECT;
		} break;
		case TOKEN_SQUARE_BRACKETS_L:{
			Branch* b = Json_LastBranch(yl,*depth);
			if(!b->Memory){
				Branch_Set(b,(Json_Pair[]){ Json_Pair_Move(NULL,NULL) },sizeof(Json_Pair));
			}
			(*depth)++;
			return JSON_STATE_LIST;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_None -> Token not expected: '%s'!\n",cstr);
			CStr_Free(&cstr);
			return False;
		}
	}
	return True;
}
Json_State Json_Build_Name(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_COLON:{
			return JSON_STATE_COLON;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_Name -> Token not expected: '%s'!\n",cstr);
			CStr_Free(&cstr);
			return JSON_STATE_ERROR;
		}
	}
}
Json_State Json_Build_Colon(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_STRING:
		case TOKEN_CONSTSTRING_SINGLE:
		case TOKEN_CONSTSTRING_DOUBLE:
		case TOKEN_NUMBER:
		case TOKEN_FLOAT:
		case TOKEN_BOOL:{
			Branch* b = Json_LastBranch(yl,*depth);
			if(!b->Memory){
				CStr cstr = Token_CStr(t);
				printf("[Json]: Build_Colon -> Branch Pair is null: '%s'!\n",cstr);
				CStr_Free(&cstr);
				return JSON_STATE_ERROR;
			}

			Json_Pair* jp = (Json_Pair*)b->Memory;
			CStr_Free(&jp->value);
			jp->value = Token_CStr(t);
			return JSON_STATE_VALUE;
		} break;
		case TOKEN_CURLY_BRACES_L:{
			(*depth)++;
			return JSON_STATE_OBJECT;
		} break;
		case TOKEN_SQUARE_BRACKETS_L:{
			(*depth)++;
			return JSON_STATE_LIST;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_Colon -> Token unkown: '%s'!\n",cstr);
			CStr_Free(&cstr);
			return JSON_STATE_ERROR;
		}
	}
}
Json_State Json_Build_Value(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_COMMA:{
			return JSON_STATE_COMMA;
		} break;
		case TOKEN_CURLY_BRACES_R:
		case TOKEN_SQUARE_BRACKETS_R:{
			(*depth)--;
			return JSON_STATE_VALUE;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_Value -> Token unkown: '%s'!\n",cstr);
			CStr_Free(&cstr);
			return JSON_STATE_ERROR;
		}
	}
}
Json_State Json_Build_Object(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_CONSTSTRING_DOUBLE:{
			Branch* b = Json_LastParent(yl,*depth);
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_Move(Token_CStr(t),NULL) },sizeof(Json_Pair));
			Branch_Add(b,newb);
			return JSON_STATE_NAME;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_Object -> Token unkown: '%s' (%u)!\n",cstr,t->tt);
			CStr_Free(&cstr);
			return JSON_STATE_ERROR;
		}
	}
}
Json_State Json_Build_List(Json* yl,Token* t,unsigned int* depth){
	switch(t->tt){
		case TOKEN_STRING:
		case TOKEN_CONSTSTRING_SINGLE:
		case TOKEN_CONSTSTRING_DOUBLE:
		case TOKEN_NUMBER:
		case TOKEN_FLOAT:
		case TOKEN_BOOL:{
			Branch* b = Json_LastParent(yl,*depth);
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_Move(NULL,Token_CStr(t)) },sizeof(Json_Pair));
			Branch_Add(b,newb);
			return JSON_STATE_VALUE;
		} break;
		case TOKEN_CURLY_BRACES_L:{
			Branch* b = Json_LastParent(yl,*depth);
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_New(NULL,NULL) },sizeof(Json_Pair));
			Branch_Add(b,newb);
			(*depth)++;
			return JSON_STATE_OBJECT;
		} break;
		case TOKEN_SQUARE_BRACKETS_L:{
			Branch* b = Json_LastParent(yl,*depth);
			Branch* newb = Branch_New((Json_Pair[]){ Json_Pair_New(NULL,NULL) },sizeof(Json_Pair));
			Branch_Add(b,newb);
			(*depth)++;
			return JSON_STATE_LIST;
		} break;
		default:{
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build_List -> Token unkown: '%s'!\n",cstr);
			CStr_Free(&cstr);
			return JSON_STATE_ERROR;
		}
	}
}
Json_State Json_Build_Comma(Json* yl,Token* t,unsigned int* depth){
	Branch* b = Json_LastParent(yl,*depth);
	
	if(Json_IsObject(yl,b)){
		return Json_Build_Object(yl,t,depth);
	}else if(Json_IsList(yl,b)){
		return Json_Build_List(yl,t,depth);
	}else{
		CStr cstr = Token_CStr(t);
		printf("[Json]: Build_Comma -> Token expected to be in { or [: '%s'!\n",cstr);
		CStr_Free(&cstr);
		return JSON_STATE_ERROR;
	}
}

Boolean Json_Build(Json* yl,TokenMap* tm){
	unsigned int depth = 0U;
	Json_State js = JSON_STATE_NONE;
	
	Json_State (*func[])(Json*,Token*,unsigned int*) = {
		Json_Build_None,
		NULL,
		Json_Build_Name,
		Json_Build_Colon,
		Json_Build_Value,
		Json_Build_Object,
		Json_Build_List,
		Json_Build_Comma
	};

	for(unsigned int i = 0;i<tm->size;i++){
		Token* t = (Token*)Vector_Get(tm,i);

		if(js == JSON_STATE_ERROR || js >= sizeof(func) / sizeof(*func)){
			CStr cstr = Token_CStr(t);
			printf("[Json]: Build -> State %u unkown: '%s'!\n",js,cstr);
			printf("[Json]: Build -> Next Token: '%s' (%s,l.%d)\n",cstr,t->file,t->line);
			CStr_Free(&cstr);
			return False;
		}
		
		js = func[js](yl,t,&depth);
	}
	if(js == JSON_STATE_ERROR)
		return False;

	if(js != JSON_STATE_VALUE){
		printf("[Json]: Build -> Return State unkown!\n");
		return False;
	}

	return True;
}
Boolean Json_Add(Json* yl,char* path){
	if(!path) return False;
	
	CStr type = Files_Type(path);
	
	if(CStr_Cmp(type,JSON_TYPE)){
		CStr data = Files_Read(path);
		
		if(data){
			Parser p = Parser_New();
        	Parser_Parse_CStr(&p,data,path);
        	Parser_TF_Std(&p);

			TT_TypeMap ttm = TT_TypeMap_Make((TT_Type[]){
				TOKEN_NEWLINE,
				TOKEN_HTAB,
				TOKEN_CARTURN,
				TOKEN_NONE
			});
			Parser_Ban(&p,&ttm);
			TT_TypeMap_Free(&ttm);

        	Boolean ret = Json_Build(yl,&p);
        	Parser_Free(&p);
			CStr_Free(&data);
			CStr_Free(&type);
			return ret;
		}else{
			printf("[Json]: New -> path invalid: %s\n",path);
		}
	}else{
		printf("[Json]: New -> type is %s but format should be " JSON_TYPE "\n",type ? type : "invalid");
	}

	CStr_Free(&type);
	return False;
}
Boolean Json_Add_C(Json* yl,char* data,char* path){
	if(data){
		Parser p = Parser_New();
    	Parser_Parse_CStr(&p,data,path);
    	Parser_TF_Std(&p);
		
		TT_TypeMap ttm = TT_TypeMap_Make((TT_Type[]){
			TOKEN_NEWLINE,
			TOKEN_NONE
		});
		
		Parser_Ban(&p,&ttm);
		TT_TypeMap_Free(&ttm);
    	
		Boolean ret = Json_Build(yl,&p);
    	Parser_Free(&p);
		return ret;
	}else{
		printf("[Json]: New -> data invalid!\n");
	}
	return False;
}

Json Json_New(){
	Json yl;
	yl.t = Tree_New();
	return yl;
}
Json Json_Make(char* path){
	Json yl = Json_New();
	Json_Add(&yl,path);
	return yl;
}
Json Json_By(char* data){
	Json yl = Json_New();
	Json_Add_C(&yl,data,NULL);
	return yl;
}
Json Json_Null(){
	Json yl;
	yl.t = Tree_Null();
	return yl;
}

Branch* Json_Into(Json* yl,Branch* b,char* path){
	if(!b || !path) return NULL;
	
	const Number n = Number_Parse((i8*)path);
	const char isNum = n != NUMBER_PARSE_ERROR;

	if(isNum){
		if(n>=0 && n < b->Childs.size)
			return *(Branch**)Vector_Get(&b->Childs,n);
		return NULL;
	}

	for(int i = 0;i<b->Childs.size;i++){
		Branch* cb = *(Branch**)Vector_Get(&b->Childs,i);
		Json_Pair* yp = (Json_Pair*)cb->Memory;

		if(CStr_Cmp(yp->tag,path))
			return cb;
	}
	return NULL;
}
Branch* Json_GetBranch(Json* yl,char* jsonpath){
	Vector path = CStr_ChopDown(jsonpath,'/');
	Branch* bb = yl->t.Root;
	
	for(int i = 0;i<path.size;i++){
		CStr* cstr = (CStr*)Vector_Get(&path,i);
		bb = Json_Into(yl,bb,*cstr);
		CStr_Free(cstr);
	}
	
	Vector_Free(&path);
	return bb;
}
Json_Pair* Json_Get(Json* yl,char* jsonpath){
	Branch* bb = Json_GetBranch(yl,jsonpath);

	if(bb){
		Json_Pair* yp = (Json_Pair*)bb->Memory;
		return yp;
	}
	return NULL;
}
void Json_Set(Json* yl,char* jsonpath,char* value){
	Vector path = CStr_ChopDown(jsonpath,'/');
	Branch* bb = yl->t.Root;
	
	for(int i = 0;i<path.size;i++){
		CStr* cstr = (CStr*)Vector_Get(&path,i);
		bb = Json_Into(yl,bb,*cstr);
		CStr_Free(cstr);
	}

	if(bb){
		Json_Pair* yp = (Json_Pair*)bb->Memory;
		CStr_Set(&yp->value,value);
	}
}
CStr Json_GetCStr(Json* yl,char* jsonpath){
	Json_Pair* yp = Json_Get(yl,jsonpath);
	if(yp) return yp->value;
	printf("[Json]: Tree -> element in path '%s' doesn't exist!\n",jsonpath);
	return NULL;
}
Number Json_GetNumber(Json* yl,char* jsonpath){
	Json_Pair* yp = Json_Get(yl,jsonpath);
	if(yp) return Number_Parse((i8*)yp->value);
	printf("[Json]: Tree -> element in path '%s' doesn't exist!\n",jsonpath);
	return NUMBER_PARSE_ERROR;
}
Double Json_GetDouble(Json* yl,char* jsonpath){
	Json_Pair* yp = Json_Get(yl,jsonpath);
	if(yp) return Double_Parse((i8*)yp->value,1);
	printf("[Json]: Tree -> element in path '%s' doesn't exist!\n",jsonpath);
	return DOUBLE_PARSE_ERROR;
}
Boolean Json_GetBool(Json* yl,char* jsonpath){
	Json_Pair* yp = Json_Get(yl,jsonpath);
	if(yp) return Boolean_Parse((i8*)yp->value);
	printf("[Json]: Tree -> element in path '%s' doesn't exist!\n",jsonpath);
	return BOOL_PARSE_ERROR;
}

Boolean Json_Branch_IsValue(Json* yl,Branch* n){
	Json_Pair* yp = (Json_Pair*)n->Memory;
	return yp && !yp->tag && yp->value;
}
Boolean Json_Branch_IsList(Json* yl,Branch* n){
	if(n->Childs.size == 0) return 0; 
	
	for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        
		if(!Json_Branch_IsValue(yl,c)){
			return 0;
		}
    }
	return 1;
}

void Json_Branch_Save(Json* yl,Branch* n,int index,int align,DataStream* ds){
	if(!n) return;
	
    for(int i = 0;i<align * 2;i++) DataStream_Printf(ds,"  ");
    
    if(Json_IsObject(yl,n)){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		DataStream_Printf(ds,"\"%s\": ",yp->tag);
		//else			DataStream_Printf(ds,"%d: ",index);
		
		DataStream_Printf(ds,"{\n");

		for(int i = 0;i<n->Childs.size;i++){
    	    Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
    	    Json_Branch_Save(yl,c,i,align + 1,ds);
    	}
	
    	for(int i = 0;i<align * 2;i++) DataStream_Printf(ds,"  ");
		DataStream_Printf(ds,"}");
	}else if(Json_IsList(yl,n)){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		DataStream_Printf(ds,"\"%s\": ",yp->tag);
		//else			DataStream_Printf(ds,"%d: ",index);
		
		DataStream_Printf(ds,"[\n");
		
		for(int i = 0;i<n->Childs.size;i++){
    	    Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
    	    Json_Branch_Save(yl,c,i,align + 1,ds);
    	}
		
    	for(int i = 0;i<align * 2;i++) DataStream_Printf(ds,"  ");
		DataStream_Printf(ds,"]");
	}else if(n->Memory){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		DataStream_Printf(ds,"\"%s\": ",yp->tag);
		//else			DataStream_Printf(ds,"%d: ",index);

		if(yp->value){
			DataStream_Printf(ds,"\"");
			DataStream_Print_N(ds,yp->value);
			DataStream_Printf(ds,"\"");
		}
	}else{
		DataStream_Printf(ds,"[!ERROR!]");
	}

	Branch* pb = n->Parent;
	if(pb && index + 1 < pb->Childs.size){
		DataStream_Printf(ds,",");
	}
	DataStream_Printf(ds,"\n");
}
void Json_Save(Json* yl,char* path){
	DataStream ds = DataStream_New();
	Json_Branch_Save(yl,yl->t.Root,-1,0,&ds);
	Files_WriteT(path,ds.Memory,ds.size);
	DataStream_Free(&ds);
}

void Json_Branch_Print(Json* yl,Branch* n,int index,int align){
	if(!n) return;
	
	printf("| ");
    for(int i = 0;i<align * 2;i++) printf(" ");
    
    if(Json_IsObject(yl,n)){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		printf("\"%s\": ",yp->tag);
		else			printf("%d: ",index);
		
		printf("{\n");

		for(int i = 0;i<n->Childs.size;i++){
    	    Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
    	    Json_Branch_Print(yl,c,i,align + 1);
    	}
	
		printf("| ");
    	for(int i = 0;i<align * 2;i++) printf(" ");
		printf("}");
	}else if(Json_IsList(yl,n)){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		printf("\"%s\": ",yp->tag);
		else			printf("%d: ",index);
		
		printf("[\n");
		
		for(int i = 0;i<n->Childs.size;i++){
    	    Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
    	    Json_Branch_Print(yl,c,i,align + 1);
    	}
		
		printf("| ");
    	for(int i = 0;i<align * 2;i++) printf(" ");
		printf("]");
	}else if(n->Memory){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		if(yp->tag)		printf("\"%s\": ",yp->tag);
		else			printf("%d: ",index);

		if(yp->value)
			printf("\"%s\"",yp->value);
	}else{
		printf("[!ERROR!]");
	}

	Branch* pb = n->Parent;
	if(pb && index + 1 < pb->Childs.size){
		printf(",");
	}
	printf("\n");
}
void Json_Print(Json* yl){
	printf("---- Json ----\n");
	Json_Branch_Print(yl,yl->t.Root,-1,0);
	printf("--------------\n");
}
void Json_Branch_Free(Json* yl,Branch* n){
	if(!n) return;
	
	if(n->Memory){
		Json_Pair* yp = (Json_Pair*)n->Memory;
		Json_Pair_Free(yp);
	}

    for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        Json_Branch_Free(yl,c);
    }
}
void Json_Clear(Json* yl){
	if(yl->t.Root){
		Json_Branch_Free(yl,yl->t.Root);
		Tree_Free(&yl->t);
		yl->t = Tree_New();
	}
}
void Json_Free(Json* yl){
	Json_Branch_Free(yl,yl->t.Root);
	Tree_Free(&yl->t);
}

#endif //!JSON_HA