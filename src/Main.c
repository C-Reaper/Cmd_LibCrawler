#include "/home/codeleaded/System/Static/Library/LibCrawler.h"

int main(int argc,char* argv[]){
	if(argc != 3 || !Files_isDir(argv[1]) || !Files_isDir(argv[2])){
		printf("[LibCrawler]: Usage: <inc-dir> <src-dir>\n");
	}else{
		LibCrawler lc = LibCrawler_New(argv[1]);
		
		Vec_CStr childs = Files_GetChilds(argv[2]);
		for(int i = 0;i<childs.size;i++){
			CStr file = *(CStr*)Vector_Get(&childs,i);
			CStr type = Files_Type(file);
			
			if(CStr_Cmp(type,"c") || CStr_Cmp(type,"h") || CStr_Cmp(type,"cpp") || CStr_Cmp(type,"hpp")){
				printf("[LibCrawler]: Crawl C-File: %s\n",file);
				LibCrawler_EntryFind(&lc,file);
			}else{
				printf("[LibCrawler]: Ignoring Path: %s\n",file);
			}

			CStr_Free(&type);
		}
		Vec_CStr_Free(&childs);

		LibCrawler_Build(&lc);
		LibCrawler_Free(&lc);
	}
    return 0;
}