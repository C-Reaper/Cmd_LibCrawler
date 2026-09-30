#ifndef FILES_H
#define FILES_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "String.h"

#if defined __linux__
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <fcntl.h>
#elif defined _WINE || _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>

static const char* Explorer_Basename(const char* path){
    const char* p1 = strrchr(path, '/');
    const char* p2 = strrchr(path, '\\');

    const char* p = p1;

    if(p2 && (!p || p2 > p))
        p = p2;

    return p ? p + 1 : path;
}

#include <libgen.h>

#define Explorer_Basename basename

#elif defined __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#error "Plattform not supported!"
#endif

typedef unsigned int FilesSize;
#define FILES_INVALID_SIZE      0xFFFFFFFF

#define FILES_INVALID           0
#define FILES_DIRECTORY         1
#define FILES_FILE              2
#define FILES_UNKOWNTYPE        3
#define FILES_MAX_PATH          4096
#define FILES_BUFFER_SIZE       8192

/*
 * Access environment variables through the files/platform abstraction.
 * getenv is part of the C runtime on Linux, Windows/Wine and Emscripten.
 */
const char* Files_GetEnv(const char* name){
    if(!name || !name[0])
        return NULL;

    return getenv(name);
}

char* Files_cwd() {
    char buffer[FILES_MAX_PATH];
    memset(buffer, 0, sizeof(buffer));

    #if defined(_WIN32)
    if (_getcwd(buffer, sizeof(buffer)) == NULL)
        return NULL;
    #elif defined(__EMSCRIPTEN__)
    return CStr_Cpy("/");
    #elif defined(__linux__)
    if (getcwd(buffer, sizeof(buffer)) == NULL)
        return NULL;
    #else
    #error "Plattform not supported!"
    #endif

    return CStr_Cpy(buffer);
}

