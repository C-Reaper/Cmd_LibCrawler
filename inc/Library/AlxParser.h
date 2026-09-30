#ifndef ALXPARSER_H
#define ALXPARSER_H

#include "../Container/Vector.h"
#include "String.h"
#include "ConstParser.h"

typedef unsigned short  TT_Type;
typedef unsigned short  TT_ACount;
typedef char            TT_Char;

typedef unsigned int    TT_IterI;
typedef long            TT_Iter;
#define     TT_INVALID      -1

typedef unsigned char   TT_Bool;
#define     TT_False        0
#define     TT_True         1

typedef unsigned int    TT_Count;
#define     ARGS_IGNORE     0xFFFFFFFF

typedef unsigned int    TT_Behave;
#define     RESETER_NONE    0
#define     RESETER_RST     1
#define     RESETER_TO1     2
#define     RESETER_TO2     3
#define     RESETER_ADD1    4
#define     RESETER_ADD2    5


#define     TOKEN_NONE                        0      //
#define     TOKEN_NUMBER                      1      // 0123456789
#define     TOKEN_FLOAT                       2      // 0123456789.
#define     TOKEN_STRING                      3      // A-Z,a-z
#define     TOKEN_TYPE                        4      // is created if TOKEN_STRING is a type
#define     TOKEN_SPACE                       5      //( )
#define     TOKEN_NEWLINE                     6      //(\n)
#define     TOKEN_CARTURN                     7      //(\r)
#define     TOKEN_HTAB                        8      //(\t)
#define     TOKEN_VTAB                        9      //(\v)
#define     TOKEN_PERIOD                      10     //(.)
#define     TOKEN_COMMA                       11     //(,)
#define     TOKEN_SEMICOLON                   12     //(;)
#define     TOKEN_COLON                       13     //(:)
#define     TOKEN_EXCLAMATION_MARK            14     //(!)
#define     TOKEN_QUESTION_MARK               15     //(?)
#define     TOKEN_ELLIPSIS                    16     //(…)
#define     TOKEN_PARENTHESES_L               17     //(
#define     TOKEN_PARENTHESES_R               18     //)
#define     TOKEN_SQUARE_BRACKETS_L           19     //[
#define     TOKEN_SQUARE_BRACKETS_R           20     //]
#define     TOKEN_CURLY_BRACES_L              21     //{
#define     TOKEN_CURLY_BRACES_R              22     //}
#define     TOKEN_ANGLE_BRACKETS_L            23     //<
#define     TOKEN_ANGLE_BRACKETS_R            24     //>
#define     TOKEN_DOUBLE_ANGLE_BRACKETS_L     26     //«
#define     TOKEN_DOUBLE_ANGLE_BRACKETS_R     27     //»
#define     TOKEN_WHITE_SQUARE_BRACKETS_L     28     //⟦
#define     TOKEN_WHITE_SQUARE_BRACKETS_R     29     //⟧
#define     TOKEN_PLUS_SIGN                   30     //(+)
#define     TOKEN_MINUS_SIGN                  31     //(-)
#define     TOKEN_MULTIPLICATION_SIGN         32     //(×)
#define     TOKEN_DIVISION_SIGN               33     //(÷)
#define     TOKEN_EQUAL_SIGN                  34     //(=)
#define     TOKEN_NOT_EQUAL_SIGN              35     //(≠)
#define     TOKEN_LESS_THAN                   36     //(<)
#define     TOKEN_GREATER_THAN                37     //(>)
#define     TOKEN_LESS_THAN_OR_EQUAL_TO       38     //(≤)
#define     TOKEN_GREATER_THAN_OR_EQUAL_TO    39     //(≥)
#define     TOKEN_APPROXIMATION_SIGN          40     //(≈)
#define     TOKEN_PLUS_MINUS_SIGN             41     //(±)
#define     TOKEN_INFINITY                    42     //(∞)
#define     TOKEN_SQUARE_ROOT                 43     //(√)
#define     TOKEN_PI                          44     //(Π)
#define     TOKEN_DOLLAR_SIGN                 45     //($)
#define     TOKEN_EURO_SIGN                   46     //(€)
#define     TOKEN_POUND_SIGN                  47     //(£)
#define     TOKEN_YEN_SIGN                    48     //(¥)
#define     TOKEN_CENT_SIGN                   49     //(¢)
#define     TOKEN_AMPERSAND                   50     //(&)
#define     TOKEN_AT_SIGN                     51     //(@)
#define     TOKEN_ASTERISK                    52     //(*)
#define     TOKEN_CARET                       53     //(^)
#define     TOKEN_TILDE                       54     //(~)
#define     TOKEN_SLASH                       55     //(/)
#define     TOKEN_BACKSLASH                   56     //(\)
#define     TOKEN_UNDERSCORE                  57     //(_)
#define     TOKEN_PIPE                        58     //(|)
#define     TOKEN_PERCENT                     59     //(%)
#define     TOKEN_HASH_POUND_SIGN             60     //(#)
#define     TOKEN_SINGLE_QUOTE                61     //(')
#define     TOKEN_DOUBLE_QUOTE                62     //(")
#define     TOKEN_CHAR                        63     //(any char)
#define     TOKEN_BACKTICK                    64     //(`)
#define     TOKEN_PRIME                       65     //(′)
#define     TOKEN_DOUBLE_PRIME                66     //(″)
#define     TOKEN_COPYRIGHT_SYMBOL            67     //(©)
#define     TOKEN_REGISTERED_TRADEMARK        68     //(®)
#define     TOKEN_TRADEMARK_SYMBOL            69     //(™)
#define     TOKEN_SECTION_SYMBOL              70     //(§)
#define     TOKEN_DEGREE_SYMBOL               71     //(°)
#define     TOKEN_FRACTION_SLASH              72     //(⁄)
#define     TOKEN_INTERROBANG                 73     //(‽)
#define     TOKEN_BULLET_POINT                74     //(•)
#define     TOKEN_DAGGER                      75     //(†)
#define     TOKEN_DOUBLE_DAGGER               76     //(‡)
#define     TOKEN_NUMBER_SIGN                 77     //(#)
#define     TOKEN_NUMBER_SUBTRACT             78     //(−)
#define     TOKEN_CONSTSTRING_SINGLE          79     // '...'
#define     TOKEN_CONSTSTRING_DOUBLE          80     // "..."
#define     TOKEN_BOOL                        81     // "..."
#define     TOKEN_END                         82     // Init-List End
#define     TOKEN_CAST                        83     // Cast to different type
#define     TOKEN_FUNCTIONDECL                84     // function decl
#define     TOKEN_FUNCTION                    85     // function
#define     TOKEN_FUNCTIONPOINTER             86     // functionpointer
#define     TOKEN_OBJECT                      87
#define     TOKEN_INIT                        88
#define     TOKEN_DESTROY                     89
#define     TOKEN_START                       90     // first custom token starts here


