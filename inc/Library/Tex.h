#ifndef TEX_H
#define TEX_H

#include "../Container/Vector.h"
#include "../Container/Pair.h"

#include "Json.h"
#include "ConstParser.h"
#include "String.h"

#define HIGHLIGHT_SELECTOR_CSTR          "CSTR"
#define HIGHLIGHT_SELECTOR_CHAR          "CHAR"
#define HIGHLIGHT_SELECTOR_STR           "STR"
#define HIGHLIGHT_SELECTOR_NUM           "NUM"
#define HIGHLIGHT_SELECTOR_CUSTOM        "CUSTOM"
#define HIGHLIGHT_SELECTOR_LINE          "LINE"
#define HIGHLIGHT_SELECTOR_LINE_START    "START"
#define HIGHLIGHT_SELECTOR_LINE_COLOR    "COLOR"
#define HIGHLIGHT_SELECTOR_BLOCK         "BLOCK"
#define HIGHLIGHT_SELECTOR_BLOCK_START   "START"
#define HIGHLIGHT_SELECTOR_BLOCK_END     "END"
#define HIGHLIGHT_SELECTOR_BLOCK_COLOR   "COLOR"

typedef struct SymbolInfo {
    unsigned int c;
} SymbolInfo;

typedef enum HighLight_State {
    HIGHLIGHT_NONE = 0,
    HIGHLIGHT_CSTR = 1,
    HIGHLIGHT_CHAR = 2,
    HIGHLIGHT_CUSTOM = 3,
    HIGHLIGHT_CLINE = 4,
    HIGHLIGHT_CBLOCK = 5,
    HIGHLIGHT_CBLOCKRET = 6
} HighLight_State;

typedef Json HighLight;
typedef Vector Tex;

void Tex_SyncString(Tex* t,String* s){
    if(t->size!=s->size){
        if(t->size<s->size){
            int Delta = s->size - t->size;
            for(int i = 0;i<Delta;i++){
                SymbolInfo si = { 0xFFFFFFFF };
                Vector_Push(t,&si);
            }
        }else{
            int Delta = t->size - s->size;
            for(int i = 0;i<Delta;i++){
                Vector_PopTop(t);
            }
        }
    }
}

unsigned int HighLight_ColorOf_Value(Json_Pair* yp){
    if(yp && yp->value) return (unsigned int)Number_Parse((i8*)yp->value);
    else                return 0xFFFFFFFF;
}
unsigned int HighLight_ColorOf(HighLight* ps,char* cstr,unsigned int size){
    Branch* const b_cstr = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CSTR);
    if(!b_cstr) return 0xFFFFFFFF;
    Json_Pair* const yp_cstr = (Json_Pair*)b_cstr->Memory;

    Branch* const b_char = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CHAR);
    if(!b_char) return 0xFFFFFFFF;
    Json_Pair* const yp_char = (Json_Pair*)b_char->Memory;

    Branch* const b_str = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_STR);
    if(!b_str) return 0xFFFFFFFF;
    Json_Pair* const yp_str = (Json_Pair*)b_str->Memory;

    Branch* const b_num = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_NUM);
    if(!b_num) return 0xFFFFFFFF;
    Json_Pair* const yp_num = (Json_Pair*)b_num->Memory;
    
    CStr color_value = NULL;
    if(cstr[0]=='\"')                               color_value = yp_cstr->value;
    else if(cstr[0]=='\'')                          color_value = yp_char->value;
    else if(Char_Alpha(cstr[0]) || cstr[0]=='_')    color_value = yp_str->value;
    else if(Char_Num(cstr[0]))                      color_value = yp_num->value;
    
    if(color_value) return (unsigned int)Number_Parse((i8*)color_value);
    else            return 0xFFFFFFFF;
}
void HighLight_Set(HighLight* ps,char* Path){
    Json_Clear(ps);
    Json_Add(ps,Path);
}
void HighLight_Clear(HighLight* ps){
    Json_Clear(ps);
}
void HighLight_Tex_String_Set(Tex* t,unsigned int c,unsigned int start,unsigned int size){
    for(int j = 0;j<size;j++){
        Vector_Set(
            t,
            (SymbolInfo[]){{ c }},
            start + j
        );
    }
}