void Files_Mkdir(const char* dir){
    #if defined _WINE || _WIN32
    DWORD attr = GetFileAttributesA(dir);

    if (attr == INVALID_FILE_ATTRIBUTES) {
        if (CreateDirectoryA(dir, NULL)) {
            printf("[Files]: Mkdir -> Dir '%s' created!\n", dir);
        } else {
            printf("[Files]: Mkdir -> Error '%s': %lu\n",dir,GetLastError());
        }
    } else if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        printf("[Files]: Mkdir -> Dir '%s' exists already!\n",dir);
    } else {
        printf("[Files]: Mkdir -> Path '%s' exists but is not a directory!\n",dir);
    }

    #elif defined __linux__
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        if (mkdir(dir, 0755) == 0) {
            printf("[Files]: Mkdir -> Dir '%s' created!\n",dir);
        } else {
            printf("[Files]: Mkdir -> Dir '%s' ",dir);
            fflush(stdout);
            perror("Error during creation");
        }
    } else {
        printf("[Files]: Mkdir -> Dir '%s' exists already!\n",dir);
    }

    #elif defined(__EMSCRIPTEN__)
    EM_ASM({
        try { FS.mkdir(UTF8ToString($0)); }
        catch(e){ console.log(e); }
    }, dir);
    printf("[Files]: Mkdir -> '%s' (web)\n", dir);

    #else
    #error "Plattform not supported!"
    #endif
}
void Files_Create(const char* file){
    #if defined _WINE || _WIN32
    DWORD attr = GetFileAttributesA(file);

    if (attr == INVALID_FILE_ATTRIBUTES) {
        HANDLE hFile = CreateFileA(file, GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            printf("[Files]: Create -> File '%s' created!\n", file);
        } else {
            printf("[Files]: Create -> Error %lu\n", GetLastError());
        }
    } else if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        printf("[Files]: Create -> File '%s' exists already!\n", file);
    } else {
        printf("[Files]: Create -> Path exists but is a directory!\n");
    }

    #elif defined __linux__
    struct stat st = {0};

    if (stat(file, &st) == -1) {
        FILE* f = fopen(file, "w");
        if (f) {
            fclose(f);
            printf("[Files]: Create -> File '%s' created!\n", file);
        } else {
            perror("[Files]: Create -> Error during creation!");
        }
    } else if (S_ISREG(st.st_mode)) {
        printf("[Files]: Create -> File '%s' exists already!\n", file);
    } else {
        printf("[Files]: Create -> Path exists but is not a regular file!\n");
    }

    #elif defined(__EMSCRIPTEN__)
    FILE* f = fopen(file, "w");
    if (f) {
        fclose(f);
        printf("[Files]: Create -> '%s' (web)\n", file);
    }

    #else
    #error "Plattform not supported!"
    #endif
}
void Files_Rmdir(const char* dir){
    #if defined _WINE || _WIN32
    DWORD attr = GetFileAttributesA(dir);

    if (attr == INVALID_FILE_ATTRIBUTES) {
        printf("[Files]: Rmdir -> Dir '%s' does not exist!\n", dir);
    } 
    else if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        if (RemoveDirectoryA(dir)) {
            printf("[Files]: Rmdir -> Dir '%s' removed!\n", dir);
        } else {
            printf("[Files]: Rmdir -> Error '%s': %lu\n", dir, GetLastError());
        }
    } 
    else {
        printf("[Files]: Rmdir -> Path '%s' is not a directory!\n", dir);
    }

    #elif defined __linux__
    struct stat st;

    if (stat(dir, &st) == -1) {
        printf("[Files]: Rmdir -> Dir '%s' does not exist!\n", dir);
    }
    else if (S_ISDIR(st.st_mode)) {
        if (rmdir(dir) == 0) {
            printf("[Files]: Rmdir -> Dir '%s' removed!\n", dir);
        } else {
            perror("[Files]: Rmdir -> Error during removal");
        }
    }
    else {
        printf("[Files]: Rmdir -> Path '%s' is not a directory!\n", dir);
    }

    #elif defined(__EMSCRIPTEN__)
    EM_ASM({
        try { FS.rmdir(UTF8ToString($0)); }
        catch(e){ console.log(e); }
    }, dir);
    printf("[Files]: Rmdir -> '%s' (web)\n", dir);

    #else
    #error "Plattform not supported!"
    #endif
}
int Files_Rmdir_R(const char *path) {
    #ifdef _WIN32
    // (unverändert)
    struct _finddata_t file;
    intptr_t handle;
    char search_path[1024];
    char fullpath[1024];

    snprintf(search_path, sizeof(search_path), "%s/*", path);
    handle = _findfirst(search_path, &file);
    if (handle == -1) return -1;

    do {
        if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, file.name);

        if (file.attrib & _A_SUBDIR) {
            Files_Rmdir_R(fullpath);
        } else {
            DeleteFile(fullpath);
        }
    } while (_findnext(handle, &file) == 0);

    _findclose(handle);
    return _rmdir(path);

    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: Rmdir_R not supported in Web!\n");
    return -1;

    #else
    // Linux unverändert
    DIR *d = opendir(path);
    struct dirent *entry;
    char fullpath[1024];

    if (!d) return -1;

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                Files_Rmdir_R(fullpath);
            } else {
                unlink(fullpath);
            }
        }
    }

    closedir(d);
    return rmdir(path);
    #endif
}
void Files_Rm(const char* file){
    #if defined _WINE || _WIN32
    DWORD attr = GetFileAttributesA(file);

    if (attr == INVALID_FILE_ATTRIBUTES) {
        printf("[Files]: Rm -> File '%s' does not exist!\n", file);
    }
    else if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        if (DeleteFileA(file)) {
            printf("[Files]: Rm -> File '%s' removed!\n", file);
        } else {
            printf("[Files]: Rm -> Error '%s': %lu\n", file, GetLastError());
        }
    }
    else {
        printf("[Files]: Rm -> Path '%s' is a directory!\n", file);
    }

    #elif defined __linux__
    struct stat st;

    if (stat(file, &st) == -1) {
        printf("[Files]: Rm -> File '%s' does not exist!\n", file);
    }
    else if (S_ISREG(st.st_mode)) {
        if (unlink(file) == 0) {
            printf("[Files]: Rm -> File '%s' removed!\n", file);
        } else {
            perror("[Files]: Rm -> Error during removal");
        }
    }
    else {
        printf("[Files]: Rm -> Path '%s' is not a regular file!\n", file);
    }

    #elif defined(__EMSCRIPTEN__)
    EM_ASM({
        try { FS.unlink(UTF8ToString($0)); }
        catch(e){ console.log(e); }
    }, file);
    printf("[Files]: Rm -> '%s' (web)\n", file);

    #else
    #error "Plattform not supported!"
    #endif
}
void Files_Rm_Path(const char* Path,const char* name){
    char file[128];
    snprintf(
        file,
        sizeof(file),
        "%s/%s",Path,name
    );
    Files_Rm(file);
}
char Files_Remove(char* Path){
    return remove(Path);
}

