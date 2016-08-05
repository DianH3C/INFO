/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: 内部数据操作
                包括创建、删除、设置、获取、遍历

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD  

*******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/* standard library */
#include <stdlib.h>
#include <string.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_data.h"

/* struct len*/
#define INFO_CFG_S_LEN 32
/* 获取数据标志 */
BOOL_T info_data_Init = BOOL_FALSE;

/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 判断指定工号的数据是否存在
        Input: IN UINT uiId         工号
       Output: 
       Return: BOOL_T, BOOL_TRUE    存在
                       BOOL_FALSE   不存在
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsExist(IN UINT uiId)
{
    UINT uiLine = INFO_FIRST;
    for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
    {
        if(uiId == alData[uiLine].stCfg.uiId)
        {
            return BOOL_TRUE;
		}
	}

	return BOOL_FALSE;
}

/*****************************************************************************
    Func Name: INFO_data_IsEmpty[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 判断整个数据组织是否为空
        Input: 
       Output: 
       Return: BOOL_T, BOOL_TRUE    数据组织为空
                       BOOL_FALSE   数据组织非空
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsEmpty(VOID)
{
    UINT uiEndLine = INFO_FIRST;
    for(uiEndLine = INFO_DATA_MAX; alData[uiEndLine].stCfg.uiId == INFO_ID_INVALID; uiEndLine --);
	
    if(uiEndLine == INFO_FIRST)
    {
         return BOOL_FALSE;
    }
	else
		return BOOL_TRUE;
     
}

/*****************************************************************************
    Func Name: INFO_data_GetData[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: 获取配置数据
        Input: IN UINT uiId             工号
       Output: OUT INFO_CFG_S *pstCfg   配置数据
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution: 出参仅在返回成功时有效
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    UINT uiLine;
	
	/* 出参初始化为非法值 */    

	 for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine++)
	{
         if(alData[uiLine].stCfg.uiId == uiId)
         {
             pstCfg->enSex = alData[uiLine].stCfg.enSex;
	         strncpy(pstCfg->szName, alData[uiLine].stCfg.szName, sizeof(pstCfg->szName) - 1);		
	         pstCfg->uiAge= alData[uiLine].stCfg.uiAge;
       	     pstCfg->uiHeight= alData[uiLine].stCfg.uiHeight;
	         pstCfg->uiId= alData[uiLine].stCfg.uiId;
			 	return;
		 }
	}
	
	pstCfg->uiId = INFO_ID_INVALID;
	return;
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: 获取第一个有数据工号
        Input: VOID
       Output: 
       Return: UINT, != INFO_ID_INVALID     第一个有数据的工号
                     == INFO_ID_INVALID     未找到
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetFirst(VOID)
{
    if(alData[INFO_FIRST].stCfg.uiId != 0)
    {
	    return alData[INFO_FIRST].stCfg.uiId;
    }
	return INFO_ID_INVALID;
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: 获取下一个有数据工号
        Input: IN UINT uiId                 当前工号
       Output: 
       Return: UINT, != INFO_ID_INVALID     下一个工号
                     == INFO_ID_INVALID     未找到
      Caution: 此接口获取下一个工号不依赖于入参uiId本身是否有数据
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetNext(IN UINT uiId)
{
    UINT uiLine;
	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine++)
	{
	    if(alData[uiLine].stCfg.uiId == uiId)
	    {
	        return alData[uiLine+1].stCfg.uiId;
	    }
	}
    return INFO_ID_INVALID;
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: 模块初始化
        Input: 
       Output: 
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
/**/ULONG INFO_data_Init(VOID)
{
	UINT uiDataLine = INFO_FIRST;	  

    for(uiDataLine = INFO_FIRST; uiDataLine < INFO_DATA_MAX; uiDataLine ++)
    {
        alData[uiDataLine].stCfg.uiId = INFO_ID_INVALID;
    }

	info_data_Init= BOOL_TRUE;

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: 模块退出
        Input: 
       Output: 
       Return: VOID
      Caution: 调用此接口前，必须已经初始化过
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Fini(VOID)
{
    UINT uiLine = INFO_FIRST;

	if(info_data_Init == BOOL_FALSE)
    {
        printf("Forbidden!!!Did not use initial method!");
		return;
    }

	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
    {
        alData[uiLine].stCfg.uiId = INFO_ID_INVALID;
    }

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

