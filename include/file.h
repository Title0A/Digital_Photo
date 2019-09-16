
#ifndef _FILE_H
#define _FILE_H
#include <stdio.h>
typedef struct FileMap {
	char strFileName[256];
	//int iFd;
	FILE * tFp;
	int iFileSize;
	unsigned char *pucFileMapMem;
}T_FileMap, *PT_FileMap;

typedef enum {
	FILETYPE_DIR = 0,
	FILETYPE_FILE,
}E_FileType;

typedef struct DirContent {
	char strName[256];
	E_FileType eFileType;
	
}T_DirContent, *PT_DirContent;
int MapFile(PT_FileMap ptFileMap);
void UnMapFile(PT_FileMap ptFileMap);
int GetDirContents(char *strDirName, PT_DirContent **pptDirContents , int *piNumber);
void FreeDirContents(PT_DirContent *aptDirContents, int iNumber);
int GetFilesIndir(char *strDirName , int *piStartNumberToRecord,int *piCurFileNumber, int *piFileCountHaveGet, int iFileCountTotal , char apstrFileNames[][256]);
#endif /* _FILE_H */

