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
    char *sex;
	UINT infoLine;
    if(INFO_DATA_LINE == INFO_FIRST)
	{
	    printf("No info!");
		return ERROR_SUCCESS;
	}

	printf("ID:    Age:     Sex:    Height:     Name:");
	for(infoLine = INFO_FIRST; infoLine < INFO_DATA_LINE; infoLine ++)
    { 
        if(alData[infoLine].stCfg.enSex == INFO_SEX_FEMALE)
        {
            sex = "FEMALE";
		}
		else
		{
            sex = "MALE";
		}
        printf("%u %u %u %s %s",alData[infoLine].stCfg.uiId,
			                     alData[infoLine].stCfg.uiAge,
			                      alData[infoLine].stCfg.uiHeight,
			                       sex,
			                        alData[infoLine].stCfg.szName); 
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
    UINT infoLine = INFO_FIRST; 
	INFO_CFG_S newData;

	newData = INFO_parse_InputStr(pcInputStr);
	if(!INFO_ALL_ISVALID(newData))
	{
	    return ERROR_INVALID_PARAMETER;
	}

	for(infoLine = INFO_FIRST; infoLine < INFO_DATA_LINE; infoLine ++)
	{
        if(alData[infoLine].stCfg.uiId == newData.uiId)
        {
            return ERROR_ALREADY_EXIST;
		}
	}
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
    BOOL_T findFLAG = BOOL_FALSE;
	UINT infoLine = INFO_FIRST;
	INFO_CFG_S delData;
	delData = INFO_parse_InputStr(pcInputStr);

	for( infoLine = INFO_FIRST; infoLine < INFO_DATA_LINE; infoLine ++)
    {
        if(delData.uiId == alData[infoLine].stCfg.uiId)
        {
            findFLAG = BOOL_TRUE;
            alData[infoLine].stCfg = NULL;
		}
	}

    if( findFLAG == BOOL_FALSE)
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
    BOOL_T findFLAG = BOOL_FALSE;
	UINT infoLine = INFO_FIRST;
	INFO_CFG_S modData;

	modData = INFO_parse_InputStr(pcInputStr);
	if(!INFO_ID_ISVALID(modData.uiId))
	{
         return ERROR_INVALID_PARAMETER;
	}

	for(infoLine = INFO_FIRST; infoLine < INFO_DATA_LINE; infoLine ++)
	{
        if(alData[infoLine].stCfg.uiId == modData.uiId)
		{
             if(INFO_AGE_ISVALID(modData.uiAge))
             {
                 alData[infoLine].stCfg.uiAge = modData.uiAge;
			 }
			 if(INFO_HEIGHT_ISVALID(modData.uiHeight))
			 {
                 alData[infoLine].stCfg.uiHeight = modData.uiHeight;
			 }
			 if(INFO_NAME_ISVALID(modData.szName))
			 {
                 alData[infoLine].stCfg.szName = modData.szName;
			 }
			 if(INFO_SEX_ISVALID(modData.enSex))
			 {
                 alData[infoLine].stCfg.enSex = modData.enSex;
			 }
			 findFLAG = BOOL_TRUE;
		}
	}

	if(findFLAG == BOOL_FALSE)
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

