#ifndef CONSTPARSER_H
#define CONSTPARSER_H

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "Types.h"
#include "CStr.h"
#include "Math.h"
#include "Integer.h"
#include "Float.h"

typedef i64 Number;
typedef f64 Double;
typedef i8  Boolean;

#define NUMBER_PARSE_ERROR  9223372036854775801LL
#define DOUBLE_PARSE_ERROR   1.2E-320
#define BOOL_PARSE_ERROR    -1

#define False       0
#define True        1

#define DOUBLE_UNSIGNED 0
#define DOUBLE_SIGNED   1

i8* Number_Get(Number n){
    if(n==0){
        return (i8*)CStr_Cpy((char*)"0");
    }
    
    i32 Size = 1;
    i32 Minus = 0;
    if(n<0){
        Minus = 1;
        n = -n;
    }
    Size = (i32)log10((f64)n)+1;
    
    i8* out = (i8*)malloc(Size+Minus+1);
    if(!out) return NULL;

    out[Size+Minus] = '\0';
    if(Minus) out[0] = '-';
    
    for(i32 i = Minus+Size-1;i>=Minus;i--){
        i32 dig = n % 10;
        out[i] = (i8)(dig + 48);
        n /= 10;
    }
    return out;
}
i8* Double_Get(Double n,i32 PostDigits){
    Double Abs = F64_Abs(n);
    i8* hole = Number_Get((Number)Abs);
    if(!hole) return NULL;

    Number commaNumber = (Number)((Abs - F64_Floor(Abs)) * (Double)pow(10.0,PostDigits));
    i8* comma = Number_Get(commaNumber);
    i32 holeSize = CStr_Size((char*)hole);
    i32 commaSize = CStr_Size((char*)comma);
    Number errordigits = PostDigits - commaSize;

    i32 Minus = n<0?1:0;
    i8* out = (i8*)malloc(Minus+holeSize+1+errordigits+commaSize+1);
    if(!out) return NULL;
    
    if(Minus) out[0] = '-';
    memcpy(out+Minus,hole,holeSize);
    out[Minus+holeSize] = '.';
    
    for(i32 i = 0;i<errordigits;i++)
        out[Minus+holeSize+1+i] = '0';

    memcpy(out+Minus+holeSize+1+errordigits,comma,commaSize);
    out[Minus+holeSize+1+errordigits+commaSize] = '\0';
    
    free(hole);
    free(comma);
    return out;
}
i8* Boolean_Get(Boolean n){
    if(n==False)    return (i8*)CStr_Cpy((char*)"false");
    if(n==True)     return (i8*)CStr_Cpy((char*)"true");
    else            return (i8*)CStr_Cpy((char*)"None");
}