char Files_getType(char* Path) {
    #if defined(_WIN32)
    struct _stat st;
    if (_stat(Path, &st) != 0)
        return FILES_INVALID;

    if (st.st_mode & _S_IFDIR)
        return FILES_DIRECTORY;
    if (st.st_mode & _S_IFREG)
        return FILES_FILE;

    #elif defined(__EMSCRIPTEN__)
    int exists = EM_ASM_INT({
        try { return FS.analyzePath(UTF8ToString($0)).exists; }
        catch(e){ return 0; }
    }, Path);

    if (!exists) return FILES_INVALID;

    int isDir = EM_ASM_INT({
        try { return FS.analyzePath(UTF8ToString($0)).object.isFolder; }
        catch(e){ return 0; }
    }, Path);

    if (isDir) return FILES_DIRECTORY;
    return FILES_FILE;
    #elif defined(__linux__)
    struct stat st;
    if (stat(Path, &st) != 0)
        return FILES_INVALID;

    if (S_ISDIR(st.st_mode))
        return FILES_DIRECTORY;
    if (S_ISREG(st.st_mode))
        return FILES_FILE;
    #else
    #error "Plattform not supported!"
    #endif

    return FILES_UNKOWNTYPE;
}
char Files_isDir(char* Path){
    return Files_getType(Path)==FILES_DIRECTORY;
}
char Files_isFile(char* Path){
    return Files_getType(Path)==FILES_FILE;
}
char Files_Exists(char* Path) {
    #if defined(_WIN32)
    struct _stat st;
    return _stat(Path, &st) == 0;

    #elif defined(__EMSCRIPTEN__)
    return EM_ASM_INT({
        try { return FS.analyzePath(UTF8ToString($0)).exists; }
        catch(e){ return 0; }
    }, Path);

    #elif defined(__linux__)
    struct stat st;
    return stat(Path, &st) == 0;

    #else
    #error "Plattform not supported!"
    #endif
}

FilesSize Files_Size(const char* Path){
    FILE* f = fopen(Path,"rb");
    if(f){
        fseek(f,0,SEEK_END);
        int Size = ftell(f);
        fseek(f,0,SEEK_SET);
        fclose(f);
        return Size;
    }
    return FILES_INVALID_SIZE;
}
char* Files_Read(const char* Path){
    FILE* f = fopen(Path,"rb");
    if(f){
        FilesSize Size = Files_Size(Path);
        char* Buffer = (char*)malloc(Size+1);
        memset(Buffer,0,Size+1);
        
        size_t bytes = fread(Buffer,sizeof(char),Size,f);
        if(bytes != Size){
            printf("[Files]: Read -> try %u got %u!\n",(unsigned int)Size,(unsigned int)bytes);
        }

        fclose(f);
        return Buffer;
    }
    return NULL;
}
char* Files_ReadB(const char* Path,FilesSize* size){
    FILE* f = fopen(Path,"rb");
    if(f){
        FilesSize Size = Files_Size(Path);
        char* Buffer = (char*)malloc(Size);
        memset(Buffer,0,Size);
        
        size_t bytes = fread(Buffer,sizeof(char),Size,f);
        if(bytes != Size){
            printf("[Files]: Read -> try %u got %u!\n",(unsigned int)Size,(unsigned int)bytes);
        }

        *size = Size;
        fclose(f);
        return Buffer;
    }
    return NULL;
}
char* Files_ReadT(const char* Path){
    FILE* f = fopen(Path,"r");
    if(f){
        FilesSize Size = Files_Size(Path);
        char* Buffer = (char*)malloc(Size+1);
        memset(Buffer,0,Size+1);
        
        size_t bytes = fread(Buffer,sizeof(char),Size,f);
        if(bytes != Size){
            printf("[Files]: Read -> try %u got %u!\n",(unsigned int)Size,(unsigned int)bytes);
        }

        fclose(f);
        return Buffer;
    }
    return NULL;
}
char* Files_ReadTB(const char* Path,FilesSize* size){
    FILE* f = fopen(Path,"r");
    if(f){
        FilesSize Size = Files_Size(Path);
        char* Buffer = (char*)malloc(Size);
        memset(Buffer,0,Size);
        
        size_t bytes = fread(Buffer,sizeof(char),Size,f);
        if(bytes != Size){
            printf("[Files]: Read -> try %u got %u!\n",(unsigned int)Size,(unsigned int)bytes);
        }
        
        *size = Size;
        fclose(f);
        return Buffer;
    }
    return NULL;
}