TT_Type TT_Type_Get(TT_Char ch){
    switch(ch){
    case ' ':   return TOKEN_SPACE;
    case '\n':  return TOKEN_NEWLINE;
    case '\r':  return TOKEN_CARTURN;
    case '\t':  return TOKEN_HTAB;
    case '\v':  return TOKEN_VTAB;
    case '.':   return TOKEN_PERIOD;
    case ',':   return TOKEN_COMMA;
    case ';':   return TOKEN_SEMICOLON;
    case ':':   return TOKEN_COLON;
    case '!':   return TOKEN_EXCLAMATION_MARK;
    case '?':   return TOKEN_QUESTION_MARK;
    case '\'':  return TOKEN_SINGLE_QUOTE;
    case '\"':  return TOKEN_DOUBLE_QUOTE;
    case '(':   return TOKEN_PARENTHESES_L;
    case ')':   return TOKEN_PARENTHESES_R;
    case '[':   return TOKEN_SQUARE_BRACKETS_L;
    case ']':   return TOKEN_SQUARE_BRACKETS_R;
    case '{':   return TOKEN_CURLY_BRACES_L;
    case '}':   return TOKEN_CURLY_BRACES_R;
    case '<':   return TOKEN_ANGLE_BRACKETS_L;
    case '>':   return TOKEN_ANGLE_BRACKETS_R;
    case '+':   return TOKEN_PLUS_SIGN;
    case '-':   return TOKEN_MINUS_SIGN;
    case '=':   return TOKEN_EQUAL_SIGN;
    //case '<':   return TOKEN_LESS_THAN;
    //case '>':   return TOKEN_GREATER_THAN;
    case '$':   return TOKEN_DOLLAR_SIGN;
    case '&':   return TOKEN_AMPERSAND;
    case '@':   return TOKEN_AT_SIGN;
    case '*':   return TOKEN_ASTERISK;
    case '^':   return TOKEN_CARET;
    case '~':   return TOKEN_TILDE;
    case '/':   return TOKEN_SLASH;
    case '\\':  return TOKEN_BACKSLASH;
    case '|':   return TOKEN_PIPE;
    case '%':   return TOKEN_PERCENT;
    case '#':   return TOKEN_HASH_POUND_SIGN;
    /*
    case '`':   return TOKEN_BACKTICK;
    case '′':   return TOKEN_PRIME;
    case '″':   return TOKEN_DOUBLE_PRIME;
    case '⁄':   return TOKEN_FRACTION_SLASH;
    case '…':   return TOKEN_ELLIPSIS;
    case '«':   return TOKEN_DOUBLE_ANGLE_BRACKETS_L;
    case '»':   return TOKEN_DOUBLE_ANGLE_BRACKETS_R;
    case '⟦':   return TOKEN_WHITE_SQUARE_BRACKETS_L;
    case '⟧':   return TOKEN_WHITE_SQUARE_BRACKETS_R;
    case '×':   return TOKEN_MULTIPLICATION_SIGN;
    case '÷':   return TOKEN_DIVISION_SIGN;
    case '≠':   return TOKEN_NOT_EQUAL_SIGN;
    case '≤':   return TOKEN_LESS_THAN_OR_EQUAL_TO;
    case '≥':   return TOKEN_GREATER_THAN_OR_EQUAL_TO;
    case '≈':   return TOKEN_APPROXIMATION_SIGN;
    case '±':   return TOKEN_PLUS_MINUS_SIGN;
    case '∞':   return TOKEN_INFINITY;
    case '√':   return TOKEN_SQUARE_ROOT;
    case 'Π':   return TOKEN_PI;
    case '€':   return TOKEN_EURO_SIGN;
    case '£':   return TOKEN_POUND_SIGN;
    case '¥':   return TOKEN_YEN_SIGN;
    case '¢':   return TOKEN_CENT_SIGN;
    case '©':   return TOKEN_COPYRIGHT_SYMBOL;
    case '®':   return TOKEN_REGISTERED_TRADEMARK;
    case '™':   return TOKEN_TRADEMARK_SYMBOL;
    case '§':   return TOKEN_SECTION_SYMBOL;
    case '°':   return TOKEN_DEGREE_SYMBOL;
    case '‽':   return TOKEN_INTERROBANG;
    case '•':   return TOKEN_BULLET_POINT;
    case '†':   return TOKEN_DAGGER;
    case '‡':   return TOKEN_DOUBLE_DAGGER;
    case '−':   return TOKEN_NUMBER_SUBTRACT;
    */
    }
    return TOKEN_NONE;
}
TT_Bool TT_Type_isSpezial(TT_Char ch){
    switch(ch){
    case ' ':   return TT_True;
    case '\n':  return TT_True;
    case '\r':  return TT_True;
    case '\t':  return TT_True;
    case '\v':  return TT_True;
    case '.':   return TT_True;
    case ',':   return TT_True;
    case ';':   return TT_True;
    case ':':   return TT_True;
    case '!':   return TT_True;
    case '?':   return TT_True;
    case '\'':  return TT_True;
    case '\"':  return TT_True;
    case '(':   return TT_True;
    case ')':   return TT_True;
    case '[':   return TT_True;
    case ']':   return TT_True;
    case '{':   return TT_True;
    case '}':   return TT_True;
    case '<':   return TT_True;
    case '>':   return TT_True;
    case '+':   return TT_True;
    case '-':   return TT_True;
    case '=':   return TT_True;
    case '$':   return TT_True;
    case '&':   return TT_True;
    case '@':   return TT_True;
    case '*':   return TT_True;
    case '^':   return TT_True;
    case '~':   return TT_True;
    case '/':   return TT_True;
    case '\\':  return TT_True;
    case '|':   return TT_True;
    case '%':   return TT_True;
    case '#':   return TT_True;
    /*
    case '`':   return TT_True;
    case '′':   return TT_True;
    case '″':   return TT_True;
    case '⁄':   return TT_True;
    case '…':   return TT_True;
    case '«':   return TT_True;
    case '»':   return TT_True;
    case '⟦':   return TT_True;
    case '⟧':   return TT_True;
    case '×':   return TT_True;
    case '÷':   return TT_True;
    case '≠':   return TT_True;
    case '≤':   return TT_True;
    case '≥':   return TT_True;
    case '≈':   return TT_True;
    case '±':   return TT_True;
    case '∞':   return TT_True;
    case '√':   return TT_True;
    case 'Π':   return TT_True;
    case '€':   return TT_True;
    case '£':   return TT_True;
    case '¥':   return TT_True;
    case '¢':   return TT_True;
    case '©':   return TT_True;
    case '®':   return TT_True;
    case '™':   return TT_True;
    case '§':   return TT_True;
    case '°':   return TT_True;
    case '‽':   return TT_True;
    case '•':   return TT_True;
    case '†':   return TT_True;
    case '‡':   return TT_True;
    case '−':   return TT_True;
    */
    }
    return TT_False;
}
TT_Bool TT_Type_isStr(TT_Char ch){
    return (ch>=65 && ch<91) || (ch>=97 && ch<123) || ch=='_';
}
TT_Bool TT_Type_isNum(TT_Char ch){
    return (ch>=48 && ch<58);
}


typedef Vector TokenMap;
TokenMap TokenMap_Cpy(TokenMap* tm);
void TokenMap_Free(TokenMap* rm);

#define TOKEN_ITINVALID -1

typedef struct Token {
    TT_Type tt;
    TT_ACount ac;
    TT_IterI it;     // for recongnizing the previous index
    unsigned int ch;
    unsigned int line;
    TT_Char* file;
    union {
        Boolean v_b1;
        i64 v_i64;
        f64 v_f64;
        TT_Char* str;
    };
    Vector* args;   // Vector<TokenMap>
} Token;

Token Token_New(TT_Type tt,String* Buffer){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    String_AppendChar(Buffer,'\0');
    t.str = CStr_Cpy((char*)Buffer->Memory);
    t.args = NULL;
    return t;
}
Token Token_Move(TT_Type tt,TT_Char* cstr){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;

    if(t.tt == TOKEN_NUMBER)
        t.v_i64 = Number_Parse((i8*)cstr);
    else if(t.tt == TOKEN_FLOAT)
        t.v_f64 = Double_Parse((i8*)cstr,1);
    else if(t.tt == TOKEN_BOOL)
        t.v_b1 = Boolean_Parse((i8*)cstr);
    else{
        t.str = cstr;
        cstr = NULL;
    }

    CStr_Free(&cstr);
    t.args = NULL;
    return t;
}
Token Token_By(TT_Type tt,TT_Char* cstr){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    t.args = NULL;

    if(t.tt == TOKEN_NUMBER)
        t.v_i64 = Number_Parse((i8*)cstr);
    else if(t.tt == TOKEN_FLOAT)
        t.v_f64 = Double_Parse((i8*)cstr,1);
    else if(t.tt == TOKEN_BOOL)
        t.v_b1 = Boolean_Parse((i8*)cstr);
    else
        t.str = CStr_Cpy(cstr);

    return t;
}
Token Token_Cpy(Token* t){
    Token cpy;
    cpy.tt = t->tt;
    cpy.ac = t->ac;
    cpy.it = t->it;
    cpy.ch = t->ch;
    cpy.line = t->line;
    cpy.file = CStr_Cpy(t->file);
    
    if(cpy.tt == TOKEN_NUMBER)
        cpy.v_i64 = t->v_i64;
    else if(cpy.tt == TOKEN_FLOAT)
        cpy.v_f64 = t->v_f64;
    else if(cpy.tt == TOKEN_BOOL)
        cpy.v_b1 = t->v_b1;
    else
        cpy.str = CStr_Cpy(t->str);

    if(t->args){
        cpy.args = malloc(sizeof(Vector));
        *cpy.args = Vector_New(sizeof(TokenMap));

        for(int i = 0;i<t->args->size;i++){
            TokenMap* tm = (TokenMap*)Vector_Get(t->args,i);
            TokenMap cpytm = TokenMap_Cpy(tm);
            Vector_Push(cpy.args,&cpytm);
        }
    }else{
        cpy.args = NULL;
    }

    return cpy;
}
Token Token_Args(TT_Type tt,TT_Char* cstr,TokenMap* ts){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
   
    if(t.tt == TOKEN_NUMBER)
        t.v_i64 = Number_Parse((i8*)cstr);
    else if(t.tt == TOKEN_FLOAT)
        t.v_f64 = Double_Parse((i8*)cstr,1);
    else if(t.tt == TOKEN_BOOL)
        t.v_b1 = Boolean_Parse((i8*)cstr);
    else
        t.str = CStr_Cpy(cstr);

    t.args = malloc(sizeof(Vector));
    *t.args = Vector_New(sizeof(TokenMap));

    for(int i = 0;ts[i].Memory!=NULL;i++){
        Vector_Push(t.args,&ts[i]);
    }

    return t;
}
Token Token_New_B1(TT_Type tt,Boolean v_b1){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    t.v_b1 = v_b1;
    t.args = NULL;
    return t;
}
Token Token_New_I64(TT_Type tt,i64 v_i64){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    t.v_i64 = v_i64;
    t.args = NULL;
    return t;
}
Token Token_New_F64(TT_Type tt,f64 v_f64){
    Token t;
    t.tt = tt;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    t.v_f64 = v_f64;
    t.args = NULL;
    return t;
}
Token Token_Null(){
    Token t;
    t.tt = TOKEN_NONE;
    t.ac = 0U;
    t.it = TOKEN_ITINVALID;
    t.ch = 0U;
    t.line = 0U;
    t.file = NULL;
    t.str = NULL;
    t.args = NULL;
    return t;
}
void Token_Free(Token* t){
    if(t->tt != TOKEN_NUMBER && t->tt != TOKEN_FLOAT && t->tt != TOKEN_BOOL)
        CStr_Free(&t->str);

    t->tt = TOKEN_NONE;
    t->it = -1;
    t->ch = 0U;
    t->line = 0U;
    CStr_Free(&t->file);
    
    if(t->args){
        for(int i = 0;i<t->args->size;i++){
            TokenMap* tm = (TokenMap*)Vector_Get(t->args,i);
            TokenMap_Free(tm);
        }
        Vector_Free(t->args);
        free(t->args);
        t->args = NULL;
    }
}
void Token_Set(Token* t,Token* set){
    Token_Free(t);
    *t = *set;
}
void Token_Value(Token* t,CStr value){
    const Number n = Number_Parse((i8*)value);
    const Double f = Double_Parse((i8*)value,1);
    const Boolean b = Boolean_Parse((i8*)value);

    if(n != NUMBER_PARSE_ERROR){
        t->v_i64 = n;
        t->tt = TOKEN_NUMBER;
    }else if(f != DOUBLE_PARSE_ERROR){
        t->v_f64 = f;
        t->tt = TOKEN_FLOAT;
    }else if(b != BOOL_PARSE_ERROR){
        t->v_b1 = b;
        t->tt = TOKEN_BOOL;
    }else
        CStr_Set(&t->str,value);
}
char Token_IsCStr(Token* t){
    return !(t->tt == TOKEN_NUMBER || t->tt == TOKEN_FLOAT || t->tt == TOKEN_BOOL);
}
void Token_Args_Push(Token* t,TokenMap* tm){
    if(t->args==NULL){
        t->args = malloc(sizeof(Vector));
        *t->args = Vector_New(sizeof(TokenMap));
    }
    Vector_Push(t->args,tm);
}
void Token_Args_PushC(Token* t,TokenMap* tms){
    if(t->args==NULL){
        t->args = malloc(sizeof(Vector));
        *t->args = Vector_New(sizeof(TokenMap));
    }

    for(int i = 0;tms[i].Memory!=NULL;i++){
        Vector_Push(t->args,&tms[i]);
    }
}
CStr Token_CStr(Token* t){
    if(t->tt == TOKEN_NUMBER)
        return (CStr)Number_Get(t->v_i64);
    else if(t->tt == TOKEN_FLOAT)
        return (CStr)Double_Get(t->v_f64,8);
    else if(t->tt == TOKEN_BOOL)
        return (CStr)Boolean_Get(t->v_b1);
    else
        return CStr_Cpy(t->str);
}
TT_Char** Token_GetOfArray(Token* t,int Count){
    TT_Char** out = (TT_Char**)malloc(Count * sizeof(TT_Char*));
    for(int i = 0;i<Count;i++){
        out[i] = Token_CStr(t + i);
    }
    return out;
}
void Token_Append(Token* t,String* Buffer){
    if(t->tt == TOKEN_NUMBER)
        String_AppendNumber(Buffer,t->v_i64);
    else if(t->tt == TOKEN_FLOAT)
        String_AppendDouble(Buffer,t->v_f64);
    else if(t->tt == TOKEN_BOOL)
        String_AppendBoolean(Buffer,t->v_b1);
    else
        String_Append(Buffer,t->str);
}
void Token_Print_N(Token* t,int Depth){
    for(int k = 0;k<Depth;k++){
        printf("  ");
    }

    if(t->tt == TOKEN_NUMBER)
        printf("%u -> %ld ",t->tt,t->v_i64);
    else if(t->tt == TOKEN_FLOAT)
        printf("%u -> %lf ",t->tt,t->v_f64);
    else if(t->tt == TOKEN_BOOL)
        printf("%u -> %d ",t->tt,t->v_b1);
    else
        printf("%u -> %s ",t->tt,t->str);
    
    if(t->args){
        printf("{\n");
        for(int i = 0;i<t->args->size;i++){
            TokenMap* tm = (TokenMap*)Vector_Get(t->args,i);
            
            for(int k = 0;k<Depth;k++){
                printf("  ");
            }
            printf(" {\n");

            for(int j = 0;j<tm->size;j++){
                Token* tok = (Token*)Vector_Get(tm,j);
                Token_Print_N(tok,Depth+1);
                printf("\n");
            }

            for(int k = 0;k<Depth;k++){
                printf("  ");
            }
            printf(" }\n");
        }

        for(int k = 0;k<Depth;k++){
            printf("  ");
        }
        printf("}");
    }
}
void Token_Print(Token* t){
    printf("--- Token ---\n");
    Token_Print_N(t,0);
    printf("\n-------------\n");
}
void Token_Print_V(Token* t){
    if(t->tt == TOKEN_NUMBER)
        printf("%ld",t->v_i64);
    else if(t->tt == TOKEN_FLOAT)
        printf("%lf",t->v_f64);
    else if(t->tt == TOKEN_BOOL)
        printf("%d",t->v_b1);
    else
        printf("%s",t->str);
}
void Token_Print_S(Token* t){
    printf("%u -> ",t->tt);
    Token_Print_V(t);
    printf(" ");

    if(t->args){
        printf("{ ... }");
    }
}