Number Number_Parse(i8* cstr){
    i32 Size = CStr_Size((char*)cstr);
    Number out = 0;
    if(!Char_Num(cstr[0]) && cstr[0]!='-') return NUMBER_PARSE_ERROR;
    
    i32 StartDigit = 0;
    if(cstr[StartDigit]=='-'){
        StartDigit++;
    }
    if(cstr[StartDigit]=='0'){
        if(Size<=StartDigit+1){
            return 0;
        }else if(cstr[StartDigit+1]=='d'){
            i32 Digits = 0;
            for(i32 i = Size-1;i>=StartDigit+2;i--){
                i8 c = cstr[i];
                if(!Char_Num(c) && c!='_') return NUMBER_PARSE_ERROR;
                if(c!='_'){
                    out += Char_IntofNum(c) * pow(10,Digits);
                    Digits++;
                }
            }
        }else if(cstr[StartDigit+1]=='x'){
            i32 Digits = 0;
            for(i32 i = Size-1;i>=StartDigit+2;i--){
                i8 c = cstr[i];
                if(!Char_Hex(c) && c!='_') return NUMBER_PARSE_ERROR;
                if(c!='_'){
                    out += Char_IntofHex(c) * pow(16,Digits);
                    Digits++;
                }
            }
        }else if(cstr[StartDigit+1]=='o'){
            i32 Digits = 0;
            for(i32 i = Size-1;i>=StartDigit+2;i--){
                i8 c = cstr[i];
                if(!Char_Oct(c) && c!='_') return NUMBER_PARSE_ERROR;
                if(c!='_'){
                    out += Char_IntofOct(c) * pow(8,Digits);
                    Digits++;
                }
            }
        }else if(cstr[StartDigit+1]=='b'){
            i32 Digits = 0;
            for(i32 i = Size-1;i>=StartDigit+2;i--){
                i8 c = cstr[i];
                if(!Char_Bin(c) && c!='_') return NUMBER_PARSE_ERROR;
                if(c!='_'){
                    out += Char_IntofBin(c) * pow(2,Digits);
                    Digits++;
                }
            }
        }else{
            return NUMBER_PARSE_ERROR;
        }
    }else{
        i32 Digits = 0;
        for(i32 i = Size-1;i>=StartDigit;i--){
            i8 c = cstr[i];
            if(!Char_Num(c) && c!='_') return NUMBER_PARSE_ERROR;
            if(c!='_'){
                out += (c-48) * pow(10,Digits);
                Digits++;
            }
        }
    }
    if(StartDigit==1) out = -out;
    return out;
}
Double Double_Parse(i8* cstr,i8 AllowSigned){
    i32 Size = CStr_Size((char*)cstr);
    if(Size<=0) return DOUBLE_PARSE_ERROR;
    i32 Digits = -1;
    i32 Minus = 0;
    if(cstr[0]=='-') Minus = 1;
    if(Minus && !AllowSigned) return DOUBLE_PARSE_ERROR;

    i32 Digs = 0;
    Double out = 0.0;
    for(i32 i = Minus;i<Size;i++){
        i8 c = cstr[i];
        if(Char_Num(c)){
            Double n = (Double)(c - 48);
            out += pow(0.1,Digs-Minus) * n;
            Digs++;
        }else if(c=='.'){
            if(Digits>0)    return DOUBLE_PARSE_ERROR;
            Digits = Digs-Minus;
        }else if(c=='_')    continue;
        else                return DOUBLE_PARSE_ERROR;
    }
    if(Minus>0 && Digits==-1){
        Digits = Digs;
        Digits--;
    }
    else if(Digits==-1) Digits = Digs;
    out *= pow(10.0,Digits-1);
    if(Minus>0)    out *= -1.0;
    return out;
}
Boolean Boolean_Parse(i8* cstr){
    if(CStr_Cmp((char*)cstr,(char*)"false"))  return 0;
    if(CStr_Cmp((char*)cstr,(char*)"true"))   return 1;
    if(CStr_Cmp((char*)cstr,(char*)"False"))  return 0;
    if(CStr_Cmp((char*)cstr,(char*)"True"))   return 1;
    if(CStr_Cmp((char*)cstr,(char*)"FALSE"))  return 0;
    if(CStr_Cmp((char*)cstr,(char*)"TRUE"))   return 1;
    if(CStr_Cmp((char*)cstr,(char*)"0"))      return 0;
    if(CStr_Cmp((char*)cstr,(char*)"1"))      return 1;
    return BOOL_PARSE_ERROR;
}

i8* Float_CStr(f32* n){
    return Double_Get((Double)*n,8);
}
i8* Double_CStr(Double* n){
    return Double_Get(*n,8);
}


