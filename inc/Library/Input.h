#ifndef INPUT_H
#define INPUT_H

#include "String.h"
#include "AlxTime.h"
#include "Stroke.h"
#include "Thread.h"
#include "Clipboard.h"

#define INPUT_COMBINATION_NONE  0

typedef struct InputCombination{
    States state;
    unsigned char stroke;
} InputCombination;

InputCombination InputCombination_New(States state,unsigned char stroke){
    InputCombination ic;
    ic.state = state;
    ic.stroke = stroke;
    return ic;
}
InputCombination InputCombination_Null(){
    InputCombination ic;
    ic.state = (States){ 0U };
    ic.stroke = 0U;
    return ic;
}


typedef struct InputHandler{
    Vector combinations;
    char (*handler)(void*);
} InputHandler;

InputHandler InputHandler_New(InputCombination* ic,char (*handler)(void*)){
    InputHandler ih;
    ih.combinations = Vector_New(sizeof(InputCombination));

    for(int i = 0;ic[i].stroke!=INPUT_COMBINATION_NONE;i++){
        Vector_Push(&ih.combinations,&ic[i]);
    }

    ih.handler = handler;
    return ih;
}
void InputHandler_Free(InputHandler* ih){
    Vector_Free(&ih->combinations);
}


#define INPUT_MAXLENGTH     0x0000FFFF 
#define INPUT_GAP_FAKTOR    1.2f
#define INPUT_BLOCKCHAR     19

typedef struct Input{
    String Buffer;
    int MaxLength;
    char Enabled;
    int MaxLine;
    short LastKey;
    short LastChar;
    short CharBefore;
    short KeyBefore;
    int Curser;
    int CurserEnd;
    char Again;
    char changed;
    Timepoint FirstPress;
    Timepoint LastTime;
    States Strokes[MAX_STROKES];
    Vector Handlers;
} Input;

Input Input_New(int MaxLength,int MaxLines){
    Input In;
    In.Buffer = String_New();
    In.MaxLength = MaxLength;
    In.Enabled = 0;
    In.MaxLine = MaxLines;
    In.LastKey = 0;
    In.LastChar = 0;
    In.CharBefore = 0;
    In.KeyBefore = 0;
    In.Curser = 0;
    In.CurserEnd = -1;
    In.Again = 0;
    In.changed = 0;
    In.FirstPress = 0;
    In.LastTime = 0;
    In.Handlers = Vector_New(sizeof(InputHandler));
    return In;
}
States Input_Stroke(Input* In,short Key){
    return In->Strokes[Key];
}