char HighLight_String_Keyword(char* cstr,unsigned int size){
    for(unsigned int i = 0U;i<size;i++){
        if(!Char_StringPart(cstr[i]))
            return 0;
    }
    return 1;
}
Json_Pair* HighLight_Tex_String_Cmp_Custom(HighLight* ps,HighLight_State* state,String* s,unsigned int* start,unsigned int offset,unsigned int* found_size){
    Branch* const b_custom = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CUSTOM);
    if(!b_custom) return NULL;

    for(unsigned int j = 0U;j<b_custom->Childs.size;j++){
        Branch* cb = *(Branch**)Vector_Get(&b_custom->Childs,j);
	    Json_Pair* yp = (Json_Pair*)cb->Memory;
        const unsigned int tag_size = CStr_Size(yp->tag);
        char* const s_cstr = (char*)s->Memory;
        const char kw = HighLight_String_Keyword(s_cstr + *start + offset,tag_size);

        if(
            tag_size + offset <= *found_size &&
            CStr_CmpSize_S(s_cstr + *start + offset,yp->tag,tag_size) && (
                (Char_StringPart(s_cstr[*start + offset]) && (
                    (*start + offset > 0U && !Char_StringPart(s_cstr[*start + offset - 1U])) ||
                    (*start + offset == 0U)
                ) && (
                    (*start + offset + tag_size < s->size && !Char_StringPart(s_cstr[*start + offset + tag_size])) ||
                    (*start + offset + tag_size >= s->size)
                )) ||
                !Char_StringPart(s_cstr[*start + offset])
            )
        ){
            *found_size = tag_size;
            return yp;
        }
    }

    return NULL;
}
Json_Pair* HighLight_Tex_String_Cmp_CLine(HighLight* ps,HighLight_State* state,String* s,unsigned int* start,unsigned int offset,unsigned int* found_size){
    Branch* const b_cline = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_LINE "/" HIGHLIGHT_SELECTOR_LINE_START);
    if(!b_cline) return NULL;

    Json_Pair* yp_cline = (Json_Pair*)b_cline->Memory;
    const unsigned int value_size = CStr_Size(yp_cline->value);
    char* const value = (char*)s->Memory + *start + offset;

    Branch* const b_cline_c = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_LINE "/" HIGHLIGHT_SELECTOR_LINE_COLOR);
    if(!b_cline_c) return NULL;
    
    Json_Pair* const yp_cline_c = (Json_Pair*)b_cline_c->Memory;

    if(value_size + offset <= *found_size && CStr_CmpSize_S(value,yp_cline->value,value_size)){
        *state = HIGHLIGHT_CLINE;
        const unsigned int max_size = s->size - (*start + offset);
        
        for(unsigned int i = 0U;i<max_size;i++){
            if(String_Get(s,*start + offset + i) == '\n' || i + 1U == max_size){
                *found_size = i + 1U;
                return yp_cline_c;
            }
        }
        
        *found_size = max_size;
        return yp_cline_c;
    }

    return NULL;
}
Json_Pair* HighLight_Tex_String_Cmp_CBlock(HighLight* ps,HighLight_State* state,String* s,unsigned int* start,unsigned int offset,unsigned int* found_size){
    Branch* const b_cblock_start = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_START);
    if(!b_cblock_start) return NULL;
    Json_Pair* yp_cblock_start = (Json_Pair*)b_cblock_start->Memory;
    const unsigned int start_op_size = CStr_Size(yp_cblock_start->value);

    Branch* const b_cblock_c = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_COLOR);
    if(!b_cblock_c) return NULL;
    Json_Pair* const yp_cblock_c = (Json_Pair*)b_cblock_c->Memory;

    Branch* const b_cblock_end = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_END);
    if(!b_cblock_end) return NULL;
    Json_Pair* yp_cblock_end = (Json_Pair*)b_cblock_end->Memory;
    const unsigned int end_op_size = CStr_Size(yp_cblock_end->value);

    if(start_op_size + offset <= s->size && CStr_CmpSize_S((char*)s->Memory + *start + offset,yp_cblock_start->value,start_op_size)){
        const unsigned int max_size = s->size - (*start + offset);
        
        for(unsigned int i = start_op_size;i<max_size;i++){
            if(end_op_size + offset + i <= s->size && CStr_CmpSize_S((char*)s->Memory + *start + offset + i,yp_cblock_end->value,start_op_size)){
                *state = HIGHLIGHT_CBLOCKRET;
                *found_size = i + 2U;
                return yp_cblock_c;
            }
        }
        
        *state = HIGHLIGHT_CBLOCK;
        *found_size = max_size;
        return yp_cblock_c;
    }

    return NULL;
}