// Buffersize = 24 + 2
void I64_Print_D(I64 n,i8* buffer_out){
    if(n==0){
        memcpy(buffer_out,"0",2);
        return;
    }
    
    const i32 buffersize = 24;
    i8 buffer[buffersize];
    memset(buffer,0,sizeof(buffer));

    i8* cstr = buffer + buffersize - 1;
    u64 v = I64_Abs(n);

    i32 digits = 0;
    for(;v>0;digits++){
        i8 dig = (i8)(v % 10);
        *cstr = (i8)(dig + 48);
        
        cstr--;
        v /= 10;
    }

    const i8 minus = (n & (1ULL << 63ULL)) ? 1 : 0;
    const i32 size = minus + digits;

    if(minus) *buffer_out = '-';
    memcpy(buffer_out + minus,buffer + (buffersize - digits),digits);
    buffer_out[size] = '\0';
}
// Buffersize = 24 + 1
void U64_Print_D(U64 n,i8* buffer_out){
    if(n==0){
        memcpy(buffer_out,"0",2);
        return;
    }
    
    const i32 buffersize = 24;
    i8 buffer[buffersize];
    memset(buffer,0,sizeof(buffer));

    i8* cstr = buffer + buffersize - 1;
    u64 v = n;

    i32 digits = 0;
    for(;v>0;digits++){
        i8 dig = (i8)(v % 10);
        *cstr = (i8)(dig + 48);
        
        cstr--;
        v /= 10;
    }

    memcpy(buffer_out,buffer + (buffersize - digits),digits);
    buffer_out[digits] = '\0';
}
// Buffersize = 16 + 3
void U64_Print_X(U64 n,i8* buffer_out){
    if(n==0){
        memcpy(buffer_out,"0x0",4);
        return;
    }
    
    const i32 buffersize = 16;
    i8 buffer[buffersize];
    memset(buffer,0,sizeof(buffer));

    i8* cstr = buffer + buffersize - 1;
    u64 v = I64_Abs(n);

    i32 digits = 0;
    for(;v>0;digits++){
        i8 dig = (i8)(v % 16);
        *cstr = (i8)Char_HexofInt_U(dig);
        
        cstr--;
        v /= 16;
    }

    const i32 size = 2 + digits;

    memcpy(buffer_out + 2,buffer + (buffersize - digits),digits);
    buffer_out[0] = '0';
    buffer_out[1] = 'x';
    buffer_out[size] = '\0';
}
// Buffersize = 22 + 3
void U64_Print_O(U64 n,i8* buffer_out){
    if(n==0){
        memcpy(buffer_out,"0o0",4);
        return;
    }
    
    const i32 buffersize = 22;
    i8 buffer[buffersize];
    memset(buffer,0,sizeof(buffer));

    i8* cstr = buffer + buffersize - 1;
    u64 v = I64_Abs(n);

    i32 digits = 0;
    for(;v>0;digits++){
        i8 dig = (i8)(v % 8);
        *cstr = (i8)Char_HexofInt_U(dig);
        
        cstr--;
        v /= 8;
    }

    const i32 size = 2 + digits;

    memcpy(buffer_out + 2,buffer + (buffersize - digits),digits);
    buffer_out[0] = '0';
    buffer_out[1] = 'o';
    buffer_out[size] = '\0';
}
// Buffersize = 64 + 3
void U64_Print_B(U64 n,i8* buffer_out){
    if(n==0){
        memcpy(buffer_out,"0b0",4);
        return;
    }
    
    const i32 buffersize = 64;
    i8 buffer[buffersize];
    memset(buffer,0,sizeof(buffer));

    i8* cstr = buffer + buffersize - 1;
    u64 v = I64_Abs(n);

    i32 digits = 0;
    for(;v>0;digits++){
        i8 dig = (i8)(v % 2);
        *cstr = (i8)Char_HexofInt_U(dig);
        
        cstr--;
        v /= 2;
    }

    const i32 size = 2 + digits;

    memcpy(buffer_out + 2,buffer + (buffersize - digits),digits);
    buffer_out[0] = '0';
    buffer_out[1] = 'b';
    buffer_out[size] = '\0';
}