float Input_CurserX(Input* In,int Curser){
    float CurserX = 0;
    for(int i = Curser-1;i>=0;i--){
        if(String_Get(&In->Buffer,i)=='\n' || i==0){
            CurserX = Curser - i - 1;
            if(i==0) CurserX++;
            break;
        }
    }
    return CurserX;
}
float Input_CurserY(Input* In,int Curser){
    float Lines = 0;
    for(int i = 0;i<Curser;i++){
        if(String_Get(&In->Buffer,i)=='\n'){
            Lines++;
        }
    }
    return Lines;
}
int Input_UpCurser(Input* In,int Curser){
    int CurserX = 0;
    for(int i = Curser;i>=0;i--){
        if(String_Get(&In->Buffer,i)=='\n'){
            CurserX = Curser - i;
            break;
        }
    }
    int TopX = 0;
    for(int i = Curser-CurserX-1;i>=0;i--){
        if(String_Get(&In->Buffer,i)=='\n' || i==0){
            TopX = (Curser - CurserX) - i;
            if(CurserX<TopX)    Curser = i + CurserX;
            else                Curser = i + TopX;
            if(i==0) Curser--;
            break;
        }
    }
    return Curser;
}
int Input_DownCurser(Input* In,int Curser){
    int CurserX = 0;
    for(int i = Curser-1;i>=0;i--){
        if(String_Get(&In->Buffer,i)=='\n' || i==0){
            CurserX = Curser - i;
            if(i==0) CurserX++;
            break;
        }
    }
    if(Curser==0) CurserX++;
    
    int BottomX = 0;
    for(int i = Curser;i<=In->Buffer.size;i++){
        if(String_Get(&In->Buffer,i)=='\n' || i==In->Buffer.size){
            BottomX = i;
            break;
        }
    }

    //int TopX = 0;
    for(int i = BottomX+1;i<In->Buffer.size;i++){
        if(String_Get(&In->Buffer,i)=='\n' || i==In->Buffer.size-1){
            if(i-BottomX<CurserX)       Curser = i + 1;
            else                        Curser = BottomX + CurserX;
            if(Curser==In->Buffer.size-1)   Curser = i;
            break;
        }
    }
    return Curser;
}
int Input_GetCurser(Input* In,int x,int y){
    int Lines = 0;
    int Chars = 0;
    for(int i = 0;i<In->Buffer.size;i++){
        if(String_Get(&In->Buffer,i)=='\n'){
            if(Lines==y && Chars<=x){
                return i;
            }
            Lines++;
            Chars = 0;
        }
        if(Lines==y && Chars==x){
            if(Lines==0) i--;
            return i+1;
        }
        Chars++;
    }
    return In->Buffer.size;
}
int Input_FindChar(Input* In,int begin,int end,char stop){
    if(begin == end) return begin;
    
    const int dir = I64_Sign(end - begin);
    int i = begin + dir;
    for(;i!=end && i>=0 && i<In->Buffer.size;i+=dir){
        if(String_Get(&In->Buffer,i) == stop){
            return i - dir;
        }
    }
    if(i < 0)                               return 0;
    if(String_Get(&In->Buffer,i) == stop)   return i - dir;
    return i;
}