void HighLight_Tex_String_Build_CStr(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    if(char_new <= *start){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }
    
    const unsigned int size = char_new - *start;
    Branch* const b_cstr = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CSTR);
    if(!b_cstr){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }

    Json_Pair* const yp_cstr = (Json_Pair*)b_cstr->Memory;
    const unsigned int c = HighLight_ColorOf_Value(yp_cstr);
    HighLight_Tex_String_Set(t,c,*start,size);
    
    *state = HIGHLIGHT_NONE;
    *start = char_new;
}
void HighLight_Tex_String_Build_Char(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    if(char_new <= *start){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }
    
    const unsigned int size = char_new - *start;
    Branch* const b_char = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CHAR);
    if(!b_char){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }

    Json_Pair* const yp_char = (Json_Pair*)b_char->Memory;
    const unsigned int c = HighLight_ColorOf_Value(yp_char);
    HighLight_Tex_String_Set(t,c,*start,size);
    
    *state = HIGHLIGHT_NONE;
    *start = char_new;
}
void HighLight_Tex_String_Build_Custom(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    if(char_new <= *start){
        return;
    }

    const unsigned int size = char_new - *start;
    char* const s_cstr = (char*)s->Memory;

    unsigned int last = 0U;
    Branch* const b_custom = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_CUSTOM);
    if(!b_custom){
        return;
    }
    
    for(unsigned int i = 0U;i<size;i++){
        unsigned int found_size = size;
        Json_Pair* yp = HighLight_Tex_String_Cmp_CLine(ps,state,s,start,i,&found_size);
        if(!yp) yp = HighLight_Tex_String_Cmp_CBlock(ps,state,s,start,i,&found_size);
        if(!yp) yp = HighLight_Tex_String_Cmp_Custom(ps,state,s,start,i,&found_size);
        
        if(yp){
            const unsigned int delta_size = i - last;

            if(delta_size > 0U){
                const unsigned int c = HighLight_ColorOf(ps,s_cstr + *start + last,delta_size);
                HighLight_Tex_String_Set(t,c,*start + last,delta_size);
            }

            const unsigned int c = HighLight_ColorOf_Value(yp);
            HighLight_Tex_String_Set(t,c,*start + i,found_size);
            
            last = i + found_size;
            i += found_size - 1U;
        }
    }

    const unsigned int delta_size = size - last;

    if(size > last){
        const unsigned int c = HighLight_ColorOf(ps,s_cstr + *start + last,delta_size);
        HighLight_Tex_String_Set(t,c,*start + last,delta_size);
        last += delta_size;
    }
    
    *start += last;
    
    if(*state != HIGHLIGHT_CLINE && *state != HIGHLIGHT_CBLOCK && *state != HIGHLIGHT_CBLOCKRET)
        *state = HIGHLIGHT_NONE;
}
void HighLight_Tex_String_Build_CLine(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    if(char_new <= *start){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }

    const unsigned int size = char_new - *start;
    Branch* const b_cline = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_LINE "/" HIGHLIGHT_SELECTOR_LINE_COLOR);
    if(!b_cline){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }
    
    Json_Pair* const yp_cline = (Json_Pair*)b_cline->Memory;
    const unsigned int c = HighLight_ColorOf_Value(yp_cline);
    HighLight_Tex_String_Set(t,c,*start,size);
    
    *state = HIGHLIGHT_NONE;
    *start = char_new;
}
void HighLight_Tex_String_Build_CBlock(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    if(char_new <= *start){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }

    unsigned int end = *start;
    Branch* const b_cblock_end = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_END);
    if(!b_cblock_end){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }
    
    Json_Pair* const yp_cblock_end = (Json_Pair*)b_cblock_end->Memory;
    const unsigned int end_size = CStr_Size(yp_cblock_end->value);

    for(;end<char_new;end++){
        if(end + end_size <= char_new && CStr_CmpSize_S((char*)s->Memory + end,yp_cblock_end->value,end_size)){
            *state = HIGHLIGHT_NONE;
            end += end_size;
            break;
        }
    }

    const unsigned int size = end - *start;
    Branch* const b_cblock = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_COLOR);
    if(!b_cblock){
        *state = HIGHLIGHT_NONE;
        *start = char_new;
        return;
    }
    
    Json_Pair* const yp_cblock = (Json_Pair*)b_cblock->Memory;
    const unsigned int c = HighLight_ColorOf_Value(yp_cblock);
    HighLight_Tex_String_Set(t,c,*start,size);

    *start = end;
}
void HighLight_Tex_String_Build_CBlockRet(HighLight* ps,Tex* t,HighLight_State* state,String* s,unsigned int* start,unsigned int char_new){
    *state = HIGHLIGHT_NONE;

    if(char_new <= *start){
        *start = char_new;
        return;
    }

    unsigned int end = *start;
    Branch* const b_cblock_end = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_END);
    if(!b_cblock_end){
        *start = char_new;
        return;
    }
    
    Json_Pair* const yp_cblock_end = (Json_Pair*)b_cblock_end->Memory;
    const unsigned int end_size = CStr_Size(yp_cblock_end->value);

    for(;end<char_new;end++){
        if(end + end_size <= char_new && CStr_CmpSize_S((char*)s->Memory + end,yp_cblock_end->value,end_size)){
            end += end_size;
            break;
        }
    }

    const unsigned int size = end - *start;
    Branch* const b_cblock = Json_GetBranch(ps,HIGHLIGHT_SELECTOR_BLOCK "/" HIGHLIGHT_SELECTOR_BLOCK_COLOR);
    if(!b_cblock){
        *start = char_new;
        return;
    }
    
    Json_Pair* const yp_cblock = (Json_Pair*)b_cblock->Memory;
    const unsigned int c = HighLight_ColorOf_Value(yp_cblock);
    HighLight_Tex_String_Set(t,c,*start,size);

    *start = end;
}

