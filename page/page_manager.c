#include <config.h>
#include <page_manager.h>
#include <render.h>
#include <string.h>
#include <stdlib.h>


static PT_PageAction g_ptPageActionHead;
/*********************************************************
*��������:RegisterPageAction
*��������:ע��"ҳ��ģ��","ҳ��ģ��"����ҳ����ʾ�ĺ���
*�������:ptPageAction   - �ṹ��ָ��,�ں�"ҳ��ģ��"�Ĳ�������
*�������:��
*����ֵ  :0      - �ɹ�
		 :����ֵ - ʧ��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
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
*��������:ShowPages
*��������:��ʾ��������֧�ֵ�"ҳ��ģ��"
*�������:��
*�������:��
*����ֵ  :��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
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
*��������:Page
*��������:��������ȡ��ָ����"ҳ��ģ��"
*�������:pcName   - ����
*�������:��
*����ֵ  :NULL     - ʧ��,û��ָ����ģ��
		 :��NULL   - "ҳ��ģ��"��PT_PageAction�ṹָ��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
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
*��������:ID
*��������:�����������һ��Ψһ������,��������ʶvideoMem �е���ʾ����
*�������:strName   - ����
*�������:��
*����ֵ  :Ψһһ������
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
*
**********************************************************/
int ID(char *strName)
{
	return (int)(strName[0]) + (int)(strName[1]) + (int)(strName[2]) + (int)(strName[3]);
}
/*********************************************************
*��������:GeneratePage
*��������:��ͼ���ļ��н�����ͼ�����ݲ�����ָ������,�Ӷ�����ҳ������
*�������:ptPageLayout   - �ں����ͼ����ļ�������ʾ����
		 :ptVideoMem     - �����VideoMem�ﹹ��ҳ������
*�������:��
*����ֵ  :0              - �ɹ�
		 :����ֵ         - ʧ��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
*
**********************************************************/

int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem)
{
	T_PixelDatas tOriginIconPixelDatas;
	T_PixelDatas tIconPixelDatas;
	int iError;
	PT_Layout atLayout = ptPageLayout->atLayout;
		
	/* �軭���� */
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
*��������:TimeMSBetween
*��������:����ʱ���ļ��:��λms
*�������:tTimeStart   - ��ʼʱ���
		 :tTimeEnd     - ����ʱ���
*�������:��
*����ֵ  :���,��λms
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
*
**********************************************************/
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd)
{
	int iMS;
	iMS = (tTimeEnd.tv_sec - tTimeStart.tv_sec)*1000 + (tTimeEnd.tv_usec - tTimeStart.tv_usec)/1000;
	return iMS;

}

/*********************************************************
*��������:GenericGetInputEvent
*��������:��ȡ��������,���ж���λ���ĸ�ͼƬ��
*�������:ptPageLayout - �ں����ͼ�����ʾ����
*�������:ptInputEvent - �ں��õ�����������
*����ֵ  :-1           - �������ݲ�λ���κ�һ��ͼ��֮��
		 :����ֵ       - �������������ڵ�ͼ��(ptPageLayout->atLayout�������һ��)
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
*
**********************************************************/
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	T_InputEvent tInputEvent;
	int iRet;
	int i = 0;
	PT_Layout atLayout = ptPageLayout->atLayout;
	
	/* ���ԭʼ�Ĵ��������� 
	 * ���ǵ���input_manager.c�ĺ������˺������õ�ǰ�߷�����
	 * ���������̻߳�����ݺ󣬻��������
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
	
	/* �������� */
	/* ȷ������λ����һ����ť�� */
	while (atLayout[i].strIconName)
	{
		if ((tInputEvent.iX >= atLayout[i].iTopLeftX) && (tInputEvent.iX <= atLayout[i].iBotRightX) && \
			 (tInputEvent.iY >= atLayout[i].iTopLeftY) && (tInputEvent.iY <= atLayout[i].iBotRightY))
		{
			/* �ҵ��˱����еİ�ť */
			return i;
		}
		else
		{
			i++;
		}			
	}

	/* ����û�����ڰ�ť�� */
	return -1;
}

/*********************************************************
*��������:GetPageCfg
*��������:���ҳ������ò���
		 : ������������ҳ��,����Ҫ�õ�2������:�����ĸ�Ŀ¼�µ�ͼƬ,ͼƬ֮��Ĳ��ż��
*�������:��
*�������:ptPageCfg - �ں��õ��Ĳ���
*����ֵ  :��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
*
**********************************************************/
void GetPageCfg(PT_PageCfg ptPageCfg)
{
	GetSelectDir(ptPageCfg->strSeletedDir);
	GetIntervalTime(&ptPageCfg->iIntervalSecond);

}

/*********************************************************
*��������:PagesInit
*��������:���ø���"ҳ��ģ��"�ĳ�ʼ������,����ע������
*�������:��
*�������:��
*����ֵ  :0    - �ɹ�
		  ���� - ʧ��
*�޸�����		�汾��		�޸���		�޸�����
*---------------------------------------------------------
*2019/8          V1.0       �����      ����
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


