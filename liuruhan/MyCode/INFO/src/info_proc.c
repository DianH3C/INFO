/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: 具体功能处理

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
#include <stdio.h>
#include <string.h>
/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_parse.h"
#include "info_data.h"


/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 显示
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:

       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
ULONG INFO_proc_Display(IN const CHAR *pcInputStr)
{
    char *cSex;
	UINT uiLine;
	
    if(INFO_data_IsEmpty())
	{
	    printf("No info!");
		return ERROR_SUCCESS;
	}

	printf("ID:    Age:     Sex:    Height:     Name:");
	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
    { 
        if(alData[uiLine].stCfg.uiId == INFO_ID_INVALID)
        {
             continue;
		}
        if(alData[uiLine].stCfg.enSex == INFO_SEX_FEMALE)
        {
            cSex = "FEMALE";
		}
		else
		{
            cSex = "MALE";
		}
        printf("%u %u %u %s %s",alData[uiLine].stCfg.uiId,
			                     alData[uiLine].stCfg.uiAge,
			                      alData[uiLine].stCfg.uiHeight,
			                       cSex,
			                        alData[uiLine].stCfg.szName); 
	}
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 添加
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Add(IN const CHAR *pcInputStr)
{ 
    UINT uiLine = INFO_FIRST;
	INFO_CFG_S *sNewData;
    INFO_CFG_S *sIsGetData;
	
	INFO_parse_InputStr(pcInputStr,sNewData);
    INFO_data_GetData(sNewData->uiId,sIsGetData);
	
	if(!INFO_ALL_ISVALID(sNewData))
	{
	    return ERROR_INVALID_PARAMETER;
	}

    if(sIsGetData->uiId != 0)
	{
	    return ERROR_ALREADY_EXIST;
    }
    for(uiLine = INFO_DATA_MAX; alData[uiLine].stCfg.uiId == INFO_ID_INVALID; uiLine --);
	
    alData[uiLine].stCfg.enSex = sNewData->enSex;
	strncpy(alData[uiLine].stCfg.szName, sNewData->szName, sizeof(alData[uiLine].stCfg.szName) - 1);		
	alData[uiLine].stCfg.uiAge= sNewData->uiAge;
	alData[uiLine].stCfg.uiHeight= sNewData->uiHeight;
	alData[uiLine].stCfg.uiId= sNewData->uiId;
	
	return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 删除
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Delete(IN const CHAR *pcInputStr)
{
    BOOL_T bFindFLAG = BOOL_FALSE;
    UINT uiLine = INFO_FIRST;
	UINT uiCalLine = INFO_FIRST;
	INFO_CFG_S *sDelData;

	INFO_parse_InputStr(pcInputStr,sDelData);
    if(!INFO_ID_ISVALID(sDelData->uiId))
    {
        return ERROR_ALREADY_EXIST;
	}
	
	while(uiLine < INFO_DATA_MAX)
    {
        if(sDelData->uiId == alData[uiLine++].stCfg.uiId)
        {
            bFindFLAG = BOOL_TRUE;
            alData[uiLine].stCfg.uiId = INFO_ID_INVALID;
		}
		if(alData[uiLine].stCfg.uiId == INFO_ID_INVALID)
		{
            continue;
		}
		uiCalLine ++;
	}

    if( bFindFLAG == BOOL_FALSE)
    {
        return ERROR_INVALID_PARAMETER;
	}

	    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 修改
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution: 支持仅修改部分配置数据
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Modify(IN const CHAR *pcInputStr)
{
    BOOL_T bFindFLAG = BOOL_FALSE;
	UINT uiLine = INFO_FIRST;
	INFO_CFG_S *sModData;

	INFO_parse_InputStr(pcInputStr,sModData);
	if(!INFO_ID_ISVALID(sModData->uiId))
	{
         return ERROR_INVALID_PARAMETER;
	}

	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
	{
        if(alData[uiLine].stCfg.uiId == sModData->uiId)
		{
             if(INFO_AGE_ISVALID(sModData->uiAge))
             {
                 alData[uiLine].stCfg.uiAge = sModData->uiAge;
			 }
			 if(INFO_HEIGHT_ISVALID(sModData->uiHeight))
			 {
                 alData[uiLine].stCfg.uiHeight = sModData->uiHeight;
			 }
			 if(INFO_NAME_ISVALID(sModData->szName))
			 {
			     strncpy(alData[uiLine].stCfg.szName, sModData->szName, sizeof(alData[uiLine].stCfg.szName) - 1);
    		 }
			 if(INFO_SEX_ISVALID(sModData->enSex))
			 {
                 alData[uiLine].stCfg.enSex = sModData->enSex;
			 }
			 bFindFLAG = BOOL_TRUE;
		}
	}

	if(bFindFLAG == BOOL_FALSE)
    {
         return ERROR_NOT_FOUND;
	}
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Exit
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 退出
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Exit(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);
	
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_OpenDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 打开调试开关
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_OpenDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_TRUE);

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_CloseDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 关闭调试开关
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_CloseDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_FALSE);

    return ERROR_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