I64 I64_Parse_D(i8* s){
    i32 size = CStr_Size((char*)s);
    
    const i8 minus = *s == '-';
    I64 v = 0;

    U64 dec = 1;
    i32 digits = size - 1;
    for(;digits>=minus;digits--){
        i8 inum = Char_IntofNum(s[digits]);
        if(inum<0) return I64_PARSEERROR;
        
        v += dec * inum;
        dec *= 10;
    }

    if(minus) v = -v;
    return v;
}
U64 U64_Parse_D(i8* s){
    i32 size = CStr_Size((char*)s);
    
    U64 v = 0;

    U64 dec = 1;
    i32 digits = size - 1;
    for(;digits>=0;digits--){
        i8 inum = Char_IntofNum(s[digits]);
        if(inum<0) return U64_PARSEERROR;
        
        v += dec * inum;
        dec *= 10;
    }

    return v;
}
U64 U64_Parse_X(i8* s){
    i32 size = CStr_Size((char*)s);
    
    U64 v = 0;

    U64 dec = 1;
    i32 digits = size - 1;
    for(;digits>=0;digits--){
        i8 inum = Char_IntofHex(s[digits]);
        if(inum<0) return U64_PARSEERROR;
        
        v += dec * inum;
        dec *= 16;
    }

    return v;
}
U64 U64_Parse_O(i8* s){
    i32 size = CStr_Size((char*)s);
    
    U64 v = 0;

    U64 dec = 1;
    i32 digits = size - 1;
    for(;digits>=0;digits--){
        i8 inum = Char_IntofOct(s[digits]);
        if(inum<0) return U64_PARSEERROR;
        
        v += dec * inum;
        dec *= 8;
    }

    return v;
}
U64 U64_Parse_B(i8* s){
    i32 size = CStr_Size((char*)s);
    
    U64 v = 0;

    U64 dec = 1;
    i32 digits = size - 1;
    for(;digits>=0;digits--){
        i8 inum = Char_IntofBin(s[digits]);
        if(inum<0) return U64_PARSEERROR;
        
        v += dec * inum;
        dec *= 2;
    }

    return v;
}
I64 I64_Parse(i8* s){
    if(s[0] == '0'){
        if(CStr_Size((char*)s) == 1)    return 0;
        if(s[1] == 'x')                 return (I64)U64_Parse_X(s + 2);
        if(s[1] == 'o')                 return (I64)U64_Parse_O(s + 2);
        if(s[1] == 'b')                 return (I64)U64_Parse_B(s + 2);
        return I64_PARSEERROR;
    }
    return I64_Parse_D(s);
}