TokenMap TokenMap_New(){
    TokenMap rm = Vector_New(sizeof(Token));
    return rm;
}
TokenMap TokenMap_Make(Token* initlist){
    TokenMap rm = TokenMap_New();
    for(int i = 0;;i++){
        Token* rp = &initlist[i];
        if(rp->tt==TOKEN_END || rp->tt==TOKEN_NONE) break;
        Vector_Push(&rm,rp);
    }
    return rm;
}
TokenMap TokenMap_Cpy(TokenMap* tm){
    TokenMap m = TokenMap_New();
    for(int i = 0;tm && i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);
        Vector_Push(&m,(Token[]){ Token_Cpy(t) });
    }
    return m;
}
TokenMap TokenMap_Null(){
    TokenMap m = Vector_Null();
    return m;
}
TT_Iter TokenMap_FindX(TokenMap* tm,Token* tok){
    for(int i = 0;i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);

        if(t->tt == tok->tt){
            if(t->tt == TOKEN_NUMBER && t->v_i64 == tok->v_i64)
                printf("%ld",t->v_i64);
            else if(t->tt == TOKEN_FLOAT && t->v_f64 == tok->v_f64)
                printf("%lf",t->v_f64);
            else if(t->tt == TOKEN_BOOL && t->v_b1 == tok->v_b1)
                printf("%d",t->v_b1);
            else if(CStr_Cmp(t->str,tok->str))
                printf("%s",t->str);
        }
    }
    return TT_INVALID;
}
TT_Iter TokenMap_Find(TokenMap* tm,TT_Type tt){
    for(int i = 0;i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);
        if(t->tt==tt) return i;
    }
    return TT_INVALID;
}
TT_Iter TokenMap_FindS(TokenMap* tm,TT_Type tt,TT_Iter start){
    for(;start<tm->size;start++){
        Token* t = (Token*)Vector_Get(tm,start);
        if(t->tt==tt) return start;
    }
    return TT_INVALID;
}
TT_Iter TokenMap_FindSE(TokenMap* tm,TT_Type tt,TT_Iter start,TT_Iter end){
    for(;start<tm->size && start<end;start++){
        Token* t = (Token*)Vector_Get(tm,start);
        if(t->tt==tt) return start;
    }
    return TT_INVALID;
}

Token* TokenMap_FindT(TokenMap* tm,TT_Type tt){
    for(int i = 0;i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);
        if(t->tt==tt) return t;
    }
    return NULL;
}

void TokenMap_Push_TM(TokenMap* rm,TokenMap* tm){
    for(int i = 0;i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);
        Vector_Push(rm,(Token[]){ Token_Cpy(t) });
    }
}
void TokenMap_Add_TM(TokenMap* rm,TokenMap* tm,int index){
    for(int i = 0;i<tm->size;i++){
        Token* t = (Token*)Vector_Get(tm,i);
        Vector_Add(rm,(Token[]){ Token_Cpy(t) },index + i);
    }
}

void TokenMap_Push(TokenMap* rm,Token* t){
    Vector_Push(rm,t);
}
void TokenMap_Pop(TokenMap* rm){
    Token* t = (Token*)Vector_Get(rm,rm->size-1);
    Token_Free(t);
    Vector_PopTop(rm);
}
TokenMap TokenMap_SubMove(TokenMap* rm,int s,int e){
    TokenMap tm = TokenMap_New();
    for(int i = s;i<e && i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,t);
    }
    return tm;
}
TokenMap TokenMap_Sub(TokenMap* rm,int s,int e){
    TokenMap tm = TokenMap_New();
    for(int i = s;i<e && i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
    }
    return tm;
}
TokenMap TokenMap_SubToToken(TokenMap* rm,TT_Type tt){// Without Token itself
    TokenMap tm = TokenMap_New();
    
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        if(t->tt!=tt) Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
        else break;
    }

    return tm;
}
TokenMap TokenMap_SubFromToken(TokenMap* rm,TT_Type tt){// Without Token itself
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        if(t->tt==tt){
            TokenMap tm = TokenMap_New();
            for(int j = i + 1;j<rm->size;j++){
                Token* t = (Token*)Vector_Get(rm,j);
                Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
            }
            return tm;
        }
    }
    return TokenMap_New();
}

TokenMap TokenMap_SubToTokens(TokenMap* rm,TT_Type* tt){// Without Tokens itself
    TokenMap tm = TokenMap_New();
    
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
    
        for(int j = 0;tt[j]!=TOKEN_NONE && tt[j]!=TOKEN_END;j++){
            if(t->tt!=tt[j]) Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
            else return tm;
        }
    }

    return tm;
}
TokenMap TokenMap_SubFromTokens(TokenMap* rm,TT_Type* tt){// Without Tokens itself
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        
        for(int j = 0;tt[j]!=TOKEN_NONE && tt[j]!=TOKEN_END;j++){
            if(t->tt==tt[j]){
                TokenMap tm = TokenMap_New();
                for(int k = i + 1;k<rm->size;k++){
                    Token* t = (Token*)Vector_Get(rm,k);
                    Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
                }
                return tm;
            }
        }
    }
    return TokenMap_New();
}

TokenMap TokenMap_Section_LRB(TokenMap* rm,TT_Type start,TT_Type end){// With borders
    TT_Iter start_it = TokenMap_Find(rm,start);
    TT_Iter end_it = TokenMap_Find(rm,end);

    if(start_it<0)          start_it = 0;
    if(start_it>=rm->size)  start_it = rm->size - 1;
    if(end_it<0)            end_it = 0;
    if(end_it>=rm->size)    end_it = rm->size - 1;
    
    TokenMap tm = TokenMap_New();
    for(int i = start_it;i<=end_it;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
    }
    return tm;
}
TokenMap TokenMap_Section_LB(TokenMap* rm,TT_Type start,TT_Type end){// With left borders
    TT_Iter start_it = TokenMap_Find(rm,start);
    TT_Iter end_it = TokenMap_Find(rm,end);
    
    if(start_it<0)          start_it = 0;
    if(start_it>=rm->size)  start_it = rm->size - 1;
    if(end_it<0)            end_it = 0;
    if(end_it>=rm->size)    end_it = rm->size - 1;

    TokenMap tm = TokenMap_New();
    for(int i = start_it;i<end_it;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
    }
    return tm;
}
TokenMap TokenMap_Section_RB(TokenMap* rm,TT_Type start,TT_Type end){// With right borders
    TT_Iter start_it = TokenMap_Find(rm,start);
    TT_Iter end_it = TokenMap_Find(rm,end);
    
    if(start_it<0)          start_it = 0;
    if(start_it>=rm->size)  start_it = rm->size - 1;
    if(end_it<0)            end_it = 0;
    if(end_it>=rm->size)    end_it = rm->size - 1;
    
    TokenMap tm = TokenMap_New();
    for(int i = start_it+1;i<=end_it;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
    }
    return tm;
}
TokenMap TokenMap_Section_B(TokenMap* rm,TT_Type start,TT_Type end){// Without borders
    TT_Iter start_it = TokenMap_Find(rm,start);
    TT_Iter end_it = TokenMap_Find(rm,end);
    
    if(start_it<0)          start_it = 0;
    if(start_it>=rm->size)  start_it = rm->size - 1;
    if(end_it<0)            end_it = 0;
    if(end_it>=rm->size)    end_it = rm->size - 1;
    
    TokenMap tm = TokenMap_New();
    for(int i = start_it+1;i<end_it;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Vector_Push(&tm,(Token[]){ Token_Cpy(t) });
    }
    return tm;
}