char HighLight_String_Escaped(char* cstr,unsigned int size,unsigned int index){
    if(index == 0U) return 0;
    
    unsigned int i = index - 1U;
    for(;i<index;i--){
        if(cstr[i] != '\\')
            break;
    }

    return (i != index - 1U) && (index - i) % 2 == 0;
}
void HighLight_Tex_String(HighLight* ps,Tex* t,String* s,HighLight_State* state){
    Tex_SyncString(t,s);
    unsigned int Last = 0U;

    for(unsigned int i = 0U;i<s->size;i++){
        char ch = *(char*)Vector_Get(s,i);
        
        switch (*state){
            case HIGHLIGHT_NONE:{
                Last = i;

                if(ch=='\"' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    *state = HIGHLIGHT_CSTR;
                }else if(ch=='\'' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    *state = HIGHLIGHT_CHAR;
                }else if(ch==' ' || ch=='\n' || ch=='\t'){
                    Last++;
                    break;
                }else{
                    *state = HIGHLIGHT_CUSTOM;
                    if(i + 1U == s->size) i--;
                }
            } break;
            case HIGHLIGHT_CSTR:{
                if(ch=='\"' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    char* s_cstr = (char*)s->Memory;
                    char* s_cstrl = (char*)s->Memory + Last;
                    char* s_cstri = (char*)s->Memory + i;
                    HighLight_Tex_String_Build_CStr(ps,t,state,s,&Last,i + 1U);
                    i = Last - 1U;
                }
            } break;
            case HIGHLIGHT_CHAR:{
                if(ch=='\'' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    HighLight_Tex_String_Build_Char(ps,t,state,s,&Last,i + 1U);
                    i = Last - 1U;
                }
            } break;
            case HIGHLIGHT_CUSTOM:{
                if(ch=='\"' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    HighLight_Tex_String_Build_Custom(ps,t,state,s,&Last,i);
                    i = Last;
                    *state = HIGHLIGHT_CSTR;
                }else if(ch=='\'' && !HighLight_String_Escaped((char*)s->Memory,s->size,i)){
                    HighLight_Tex_String_Build_Custom(ps,t,state,s,&Last,i);
                    i = Last;
                    *state = HIGHLIGHT_CHAR;
                }else if(ch==' ' || (ch=='\n' || i + 1U == s->size) || ch=='\t'){
                    HighLight_Tex_String_Build_Custom(ps,t,state,s,&Last,i + (i + 1U == s->size ? 1 : 0));
                    
                    if(*state == HIGHLIGHT_CLINE || *state == HIGHLIGHT_CBLOCK || *state == HIGHLIGHT_CBLOCKRET) i = Last;
                    if(*state == HIGHLIGHT_CBLOCKRET) i--;
                    if(*state == HIGHLIGHT_CLINE || *state == HIGHLIGHT_CBLOCKRET) *state = HIGHLIGHT_NONE;
                    Last++;
                }
            } break;
            case HIGHLIGHT_CLINE:{
                if(ch=='\n' || i + 1U == s->size){
                    HighLight_Tex_String_Build_CLine(ps,t,state,s,&Last,i + 1U);
                }
            } break;
            case HIGHLIGHT_CBLOCK:{
                HighLight_Tex_String_Build_CBlock(ps,t,state,s,&Last,s->size);
                i = Last - 1U;
            } break;
        }
    }
}
HighLight HighLight_New(){
    return Json_New();
}
void HighLight_Free(HighLight* ps){
    Json_Free(ps);
}

typedef struct GHighLight {
    void* highlighter;
    void (*fn_init)(void*,char*);
    void (*fn_update)(void*,char*);
    void (*fn_free)(void*,char*);
} GHighLight;

GHighLight GHighLight_StdHL(){
    GHighLight ghl;
    memset(&ghl,0,sizeof(ghl));
    return ghl;
}
void GHighLight_StdHL_Free(){
    
}

#endif //!TEX