void Input_SetText(Input* In,char* Text){
    String_Free(&In->Buffer);
    In->Buffer = String_Make(Text);
    In->Curser = 0;
    In->CurserEnd = -1;
}
void Input_UDKB(Input* In,States Strokes[MAX_STROKES]){
    memcpy(In->Strokes,Strokes,sizeof(States) * MAX_STROKES);

    const char Shift = Strokes[ALX_KEY_SHIFT].DOWN || Strokes[ALX_KEY_R_SHIFT].DOWN;
    const char Ctrl = Strokes[ALX_KEY_CTRL].DOWN || Strokes[ALX_KEY_ALTGR].DOWN;
    const short CharThen = In->LastChar;
    
    if(!In->Strokes[In->LastKey].DOWN){
        In->LastKey = 0;
        In->LastChar = 0; 
    }
    for (int i = 0; i < MAX_STROKES; i++){
		if(In->Strokes[i].PRESSED){
            In->LastKey = i;
            In->LastChar = AlxKey_toAscii(i,Shift,Ctrl);
            if(In->LastChar==0) In->LastChar = 1; 
		}
	}
    if(CharThen!=In->LastChar){
        In->FirstPress = 0;
    }
}
void Input_DefaultReact(Input* In,void* Parent){
    if(In->Enabled){
        //In->changed = 0;

        if(In->Again){
            for(int i = 0;i<In->Handlers.size;i++){
                InputHandler* ih = (InputHandler*)Vector_Get(&In->Handlers,i);
                
                for(int j = 0;j<ih->combinations.size;j++){
                    InputCombination* ic = (InputCombination*)Vector_Get(&ih->combinations,j);
                    
                    States s = Input_Stroke(In,ic->stroke);
                    unsigned char Bits1 = *(unsigned char*)&s;
                    unsigned char Bits2 = *(unsigned char*)&ic->state;
                    if(!(Bits1 & Bits2)) break;

                    if(j==ih->combinations.size-1){
                        char ret = ih->handler(Parent);
                        if(ret) return;
                    }
                }
            }
        }

        if(In->Again && Input_Stroke(In,ALX_KEY_C).DOWN && Input_Stroke(In,ALX_KEY_CTRL).DOWN){
            if(In->CurserEnd>=0){
                int Up = In->Curser<In->CurserEnd?In->Curser:In->CurserEnd;
                int Down = In->Curser>In->CurserEnd?In->Curser:In->CurserEnd;

                char* cstr = String_SubCStr(&In->Buffer,Up,Down-Up+1);
                int size = CStr_Size(cstr) + 1;

                Clipboard_Cpy(cstr,size);

                free(cstr);
                In->CurserEnd = -1;
            }
        }else if(In->Again && Input_Stroke(In,ALX_KEY_V).DOWN && Input_Stroke(In,ALX_KEY_CTRL).DOWN){
            char* ret = Clipboard_Paste();
            if(ret) String_Add(&In->Buffer,ret,In->Curser);
            In->CurserEnd = -1;
            In->changed = 1;
        }else if(In->Again && Input_Stroke(In,ALX_KEY_X).DOWN && Input_Stroke(In,ALX_KEY_CTRL).DOWN){
            if(In->CurserEnd>=0){
                int Up = In->Curser<In->CurserEnd?In->Curser:In->CurserEnd;
                int Down = In->Curser>In->CurserEnd?In->Curser:In->CurserEnd;

                char* cstr = String_SubCStr(&In->Buffer,Up,Down-Up+1);
                int size = CStr_Size(cstr) + 1;

                Clipboard_Cpy(cstr,size);

                for(int i = Down;i>=Up;i--){
                    String_Remove(&In->Buffer,i);
                }

                free(cstr);
                In->CurserEnd = -1;
                In->changed = 1;
            }
        }else if(In->Again && (In->LastChar>=32 && In->LastChar<127)){
            if(In->CurserEnd>=0){
                if(In->Curser!=In->CurserEnd){
                    int Up = In->Curser<In->CurserEnd?In->Curser:In->CurserEnd;
                    int Down = In->Curser>In->CurserEnd?In->Curser:In->CurserEnd;
                    
                    int i = Down;
                    for(;i>=Up;i--){
                        if(String_Get(&In->Buffer,i) == INPUT_BLOCKCHAR)
                            break;
                        
                        String_Remove(&In->Buffer,i);
                    }
                    In->Curser = i + 1;
                }
            }else{
                String_AddChar(&In->Buffer,In->LastChar,In->Curser);

                In->Curser++;
                In->CharBefore = In->LastChar;
            }
            In->CurserEnd = -1;
            In->changed = 1;
        }else if(In->Again && Input_Stroke(In,ALX_KEY_BACKSPACE).DOWN){
            if(In->CurserEnd>=0){
                int Up = In->Curser<In->CurserEnd?In->Curser:In->CurserEnd;
                int Down = In->Curser>In->CurserEnd?In->Curser:In->CurserEnd;

                if(Up<0)                    Up = 0;
                if(Up>In->Buffer.size-1)    Up = In->Buffer.size-1;
                if(Down<0)                  Down = 0;
                if(Down>In->Buffer.size-1)  Down = In->Buffer.size-1;

                int i = Down;
                for(;i>=Up;i--){
                    if(String_Get(&In->Buffer,i) == INPUT_BLOCKCHAR)
                        break;
                        
                    String_Remove(&In->Buffer,i);
                }
                In->Curser = i + 1;
            }else{
                In->Curser = (int)F32_Clamp((float)In->Curser,(float)0,(float)In->Buffer.size);
                if(In->Curser>0){
                    if(String_Get(&In->Buffer,In->Curser - 1) != INPUT_BLOCKCHAR){
                        In->Curser--;
                        String_Remove(&In->Buffer,In->Curser);
                    }
                }
                else
                    In->Curser = 0;
            }
            In->CurserEnd = -1;
            In->changed = 1;
        }else if(In->Again && Input_Stroke(In,ALX_KEY_ENTER).DOWN){
            char* cstr = String_CStr(&In->Buffer);
            int Count = CStr_CountOf(cstr,'\n');
            free(cstr);
            if(Count<In->MaxLine-1){
                String_AddChar(&In->Buffer,'\n',In->Curser);
                In->Curser++;
                In->CharBefore = '\n';
                In->CurserEnd = -1;
            }
            In->changed = 1;
        }else if(In->Again && !Input_Stroke(In,ALX_KEY_CTRL).DOWN && Input_Stroke(In,ALX_KEY_LEFT).DOWN){
            if(Input_Stroke(In,ALX_KEY_SHIFT).DOWN && In->CurserEnd==-1) In->CurserEnd = In->Curser;
            In->Curser = Input_FindChar(In,In->Curser,In->Curser - 1,INPUT_BLOCKCHAR);
            
            if(In->Curser<0) In->Curser = 0;
            In->KeyBefore = In->LastKey;
            if(!Input_Stroke(In,ALX_KEY_SHIFT).DOWN) In->CurserEnd = -1;
        }else if(In->Again && !Input_Stroke(In,ALX_KEY_CTRL).DOWN && Input_Stroke(In,ALX_KEY_RIGHT).DOWN){
            if(Input_Stroke(In,ALX_KEY_SHIFT).DOWN && In->CurserEnd==-1) In->CurserEnd = In->Curser;
            In->Curser = Input_FindChar(In,In->Curser,In->Curser + 1,INPUT_BLOCKCHAR);

            if(In->Curser>In->Buffer.size) In->Curser = In->Buffer.size;
            In->KeyBefore = In->LastKey;
            if(!Input_Stroke(In,ALX_KEY_SHIFT).DOWN) In->CurserEnd = -1;
        }else if(In->Again && !Input_Stroke(In,ALX_KEY_CTRL).DOWN && Input_Stroke(In,ALX_KEY_UP).DOWN){
            if(Input_Stroke(In,ALX_KEY_SHIFT).DOWN && In->CurserEnd==-1) In->CurserEnd = In->Curser;
            In->Curser = Input_FindChar(In,In->Curser,Input_UpCurser(In,In->Curser),INPUT_BLOCKCHAR);

            In->KeyBefore = In->LastKey;
            if(!Input_Stroke(In,ALX_KEY_SHIFT).DOWN) In->CurserEnd = -1;
        }else if(In->Again && !Input_Stroke(In,ALX_KEY_CTRL).DOWN && Input_Stroke(In,ALX_KEY_DOWN).DOWN){
            if(Input_Stroke(In,ALX_KEY_SHIFT).DOWN && In->CurserEnd==-1) In->CurserEnd = In->Curser;
            In->Curser = Input_FindChar(In,In->Curser,Input_DownCurser(In,In->Curser),INPUT_BLOCKCHAR);

            In->KeyBefore = In->LastKey;
            if(!Input_Stroke(In,ALX_KEY_SHIFT).DOWN) In->CurserEnd = -1;
        }
    }
}
void Input_Update(Input* In,States* Strokes){
    Input_UDKB(In,Strokes);
    
    In->Again = Time_Nano()-In->LastTime>50000000;
    if(In->FirstPress==0){
        In->FirstPress = Time_Nano();
        In->Again = 1;
    }else if(Time_Nano()-In->FirstPress<500000000){
        In->Again = 0;
    }
    if(In->LastChar==0){//In->LastKey<32
        In->FirstPress = 0;
    }
    if(In->Again){
        In->CharBefore = 0;
        In->KeyBefore = 0;
        In->LastTime = Time_Nano();
    }
    
    In->Curser = (int)F32_Clamp((float)In->Curser,(float)0,(float)In->Buffer.size);
}
void Input_Gets(Input* In,States* strokes,char* Buffer,int MaxCount){
    String_Clear(&In->Buffer);
    Input_Update(In,strokes);
    In->Enabled = 1;
    while(!In->Strokes[ALX_KEY_ENTER].PRESSED){
        Input_Update(In,strokes);
        Thread_Sleep_M(10);
    }
    memcpy(Buffer,In->Buffer.Memory,In->Buffer.size);
}
void Input_Free(Input* In){
    String_Free(&In->Buffer);
    Vector_Free(&In->Handlers);
}

#endif // INPUT_Htb_Image¿6