Vector TokenMap_ChopDownSepTokens(TokenMap* rm,TT_Type* tt){// Without Tokens itself
    Vector tms = Vector_New(sizeof(TokenMap));

    int Last = 0;
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);

        for(int i = 0;tt[i]!=TOKEN_NONE;i++){
            if(t->tt==tt[i]){
                if(i==Last) break;

                TokenMap tm = TokenMap_Sub(rm,Last,i);
                Vector_Push(&tms,&tm);
                Last = i+1;
                break;
            }
        }
    }

    return tms;
}

void TokenMap_RemoveI(TokenMap* rm,int i){
    Token* t = (Token*)Vector_Get(rm,i);
    Token_Free(t);
    Vector_Remove(rm,i);
}
void TokenMap_Remove(TokenMap* rm,int s,int e){
    for(int i = s;i<e && i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Token_Free(t);
    }
    Vector_RemoveCount(rm,s,e - s);
}
void TokenMap_RemoveM(TokenMap* rm,int s,int e){
    Vector_RemoveCount(rm,s,e - s);
}

TokenMap* TokenMap_IterateToParent(TokenMap* sm,Vector* iters,int StartIndex){
    if(iters){
        TT_Iter it1 = *(TT_Iter*)Vector_Get(iters,StartIndex);
        Token* t = (Token*)Vector_Get(sm,it1);
        if(t->args){
            if(StartIndex+2<iters->size){
                TT_Iter it2 = *(TT_Iter*)Vector_Get(iters,StartIndex+1);
                TokenMap* tm = (TokenMap*)Vector_Get(t->args,it2);
                return TokenMap_IterateToParent(tm,iters,StartIndex+2);
            }else{
                return sm;
            }
        }
    }
    return NULL;
}
Token* TokenMap_IterateInto(TokenMap* sm,Vector* iters,int StartIndex){
    if(iters){
        TT_Iter it1 = *(TT_Iter*)Vector_Get(iters,StartIndex);
        Token* t = (Token*)Vector_Get(sm,it1);
        if(t->args){
            if(StartIndex+2<iters->size){
                TT_Iter it2 = *(TT_Iter*)Vector_Get(iters,StartIndex+1);
                TokenMap* tm = (TokenMap*)Vector_Get(t->args,it2);
                return TokenMap_IterateInto(tm,iters,StartIndex+2);
            }else{
                return t;
            }
        }
    }
    return NULL;
}

void TokenMap_Clear(TokenMap* rm){
    for(int i = 0;i<rm->size;i++){
        Token* t = (Token*)Vector_Get(rm,i);
        Token_Free(t);
    }
    Vector_Clear(rm);
}
void TokenMap_Free(TokenMap* rm){
    if(rm->Memory){
        for(int i = 0;i<rm->size;i++){
            Token* t = (Token*)Vector_Get(rm,i);
            Token_Free(t);
        }
        Vector_Free(rm);
    }
}
void TokenMap_Print(TokenMap* rm){
    printf("--- TokenMap ---\n");
    if(rm){
        for(int i = 0;i<rm->size;i++){
            Token* t = (Token*)Vector_Get(rm,i);
            printf("| (%s: %d) %d Token: ",t->file,t->line,i);
            Token_Print_N(t,0);
            printf("\n");
        }
    }
    printf("----------------\n");
}
void TokenMap_Print_S(TokenMap* rm){
    if(rm){
        for(int i = 0;i<rm->size;i++){
            Token* t = (Token*)Vector_Get(rm,i);
            if(t){
                if(t->tt == TOKEN_NUMBER)
                    printf("%ld",t->v_i64);
                else if(t->tt == TOKEN_FLOAT)
                    printf("%lf",t->v_f64);
                else if(t->tt == TOKEN_BOOL)
                    printf("%d",t->v_b1);
                else if(t->tt==TOKEN_CONSTSTRING_SINGLE)
                    printf("\'%s\'",t->str);
                else if(t->tt==TOKEN_CONSTSTRING_DOUBLE)
                    printf("\"%s\"",t->str);
                else
                    printf("%s",t->str);

                if(t->args){
                    printf("(");
                    for(int j = 0;j<t->args->size;j++){
                        TokenMap* ptm = (TokenMap*)Vector_Get(t->args,j);
                        TokenMap_Print_S(ptm);
                        if(j<t->args->size - 1)
                            printf(",");
                    }
                    printf(")");
                }
                if(i<rm->size - 1)
                    printf(" ");
            }
        }
    }
}



typedef struct KeywordRP {
    char* cstr;
    TT_Type rep;
} KeywordRP;

KeywordRP KeywordRP_New(char* cstr,TT_Type tt){
    return (KeywordRP){ CStr_Cpy(cstr),tt };
}
KeywordRP KeywordRP_Null(){
    return (KeywordRP){ NULL,TOKEN_NONE };
}
void KeywordRP_Free(KeywordRP* rp){
    rp->rep = TOKEN_NONE;
    CStr_Free((CStr*)&rp->cstr);
}
void KeywordRP_Print(KeywordRP* rp){
    printf("%d: %s\n",rp->rep,rp->cstr);
}
#define KEYWORD_END KeywordRP_New(NULL,TOKEN_END)

typedef Vector KeywordMap;

KeywordMap KeywordMap_New(){
    KeywordMap rm = Vector_New(sizeof(KeywordRP));
    return rm;
}
KeywordMap KeywordMap_Make(KeywordRP* initlist){
    KeywordMap rm = Vector_New(sizeof(KeywordRP));
    for(int i = 0;initlist[i].rep != TOKEN_END && initlist[i].rep != TOKEN_NONE;i++){
        Vector_Push(&rm,initlist + i);
    }
    return rm;
}
void KeywordMap_Push(KeywordMap* rm,KeywordRP rp){
    Vector_Push(rm,&rp);
}
void KeywordMap_Pop(KeywordMap* rm){
    Vector_PopTop(rm);
}
void KeywordMap_Free(KeywordMap* rm){
    for(int i = 0;i<rm->size;i++){
        KeywordRP* t = (KeywordRP*)Vector_Get(rm,i);
        KeywordRP_Free(t);
    }
    Vector_Free(rm);
}
void KeywordMap_Print(KeywordMap* rm){
    printf("--- KeywordMap ---\n");
    for(int i = 0;i<rm->size;i++){
        KeywordRP* t = (KeywordRP*)Vector_Get(rm,i);
        KeywordRP_Print(t);
    }
    printf("------------------\n");
}



typedef struct OperatorRP {
    Vector tts;
    TT_Type rep;
    TT_Count args;
} OperatorRP;

OperatorRP OperatorRP_New(TT_Type tt,TT_Count args){
    OperatorRP orp = { Vector_New(sizeof(TT_Type)),tt,args };
    return orp;
}
OperatorRP OperatorRP_Make(TT_Type* initlist,TT_Type tt,TT_Count args){
    OperatorRP orp = OperatorRP_New(tt,args);

    for(int i = 0;;i++){
        TT_Type rp = initlist[i];
        if(rp==TOKEN_END || rp==TOKEN_NONE) break;
        Vector_Push(&orp.tts,&rp);
    }
    
    return orp;
}
OperatorRP OperatorRP_End(){
    return (OperatorRP){ {},TOKEN_END,ARGS_IGNORE };
}
OperatorRP OperatorRP_Null(){
    return (OperatorRP){ {},TOKEN_END,ARGS_IGNORE };
}
void OperatorRP_Push(OperatorRP* rm,TT_Type rp){
    Vector_Push(&rm->tts,&rp);
}
void OperatorRP_Pop(OperatorRP* rm){
    Vector_PopTop(&rm->tts);
}
void OperatorRP_Free(OperatorRP* rp){
    rp->rep = TOKEN_NONE;
    Vector_Free(&rp->tts);
}
void OperatorRP_Print_S(OperatorRP* rp){
    printf("Tok:%d  Args:%d",rp->rep,rp->args);
}
void OperatorRP_Print(OperatorRP* rp){
    printf("--- OperatorRP ---\n");
    OperatorRP_Print_S(rp);
    printf("\n------------------\n");
}

/*
    Important: can not build ops of two if the first Token is no op!
    
    Example: -- only if - is a op!
    
    If the first Token is defined as a op but with a specific amount of args then you should
    use ARGS_IGNORE or define the operator for every arg count
*/
typedef Vector OperatorMap;

OperatorMap OperatorMap_New(){
    OperatorMap rm = Vector_New(sizeof(OperatorRP));
    return rm;
}
OperatorMap OperatorMap_Make(OperatorRP* initlist){
    OperatorMap rm = Vector_New(sizeof(OperatorRP));
    for(int i = 0;;i++){
        OperatorRP rp = initlist[i];
        if(rp.rep==TOKEN_END || rp.rep==TOKEN_NONE) break;
        Vector_Push(&rm,&rp);
    }
    return rm;
}
void OperatorMap_Push(OperatorMap* rm,OperatorRP rp){
    Vector_Push(rm,&rp);
}
void OperatorMap_Pop(OperatorMap* rm){
    Vector_PopTop(rm);
}
TT_Iter OperatorMap_getOPRP(OperatorMap* rm,Vector* tts,TT_Count Args){
    if(rm->size==0)     return -1;
    if(tts->size==0)    return -1;

    for(int i = 0;i<rm->size;i++){
        OperatorRP* rp = (OperatorRP*)Vector_Get(rm,i);
        if(rp->tts.size==tts->size && (rp->args==ARGS_IGNORE || rp->args==Args)){
            for(int j = 0;j<tts->size;j++){
                TT_Type tt1 = *(TT_Type*)Vector_Get(&rp->tts,j);
                TT_Type tt2 = *(TT_Type*)Vector_Get(tts,j);

                //printf("%d vs %d\n",tt1,tt2);
                if(tt1!=tt2) break;
                if(j==tts->size-1){
                    return i;
                }
            }
        }
    }
    return -1;
}
TT_Bool OperatorMap_isOP(OperatorMap* rm,Vector* tts,TT_Count Args){
    return OperatorMap_getOPRP(rm,tts,Args)>=0;
}
int OperatorMap_Depth(OperatorMap* rm){
    int Depth = 0;
    for(int i = 0;i<rm->size;i++){
        OperatorRP* rp = (OperatorRP*)Vector_Get(rm,i);
        if(rp->tts.size>Depth)  Depth = rp->tts.size;
    }
    return Depth;
}
void OperatorMap_Free(OperatorMap* rm){
    for(int i = 0;i<rm->size;i++){
        OperatorRP* t = (OperatorRP*)Vector_Get(rm,i);
        OperatorRP_Free(t);
    }
    Vector_Free(rm);
}
void OperatorMap_Print(OperatorMap* rm){
    printf("--- OperatorMap ---\n");
    for(int i = 0;i<rm->size;i++){
        OperatorRP* t = (OperatorRP*)Vector_Get(rm,i);
        printf("%d: ",i);
        OperatorRP_Print_S(t);
        printf("\n");
    }
    printf("-------------------\n");
}