void Files_Write(const char* Path,void* Data,FilesSize Size){
    FILE* f = fopen(Path,"wb");
    if(f){
        if(Data && Size > 0) fwrite(Data,sizeof(char),Size,f);
        fclose(f);
    }
}
void Files_WriteT(const char* Path,void* Data,FilesSize Size){
    FILE* f = fopen(Path,"w");
    if(f){
        fwrite(Data,sizeof(char),Size,f);
        fclose(f);
    }
}

void Files_Append(const char* Path,void* Data,FilesSize Size){
    FILE* f = fopen(Path,"ab");
    if(f){
        if(Data && Size > 0) fwrite(Data,sizeof(char),Size,f);
        fclose(f);
    }
}
void Files_AppendT(const char* Path,void* Data,FilesSize Size){
    FILE* f = fopen(Path,"a");
    if(f){
        fwrite(Data,sizeof(char),Size,f);
        fclose(f);
    }
}

void Files_Cpy(const char* src,const char* dst){
    FilesSize size;
    char* data = Files_ReadB(src,&size);
    Files_Write(dst,data,size);
    if(data) free(data);
}
void Files_CpyT(const char* src,const char* dst){
    FilesSize size;
    char* data = Files_ReadTB(src,&size);
    Files_WriteT(dst,data,size);
    if(data) free(data);
}

char* Files_NameFull(char* Path){
    return CStr_ChopEndFrom(Path,'/');
}
char* Files_Name(char* Path){
    char* name_type = CStr_ChopEndFrom(Path,'/');
    char* name = CStr_ChopEndTo(name_type,'.');

    if(name_type) free(name_type);
    return name;
}
char* Files_NameStep(char* Path,int step){
    int Size = CStr_Size(Path);
    for(int i = Size-1;i>=0;i--){
        if(Path[i]=='/'){
            step--;
            if(step < 0) return CStr_Cpy_From_To(Path,i + 1,Size);
        }
    }
    return CStr_Cpy(Path);
}

char* Files_Type(char* Path){
    return CStr_ChopEndFrom(Path,'.');
}

char Files_PathEndsWith(char* Path,char c){
    int s = CStr_Size(Path);
    return Path[s-1] == c;
}
char* Files_Path(char* Path){
    return CStr_ChopEndTo(Path,'/');
}
char* Files_UnixPath(char* Path){
    return CStr_ReplaceAll(Path,'/','/');
}
int Files_StepBackPath(Vector* v,int* i,int rm){
    if(*i >= 0){
        CStr spcstr = *(CStr*)Vector_Get(v,*i - 1);

        if(CStr_Cmp(spcstr,"..")){
            rm = Files_StepBackPath(v,i,rm + 1);
            CStr_Free((CStr*)Vector_Get(v,*i));
            Vector_Remove(v,*i);
            (*i)--;
        }else{
            CStr_Free((CStr*)Vector_Get(v,*i));
            Vector_Remove(v,*i);
            (*i)--;
        }
        rm--;
    }
    return rm;
}
char* Files_CompressPath(const char* Path){
    Vector split = CStr_ChopDown(Path,'/');
    String builder = String_New();

    if(Path[0] == '.'){
        CStr cwd = Files_cwd();
        Vector_Add(&split,&cwd,0U);
    }

    for(int i = 0;i<split.size;i++){
        CStr spcstr = *(CStr*)Vector_Get(&split,i);
        
        if(CStr_Cmp(spcstr,".")){
            CStr_Free(&spcstr);
            Vector_Remove(&split,i);
            i--;
        }else if(CStr_Cmp(spcstr,"..")){
            Files_StepBackPath(&split,&i,1);
            CStr_Free((CStr*)Vector_Get(&split,i));
            Vector_Remove(&split,i);
            i--;
        }else if(CStr_Size(spcstr) == 0){
            Vector_Remove(&split,i);
            i--;
        }
    }

    for(int i = 0;i<split.size - 1;i++){
        CStr spcstr = *(CStr*)Vector_Get(&split,i);
        String_Append(&builder,spcstr);
        String_AppendChar(&builder,'/');
        CStr_Free(&spcstr);
    }
    if(split.size > 0){
        CStr spcstr = *(CStr*)Vector_Get(&split,split.size - 1);
        String_Append(&builder,spcstr);
        CStr_Free(&spcstr);
    }
    Vector_Free(&split);
    
    if(Path[0] == '/') String_AddChar(&builder,'/',0U);
    CStr out = String_CStr(&builder);
    String_Free(&builder);
    return out;
}
char* Files_FromPath(char* Path,char* relative){
    String builder = String_Make(Path);
    if (!Files_PathEndsWith(Path,'/')){
        String_AppendChar(&builder,'/');
    }

    String_Append(&builder,relative);
    
    CStr out = String_CStr(&builder);
    String_Free(&builder);

    CStr comp = Files_CompressPath(out);
    CStr_Free(&out);
    return comp;
}

