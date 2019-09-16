
#ifndef _PICFMT_MANAGER_H
#define _PICFMT_MANAGER_H


#include <pic_operation.h>







int RegisterPicFileParser(PT_PicFileParser ptPicFileParser);

int PicFmtsInit(void);

int JPGParserInit(void);
int BMPParserInit(void);
PT_PicFileParser Parser(char *pcName);
PT_PicFileParser GetParser(PT_FileMap ptFileMap);



#endif /*_PICFMT_MANAGER_H*/