typedef struct Reseter {
    TT_Type tt;
    TT_Behave behave;
} Reseter;

Reseter Reseter_New(TT_Type tt,TT_Behave behave){
    Reseter orp;
    orp.tt = tt;
    orp.behave = behave;
    return orp;
}
Reseter Reseter_End(){
    return Reseter_New(TOKEN_END,RESETER_NONE);
}

typedef Vector ReseterMap;

ReseterMap ReseterMap_New(){
    ReseterMap orp = Vector_New(sizeof(Reseter));
    return orp;
}
ReseterMap ReseterMap_Make(Reseter* initlist){
    ReseterMap orp = ReseterMap_New();

    for(int i = 0;initlist[i].tt!=TOKEN_END;i++){
        Vector_Push(&orp,&initlist[i]);
    }
    
    return orp;
}
ReseterMap ReseterMap_Std(){
    ReseterMap rsm = ReseterMap_Make((Reseter[]){ 
        // operators who might have two versions: 1 arg vs. 2 args are not listed: -
        { TOKEN_NONE,RESETER_NONE },
        { TOKEN_NUMBER,RESETER_ADD1 },
        { TOKEN_FLOAT,RESETER_ADD1 },
        { TOKEN_STRING,RESETER_ADD1 },
        
        { TOKEN_SPACE,RESETER_NONE },
        { TOKEN_NEWLINE,RESETER_NONE },
        { TOKEN_CARTURN,RESETER_NONE },
        { TOKEN_HTAB,RESETER_NONE },
        { TOKEN_VTAB,RESETER_NONE },
        
        { TOKEN_PARENTHESES_R,RESETER_NONE },
        { TOKEN_SQUARE_BRACKETS_R,RESETER_NONE },
        { TOKEN_CURLY_BRACES_R,RESETER_NONE },
        //{ TOKEN_ANGLE_BRACKETS_R,RESETER_NONE }, //also < and > operator
        { TOKEN_DOUBLE_ANGLE_BRACKETS_R,RESETER_NONE },
        { TOKEN_WHITE_SQUARE_BRACKETS_R,RESETER_NONE },
        
        { TOKEN_PARENTHESES_L,RESETER_RST },
        { TOKEN_SQUARE_BRACKETS_L,RESETER_RST },
        { TOKEN_CURLY_BRACES_L,RESETER_RST },
        //{ TOKEN_ANGLE_BRACKETS_L,RESETER_RST }, //also < and > operator
        { TOKEN_DOUBLE_ANGLE_BRACKETS_L,RESETER_RST },
        { TOKEN_WHITE_SQUARE_BRACKETS_L,RESETER_RST },
        { TOKEN_SEMICOLON,RESETER_RST },
        { TOKEN_COLON,RESETER_RST },
        { TOKEN_EXCLAMATION_MARK,RESETER_RST },
        { TOKEN_QUESTION_MARK,RESETER_RST },
        { TOKEN_ELLIPSIS,RESETER_RST },
        { TOKEN_PLUS_SIGN,RESETER_RST },
        { TOKEN_MULTIPLICATION_SIGN,RESETER_RST },
        { TOKEN_DIVISION_SIGN,RESETER_RST },
        { TOKEN_EQUAL_SIGN,RESETER_RST },
        { TOKEN_NOT_EQUAL_SIGN,RESETER_RST },
        { TOKEN_LESS_THAN,RESETER_RST },
        { TOKEN_GREATER_THAN,RESETER_RST },
        { TOKEN_LESS_THAN_OR_EQUAL_TO,RESETER_RST },
        { TOKEN_GREATER_THAN_OR_EQUAL_TO,RESETER_RST },
        { TOKEN_APPROXIMATION_SIGN,RESETER_RST },
        { TOKEN_PLUS_MINUS_SIGN,RESETER_RST },
        { TOKEN_AMPERSAND,RESETER_RST },
        { TOKEN_AT_SIGN,RESETER_RST },
        { TOKEN_ASTERISK,RESETER_RST },
        { TOKEN_CARET,RESETER_RST },
        { TOKEN_TILDE,RESETER_RST },
        { TOKEN_SLASH,RESETER_RST },
        { TOKEN_BACKSLASH,RESETER_RST },
        { TOKEN_UNDERSCORE,RESETER_RST },
        { TOKEN_PIPE,RESETER_RST },
        { TOKEN_PERCENT,RESETER_RST },
        { TOKEN_HASH_POUND_SIGN,RESETER_RST },
        { TOKEN_SINGLE_QUOTE,RESETER_RST },
        { TOKEN_DOUBLE_QUOTE,RESETER_RST },
        { TOKEN_BACKTICK,RESETER_RST },
        { TOKEN_PRIME,RESETER_RST },
        { TOKEN_DOUBLE_PRIME,RESETER_RST },
        { TOKEN_SECTION_SYMBOL,RESETER_RST },
        { TOKEN_DEGREE_SYMBOL,RESETER_RST },
        { TOKEN_FRACTION_SLASH,RESETER_RST },
        { TOKEN_INTERROBANG,RESETER_RST },
        { TOKEN_BULLET_POINT,RESETER_RST },
        { TOKEN_END,RESETER_RST } 
    });

    return rsm;
}
void ReseterMap_Push(ReseterMap* rm,Reseter rs){
    Vector_Push(rm,&rs);
}
void ReseterMap_Pop(ReseterMap* rm){
    Vector_PopTop(rm);
}
void ReseterMap_PushCount(ReseterMap* rm,Reseter* list){
    for(int i = 0;list[i].tt!=TOKEN_END;i++){
        Vector_Push(rm,&list[i]);
    }
}
void ReseterMap_Do(ReseterMap* rm,TT_Type rp,TT_Count* args){
    for(int i = 0;i<rm->size;i++){
        Reseter* tt = (Reseter*)Vector_Get(rm,i);

        if(tt->tt==rp){
            if(tt->behave==RESETER_NONE){}
            else if(tt->behave==RESETER_RST)     *args = 0;
            else if(tt->behave==RESETER_TO1)     *args = 1;
            else if(tt->behave==RESETER_TO2)     *args = 2;
            else if(tt->behave==RESETER_ADD1)    *args += 1; 
            else if(tt->behave==RESETER_ADD2)    *args += 2;
            return;
        }
    }
}
void ReseterMap_Free(ReseterMap* rp){
    Vector_Free(rp);
}


typedef struct Directive {
    TT_Type tt;
    void* data;
    int (*fn)(struct Directive*,int,Vector*);
    void (*freefn)(void*);
} Directive;

Directive Directive_New(TT_Type tt,int (*fn)(Directive*,int,Vector*)){
    Directive orp;
    orp.tt = tt;
    orp.data = NULL;
    orp.fn = fn;
    orp.freefn = NULL;
    return orp;
}
Directive Directive_Make(TT_Type tt,void* data,int (*fn)(Directive*,int,Vector*),void (*freefn)(void*)){
    Directive orp;
    orp.tt = tt;
    orp.data = data;
    orp.fn = fn;
    orp.freefn = freefn;
    return orp;
}
Directive Directive_End(){
    Directive orp;
    orp.tt = TOKEN_END;
    orp.data = NULL;
    orp.fn = NULL;
    orp.freefn = NULL;
    return orp;
}
void Directive_Free(Directive* d){
    if(d->freefn) d->freefn(d->data);
    
    d->tt = TOKEN_NONE;
    if(d->data) free(d->data);
    d->data = NULL;
    d->fn = NULL;
    d->freefn = NULL;
}
void Directive_Print(Directive* d){
    printf("%d: D:%p (%p,%p)\n",d->tt,d->data,d->fn,d->freefn);
}


typedef struct DirectiveEnd {
    TT_Type end;
    TT_Type target;
    Boolean rmBegin;
    Boolean rmEnd;
} DirectiveEnd;

