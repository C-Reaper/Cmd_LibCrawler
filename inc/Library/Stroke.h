#ifndef STROKE_H
#define STROKE_H

typedef unsigned char AlxKey;
#define MOUSE_STROKES      10
#define MAX_STROKES        256

#ifdef __linux__

#define ALX_NONE            0

#define ALX_MOUSE_L         1
#define ALX_MOUSE_M         2
#define ALX_MOUSE_R         3
#define ALX_MOUSE_S_UP      4
#define ALX_MOUSE_S_DOWN    5
#define ALX_MOUSE_SL        6
#define ALX_MOUSE_SR        7
#define ALX_MOUSE_E_FRONT   8
#define ALX_MOUSE_E_BACK    9

#define ALX_KEY_ESC         9
#define ALX_KEY_1           10
#define ALX_KEY_2           11
#define ALX_KEY_3           12
#define ALX_KEY_4           13
#define ALX_KEY_5           14
#define ALX_KEY_6           15
#define ALX_KEY_7           16
#define ALX_KEY_8           17
#define ALX_KEY_9           18
#define ALX_KEY_0           19
#define ALX_KEY_DE_SHARP_S  20
#define ALX_KEY_OTHERQUOTE  21
#define ALX_KEY_BACKSPACE   22
#define ALX_KEY_TAB         23
#define ALX_KEY_Q           24
#define ALX_KEY_W           25
#define ALX_KEY_E           26
#define ALX_KEY_R           27
#define ALX_KEY_T           28
#define ALX_KEY_Z           29
#define ALX_KEY_U           30
#define ALX_KEY_I           31
#define ALX_KEY_O           32
#define ALX_KEY_P           33
#define ALX_KEY_DE_UE       34
#define ALX_KEY_PLUS        35
#define ALX_KEY_ENTER       36
#define ALX_KEY_CTRL        37
#define ALX_KEY_A           38
#define ALX_KEY_S           39
#define ALX_KEY_D           40
#define ALX_KEY_F           41
#define ALX_KEY_G           42
#define ALX_KEY_H           43
#define ALX_KEY_J           44
#define ALX_KEY_K           45
#define ALX_KEY_L           46
#define ALX_KEY_DE_OE       47
#define ALX_KEY_DE_AE       48
#define ALX_KEY_SURCONFLEX  49
#define ALX_KEY_SHIFT       50
#define ALX_KEY_SHARP       51
#define ALX_KEY_Y           52
#define ALX_KEY_X           53
#define ALX_KEY_C           54
#define ALX_KEY_V           55
#define ALX_KEY_B           56
#define ALX_KEY_N           57
#define ALX_KEY_M           58
#define ALX_KEY_COMMA       59
#define ALX_KEY_PERIOD      60
#define ALX_KEY_DASH        61
#define ALX_KEY_R_SHIFT     62
#define ALX_KEY_R_ASTERISK  63
#define ALX_KEY_ALT         64
#define ALX_KEY_SPACE       65
#define ALX_KEY_CAPSLOCK    66

#define ALX_KEY_F1          67
#define ALX_KEY_F2          68
#define ALX_KEY_F3          69
#define ALX_KEY_F4          70
#define ALX_KEY_F5          71
#define ALX_KEY_F6          72
#define ALX_KEY_F7          73
#define ALX_KEY_F8          74
#define ALX_KEY_F9          75
#define ALX_KEY_F10         76

#define ALX_KEY_NUM         77
#define ALX_KEY_ROLLEN      78

#define ALX_KEY_R_7         79
#define ALX_KEY_R_8         80
#define ALX_KEY_R_9         81
#define ALX_KEY_R_DASH      82
#define ALX_KEY_R_4         83
#define ALX_KEY_R_5         84
#define ALX_KEY_R_6         85
#define ALX_KEY_R_PLUS      86
#define ALX_KEY_R_1         87
#define ALX_KEY_R_2         88
#define ALX_KEY_R_3         89
#define ALX_KEY_R_0         90
#define ALX_KEY_R_REMOVE    91

#define ALX_KEY_LESS        94
#define ALX_KEY_F11         95
#define ALX_KEY_F12         96

#define ALX_KEY_R_ENTER     104

#define ALX_KEY_R_SLASH     106
#define ALX_KEY_DRUCK       107
#define ALX_KEY_ALTGR       108

#define ALX_KEY_POS1        110
#define ALX_KEY_UP          111
#define ALX_KEY_BILD_UP     112
#define ALX_KEY_LEFT        113
#define ALX_KEY_RIGHT       114
#define ALX_KEY_END         115
#define ALX_KEY_DOWN        116

#define ALX_KEY_BILD_DOWN   117
#define ALX_KEY_PASTE       118
#define ALX_KEY_REMOVE      119

#define ALX_KEY_PAUSE       127 // Moon symbol
#define ALX_KEY_HOME        133 // windows key
#define ALX_KEY_R_HOME      134 // Moon symbol
#define ALX_KEY_DOC         135 // left key of RCTRL