i8* I64_Get_D(I64 n){
    i8 buffer_out[26];
    memset(buffer_out,0,sizeof(buffer_out));

    I64_Print_D(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}
i8* U64_Get_D(U64 n){
    i8 buffer_out[25];
    memset(buffer_out,0,sizeof(buffer_out));

    U64_Print_D(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}
i8* U64_Get_X(U64 n){
    i8 buffer_out[19];
    memset(buffer_out,0,sizeof(buffer_out));

    U64_Print_X(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}
i8* U64_Get_O(U64 n){
    i8 buffer_out[25];
    memset(buffer_out,0,sizeof(buffer_out));

    U64_Print_O(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}
i8* U64_Get_B(U64 n){
    i8 buffer_out[67];
    memset(buffer_out,0,sizeof(buffer_out));

    U64_Print_B(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}


//i64 Dec_Expo(u64 Expo){
//    const i64 ret = (i64)Expo - (i64)1023;
//    return ret * 3 / 10 + (ret<0 ? -1 : 0);
//}
i64 Dec_Expo(F64 n){
    if(n == 0.0) return 0;
    const i64 ex = (i64)floor(log10(F64_Abs(n)));
    return ex;
}
u64 Expo_Dec(i64 Dec){
    return (u64)((i64)Dec + 1023);
}

u64 Dec_Of_Mant(u64 Mant,u64 length){
    const u64 target = length >> 2;
    const u64 start = U64_Pow10(target);
    u64 ret = start;
    
    for(i32 i = length - 1;i>=0;i--){
        if(Mant & (1UL << i)){
            const u64 add = start >> (length - i);
            ret += add;
        }
    }
    
    return ret << 1;
}
u64 Dec_Of_F64Mant(F64 n){//log2(10) = 3.321928095 = 10 / 3
    //const void* ptr_n = (void*)&n;
    //const u64 bits = *(u64*)ptr_n;
    //u64 bits_expo = (bits >> 52ULL) & 0b11111111111ULL;
    //i64 ep = Dec_Expo(bits_expo);
    
    i64 ep = Dec_Expo(n);
    F64 newn = F64_Abs(n) / F64_Pow10(ep) * 1.0e18;
    U64 Ipart = (U64)newn;
    return Ipart;
}

// Buffersize = 22 + 5 <
void F64_Print_Dc(F64 n,i8* buffer,i32 Postdigits){
    const void* ptr_n = (void*)&n;
    const u64 bits = *(u64*)ptr_n;
    const u64 bits_sign = (bits >> 63ULL) & 0b1ULL;
    const u64 bits_expo = (bits >> 52ULL) & 0b11111111111ULL;
    const u64 bits_mant = bits & ~(0xFFFULL << 52);
    //const u64 bits_admt = bits | ((1023UL) << 52);

    if(bits_expo == 0b11111111111ULL){
        if(bits_mant == 0ULL){
            if(bits_sign)   memcpy(buffer,"-inf",5);
            else            memcpy(buffer,"inf",4);
        }else{
            if(bits_sign)   memcpy(buffer,"-nan",5);
            else            memcpy(buffer,"nan",4);
        }
        return;
    }

    const i64 ep = Dec_Expo(bits_expo);
    const i64 epdiff = 18 - ep;

    if(epdiff>=0){
        const U64 pre = (I64)F64_Abs(n);
        const U64 pd = U64_Pow10(Postdigits);
        const F64 postd = F64_Abs(n - (I64)n);
        const U64 post = (I64)(postd * pd);

        if(bits_sign) buffer[0] = '-';
        U64_Print_D(pre,buffer + bits_sign);

        const i32 size = CStr_Size((char*)buffer);
        buffer[size] = '.';

        I64 tendigits = 10;
        I64 zerooffset = 0;
        for(;(I64)(postd * tendigits) == 0 && zerooffset < Postdigits;zerooffset++){
            tendigits *= 10;
            *(buffer + zerooffset + size + 1) = '0';
        }

        U64_Print_D(post,buffer + zerooffset + size + 1);
    }else{
        const U64 pre = (I64)(F64_Abs(n) * F64_Pow10(epdiff));

        if(bits_sign) buffer[0] = '-';
        U64_Print_D(pre,buffer + bits_sign);

        const i32 size = CStr_Size((char*)buffer);
        
        for(i32 i = 0;i<-epdiff;i++)
            buffer[size + i] = '0';
        
        buffer[size - epdiff] = '.';
        
        for(i32 i = 0;i<Postdigits;i++)
            buffer[size - epdiff + 1 + i] = '0';
    }
}
// Buffersize = 22 + 5
void F64_Print_Ex(F64 n,i8* buffer){//log10(2) = 0.301029996 = 0.3
    const void* ptr_n = (void*)&n;
    const u64 bits = *(u64*)ptr_n;
    const u64 bits_sign = (bits >> 63ULL) & 0b1ULL;
    const u64 bits_expo = (bits >> 52ULL) & 0b11111111111ULL;
    const u64 bits_mant = bits & ~(0xFFFULL << 52ULL);
    //const u64 bits_admt = bits | ((1023UL) << 52);

    if(bits_expo == 0b11111111111ULL){
        if(bits_mant == 0){
            if(bits_sign)   memcpy(buffer,"-inf",5);
            else            memcpy(buffer,"inf",4);
        }else{
            if(bits_sign)   memcpy(buffer,"-nan",5);
            else            memcpy(buffer,"nan",4);
        }
        return;
    }else if(bits_expo == 0b00000000000ULL){
        memcpy(buffer,"0.0",4);
        return;
    }

    u64 dm = Dec_Of_F64Mant(n);
    //i64 ep = Dec_Expo(bits_expo);
    i64 ep = Dec_Expo(n);
    
    U64_Print_D(dm,buffer + bits_sign + 1);
    
    if(bits_sign) buffer[0] = '-';
    buffer[bits_sign] = buffer[bits_sign + 1];
    buffer[bits_sign + 1] = '.';
    buffer[bits_sign + 20] = 'E';
    
    I64_Print_D(ep,buffer + bits_sign + 21);
}

F64 F64_Parse_Dc(i8* s){
    if(CStr_Cmp((char*)s,"-inf"))  return *(F64*)((u64[]){ 0xFFF0000000000000 });
    if(CStr_Cmp((char*)s,"inf"))   return *(F64*)((u64[]){ 0x7FF0000000000000 });
    if(CStr_Cmp((char*)s,"-nan"))  return *(F64*)((u64[]){ 0xFFF0000000000001 });
    if(CStr_Cmp((char*)s,"nan"))   return *(F64*)((u64[]){ 0x7FF0000000000001 });

    i32 dot = CStr_Find((char*)s,'.');
    const i32 size = CStr_Size((char*)s);
    const i32 sign = s[0] == '-';
    
    if(dot==-1) dot = size;

    i8 buffer[128];
    memcpy(buffer,s,size + 1);

    if(dot<size) buffer[dot] = '\0';

    F64 out = 0.0;
    if(dot - sign > 18){
        const i32 diff = (dot - sign) - 18;
        buffer[18 + sign] = '\0';

        const I64 pre = I64_Parse_D(buffer);
        out = (F64)pre * F64_Pow10(diff);
    }else{
        const I64 pre = I64_Parse_D(buffer);
        out = (F64)pre;

        if(dot<size){
            const I64 target = dot + 1;
            const I64 post = I64_Parse_D(buffer + target);
            const F64 fpost = (F64)post * F64_Pow10(target - size);
            out += F64_Sign(out) * fpost;
        }
    }
    return out;
}
F64 F64_Parse_Ex(i8* s){
    if(CStr_Cmp((char*)s,"-inf"))  return *(F64*)((u64[]){ 0xFFF0000000000000 });
    if(CStr_Cmp((char*)s,"inf"))   return *(F64*)((u64[]){ 0x7FF0000000000000 });
    if(CStr_Cmp((char*)s,"-nan"))  return *(F64*)((u64[]){ 0xFFF0000000000001 });
    if(CStr_Cmp((char*)s,"nan"))   return *(F64*)((u64[]){ 0x7FF0000000000001 });

    i64 exp = CStr_Find((char*)s,'e');
    if(exp<0) exp = CStr_Find((char*)s,'E');
    const i32 size = CStr_Size((char*)s);

    i8 buffer[128];
    memcpy(buffer,s,size + 1);

    if(exp<0)   buffer[size] = '\0';
    else        buffer[exp] = '\0';

    F64 out = F64_Parse_Dc(buffer);
    if(exp>=0){
        const I64 ep = I64_Parse_D(buffer + exp + 1);
        const F64 mul = out * F64_Pow10(ep);
        out = mul;
    }

    return out;
}
F64 F64_Parse(i8* s){
    return F64_Parse_Ex(s);
}

i8* F64_Get_Dc(F64 n){
    i8 buffer_out[128];
    memset(buffer_out,0,sizeof(buffer_out));

    F64_Print_Dc(n,buffer_out,8);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}
i8* F64_Get_Ex(F64 n){
    i8 buffer_out[128];
    memset(buffer_out,0,sizeof(buffer_out));

    F64_Print_Ex(n,buffer_out);

    i32 size = CStr_Size((char*)buffer_out) + 1;
    i8* out = (i8*)malloc(size);
    memcpy(out,buffer_out,size);
    return out;
}

#endif