int Directive_Remove_Function(Directive* d,int start,TokenMap* v){
    Token* t = (Token*)Vector_Get(v,start);
    Token_Free(t);
    Vector_Remove(v,start);
    return start - 1;
}
int Directive_Comment_Block_Function(Directive* d,int start,TokenMap* v){
    for(int i = start;i<v->size;i++){
        Token* t = (Token*)Vector_Get(v,i);

        DirectiveEnd* de = (DirectiveEnd*)d->data;
        if(t->tt==de->end || i + 1 == v->size){
            int Begin = start;
            int End = i + 1;
            if(!de->rmBegin) Begin++;
            if(!de->rmEnd) End--;

            for(int j = End-1;j>=Begin;j--){
                Token* t = (Token*)Vector_Get(v,j);
                Token_Free(t);
                Vector_Remove(v,j);
            }
            break;
        }
    }
    return start - 1;
}
int Directive_Compress_Block_Function(Directive* d,int start,TokenMap* v){
    for(int i = start;i<v->size;i++){
        Token* t = (Token*)Vector_Get(v,i);

        DirectiveEnd* de = (DirectiveEnd*)d->data;
        if(t->tt==de->end){
            int Begin = start;
            int End = i + 1;
            if(!de->rmBegin) Begin++;
            if(!de->rmEnd) End--;
            
            Token* tag = (Token*)Vector_Get(v,Begin);
            
            if(t->tt != TOKEN_NUMBER && t->tt != TOKEN_FLOAT && t->tt != TOKEN_BOOL)
                CStr_Free((CStr*)&t->str);

            t->tt = de->target;

            TokenMap tm = TokenMap_SubMove(v,Begin+1,End);
            Token_Args_Push(t,&tm);

            Token_Free(tag);
            Vector_RemoveCount(v,Begin,End - Begin);
            break;
        }
    }
    return start - 1;
}
int Directive_Compress_Tag_Function(Directive* d,int start,TokenMap* v){
    for(int i = start;i<v->size;i++){
        Token* t = (Token*)Vector_Get(v,i);

        DirectiveEnd* de = (DirectiveEnd*)d->data;
        if(t->tt==de->end){
            int Begin = start;
            int End = i + 1;
            if(!de->rmBegin) Begin++;
            if(!de->rmEnd) End--;
            
            Token* tag = (Token*)Vector_Get(v,Begin);
            
            if(t->tt != TOKEN_NUMBER && t->tt != TOKEN_FLOAT && t->tt != TOKEN_BOOL)
                CStr_Free((CStr*)&t->str);

            t->tt = de->target;

            TokenMap tagm = TokenMap_SubMove(v,Begin+1,Begin+2);
            Token_Args_Push(t,&tagm);

            for(int j = Begin+2;j<End;j+=3){
                TokenMap attrm = TokenMap_SubMove(v,j,j+3);
                Token_Args_Push(t,&attrm);
            }

            Token_Free(tag);
            Vector_RemoveCount(v,Begin,End - Begin);
            break;
        }
    }
    return start - 1;
}
Directive Directive_Comment_Block(TT_Type start,TT_Type end,Boolean rmEnd){
    DirectiveEnd* de = (DirectiveEnd*)malloc(sizeof(DirectiveEnd));
    de->end = end;
    de->target = TOKEN_NONE;
    de->rmBegin = True;
    de->rmEnd = rmEnd;
    return Directive_Make(start,de,Directive_Comment_Block_Function,NULL);
}
Directive Directive_Comment_Line(TT_Type start){
    return Directive_Comment_Block(start,TOKEN_NEWLINE,False);
}
Directive Directive_Compress_Block(TT_Type target,TT_Type start,TT_Type end){
    DirectiveEnd* de = (DirectiveEnd*)malloc(sizeof(DirectiveEnd));
    de->end = end;
    de->target = target;
    de->rmBegin = True;
    de->rmEnd = False;
    return Directive_Make(start,de,Directive_Compress_Block_Function,NULL);
}
Directive Directive_Compress_Tag(TT_Type target,TT_Type start,TT_Type end){
    DirectiveEnd* de = (DirectiveEnd*)malloc(sizeof(DirectiveEnd));
    de->end = end;
    de->target = target;
    de->rmBegin = True;
    de->rmEnd = False;
    return Directive_Make(start,de,Directive_Compress_Tag_Function,NULL);
}
Directive Directive_Remove(TT_Type start){
    return Directive_Make(start,NULL,Directive_Remove_Function,NULL);
}


typedef struct Directives {
    Vector tts;//Vector<TT_Type>
    Token replacer;
} Directives;

void Directives_Free(Directives* des){
    Vector_Free(&des->tts);
}

int Directive_Compress_Function(Directive* d,int start,TokenMap* v){
    Directives* des = (Directives*)d->data;
    for(int j = 0;j<des->tts.size;j++){
        TT_Type tt = *(TT_Type*)Vector_Get(&des->tts,j);
        if(start+j<v->size){
            Token* s = (Token*)Vector_Get(v,start+j);
            if(s->tt!=tt)   return start;
        }else               return start;
    }

    Token* t = (Token*)Vector_Get(v,start);
    Token_Free(t);
    *t = Token_Cpy(&des->replacer);

    for(int i = 1;i<des->tts.size;i++){
        Token* t = (Token*)Vector_Get(v,i);
        Token_Free(t);
    }
    Vector_RemoveCount(v,start+1,des->tts.size-1);

    return start - 1;
}
int Directive_Compress_Function_Cast(Directive* d,int start,TokenMap* v){
    Directives* des = (Directives*)d->data;
    for(int j = 0;j<des->tts.size;j++){
        TT_Type tt = *(TT_Type*)Vector_Get(&des->tts,j);
        if(start+j<v->size){
            Token* s = (Token*)Vector_Get(v,start+j);
            if(s->tt!=tt)   return start;
        }else               return start;
    }

    Token* pretl = (Token*)Vector_Get(v,start);
    Token* caster = (Token*)Vector_Get(v,start+1);
    Token* pretr = (Token*)Vector_Get(v,start+2);
    
    Token_Set(pretl,caster);
    pretl->tt = TOKEN_CAST;

    Token_Free(pretr);

    Vector_Remove(v,start+2);
    Vector_Remove(v,start+1);

    return start - 1;
}
Directive Directive_Compress(Token target,TT_Type* tts){
    Directives* des = (Directives*)malloc(sizeof(Directives));
    des->replacer = Token_Cpy(&target);
    des->tts = Vector_New(sizeof(TT_Type));

    for(int i = 0;tts[i]!=TOKEN_NONE && tts[i]!=TOKEN_END;i++){
        Vector_Push(&des->tts,&tts[i]);
    }

    return Directive_Make(tts[0],des,Directive_Compress_Function,(void*)Directives_Free);
}
Directive Directive_Compress_Cast(TT_Type* tts){
    Directives* des = (Directives*)malloc(sizeof(Directives));
    des->replacer = Token_Null();
    des->tts = Vector_New(sizeof(TT_Type));

    for(int i = 0;tts[i]!=TOKEN_NONE && tts[i]!=TOKEN_END;i++){
        Vector_Push(&des->tts,&tts[i]);
    }

    return Directive_Make(tts[0],des,Directive_Compress_Function_Cast,(void*)Directives_Free);
}
Directive Directive_Compress_F(Token target,TT_Type* tts,int (*func)(Directive *d, int start, TokenMap *v)){
    Directives* des = (Directives*)malloc(sizeof(Directives));
    des->replacer = Token_Cpy(&target);
    des->tts = Vector_New(sizeof(TT_Type));

    for(int i = 0;tts[i]!=TOKEN_NONE && tts[i]!=TOKEN_END;i++){
        Vector_Push(&des->tts,&tts[i]);
    }

    return Directive_Make(tts[0],des,func,(void*)Directives_Free);
}


typedef Vector DirectiveMap;

DirectiveMap DirectiveMap_New(){
    DirectiveMap dm = Vector_New(sizeof(Directive));
    return dm;
}
DirectiveMap DirectiveMap_Make(Directive* initlist){
    DirectiveMap dm = DirectiveMap_New();

    for(int i = 0;;i++){
        Directive rp = initlist[i];
        if(rp.tt==TOKEN_END || rp.tt==TOKEN_NONE) break;
        Vector_Push(&dm,&rp);
    }
    
    return dm;
}
void DirectiveMap_Push(DirectiveMap* dm,Directive rs){
    Vector_Push(dm,&rs);
}
void DirectiveMap_Pop(DirectiveMap* dm){
    Vector_PopTop(dm);
}
void DirectiveMap_PushCount(DirectiveMap* dm,Directive* list){
    for(int i = 0;;i++){
        Directive rp = list[i];
        if(rp.tt==TOKEN_END) break;
        Vector_Push(dm,&rp);
    }
}
TT_Iter DirectiveMap_Contains(DirectiveMap* rp,TT_Type tt){
    for(int i = 0;i<rp->size;i++){
        Directive* t = (Directive*)Vector_Get(rp,i);
        if(tt==t->tt) return i;
    }
    return TT_INVALID;
}
void DirectiveMap_Free(DirectiveMap* rp){
    for(int i = 0;i<rp->size;i++){
        Directive* t = (Directive*)Vector_Get(rp,i);
        Directive_Free(t);
    }
    Vector_Free(rp);
}
void DirectiveMap_Print(DirectiveMap* rp){
    printf("--- DirectiveMap ---\n");
    for(int i = 0;i<rp->size;i++){
        Directive* t = (Directive*)Vector_Get(rp,i);
        Directive_Print(t);
    }
    printf("--------------------\n");
}


typedef struct InstructionMap {
    Vector Insts;
    Vector Septs;
} InstructionMap;

InstructionMap InstructionMap_New(){
    InstructionMap dm;
    dm.Insts = Vector_New(sizeof(TokenMap));
    dm.Septs = Vector_New(sizeof(TT_Type));
    return dm;
}
InstructionMap InstructionMap_Make(TT_Type* initlist){
    InstructionMap dm = InstructionMap_New();

    for(int i = 0;;i++){
        TT_Type tt = initlist[i];
        if(tt==TOKEN_END || tt==TOKEN_NONE) break;
        Vector_Push(&dm.Septs,&tt);
    }
    
    return dm;
}
void InstructionMap_Push(InstructionMap* dm,TT_Type tt){
    Vector_Push(&dm->Septs,&tt);
}
void InstructionMap_Pop(InstructionMap* dm){
    Vector_PopTop(&dm->Septs);
}
void InstructionMap_PushCount(InstructionMap* dm,TT_Type* list){
    for(int i = 0;;i++){
        TT_Type tt = list[i];
        if(tt==TOKEN_END) break;
        Vector_Push(&dm->Septs,&tt);
    }
}
TT_Iter InstructionMap_Contains(InstructionMap* dm,TT_Type tt){
    for(int i = 0;i<dm->Septs.size;i++){
        TT_Type t = *(TT_Type*)Vector_Get(&dm->Septs,i);
        if(tt==t) return i;
    }
    return TT_INVALID;
}
void InstructionMap_PushInst(InstructionMap* dm,Token* t,int Count){
    TokenMap tm = TokenMap_New();
    Vector_PushCount(&tm,t,Count);
    Vector_Push(&dm->Insts,&tm);
}
void InstructionMap_Free(InstructionMap* dm){
    for(int i = 0;i<dm->Insts.size;i++){
        TokenMap* tm = (TokenMap*)Vector_Get(&dm->Insts,i);
        TokenMap_Free(tm);
    }
    Vector_Free(&dm->Insts);
    Vector_Free(&dm->Septs);
}


