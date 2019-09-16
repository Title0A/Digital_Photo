#include <config.h>
#include <page_manager.h>
#include <render.h>
#include <string.h>
#include <stdlib.h>


static PT_PageAction g_ptPageActionHead;
/*********************************************************
*函数名称:RegisterPageAction
*功能描述:注册"页面模块","页面模块"含有页面显示的函数
*输入参数:ptPageAction   - 结构体指针,内含"页面模块"的操作函数
*输出参数:无
*返回值  :0      - 成功
		 :其他值 - 失败
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
int RegisterPageAction(PT_PageAction ptPageAction)
{
	PT_PageAction ptTmp;

	if (!g_ptPageActionHead)
	{
		g_ptPageActionHead   = ptPageAction;
		ptPageAction->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptPageActionHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptPageAction;
		ptPageAction->ptNext = NULL;
	}

	return 0;
}

/*********************************************************
*函数名称:ShowPages
*功能描述:显示本程序能支持的"页面模块"
*输入参数:无
*输出参数:无
*返回值  :无
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
void ShowPages(void)
{
	int i = 0;
	PT_PageAction ptTmp = g_ptPageActionHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}
/*********************************************************
*函数名称:Page
*功能描述:根据名字取出指定的"页面模块"
*输入参数:pcName   - 名字
*输出参数:无
*返回值  :NULL     - 失败,没有指定的模块
		 :非NULL   - "页面模块"的PT_PageAction结构指针
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
PT_PageAction Page(char *pcName)
{
	PT_PageAction ptTmp = g_ptPageActionHead;
	
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}
/*********************************************************
*函数名称:ID
*功能描述:根据名字算出一个唯一的整数,用它来标识videoMem 中的显示数据
*输入参数:strName   - 名字
*输出参数:无
*返回值  :唯一一个整数
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
int ID(char *strName)
{
	return (int)(strName[0]) + (int)(strName[1]) + (int)(strName[2]) + (int)(strName[3]);
}
/*********************************************************
*函数名称:GeneratePage
*功能描述:从图标文件中解析出图像数据并放在指定区域,从而生成页面数据
*输入参数:ptPageLayout   - 内含多个图标的文件名和显示区域
		 :ptVideoMem     - 在这个VideoMem里构造页面数据
*输出参数:无
*返回值  :0              - 成功
		 :其他值         - 失败
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/

int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem)
{
	T_PixelDatas tOriginIconPixelDatas;
	T_PixelDatas tIconPixelDatas;
	int iError;
	PT_Layout atLayout = ptPageLayout->atLayout;
		
	/* 描画数据 */
	if (ptVideoMem->ePicState != PS_GENERATED)
	{
		ClearVideoMem(ptVideoMem,COLOR_BACKGROUND);

		tIconPixelDatas.iBpp          = ptPageLayout->iBpp;
		tIconPixelDatas.aucPixelDatas = malloc(ptPageLayout->iMaxTotalBytes);
		if (tIconPixelDatas.aucPixelDatas == NULL)
		{
			return -1;
		}

		while (atLayout->strIconName)
		{
			iError = GetPixelDatasForIcon(atLayout->strIconName, &tOriginIconPixelDatas);
			if (iError)
			{
				DBG_PRINTF("GetPixelDatasForIcon %s error!\n", atLayout->strIconName);
				free(tIconPixelDatas.aucPixelDatas);
				return -1;
			}
 			tIconPixelDatas.iHeight = atLayout->iBotRightY - atLayout->iTopLeftY + 1;
			tIconPixelDatas.iWidth  = atLayout->iBotRightX - atLayout->iTopLeftX+ 1;
			tIconPixelDatas.iLineBytes  = tIconPixelDatas.iWidth * tIconPixelDatas.iBpp / 8;
			tIconPixelDatas.iTotalBytes = tIconPixelDatas.iLineBytes * tIconPixelDatas.iHeight;
 			PicZoom(&tOriginIconPixelDatas, &tIconPixelDatas);
 			PicMerge(atLayout->iTopLeftX, atLayout->iTopLeftY, &tIconPixelDatas, &ptVideoMem->tPixelDatas);
 			FreePixelDatasForIcon(&tOriginIconPixelDatas);
 			atLayout++;
		}
		free(tIconPixelDatas.aucPixelDatas);
		ptVideoMem->ePicState = PS_GENERATED;
	}

	return 0;
}
/*********************************************************
*函数名称:TimeMSBetween
*功能描述:两个时间点的间隔:单位ms
*输入参数:tTimeStart   - 起始时间点
		 :tTimeEnd     - 结束时间点
*输出参数:无
*返回值  :间隔,单位ms
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd)
{
	int iMS;
	iMS = (tTimeEnd.tv_sec - tTimeStart.tv_sec)*1000 + (tTimeEnd.tv_usec - tTimeStart.tv_usec)/1000;
	return iMS;

}

/*********************************************************
*函数名称:GenericGetInputEvent
*功能描述:读取输入数据,并判断它位于哪个图片上
*输入参数:ptPageLayout - 内含多个图标的显示区域
*输出参数:ptInputEvent - 内含得到的输入数据
*返回值  :-1           - 输入数据不位于任何一个图标之上
		 :其他值       - 输入数据所落在的图标(ptPageLayout->atLayout数组的哪一项)
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	T_InputEvent tInputEvent;
	int iRet;
	int i = 0;
	PT_Layout atLayout = ptPageLayout->atLayout;
	
	/* 获得原始的触摸屏数据 
	 * 它是调用input_manager.c的函数，此函数会让当前线否休眠
	 * 当触摸屏线程获得数据后，会把它唤醒
	 */
	iRet = GetInputEvent(&tInputEvent);
	if (iRet)
	{
		return -1;
	}

	if (tInputEvent.iType != INPUT_TYPE_TOUCHSCREEN)
	{
		return -1;
	}

	*ptInputEvent = tInputEvent;
	
	/* 处理数据 */
	/* 确定触点位于哪一个按钮上 */
	while (atLayout[i].strIconName)
	{
		if ((tInputEvent.iX >= atLayout[i].iTopLeftX) && (tInputEvent.iX <= atLayout[i].iBotRightX) && \
			 (tInputEvent.iY >= atLayout[i].iTopLeftY) && (tInputEvent.iY <= atLayout[i].iBotRightY))
		{
			/* 找到了被点中的按钮 */
			return i;
		}
		else
		{
			i++;
		}			
	}

	/* 触点没有落在按钮上 */
	return -1;
}

/*********************************************************
*函数名称:GetPageCfg
*功能描述:获得页面的配置参数
		 : 对于连续播放页面,它需要得到2个参数:播放哪个目录下的图片,图片之间的播放间隔
*输入参数:无
*输出参数:ptPageCfg - 内含得到的参数
*返回值  :无
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
void GetPageCfg(PT_PageCfg ptPageCfg)
{
	GetSelectDir(ptPageCfg->strSeletedDir);
	GetIntervalTime(&ptPageCfg->iIntervalSecond);

}

/*********************************************************
*函数名称:PagesInit
*功能描述:调用各个"页面模块"的初始化函数,就是注册它们
*输入参数:无
*输出参数:无
*返回值  :0    - 成功
		  其他 - 失败
*修改日期		版本号		修改人		修改内容
*---------------------------------------------------------
*2019/8          V1.0       李德涛      创建
*
**********************************************************/
int PagesInit(void)
{
	int iError;

	iError = MainPageInit();
	iError |= SettingPageInit();
	iError |= IntervalPageInit();
	iError |= BrowsePageInit();
	iError |= AutoPageInit();
	iError |= ManualPageInit();
	
	return iError;
}