char* Files_DirNameStepBack(char* Path,char* other,int step){
    char* found = Files_NameStep(other,step);
    char* add = Files_FromPath(Path,found);
    CStr_Free(&found);
    return add;
}

void Files_Walk(const char *base_path,void (*fn)(char*)) {
    #if defined(_WIN32)
    char search_path[FILES_MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s/*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search_path, &fd);

    if (h == INVALID_HANDLE_VALUE)
        return;

    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, fd.cFileName);

        fn(full_path);

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: Walk not supported in Web!\n");
    #elif defined(__linux__)
    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

        fn(full_path);
    }

    closedir(dir);
    #else
    #error "Platform not supported!"
    #endif
}
void Files_WalkR(const char *base_path,void (*fn)(char*)) {
    #if defined(_WIN32)
    char search_path[FILES_MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s/*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search_path, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            fn(full_path);
            Files_WalkR(full_path, fn);
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: WalkR not supported in Web!\n");
    #elif defined(__linux__)
    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            fn(full_path);
            Files_WalkR(full_path, fn);
        }
    }

    closedir(dir);
    #else
    #error "Platform not supported!"
    #endif
}
void Files_WalkR_P(void* parent,const char *base_path,void (*fn)(void*,char*)) {
    #if defined(_WIN32)
    char search_path[FILES_MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s/*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search_path, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, fd.cFileName);
        
        fn(parent,full_path);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            Files_WalkR_P(parent,full_path,fn);
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: WalkR_P not supported in Web!\n");
    #elif defined(__linux__)

    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);
        
        fn(parent,full_path);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            Files_WalkR_P(parent,full_path,fn);
        }
    }

    closedir(dir);
    #else
    #error "Platform not supported!"
    #endif
}

Vec_CStr Files_GetChilds(const char *base_path) {
    Vec_CStr childs = Vector_New(sizeof(CStr));

    #if defined(_WIN32)
    char search_path[FILES_MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s/*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search_path, &fd);
    if (h == INVALID_HANDLE_VALUE) return childs;

    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, fd.cFileName);
        
        Vector_Push(&childs,(CStr[]){ CStr_Cpy(full_path) });

        //if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        //    Files_WalkR_P(full_path, fn);
        //}

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: GetChilds not supported in Web!\n");
    #elif defined(__linux__)
    DIR *dir = opendir(base_path);
    if (!dir) return childs;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        char full_path[FILES_MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

        Vector_Push(&childs,(CStr[]){ CStr_Cpy(full_path) });

        //struct stat st;
        //if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        //    
        //}
    }

    closedir(dir);
    #else
    #error "Platform not supported!"
    #endif
    return childs;
}
char Files_HasSub(char *base_path, char *sub_path) {
    #if defined(_WIN32)
    char search_path[FILES_MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s/*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search_path, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;

    do {
        if (!strcmp(fd.cFileName, sub_path)) {
            FindClose(h);
            return 1;
        }
    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return 0;
    #elif defined(__EMSCRIPTEN__)
    printf("[Files]: HasSub not supported in Web!\n");
    return 0;
    #elif defined(__linux__)
    DIR *dir = opendir(base_path);
    if (!dir) return 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, sub_path)) {
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
    #else
    #error "Platform not supported!"
    #endif
}

#endif