typedef Vector TT_TypeMap;

TT_TypeMap TT_TypeMap_New(){
    TT_TypeMap dm = Vector_New(sizeof(TT_Type));
    return dm;
}
TT_TypeMap TT_TypeMap_Make(TT_Type* initlist){
    TT_TypeMap dm = TT_TypeMap_New();

    for(int i = 0;;i++){
        TT_Type rp = initlist[i];
        if(rp==TOKEN_END || rp==TOKEN_NONE) break;
        Vector_Push(&dm,&rp);
    }
    
    return dm;
}
void TT_TypeMap_PushCount(TT_TypeMap* dm,TT_Type* list){
    for(int i = 0;;i++){
        TT_Type rp = list[i];
        if(rp==TOKEN_END) break;
        Vector_Push(dm,&rp);
    }
}
TT_Iter TT_TypeMap_Contains(TT_TypeMap* rp,TT_Type tt){
    for(int i = 0;i<rp->size;i++){
        TT_Type t = *(TT_Type*)Vector_Get(rp,i);
        if(tt==t) return i;
    }
    return TT_INVALID;
}
void TT_TypeMap_Free(TT_TypeMap* rp){
    Vector_Free(rp);
}


typedef struct DTT_Type {
    TT_Type t1;
    Token t2;
} DTT_Type;

DTT_Type DTT_Type_New(TT_Type t1,Token t2){
    DTT_Type t = { t1,t2 };
    return t;
}
DTT_Type DTT_Type_Null(){
    return DTT_Type_New(TOKEN_NONE,Token_Null());
}
void DTT_Type_Free(DTT_Type* dt){
    Token_Free(&dt->t2);
}
void DTT_Type_Print(DTT_Type* dt){
    printf("%d: ",dt->t1);
    Token_Print_S(&dt->t2);
    printf("\n");
}
#define DTT_TYPE_END    DTT_Type_Null()


typedef Vector DTT_TypeMap;

DTT_TypeMap DTT_TypeMap_New(){
    DTT_TypeMap dm = Vector_New(sizeof(DTT_Type));
    return dm;
}
DTT_TypeMap DTT_TypeMap_Make(DTT_Type* initlist){
    DTT_TypeMap dm = DTT_TypeMap_New();

    for(int i = 0;;i++){
        DTT_Type rp = initlist[i];
        if(rp.t1==TOKEN_END || rp.t1==TOKEN_NONE)
            break;
        Vector_Push(&dm,&rp);
    }
    
    return dm;
}
void DTT_TypeMap_PushCount(DTT_TypeMap* dm,DTT_Type* list){
    for(int i = 0;;i++){
        DTT_Type rp = list[i];
        if(rp.t1==TOKEN_END || rp.t1==TOKEN_NONE)
            break;
        Vector_Push(dm,&rp);
    }
}
TT_Iter DTT_TypeMap_Contains(DTT_TypeMap* rp,TT_Type tt){
    for(int i = 0;i<rp->size;i++){
        DTT_Type dt = *(DTT_Type*)Vector_Get(rp,i);
        if(tt==dt.t1) return i;
    }
    return TT_INVALID;
}
void DTT_TypeMap_Free(DTT_TypeMap* rp){
    for(int i = 0;i<rp->size;i++){
        DTT_Type* dt = (DTT_Type*)Vector_Get(rp,i);
        DTT_Type_Free(dt);
    }
    Vector_Free(rp);
}
void DTT_TypeMap_Print(DTT_TypeMap* rp){
    printf("--- DTT_TypeMap ---\n");
    for(int i = 0;i<rp->size;i++){
        DTT_Type* dt = (DTT_Type*)Vector_Get(rp,i);
        DTT_Type_Print(dt);
    }
    printf("-------------------\n");
}



typedef TokenMap Parser;

Parser Parser_New(){
    Parser p = TokenMap_New();
    return p;
}
Token* Parser_Token(Parser* p,int Index){
    if(Index>=0 && Index<p->size) return ((Token*)Vector_Get(p,Index));
    return NULL;
}
TT_Type Parser_Token_Type(Parser* p,int Index){
    if(Index>=0 && Index<p->size) return ((Token*)Vector_Get(p,Index))->tt;
    return TOKEN_NONE;
}
TT_Iter Parser_Find(Vector* toks,TT_Type tt) {
    for(int i = 0;i<toks->size;i++){
        Token* tok = (Token*)Vector_Get(toks,i);
        if(tok->tt==tt) return i;
    }
    return TT_INVALID;
}