unsigned char AlxKey_toAscii(AlxKey key,char Shift,char Ctrl){
    if(Shift && Ctrl){

    }else if(Shift){
        switch(key){
            case ALX_KEY_1           : return '!';
            case ALX_KEY_2           : return '\"';
            //case ALX_KEY_3           : return '§';
            case ALX_KEY_4           : return '$';
            case ALX_KEY_5           : return '%';
            case ALX_KEY_6           : return '&';
            case ALX_KEY_7           : return '/';
            case ALX_KEY_8           : return '(';
            case ALX_KEY_9           : return ')';
            case ALX_KEY_0           : return '=';
            case ALX_KEY_DE_SHARP_S  : return '?';
            case ALX_KEY_OTHERQUOTE  : return '`';
            case ALX_KEY_Q           : return 'Q';
            case ALX_KEY_W           : return 'W';
            case ALX_KEY_E           : return 'E';
            case ALX_KEY_R           : return 'R';
            case ALX_KEY_T           : return 'T';
            case ALX_KEY_Z           : return 'Z';
            case ALX_KEY_U           : return 'U';
            case ALX_KEY_I           : return 'I';
            case ALX_KEY_O           : return 'O';
            case ALX_KEY_P           : return 'P';
            case ALX_KEY_PLUS        : return '*';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'A';
            case ALX_KEY_S           : return 'S';
            case ALX_KEY_D           : return 'D';
            case ALX_KEY_F           : return 'F';
            case ALX_KEY_G           : return 'G';
            case ALX_KEY_H           : return 'H';
            case ALX_KEY_J           : return 'J';
            case ALX_KEY_K           : return 'K';
            case ALX_KEY_L           : return 'L';
            //case ALX_KEY_SURCONFLEX  : return '°';
            case ALX_KEY_SHARP       : return '\'';
            case ALX_KEY_Y           : return 'Y';
            case ALX_KEY_X           : return 'X';
            case ALX_KEY_C           : return 'C';
            case ALX_KEY_V           : return 'V';
            case ALX_KEY_B           : return 'B';
            case ALX_KEY_N           : return 'N';
            case ALX_KEY_M           : return 'M';
            case ALX_KEY_COMMA       : return ';';
            case ALX_KEY_PERIOD      : return ':';
            case ALX_KEY_DASH        : return '_';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '>';
            case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else if(Ctrl){
        switch(key){
            //case ALX_KEY_1           : return '¹';
            //case ALX_KEY_2           : return '²';
            //case ALX_KEY_3           : return '³';
            //case ALX_KEY_4           : return '¼';
            //case ALX_KEY_5           : return '½';
            //case ALX_KEY_6           : return '¬';
            case ALX_KEY_7           : return '{';
            case ALX_KEY_8           : return '[';
            case ALX_KEY_9           : return ']';
            case ALX_KEY_0           : return '}';
            case ALX_KEY_DE_SHARP_S  : return '\\';
            //case ALX_KEY_OTHERQUOTE  : return '¸';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return '@';
            //case ALX_KEY_W           : return 'ſ';
            //case ALX_KEY_E           : return '€';
            //case ALX_KEY_R           : return '¶';
            //case ALX_KEY_T           : return 'ŧ';
            //case ALX_KEY_Z           : return '←';
            //case ALX_KEY_U           : return '↓';
            //case ALX_KEY_I           : return '→';
            //case ALX_KEY_O           : return 'ø';
            //case ALX_KEY_P           : return 'þ';
            case ALX_KEY_PLUS        : return '~';
            case ALX_KEY_ENTER       : return '\n';
            //case ALX_KEY_A           : return 'æ';
            //case ALX_KEY_S           : return 'ſ';
            //case ALX_KEY_D           : return 'ð';
            //case ALX_KEY_F           : return 'đ';
            //case ALX_KEY_G           : return 'ŋ';
            //case ALX_KEY_H           : return 'ħ';
            //case ALX_KEY_J           : return '̣̣';
            //case ALX_KEY_K           : return 'ĸ';
            //case ALX_KEY_L           : return 'ł';
            //case ALX_KEY_SURCONFLEX  : return '′';
            //case ALX_KEY_SHARP       : return '’';
            //case ALX_KEY_Y           : return '»';
            //case ALX_KEY_X           : return '«';
            //case ALX_KEY_C           : return '¢';
            //case ALX_KEY_V           : return '„';
            //case ALX_KEY_B           : return '“';
            //case ALX_KEY_N           : return '”';
            //case ALX_KEY_M           : return 'µ';
            //case ALX_KEY_COMMA       : return '·';
            //case ALX_KEY_PERIOD      : return '…';
            //case ALX_KEY_DASH        : return '–';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '|';
            case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else{
        switch(key){
            case ALX_KEY_1           : return '1';
            case ALX_KEY_2           : return '2';
            case ALX_KEY_3           : return '3';
            case ALX_KEY_4           : return '4';
            case ALX_KEY_5           : return '5';
            case ALX_KEY_6           : return '6';
            case ALX_KEY_7           : return '7';
            case ALX_KEY_8           : return '8';
            case ALX_KEY_9           : return '9';
            case ALX_KEY_0           : return '0';
            //case ALX_KEY_DE_SHARP_S  : return 'ß';
            //case ALX_KEY_OTHERQUOTE  : return '´';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return 'q';
            case ALX_KEY_W           : return 'w';
            case ALX_KEY_E           : return 'e';
            case ALX_KEY_R           : return 'r';
            case ALX_KEY_T           : return 't';
            case ALX_KEY_Z           : return 'z';
            case ALX_KEY_U           : return 'u';
            case ALX_KEY_I           : return 'i';
            case ALX_KEY_O           : return 'o';
            case ALX_KEY_P           : return 'p';
            case ALX_KEY_PLUS        : return '+';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'a';
            case ALX_KEY_S           : return 's';
            case ALX_KEY_D           : return 'd';
            case ALX_KEY_F           : return 'f';
            case ALX_KEY_G           : return 'g';
            case ALX_KEY_H           : return 'h';
            case ALX_KEY_J           : return 'j';
            case ALX_KEY_K           : return 'k';
            case ALX_KEY_L           : return 'l';
            case ALX_KEY_SURCONFLEX  : return '^';
            case ALX_KEY_SHARP       : return '#';
            case ALX_KEY_Y           : return 'y';
            case ALX_KEY_X           : return 'x';
            case ALX_KEY_C           : return 'c';
            case ALX_KEY_V           : return 'v';
            case ALX_KEY_B           : return 'b';
            case ALX_KEY_N           : return 'n';
            case ALX_KEY_M           : return 'm';
            case ALX_KEY_COMMA       : return ',';
            case ALX_KEY_PERIOD      : return '.';
            case ALX_KEY_DASH        : return '-';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_7         : return '7';
            case ALX_KEY_R_8         : return '8';
            case ALX_KEY_R_9         : return '9';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_4         : return '4';
            case ALX_KEY_R_5         : return '5';
            case ALX_KEY_R_6         : return '6';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_R_1         : return '1';
            case ALX_KEY_R_2         : return '2';
            case ALX_KEY_R_3         : return '3';
            case ALX_KEY_R_0         : return '0';
            case ALX_KEY_LESS        : return '<';
            case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }
    return 0;
}
AlxKey AlxKey_byAscii(unsigned char ch){
    switch(ch){
        case '1'    : return ALX_KEY_1;
        case '2'    : return ALX_KEY_2;
        case '3'    : return ALX_KEY_3;
        case '4'    : return ALX_KEY_4;
        case '5'    : return ALX_KEY_5;
        case '6'    : return ALX_KEY_6;
        case '7'    : return ALX_KEY_7;
        case '8'    : return ALX_KEY_8;
        case '9'    : return ALX_KEY_9;
        case '0'    : return ALX_KEY_0;
        case '\t'   : return ALX_KEY_TAB;
        case 'Q'    : return ALX_KEY_Q;
        case 'W'    : return ALX_KEY_W;
        case 'E'    : return ALX_KEY_E;
        case 'R'    : return ALX_KEY_R;
        case 'T'    : return ALX_KEY_T;
        case 'Z'    : return ALX_KEY_Z;
        case 'U'    : return ALX_KEY_U;
        case 'I'    : return ALX_KEY_I;
        case 'O'    : return ALX_KEY_O;
        case 'P'    : return ALX_KEY_P;
        case '+'    : return ALX_KEY_PLUS;
        case '\n'   : return ALX_KEY_ENTER;
        case 'A'    : return ALX_KEY_A;
        case 'S'    : return ALX_KEY_S;
        case 'D'    : return ALX_KEY_D;
        case 'F'    : return ALX_KEY_F;
        case 'G'    : return ALX_KEY_G;
        case 'H'    : return ALX_KEY_H;
        case 'J'    : return ALX_KEY_J;
        case 'K'    : return ALX_KEY_K;
        case 'L'    : return ALX_KEY_L;
        case '^'    : return ALX_KEY_SURCONFLEX;
        case '#'    : return ALX_KEY_SHARP;
        case 'Y'    : return ALX_KEY_Y;
        case 'X'    : return ALX_KEY_X;
        case 'C'    : return ALX_KEY_C;
        case 'V'    : return ALX_KEY_V;
        case 'B'    : return ALX_KEY_B;
        case 'N'    : return ALX_KEY_N;
        case 'M'    : return ALX_KEY_M;
        case ','    : return ALX_KEY_COMMA;
        case '.'    : return ALX_KEY_PERIOD;
        case '-'    : return ALX_KEY_DASH;
        case '*'    : return ALX_KEY_R_ASTERISK;
        case ' '    : return ALX_KEY_SPACE;
        case '/'    : return ALX_KEY_R_SLASH;
        case 'q'    : return ALX_KEY_Q;
        case 'w'    : return ALX_KEY_W;
        case 'e'    : return ALX_KEY_E;
        case 'r'    : return ALX_KEY_R;
        case 't'    : return ALX_KEY_T;
        case 'z'    : return ALX_KEY_Z;
        case 'u'    : return ALX_KEY_U;
        case 'i'    : return ALX_KEY_I;
        case 'o'    : return ALX_KEY_O;
        case 'p'    : return ALX_KEY_P;
        case 'a'    : return ALX_KEY_A;
        case 's'    : return ALX_KEY_S;
        case 'd'    : return ALX_KEY_D;
        case 'f'    : return ALX_KEY_F;
        case 'g'    : return ALX_KEY_G;
        case 'h'    : return ALX_KEY_H;
        case 'j'    : return ALX_KEY_J;
        case 'k'    : return ALX_KEY_K;
        case 'l'    : return ALX_KEY_L;
        case 'y'    : return ALX_KEY_Y;
        case 'x'    : return ALX_KEY_X;
        case 'c'    : return ALX_KEY_C;
        case 'v'    : return ALX_KEY_V;
        case 'b'    : return ALX_KEY_B;
        case 'n'    : return ALX_KEY_N;
        case 'm'    : return ALX_KEY_M;
    }
    return 0;
}

#elif defined _WIN64

#define ALX_NONE            0

#define ALX_MOUSE_L         0x01
#define ALX_MOUSE_M         0x04
#define ALX_MOUSE_R         0x02
#define ALX_MOUSE_S_UP      0x05
#define ALX_MOUSE_S_DOWN    0x06
#define ALX_MOUSE_SL        0
#define ALX_MOUSE_SR        0
#define ALX_MOUSE_E_FRONT   0
#define ALX_MOUSE_E_BACK    0

#define ALX_KEY_ESC         0x1B
#define ALX_KEY_1           '1'
#define ALX_KEY_2           '2'
#define ALX_KEY_3           '3'
#define ALX_KEY_4           '4'
#define ALX_KEY_5           '5'
#define ALX_KEY_6           '6'
#define ALX_KEY_7           '7'
#define ALX_KEY_8           '8'
#define ALX_KEY_9           '9'
#define ALX_KEY_0           '0'
#define ALX_KEY_DE_SHARP_S  0xDE
#define ALX_KEY_OTHERQUOTE  0xC0
#define ALX_KEY_BACKSPACE   0x08
#define ALX_KEY_TAB         0x09
#define ALX_KEY_Q           'Q'
#define ALX_KEY_W           'W'
#define ALX_KEY_E           'E'
#define ALX_KEY_R           'R'
#define ALX_KEY_T           'T'
#define ALX_KEY_Z           'Z'
#define ALX_KEY_U           'U'
#define ALX_KEY_I           'I'
#define ALX_KEY_O           'O'
#define ALX_KEY_P           'P'
#define ALX_KEY_DE_UE       0xBA
#define ALX_KEY_PLUS        0xBB
#define ALX_KEY_ENTER       0x0D
#define ALX_KEY_CTRL        0x11
#define ALX_KEY_A           'A'
#define ALX_KEY_S           'S'
#define ALX_KEY_D           'D'
#define ALX_KEY_F           'F'
#define ALX_KEY_G           'G'
#define ALX_KEY_H           'H'
#define ALX_KEY_J           'J'
#define ALX_KEY_K           'K'
#define ALX_KEY_L           'L'
#define ALX_KEY_DE_OE       0xDD
#define ALX_KEY_DE_AE       0xDB
#define ALX_KEY_SURCONFLEX  0xDC
#define ALX_KEY_SHIFT       0x10
#define ALX_KEY_SHARP       0xBA
#define ALX_KEY_Y           'Y'
#define ALX_KEY_X           'X'
#define ALX_KEY_C           'C'
#define ALX_KEY_V           'V'
#define ALX_KEY_B           'B'
#define ALX_KEY_N           'N'
#define ALX_KEY_M           'M'
#define ALX_KEY_COMMA       0xBC
#define ALX_KEY_PERIOD      0xBE
#define ALX_KEY_DASH        0xBD
#define ALX_KEY_R_SHIFT     0xA1
#define ALX_KEY_R_ASTERISK  0x6A
#define ALX_KEY_ALT         0x12
#define ALX_KEY_SPACE       0x20
#define ALX_KEY_CAPSLOCK    0x14

#define ALX_KEY_F1          0x70
#define ALX_KEY_F2          0x71
#define ALX_KEY_F3          0x72
#define ALX_KEY_F4          0x73
#define ALX_KEY_F5          0x74
#define ALX_KEY_F6          0x75
#define ALX_KEY_F7          0x76
#define ALX_KEY_F8          0x77
#define ALX_KEY_F9          0x78
#define ALX_KEY_F10         0x79

#define ALX_KEY_NUM         0x90
#define ALX_KEY_ROLLEN      0

#define ALX_KEY_R_7         0x67
#define ALX_KEY_R_8         0x68
#define ALX_KEY_R_9         0x69
#define ALX_KEY_R_DASH      0xBD
#define ALX_KEY_R_4         0x64
#define ALX_KEY_R_5         0x65
#define ALX_KEY_R_6         0x66
#define ALX_KEY_R_PLUS      0x6B
#define ALX_KEY_R_1         0x61
#define ALX_KEY_R_2         0x62
#define ALX_KEY_R_3         0x63
#define ALX_KEY_R_0         0x60
#define ALX_KEY_R_REMOVE    0x6E

#define ALX_KEY_LESS        0xE2
#define ALX_KEY_F11         0x7A
#define ALX_KEY_F12         0x7B

#define ALX_KEY_R_ENTER     0x0D

#define ALX_KEY_R_SLASH     0x6F
#define ALX_KEY_DRUCK       0x2C
#define ALX_KEY_ALTGR       0xA5

#define ALX_KEY_POS1        0x24
#define ALX_KEY_UP          0x26
#define ALX_KEY_BILD_UP     0x21
#define ALX_KEY_LEFT        0x25
#define ALX_KEY_RIGHT       0x27
#define ALX_KEY_END         0x23
#define ALX_KEY_DOWN        0x28
#define ALX_KEY_BILD_DOWN   0x22
#define ALX_KEY_PASTE       0x2D
#define ALX_KEY_REMOVE      0x2E
#define ALX_KEY_PAUSE       0x13
#define ALX_KEY_HOME        0x5B
#define ALX_KEY_R_HOME      0x5C
#define ALX_KEY_DOC         0xA3

unsigned char AlxKey_toAscii(AlxKey key,char Shift,char Ctrl){
    if(Shift && Ctrl){

    }else if(Shift){
        switch(key){
            case ALX_KEY_1           : return '!';
            case ALX_KEY_2           : return '\"';
            //case ALX_KEY_3           : return '§';
            case ALX_KEY_4           : return '$';
            case ALX_KEY_5           : return '%';
            case ALX_KEY_6           : return '&';
            case ALX_KEY_7           : return '/';
            case ALX_KEY_8           : return '(';
            case ALX_KEY_9           : return ')';
            case ALX_KEY_0           : return '=';
            case ALX_KEY_DE_SHARP_S  : return '?';
            case ALX_KEY_OTHERQUOTE  : return '`';
            case ALX_KEY_Q           : return 'Q';
            case ALX_KEY_W           : return 'W';
            case ALX_KEY_E           : return 'E';
            case ALX_KEY_R           : return 'R';
            case ALX_KEY_T           : return 'T';
            case ALX_KEY_Z           : return 'Z';
            case ALX_KEY_U           : return 'U';
            case ALX_KEY_I           : return 'I';
            case ALX_KEY_O           : return 'O';
            case ALX_KEY_P           : return 'P';
            case ALX_KEY_PLUS        : return '*';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'A';
            case ALX_KEY_S           : return 'S';
            case ALX_KEY_D           : return 'D';
            case ALX_KEY_F           : return 'F';
            case ALX_KEY_G           : return 'G';
            case ALX_KEY_H           : return 'H';
            case ALX_KEY_J           : return 'J';
            case ALX_KEY_K           : return 'K';
            case ALX_KEY_L           : return 'L';
            //case ALX_KEY_SURCONFLEX  : return '°';
            case ALX_KEY_SHARP       : return '\'';
            case ALX_KEY_Y           : return 'Y';
            case ALX_KEY_X           : return 'X';
            case ALX_KEY_C           : return 'C';
            case ALX_KEY_V           : return 'V';
            case ALX_KEY_B           : return 'B';
            case ALX_KEY_N           : return 'N';
            case ALX_KEY_M           : return 'M';
            case ALX_KEY_COMMA       : return ';';
            case ALX_KEY_PERIOD      : return ':';
            case ALX_KEY_DASH        : return '_';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            //case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '>';
            //case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else if(Ctrl){
        switch(key){
            //case ALX_KEY_1           : return '¹';
            //case ALX_KEY_2           : return '²';
            //case ALX_KEY_3           : return '³';
            //case ALX_KEY_4           : return '¼';
            //case ALX_KEY_5           : return '½';
            //case ALX_KEY_6           : return '¬';
            case ALX_KEY_7           : return '{';
            case ALX_KEY_8           : return '[';
            case ALX_KEY_9           : return ']';
            case ALX_KEY_0           : return '}';
            case ALX_KEY_DE_SHARP_S  : return '\\';
            //case ALX_KEY_OTHERQUOTE  : return '¸';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return '@';
            //case ALX_KEY_W           : return 'ſ';
            //case ALX_KEY_E           : return '€';
            //case ALX_KEY_R           : return '¶';
            //case ALX_KEY_T           : return 'ŧ';
            //case ALX_KEY_Z           : return '←';
            //case ALX_KEY_U           : return '↓';
            //case ALX_KEY_I           : return '→';
            //case ALX_KEY_O           : return 'ø';
            //case ALX_KEY_P           : return 'þ';
            case ALX_KEY_PLUS        : return '~';
            case ALX_KEY_ENTER       : return '\n';
            //case ALX_KEY_A           : return 'æ';
            //case ALX_KEY_S           : return 'ſ';
            //case ALX_KEY_D           : return 'ð';
            //case ALX_KEY_F           : return 'đ';
            //case ALX_KEY_G           : return 'ŋ';
            //case ALX_KEY_H           : return 'ħ';
            //case ALX_KEY_J           : return '̣̣';
            //case ALX_KEY_K           : return 'ĸ';
            //case ALX_KEY_L           : return 'ł';
            //case ALX_KEY_SURCONFLEX  : return '′';
            //case ALX_KEY_SHARP       : return '’';
            //case ALX_KEY_Y           : return '»';
            //case ALX_KEY_X           : return '«';
            //case ALX_KEY_C           : return '¢';
            //case ALX_KEY_V           : return '„';
            //case ALX_KEY_B           : return '“';
            //case ALX_KEY_N           : return '”';
            //case ALX_KEY_M           : return 'µ';
            //case ALX_KEY_COMMA       : return '·';
            //case ALX_KEY_PERIOD      : return '…';
            //case ALX_KEY_DASH        : return '–';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '|';
            //case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else{
        switch(key){
            case ALX_KEY_1           : return '1';
            case ALX_KEY_2           : return '2';
            case ALX_KEY_3           : return '3';
            case ALX_KEY_4           : return '4';
            case ALX_KEY_5           : return '5';
            case ALX_KEY_6           : return '6';
            case ALX_KEY_7           : return '7';
            case ALX_KEY_8           : return '8';
            case ALX_KEY_9           : return '9';
            case ALX_KEY_0           : return '0';
            //case ALX_KEY_DE_SHARP_S  : return 'ß';
            //case ALX_KEY_OTHERQUOTE  : return '´';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return 'q';
            case ALX_KEY_W           : return 'w';
            case ALX_KEY_E           : return 'e';
            case ALX_KEY_R           : return 'r';
            case ALX_KEY_T           : return 't';
            case ALX_KEY_Z           : return 'z';
            case ALX_KEY_U           : return 'u';
            case ALX_KEY_I           : return 'i';
            case ALX_KEY_O           : return 'o';
            case ALX_KEY_P           : return 'p';
            case ALX_KEY_PLUS        : return '+';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'a';
            case ALX_KEY_S           : return 's';
            case ALX_KEY_D           : return 'd';
            case ALX_KEY_F           : return 'f';
            case ALX_KEY_G           : return 'g';
            case ALX_KEY_H           : return 'h';
            case ALX_KEY_J           : return 'j';
            case ALX_KEY_K           : return 'k';
            case ALX_KEY_L           : return 'l';
            case ALX_KEY_SURCONFLEX  : return '^';
            case ALX_KEY_SHARP       : return '#';
            case ALX_KEY_Y           : return 'y';
            case ALX_KEY_X           : return 'x';
            case ALX_KEY_C           : return 'c';
            case ALX_KEY_V           : return 'v';
            case ALX_KEY_B           : return 'b';
            case ALX_KEY_N           : return 'n';
            case ALX_KEY_M           : return 'm';
            case ALX_KEY_COMMA       : return ',';
            case ALX_KEY_PERIOD      : return '.';
            case ALX_KEY_DASH        : return '-';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_7         : return '7';
            case ALX_KEY_R_8         : return '8';
            case ALX_KEY_R_9         : return '9';
            //case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_4         : return '4';
            case ALX_KEY_R_5         : return '5';
            case ALX_KEY_R_6         : return '6';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_R_1         : return '1';
            case ALX_KEY_R_2         : return '2';
            case ALX_KEY_R_3         : return '3';
            case ALX_KEY_R_0         : return '0';
            case ALX_KEY_LESS        : return '<';
            //case ALX_KEY_R_ENTER     : return '\n';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }
    return 0;
}
AlxKey AlxKey_byAscii(unsigned char ch){
    switch(ch){
        case '1'    : return ALX_KEY_1;
        case '2'    : return ALX_KEY_2;
        case '3'    : return ALX_KEY_3;
        case '4'    : return ALX_KEY_4;
        case '5'    : return ALX_KEY_5;
        case '6'    : return ALX_KEY_6;
        case '7'    : return ALX_KEY_7;
        case '8'    : return ALX_KEY_8;
        case '9'    : return ALX_KEY_9;
        case '0'    : return ALX_KEY_0;
        case '\t'   : return ALX_KEY_TAB;
        case 'Q'    : return ALX_KEY_Q;
        case 'W'    : return ALX_KEY_W;
        case 'E'    : return ALX_KEY_E;
        case 'R'    : return ALX_KEY_R;
        case 'T'    : return ALX_KEY_T;
        case 'Z'    : return ALX_KEY_Z;
        case 'U'    : return ALX_KEY_U;
        case 'I'    : return ALX_KEY_I;
        case 'O'    : return ALX_KEY_O;
        case 'P'    : return ALX_KEY_P;
        case '+'    : return ALX_KEY_PLUS;
        case '\n'   : return ALX_KEY_ENTER;
        case 'A'    : return ALX_KEY_A;
        case 'S'    : return ALX_KEY_S;
        case 'D'    : return ALX_KEY_D;
        case 'F'    : return ALX_KEY_F;
        case 'G'    : return ALX_KEY_G;
        case 'H'    : return ALX_KEY_H;
        case 'J'    : return ALX_KEY_J;
        case 'K'    : return ALX_KEY_K;
        case 'L'    : return ALX_KEY_L;
        case '^'    : return ALX_KEY_SURCONFLEX;
        case '#'    : return ALX_KEY_SHARP;
        case 'Y'    : return ALX_KEY_Y;
        case 'X'    : return ALX_KEY_X;
        case 'C'    : return ALX_KEY_C;
        case 'V'    : return ALX_KEY_V;
        case 'B'    : return ALX_KEY_B;
        case 'N'    : return ALX_KEY_N;
        case 'M'    : return ALX_KEY_M;
        case ','    : return ALX_KEY_COMMA;
        case '.'    : return ALX_KEY_PERIOD;
        case '-'    : return ALX_KEY_DASH;
        case '*'    : return ALX_KEY_R_ASTERISK;
        case ' '    : return ALX_KEY_SPACE;
        case '/'    : return ALX_KEY_R_SLASH;
        case 'q'    : return ALX_KEY_Q;
        case 'w'    : return ALX_KEY_W;
        case 'e'    : return ALX_KEY_E;
        case 'r'    : return ALX_KEY_R;
        case 't'    : return ALX_KEY_T;
        case 'z'    : return ALX_KEY_Z;
        case 'u'    : return ALX_KEY_U;
        case 'i'    : return ALX_KEY_I;
        case 'o'    : return ALX_KEY_O;
        case 'p'    : return ALX_KEY_P;
        case 'a'    : return ALX_KEY_A;
        case 's'    : return ALX_KEY_S;
        case 'd'    : return ALX_KEY_D;
        case 'f'    : return ALX_KEY_F;
        case 'g'    : return ALX_KEY_G;
        case 'h'    : return ALX_KEY_H;
        case 'j'    : return ALX_KEY_J;
        case 'k'    : return ALX_KEY_K;
        case 'l'    : return ALX_KEY_L;
        case 'y'    : return ALX_KEY_Y;
        case 'x'    : return ALX_KEY_X;
        case 'c'    : return ALX_KEY_C;
        case 'v'    : return ALX_KEY_V;
        case 'b'    : return ALX_KEY_B;
        case 'n'    : return ALX_KEY_N;
        case 'm'    : return ALX_KEY_M;
    }
    return 0;
}

#elif defined __EMSCRIPTEN__

#define ALX_NONE            0

#define ALX_MOUSE_L         1
#define ALX_MOUSE_M         2
#define ALX_MOUSE_R         3
#define ALX_MOUSE_S_UP      4 // side button front
#define ALX_MOUSE_S_DOWN    5 // side button back
#define ALX_MOUSE_SL        6
#define ALX_MOUSE_SR        7
#define ALX_MOUSE_E_FRONT   8
#define ALX_MOUSE_E_BACK    9

#define ALX_KEY_ESC         0x1B
#define ALX_KEY_1           '1'
#define ALX_KEY_2           '2'
#define ALX_KEY_3           '3'
#define ALX_KEY_4           '4'
#define ALX_KEY_5           '5'
#define ALX_KEY_6           '6'
#define ALX_KEY_7           '7'
#define ALX_KEY_8           '8'
#define ALX_KEY_9           '9'
#define ALX_KEY_0           '0'
#define ALX_KEY_DE_SHARP_S  0xDE
#define ALX_KEY_OTHERQUOTE  0xC0
#define ALX_KEY_BACKSPACE   0x08
#define ALX_KEY_TAB         0x09
#define ALX_KEY_Q           'Q'
#define ALX_KEY_W           'W'
#define ALX_KEY_E           'E'
#define ALX_KEY_R           'R'
#define ALX_KEY_T           'T'
#define ALX_KEY_Z           'Z'
#define ALX_KEY_U           'U'
#define ALX_KEY_I           'I'
#define ALX_KEY_O           'O'
#define ALX_KEY_P           'P'
#define ALX_KEY_DE_UE       0xBA
#define ALX_KEY_PLUS        0xBB
#define ALX_KEY_ENTER       0x0D
#define ALX_KEY_CTRL        0x11
#define ALX_KEY_A           'A'
#define ALX_KEY_S           'S'
#define ALX_KEY_D           'D'
#define ALX_KEY_F           'F'
#define ALX_KEY_G           'G'
#define ALX_KEY_H           'H'
#define ALX_KEY_J           'J'
#define ALX_KEY_K           'K'
#define ALX_KEY_L           'L'
#define ALX_KEY_DE_OE       0xDD
#define ALX_KEY_DE_AE       0xDB
#define ALX_KEY_SURCONFLEX  0xDC
#define ALX_KEY_SHIFT       0x10
#define ALX_KEY_SHARP       0xBA
#define ALX_KEY_Y           'Y'
#define ALX_KEY_X           'X'
#define ALX_KEY_C           'C'
#define ALX_KEY_V           'V'
#define ALX_KEY_B           'B'
#define ALX_KEY_N           'N'
#define ALX_KEY_M           'M'
#define ALX_KEY_COMMA       0xBC
#define ALX_KEY_PERIOD      0xBE
#define ALX_KEY_DASH        0xBD
#define ALX_KEY_R_SHIFT     0xA1
#define ALX_KEY_R_ASTERISK  0x6A
#define ALX_KEY_ALT         0x12
#define ALX_KEY_SPACE       0x20
#define ALX_KEY_CAPSLOCK    0x14

#define ALX_KEY_F1          0x70
#define ALX_KEY_F2          0x71
#define ALX_KEY_F3          0x72
#define ALX_KEY_F4          0x73
#define ALX_KEY_F5          0x74
#define ALX_KEY_F6          0x75
#define ALX_KEY_F7          0x76
#define ALX_KEY_F8          0x77
#define ALX_KEY_F9          0x78
#define ALX_KEY_F10         0x79

#define ALX_KEY_NUM         0x90
#define ALX_KEY_ROLLEN      0

#define ALX_KEY_R_7         0x67
#define ALX_KEY_R_8         0x68
#define ALX_KEY_R_9         0x69
#define ALX_KEY_R_DASH      0x6D
#define ALX_KEY_R_4         0x64
#define ALX_KEY_R_5         0x65
#define ALX_KEY_R_6         0x66
#define ALX_KEY_R_PLUS      0x6B
#define ALX_KEY_R_1         0x61
#define ALX_KEY_R_2         0x62
#define ALX_KEY_R_3         0x63
#define ALX_KEY_R_0         0x60
#define ALX_KEY_R_REMOVE    0x6E

#define ALX_KEY_LESS        0xE2
#define ALX_KEY_F11         0x7A
#define ALX_KEY_F12         0x7B

#define ALX_KEY_R_ENTER     0x0D

#define ALX_KEY_R_SLASH     0x6F
#define ALX_KEY_DRUCK       0x2C
#define ALX_KEY_ALTGR       0xA5

#define ALX_KEY_POS1        0x24
#define ALX_KEY_UP          0x26
#define ALX_KEY_BILD_UP     0x21
#define ALX_KEY_LEFT        0x25
#define ALX_KEY_RIGHT       0x27
#define ALX_KEY_END         0x23
#define ALX_KEY_DOWN        0x28
#define ALX_KEY_BILD_DOWN   0x22
#define ALX_KEY_PASTE       0x2D
#define ALX_KEY_REMOVE      0x2E
#define ALX_KEY_PAUSE       0x13
#define ALX_KEY_HOME        0x5B
#define ALX_KEY_R_HOME      0x5C
#define ALX_KEY_DOC         0xA3

unsigned char AlxKey_toAscii(AlxKey key,char Shift,char Ctrl){
    if(Shift && Ctrl){

    }else if(Shift){
        switch(key){
            case ALX_KEY_1           : return '!';
            case ALX_KEY_2           : return '\"';
            case ALX_KEY_4           : return '$';
            case ALX_KEY_5           : return '%';
            case ALX_KEY_6           : return '&';
            case ALX_KEY_7           : return '/';
            case ALX_KEY_8           : return '(';
            case ALX_KEY_9           : return ')';
            case ALX_KEY_0           : return '=';
            case ALX_KEY_DE_SHARP_S  : return '?';
            case ALX_KEY_OTHERQUOTE  : return '`';
            case ALX_KEY_Q           : return 'Q';
            case ALX_KEY_W           : return 'W';
            case ALX_KEY_E           : return 'E';
            case ALX_KEY_R           : return 'R';
            case ALX_KEY_T           : return 'T';
            case ALX_KEY_Z           : return 'Z';
            case ALX_KEY_U           : return 'U';
            case ALX_KEY_I           : return 'I';
            case ALX_KEY_O           : return 'O';
            case ALX_KEY_P           : return 'P';
            case ALX_KEY_PLUS        : return '*';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'A';
            case ALX_KEY_S           : return 'S';
            case ALX_KEY_D           : return 'D';
            case ALX_KEY_F           : return 'F';
            case ALX_KEY_G           : return 'G';
            case ALX_KEY_H           : return 'H';
            case ALX_KEY_J           : return 'J';
            case ALX_KEY_K           : return 'K';
            case ALX_KEY_L           : return 'L';
            case ALX_KEY_SHARP       : return '\'';
            case ALX_KEY_Y           : return 'Y';
            case ALX_KEY_X           : return 'X';
            case ALX_KEY_C           : return 'C';
            case ALX_KEY_V           : return 'V';
            case ALX_KEY_B           : return 'B';
            case ALX_KEY_N           : return 'N';
            case ALX_KEY_M           : return 'M';
            case ALX_KEY_COMMA       : return ';';
            case ALX_KEY_PERIOD      : return ':';
            case ALX_KEY_DASH        : return '_';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '>';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else if(Ctrl){
        switch(key){
            case ALX_KEY_7           : return '{';
            case ALX_KEY_8           : return '[';
            case ALX_KEY_9           : return ']';
            case ALX_KEY_0           : return '}';
            case ALX_KEY_DE_SHARP_S  : return '\\';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return '@';
            case ALX_KEY_PLUS        : return '~';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_DASH      : return '-';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_LESS        : return '|';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }else{
        switch(key){
            case ALX_KEY_1           : return '1';
            case ALX_KEY_2           : return '2';
            case ALX_KEY_3           : return '3';
            case ALX_KEY_4           : return '4';
            case ALX_KEY_5           : return '5';
            case ALX_KEY_6           : return '6';
            case ALX_KEY_7           : return '7';
            case ALX_KEY_8           : return '8';
            case ALX_KEY_9           : return '9';
            case ALX_KEY_0           : return '0';
            case ALX_KEY_TAB         : return '\t';
            case ALX_KEY_Q           : return 'q';
            case ALX_KEY_W           : return 'w';
            case ALX_KEY_E           : return 'e';
            case ALX_KEY_R           : return 'r';
            case ALX_KEY_T           : return 't';
            case ALX_KEY_Z           : return 'z';
            case ALX_KEY_U           : return 'u';
            case ALX_KEY_I           : return 'i';
            case ALX_KEY_O           : return 'o';
            case ALX_KEY_P           : return 'p';
            case ALX_KEY_PLUS        : return '+';
            case ALX_KEY_ENTER       : return '\n';
            case ALX_KEY_A           : return 'a';
            case ALX_KEY_S           : return 's';
            case ALX_KEY_D           : return 'd';
            case ALX_KEY_F           : return 'f';
            case ALX_KEY_G           : return 'g';
            case ALX_KEY_H           : return 'h';
            case ALX_KEY_J           : return 'j';
            case ALX_KEY_K           : return 'k';
            case ALX_KEY_L           : return 'l';
            case ALX_KEY_SURCONFLEX  : return '^';
            case ALX_KEY_SHARP       : return '#';
            case ALX_KEY_Y           : return 'y';
            case ALX_KEY_X           : return 'x';
            case ALX_KEY_C           : return 'c';
            case ALX_KEY_V           : return 'v';
            case ALX_KEY_B           : return 'b';
            case ALX_KEY_N           : return 'n';
            case ALX_KEY_M           : return 'm';
            case ALX_KEY_COMMA       : return ',';
            case ALX_KEY_PERIOD      : return '.';
            case ALX_KEY_DASH        : return '-';
            case ALX_KEY_R_ASTERISK  : return '*';
            case ALX_KEY_SPACE       : return ' ';
            case ALX_KEY_R_7         : return '7';
            case ALX_KEY_R_8         : return '8';
            case ALX_KEY_R_9         : return '9';
            case ALX_KEY_R_4         : return '4';
            case ALX_KEY_R_5         : return '5';
            case ALX_KEY_R_6         : return '6';
            case ALX_KEY_R_PLUS      : return '+';
            case ALX_KEY_R_1         : return '1';
            case ALX_KEY_R_2         : return '2';
            case ALX_KEY_R_3         : return '3';
            case ALX_KEY_R_0         : return '0';
            case ALX_KEY_LESS        : return '<';
            case ALX_KEY_R_SLASH     : return '/';
        }
    }
    return 0;
}

AlxKey AlxKey_byAscii(unsigned char ch){
    switch(ch){
        case '1'    : return ALX_KEY_1;
        case '2'    : return ALX_KEY_2;
        case '3'    : return ALX_KEY_3;
        case '4'    : return ALX_KEY_4;
        case '5'    : return ALX_KEY_5;
        case '6'    : return ALX_KEY_6;
        case '7'    : return ALX_KEY_7;
        case '8'    : return ALX_KEY_8;
        case '9'    : return ALX_KEY_9;
        case '0'    : return ALX_KEY_0;
        case '\t'   : return ALX_KEY_TAB;
        case 'Q'    : return ALX_KEY_Q;
        case 'W'    : return ALX_KEY_W;
        case 'E'    : return ALX_KEY_E;
        case 'R'    : return ALX_KEY_R;
        case 'T'    : return ALX_KEY_T;
        case 'Z'    : return ALX_KEY_Z;
        case 'U'    : return ALX_KEY_U;
        case 'I'    : return ALX_KEY_I;
        case 'O'    : return ALX_KEY_O;
        case 'P'    : return ALX_KEY_P;
        case '+'    : return ALX_KEY_PLUS;
        case '\n'   : return ALX_KEY_ENTER;
        case 'A'    : return ALX_KEY_A;
        case 'S'    : return ALX_KEY_S;
        case 'D'    : return ALX_KEY_D;
        case 'F'    : return ALX_KEY_F;
        case 'G'    : return ALX_KEY_G;
        case 'H'    : return ALX_KEY_H;
        case 'J'    : return ALX_KEY_J;
        case 'K'    : return ALX_KEY_K;
        case 'L'    : return ALX_KEY_L;
        case '^'    : return ALX_KEY_SURCONFLEX;
        case '#'    : return ALX_KEY_SHARP;
        case 'Y'    : return ALX_KEY_Y;
        case 'X'    : return ALX_KEY_X;
        case 'C'    : return ALX_KEY_C;
        case 'V'    : return ALX_KEY_V;
        case 'B'    : return ALX_KEY_B;
        case 'N'    : return ALX_KEY_N;
        case 'M'    : return ALX_KEY_M;
        case ','    : return ALX_KEY_COMMA;
        case '.'    : return ALX_KEY_PERIOD;
        case '-'    : return ALX_KEY_DASH;
        case '*'    : return ALX_KEY_R_ASTERISK;
        case ' '    : return ALX_KEY_SPACE;
        case '/'    : return ALX_KEY_R_SLASH;
        case 'q'    : return ALX_KEY_Q;
        case 'w'    : return ALX_KEY_W;
        case 'e'    : return ALX_KEY_E;
        case 'r'    : return ALX_KEY_R;
        case 't'    : return ALX_KEY_T;
        case 'z'    : return ALX_KEY_Z;
        case 'u'    : return ALX_KEY_U;
        case 'i'    : return ALX_KEY_I;
        case 'o'    : return ALX_KEY_O;
        case 'p'    : return ALX_KEY_P;
        case 'a'    : return ALX_KEY_A;
        case 's'    : return ALX_KEY_S;
        case 'd'    : return ALX_KEY_D;
        case 'f'    : return ALX_KEY_F;
        case 'g'    : return ALX_KEY_G;
        case 'h'    : return ALX_KEY_H;
        case 'j'    : return ALX_KEY_J;
        case 'k'    : return ALX_KEY_K;
        case 'l'    : return ALX_KEY_L;
        case 'y'    : return ALX_KEY_Y;
        case 'x'    : return ALX_KEY_X;
        case 'c'    : return ALX_KEY_C;
        case 'v'    : return ALX_KEY_V;
        case 'b'    : return ALX_KEY_B;
        case 'n'    : return ALX_KEY_N;
        case 'm'    : return ALX_KEY_M;
    }
    return 0;
}

#endif

typedef unsigned char State;

typedef struct States{
    State PADDING       : 2;
    State PRESSED_MARK  : 1;
    State RELEASED_MARK : 1;
    State REPEAT        : 1;
    State PRESSED       : 1;
    State RELEASED      : 1;
    State DOWN          : 1;
} States;

#endif // !STROKE_H