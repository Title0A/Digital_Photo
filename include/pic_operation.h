
#ifndef _PIC_OPERATION_H
#define _PIC_OPERATION_H
#include <file.h>
typedef struct PixelDatas {
	int iWidth;
	int iHeight;
	int iBpp;
	int iLineBytes;
	int iTotalBytes;
	unsigned char *aucPixelDatas;
}T_PixelDatas, *PT_PixelDatas;


typedef struct PicFileParser {
	char *name;
	int (*isSupport)(PT_FileMap ptFileMap);
	int (*GetPixelDatas)(PT_FileMap ptFileMap, PT_PixelDatas ptPixelDatas);
	int (*FreePixelDatas)(PT_PixelDatas ptPixelDatas);
	struct PicFileParser *ptNext;
}T_PicFileParser, *PT_PicFileParser;

#endif /* _PIC_OPERATION_H */