void Parser_Ban(Parser* p,TT_TypeMap* tts){
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);
        
        for(int j = 0;j<tts->size;j++){
            TT_Type tt = *(TT_Type*)Vector_Get(tts,j);

            if(t->tt==tt){
                Token_Free(t);
                Vector_Remove(p,i);
                i--;
                break;
            }
        }
    }
}
void Parser_PushToken(Parser* p,TT_Type* State,String* Buffer,int ch,int line,CStr path){
    if(Buffer->size==0) return;
    
    Token t = Token_New(*State,Buffer);
    t.ch = ch;
    t.line = line;
    t.file = CStr_Cpy(path);

    Vector_Push(p,&t);
    String_Clear(Buffer);
    *State = TOKEN_NONE;
}
void Parser_Classify(Parser* p,TT_Type* State,String* Buffer,TT_Char c,int ch,int line,CStr path){
    TT_Type type = TT_Type_Get(c);

    if(type!=TOKEN_NONE){
        Parser_PushToken(p,State,Buffer,ch,line,path);
        *State = type;
        String_AppendChar(Buffer,c);
        Parser_PushToken(p,State,Buffer,ch,line,path);
    }else{
        if(*State==TOKEN_NONE){
            if(TT_Type_isStr(c)){
                String_AppendChar(Buffer,c);
                *State = TOKEN_STRING;
            }else if(TT_Type_isNum(c)){
                String_AppendChar(Buffer,c);
                *State = TOKEN_NUMBER;
            }
        }else if(*State==TOKEN_NUMBER && c == '_'){
            *State = TOKEN_STRING;
            String_AppendChar(Buffer,c);
        }else{
            String_AppendChar(Buffer,c);
        }
    }
}
void Parser_PreConcat(Parser* p,int Start,int End){ // [ Start;End [
    if(Start>=p->size)  return;
    if(End<=Start)      return;
    
    Token* st = Parser_Token(p,Start);
    String Buffer = String_New();
    String_Append(&Buffer,st->str);
    
    for(int i = Start+1;i<End && i<p->size;End--){
        Token* t = Parser_Token(p,i);
        String_Append(&Buffer,t->str);
        Token_Free(t);
        Vector_Remove(p,i);
    }
    
    st = Parser_Token(p,Start);
    CStr_Free(&st->str);

    String_AppendChar(&Buffer,'\0');
    Token_Value(st,(char*)Buffer.Memory);
    String_Free(&Buffer);
}
void Parser_Concat(Parser* p,TT_Type tt,int Start,int End){ // [ Start;End [
    if(Start>=p->size)  return;
    if(End<=Start)      return;
    
    Token* st = Parser_Token(p,Start);
    String Buffer = String_New();
    String_Append(&Buffer,st->str);
    
    for(int i = Start+1;i<End && i<p->size;End--){
        Token* t = Parser_Token(p,i);
        String_Append(&Buffer,t->str);
        Token_Free(t);
        Vector_Remove(p,i);
    }
    
    st = Parser_Token(p,Start);
    Token_Free(st);

    String_AppendChar(&Buffer,'\0');
    *st = Token_Move(tt,(char*)Buffer.Memory);
}
void Parser_ConcatKillBorders(Parser* p,TT_Type tt,int Start,int End){ // ] Start;End [
    if(Start>=p->size)  return;
    if(End<=Start)      return;
    
    Token* sta = (Token*)Vector_Get(p,Start);
    Token* end = (Token*)Vector_Get(p,End);

    Token_Free(end);
    Vector_Remove(p,End);

    String Buffer = String_New();

    for(int i = Start+1;i<End && i<p->size;End--){
        Token* t = (Token*)Vector_Get(p,i);
        Token_Append(t,&Buffer);
        Token_Free(t);
        Vector_Remove(p,i);
    }
    
    sta = (Token*)Vector_Get(p,Start);
    Token_Free(sta);

    String_AppendChar(&Buffer,'\0');
    *sta = Token_Move(tt,(char*)Buffer.Memory);
}
void Parser_Parse_CStr(Parser* p,TT_Char* cstr,CStr path){
    String Buffer = String_New();
    int Size = CStr_Size(cstr);
    TT_Type State = TOKEN_NONE;

    int ch = 1;
    int line = 1;

    for(int i = 0;i<Size;i++){
        TT_Char c = cstr[i];
        Parser_Classify(p,&State,&Buffer,c,ch,line,path);
        if(i==Size-1) Parser_PushToken(p,&State,&Buffer,ch,line,path);

        if(c == '\n'){
            ch = 1;
            line++;
        }else
            ch++;
    }

    String_Free(&Buffer);

    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);

        if(t->tt==TOKEN_NUMBER){
            Token* dot = (Token*)Vector_Get(p,i+1);
            Token* flt = (Token*)Vector_Get(p,i+2);

            if(i<p->size-2){
                if(dot->tt==TOKEN_PERIOD && flt->tt==TOKEN_NUMBER){
                    Parser_PreConcat(p,i,i+3);
                    t = (Token*)Vector_Get(p,i);
                    t->tt = TOKEN_FLOAT;
                    continue;
                }
            }
            
            CStr cstr = t->str;
            t->v_i64 = Number_Parse((i8*)cstr);
            CStr_Free(&cstr);
        }else if(t->tt==TOKEN_BOOL){
            CStr cstr = t->str;
            t->v_b1 = Boolean_Parse((i8*)cstr);
            CStr_Free(&cstr);
        }
    }
}
void Parser_TF_Num(Parser* p){
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);

        if(t->tt==TOKEN_STRING){
            Number n = Number_Parse((i8*)t->str);
            Double f = Double_Parse((i8*)t->str,1);
            Boolean b = Boolean_Parse((i8*)t->str);
            
            if(n != NUMBER_PARSE_ERROR){
                CStr_Free(&t->str);
                t->tt = TOKEN_NUMBER;
                t->v_i64 = n;
            }else if(f != DOUBLE_PARSE_ERROR){
                CStr_Free(&t->str);
                t->tt = TOKEN_FLOAT;
                t->v_f64 = f;
            }else if(b != BOOL_PARSE_ERROR){
                CStr_Free(&t->str);
                t->tt = TOKEN_BOOL;
                t->v_b1 = b;
            }
        }
    }
}
void Parser_TF_Esc(Parser* p){
    for(int i = 0;i<p->size-1;i++){
        Token* esc = (Token*)Vector_Get(p,i);
        Token* tok = (Token*)Vector_Get(p,i+1);

        if(esc->tt==TOKEN_BACKSLASH){
            if(tok->tt==TOKEN_STRING || tok->tt==TOKEN_NUMBER || tok->tt==TOKEN_FLOAT || tok->tt==TOKEN_BOOL){
                String builder = String_New();
                Token_Append(tok,&builder);
                String_AddChar(&builder,'\\',0);
                String_CompressEsc(&builder);

                CStr cstr = String_CStr(&builder);
                String_Free(&builder);
                
                CStr_Free(&esc->str);
                esc->str = cstr;
                esc->tt = TOKEN_CHAR;

                Token_Free(tok);
                Vector_Remove(p,i+1);
            }else{
                if(tok->tt==TOKEN_NEWLINE || tok->tt==TOKEN_SINGLE_QUOTE || tok->tt==TOKEN_DOUBLE_QUOTE || tok->tt==TOKEN_BACKSLASH)
                    esc->tt = TOKEN_CHAR;

                CStr_Set((char**)&esc->str,tok->str);

                Token_Free(tok);
                Vector_Remove(p,i+1);
            }
        }
    }
}
void Parser_TF_Str(Parser* p){
    int LastIndex = -1;
    TT_Bool QuotesSingle = TT_False;
    TT_Bool QuotesDouble = TT_False;

    Parser_TF_Esc(p);
    
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);

        if(!QuotesDouble && t->tt==TOKEN_SINGLE_QUOTE){
            QuotesSingle = !QuotesSingle;
            if(QuotesSingle) LastIndex = i;
            else{
                int size = p->size;
                Parser_ConcatKillBorders(p,TOKEN_CONSTSTRING_SINGLE,LastIndex,i);
                i -= size - p->size;

                LastIndex = -1;
            }
        }else if(!QuotesSingle && t->tt==TOKEN_DOUBLE_QUOTE){
            QuotesDouble = !QuotesDouble;
            if(QuotesDouble) LastIndex = i;
            else{
                int size = p->size;
                Parser_ConcatKillBorders(p,TOKEN_CONSTSTRING_DOUBLE,LastIndex,i);
                i -= size - p->size;

                LastIndex = -1;
            }
        }else if(QuotesSingle){
            
        }else if(QuotesDouble){
            
        }
    
    }

    Parser_TF_Num(p);
}
void Parser_TF_Std(Parser* p){
    int LastIndex = -1;
    TT_Bool QuotesSingle = TT_False;
    TT_Bool QuotesDouble = TT_False;

    Parser_TF_Esc(p);
    
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);

        if(!QuotesDouble && t->tt==TOKEN_SINGLE_QUOTE){
            QuotesSingle = !QuotesSingle;
            if(QuotesSingle) LastIndex = i;
            else{
                int size = p->size;
                Parser_ConcatKillBorders(p,TOKEN_CONSTSTRING_SINGLE,LastIndex,i);
                i -= size - p->size;

                LastIndex = -1;
            }
        }else if(!QuotesSingle && t->tt==TOKEN_DOUBLE_QUOTE){
            QuotesDouble = !QuotesDouble;
            if(QuotesDouble) LastIndex = i;
            else{
                int size = p->size;
                Parser_ConcatKillBorders(p,TOKEN_CONSTSTRING_DOUBLE,LastIndex,i);
                i -= size - p->size;

                LastIndex = -1;
            }
        }else if(QuotesSingle){
            
        }else if(QuotesDouble){
            
        }else if(t->tt==TOKEN_SPACE){
            Token_Free(t);
            Vector_Remove(p,i);
            i--;
        }
    }

    //Parser_TF_Num(p);
}
void Parser_TF_KW(Parser* p,KeywordMap* rm){
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);
        
        if(t->tt==TOKEN_STRING){
            for(int j = 0;j<rm->size;j++){
                KeywordRP* rp = (KeywordRP*)Vector_Get(rm,j);
                if(CStr_Cmp(t->str,rp->cstr)){
                    t->tt = rp->rep;
                    break;
                }
            }
        }
    }
}
void Parser_TF_OPS(Parser* p,OperatorMap* om,ReseterMap* rm){
    Vector OpStack = Vector_New(sizeof(TT_Type));
    TT_Count Args = 0; 
    
    //left out: Depth -> using own ops to create new onces
    for(int i = 0;i<p->size;i++){
        Token* t = Parser_Token(p,i);

        Vector_Push(&OpStack,&t->tt);
        if(OperatorMap_isOP(om,&OpStack,Args)){
        }else if(OpStack.size>1){
            Vector_PopTop(&OpStack);
            if(OpStack.size==1){
                int Start = i-1;
                Token* conststr = Parser_Token(p,Start);

                TT_Iter RPS = OperatorMap_getOPRP(om,&OpStack,Args);
                conststr->tt = ((OperatorRP*)Vector_Get(om,RPS))->rep;
            }else{ // >1
                int Start = i-OpStack.size; // [ Start;End [      + + + +
                int End = i;
                
                int size = p->size;
                Parser_Concat(p,((OperatorRP*)Vector_Get(om,OperatorMap_getOPRP(om,&OpStack,Args)))->rep,Start,End);
                i -= size - p->size;
            }
            Vector_Clear(&OpStack);
            i--;

            Args = 0;
        }else{
            Vector_PopTop(&OpStack);
            if(rm) ReseterMap_Do(rm,t->tt,&Args);
        }

        if(i==p->size-1){
            if(OpStack.size>0){
                if(OperatorMap_isOP(om,&OpStack,Args)){
                    if(OpStack.size==1){
                        int Start = i;
                        Token* conststr = Parser_Token(p,Start);
                        conststr->tt = ((OperatorRP*)Vector_Get(om,OperatorMap_getOPRP(om,&OpStack,Args)))->rep;
                    }else{ // >1
                        int Start = i+1-OpStack.size; // [ Start;End [      + + + +
                        int End = i+1;
                        Parser_Concat(p,((OperatorRP*)Vector_Get(om,OperatorMap_getOPRP(om,&OpStack,Args)))->rep,Start,End);
                    }
                }
            }
        }
    }

    Vector_Free(&OpStack);
}
void Parser_TF_PP(Parser* p,DirectiveMap* pm){
    for(int i = 0;i<p->size;i++){
        Token* t = Parser_Token(p,i);

        TT_Iter it = DirectiveMap_Contains(pm,t->tt);
        if(it!=TT_INVALID){
            Directive* d = (Directive*)Vector_Get(pm,it);
            i = d->fn(d,i,p);
        }
    }
}
void Parser_SPLT_FLUSH(Parser* p,InstructionMap* im){
    TT_Iter it_last = 0;
    for(int i = 0;i<p->size;i++){
        Token* t = Parser_Token(p,i);

        TT_Iter it = InstructionMap_Contains(im,t->tt);
        if(it!=TT_INVALID || i==p->size-1){
            Token* last = Parser_Token(p,it_last);
            InstructionMap_PushInst(im,last,i - it_last + 1);
            it_last = i + 1;
        }
    }

    Vector_Clear(p);
}
Vector Parser_Split(Parser* p,TT_Type tt){
    Vector tokmaps = Vector_New(sizeof(TokenMap));

    TokenMap toks = TokenMap_New();
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);
        Token cpy = Token_Cpy(t);

        if(t->tt!=tt) Vector_Push(&toks,&cpy);
        else{
            if(toks.size>0){
                TokenMap vec = Vector_Cpy(&toks);
                Vector_Push(&tokmaps,&vec);
                Vector_Clear(&toks);
            }
        }

        if(i==p->size-1){
            if(toks.size>0){
                TokenMap vec = Vector_Cpy(&toks);
                Vector_Push(&tokmaps,&vec);
                Vector_Clear(&toks);
            }
        }
    }
    TokenMap_Free(&toks);
    
    return tokmaps;
}
Vector Parser_Split_C(Parser* p,DTT_TypeMap* dttm){
    Vector tokmaps = Vector_New(sizeof(TokenMap));
    TokenMap toks = TokenMap_New();

    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);
        Token cpy = Token_Cpy(t);

        for(int j = 0;j<dttm->size;j++){
            DTT_Type dtt = *(DTT_Type*)Vector_Get(dttm,j);

            if(t->tt==dtt.t1){
                if(toks.size>0 || dtt.t2.tt!=TOKEN_NONE){
                    if(dtt.t2.tt!=TOKEN_NONE)
                        Vector_Push(&toks,(Token[]){ Token_Cpy(&dtt.t2) });

                    TokenMap vec = Vector_Cpy(&toks);
                    Vector_Push(&tokmaps,&vec);
                    Vector_Clear(&toks);
                }
                break;
            }
            if(j==dttm->size-1) Vector_Push(&toks,&cpy);
        }

        if(i==p->size-1){
            if(toks.size>0){
                TokenMap vec = Vector_Cpy(&toks);
                Vector_Push(&tokmaps,&vec);
                Vector_Clear(&toks);
            }
        }
    }
    TokenMap_Free(&toks);
    
    return tokmaps;
}
void Parser_Clear(Parser* p){
    for(int i = 0;i<p->size;i++){
        Token* t = (Token*)Vector_Get(p,i);
        Token_Free(t);
    }
    Vector_Clear(p);
}
void Parser_Print(Parser* p){
    printf("-------- Parser ------\n");
    TokenMap_Print(p);
    printf("----------------------\n");
}
void Parser_Free(Parser* p){
    TokenMap_Free(p);
}

#endif // !ALXPARSER_H