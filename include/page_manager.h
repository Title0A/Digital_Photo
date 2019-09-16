

#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include <input_manager.h>
#include <disp_manager.h>
typedef struct PageParams {
	int iPageID;                 /*页面ID*/
	char strCurPictureFile[256]; /* 要处理的第一个图片文件*/
}T_PageParams,*PT_PageParams;



typedef struct PageLayout {	
	int iTopLeftX;        /* 这个区域的左上角、右下角坐标 */
	int iTopLeftY;
	int iBotRightX;
	int iBotRightY;
	int iBpp;
	int iMaxTotalBytes;
	PT_Layout atLayout;
}T_PageLayout, *PT_PageLayout;

typedef struct PageAction {
	char *name;
	void (*Run)(PT_PageParams ptParentPageParms);
	int (*GetInputEvent)(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);
	int (*Prepare)(void);
	struct PageAction *ptNext;
}T_PageAction, *PT_PageAction;
 
typedef struct PageCfg{
	int iIntervalSecond;
	char strSeletedDir[256];

}T_PageCfg,*PT_PageCfg;

//#define ID(name)   (int(name[0]) + int(name[1]) + int(name[2]) + int(name[3]))

int ID(char *strName);
int MainPageInit(void);
int SettingPageInit(void);
int IntervalPageInit(void);
int BrowsePageInit(void);
int AutoPageInit(void);
int ManualPageInit(void);

int RegisterPageAction(PT_PageAction ptPageAction);
int PagesInit(void);
int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem);
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);
PT_PageAction Page(char *pcName);
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd);

void GetIntervalTime(int *piIntervalSecond);
void GetSelectDir(char *strSeletedDir);
void GetPageCfg(PT_PageCfg ptPageCfg);

#endif /* _PAGE_MANAGER_H */

