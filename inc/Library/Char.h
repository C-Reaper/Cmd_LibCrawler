#ifndef CHAR_H
#define CHAR_H

typedef char Char;
typedef short WChar;

char Char_Alpha(char c){
    return ((c>=65 && c<=90) || (c>=97 && c<=122));
}
char Char_Num(char c){
    return (c>=48 && c<=57);
}
char Char_Hex(char c){
    return (c>=48 && c<=57) || (c>=65 && c<=70) || (c>=97 && c<=102);
}
char Char_Oct(char c){
    return (c>=48 && c<=55);
}
char Char_Bin(char c){
    return (c==48 || c==49);
}
char Char_AlphaNum(char c){
    return Char_Alpha(c) || Char_Num(c);
}
char Char_StringPart(char c){
    return Char_AlphaNum(c) || c=='_';
}
char Char_Op(char c){
    return (c=='+' || c=='-' || c=='*' || c=='/' ||
       c=='=' || c=='<' || c=='>' ||
       c=='|' || c=='&' || c=='%' || c=='~' ||
       c=='?' || c=='!' || c=='#' ||
       c==':' || c=='.');
}

char Char_IntofNum(char c){
    if(Char_Num(c)) return c - 48;
    else return -1;
}
char Char_IntofHex(char c){
    if(Char_Num(c))             return c - 48;
    else if(c>=65 && c<=70)     return c - 55;
    else if(c>=97 && c<=102)    return c - 87;
    else                        return -1;
}
char Char_IntofOct(char c){
    if(Char_Oct(c)) return c - 48;
    else return -1;
}
char Char_IntofBin(char c){
    if(Char_Bin(c)) return c - 48;
    else return -1;
}

char Char_NumofInt(char c){
    if(c>=0 && c<10) return c + 48;
    else return 0;
}
char Char_HexofInt_L(char c){
    if(c>=0 && c<10)            return c + 48;
    else if(c>=10 && c<16)      return c + 87;
    else                        return 0;
}
char Char_HexofInt_U(char c){
    if(c>=0 && c<10)            return c + 48;
    else if(c>=10 && c<16)      return c + 55;
    else                        return 0;
}
char Char_OctofInt(char c){
    if(c>=0 && c<8) return c + 48;
    else return 0;
}
char Char_BinofInt(char c){
    if(c>=0 && c<2) return c + 48;
    else return 0;
}

char Char_toUpper(char c){
    if(Char_Alpha(c) && c>=97) c -= (97-65);
    switch(c){
        case '1':{
            c = '!';
            break;
        }
        case '2':{
            c = '\"';
            break;
        }
        case '3':{
            c = ' ';
            break;
        }
        case '4':{
            c = '$';
            break;
        }
        case '5':{
            c = '%';
            break;
        }
        case '6':{
            c = '&';
            break;
        }
        case '7':{
            c = '/';
            break;
        }
        case '8':{
            c = '(';
            break;
        }
        case '9':{
            c = ')';
            break;
        }
        case '0':{
            c = '=';
            break;
        }
        case '+':{
            c = '*';
            break;
        }
        case '#':{
            c = '\'';
            break;
        }
        case '-':{
            c = '_';
            break;
        }
        case '.':{
            c = ':';
            break;
        }
        case ',':{
            c = ';';
            break;
        }
        case '<':{
            c = '>';
            break;
        }
    }
    return c;
}
char Char_Alpha_toUpper(char c){
    if(Char_Alpha(c) && c>=97)
        c -= (97-65);
    return c;
}

#endif // !CHAR_H