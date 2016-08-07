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
#include <stdlib.h>
#include <string.h>
/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info_dbg.h"
#include "info.h"
#include "info_data.h"
#include "info_parse.h"

/* the head&tail point of pstCfg in info_data.c */
extern INFO_CFG_S* g_pstCfghead;

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
    INFO_CFG_S* pstCfgtemp = NULL;
    if(INFO_data_IsEmpty())
    {
        printf("No info\n");
        return ERROR_SUCCESS;
    }
    printf("id     name            sex age height\n");
    for(pstCfgtemp = g_pstCfghead; pstCfgtemp != NULL; pstCfgtemp = pstCfgtemp->pstCfgnext)
    {
        printf("%-7u%-16s%-4u%-4u%u\n", pstCfgtemp->uiId,
                                    pstCfgtemp->szName,
                                    pstCfgtemp->enSex,
                                    pstCfgtemp->uiAge,
                                    pstCfgtemp->uiHeight);
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
    if(g_pstCfghead == NULL)
    {
        g_pstCfghead = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
        if(g_pstCfghead == NULL)
        {
            return ERROR_NO_ENOUGH_RESOURCE;
        }
        INFO_parse_InputStr(pcInputStr, g_pstCfghead);
        if(!INFO_ALL_ISVALID(g_pstCfghead))
        {
            free(g_pstCfghead);
            g_pstCfghead = NULL;
            return ERROR_INVALID_PARAMETER;
        }
        g_pstCfghead->pstCfgnext = NULL;
    }
    else
    {
        /* ask for memory to store the information */
        INFO_CFG_S* pstCfgmem = NULL;
        pstCfgmem = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
        if(pstCfgmem == NULL)
        {
            return ERROR_NO_ENOUGH_RESOURCE;
        }
        INFO_parse_InputStr(pcInputStr, pstCfgmem);
        if(!INFO_ALL_ISVALID(pstCfgmem))
        {
            free(pstCfgmem);
            return ERROR_INVALID_PARAMETER;
        }
        if(INFO_data_IsExist(pstCfgmem->uiId))
        {
            free(pstCfgmem);
            return ERROR_ALREADY_EXIST;
        }
        pstCfgmem->pstCfgnext = NULL;

        /* find the right position to insert the point */
        INFO_CFG_S* pstCfgtemp = g_pstCfghead;
        if(g_pstCfghead->uiId > pstCfgmem->uiId)
        {
            pstCfgmem->pstCfgnext = g_pstCfghead;
            g_pstCfghead = pstCfgmem;
        }
        else
        {
            for( ; pstCfgtemp->pstCfgnext != NULL; pstCfgtemp = pstCfgtemp->pstCfgnext)
            {
                if(pstCfgtemp->pstCfgnext->uiId > pstCfgmem->uiId)
                {
                    pstCfgmem->pstCfgnext = pstCfgtemp->pstCfgnext;
                    pstCfgtemp->pstCfgnext = pstCfgmem;
                    return ERROR_SUCCESS;
                }
            }
            pstCfgtemp->pstCfgnext = pstCfgmem;
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
    UINT uiId;
    INFO_CFG_S* pstCfgtemp = NULL;
    INFO_CFG_S* pstCfgdel = NULL;
    if(sscanf(pcInputStr, "id=%u", &uiId) != 1)
    {
        return ERROR_INVALID_PARAMETER;
    }
    if(!INFO_ID_ISVALID(uiId))
    {
        return ERROR_INVALID_PARAMETER;
    }

    /* info is sorted */
    if(g_pstCfghead->uiId > uiId)
    {
        return ERROR_NOT_FOUND;
    }
    if(g_pstCfghead->uiId == uiId)
    {
        g_pstCfghead = g_pstCfghead->pstCfgnext;
        return ERROR_SUCCESS;
    }
    for(pstCfgtemp = g_pstCfghead; pstCfgtemp->pstCfgnext != NULL; pstCfgtemp = pstCfgtemp->pstCfgnext)
    {
        if(uiId < pstCfgtemp->pstCfgnext->uiId)
        {
            return ERROR_NOT_FOUND;
        }
        if(uiId == pstCfgtemp->pstCfgnext->uiId)
        {
            pstCfgdel = pstCfgtemp->pstCfgnext;
            pstCfgtemp->pstCfgnext = pstCfgtemp->pstCfgnext->pstCfgnext;
            free(pstCfgdel);
            return ERROR_SUCCESS;
        }
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
    INFO_CFG_S* pstCfgtemp = NULL;
    INFO_CFG_S* pstCfgmem = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    if(pstCfgmem == NULL)
    {
        return ERROR_NO_ENOUGH_RESOURCE;
    }
    /* initial */
    pstCfgmem->uiId = 0;
    pstCfgmem->uiHeight = 0;
    pstCfgmem->uiAge = 0;
    pstCfgmem->enSex = 0;
    pstCfgmem->szName[0] = '\0';
    INFO_parse_InputStr(pcInputStr, pstCfgmem);
    if( !INFO_ID_ISVALID(pstCfgmem->uiId) ||
        (!INFO_NAME_ISVALID(pstCfgmem->szName) && pstCfgmem->szName[0] != '\0') ||
        (!INFO_SEX_ISVALID(pstCfgmem->enSex) && pstCfgmem->enSex != 0) || 
        (!INFO_AGE_ISVALID(pstCfgmem->uiAge) && pstCfgmem->uiAge != 0) ||
        (!INFO_HEIGHT_ISVALID(pstCfgmem->uiHeight) && pstCfgmem->uiHeight != 0) )
    {
        free(pstCfgmem);
        return ERROR_INVALID_PARAMETER;
    }

    /* search */
    for(pstCfgtemp = g_pstCfghead; pstCfgtemp != NULL; pstCfgtemp = pstCfgtemp->pstCfgnext)
    {
        if(pstCfgtemp->uiId == pstCfgmem->uiId)
        {
            break;
        }
    }
    if(pstCfgtemp == NULL)
    {
        free(pstCfgmem);
        return ERROR_NOT_FOUND;
    }
    if(INFO_NAME_ISVALID(pstCfgmem->szName))
    {
        strlcpy(pstCfgtemp->szName, pstCfgmem->szName, sizeof(pstCfgtemp->szName));
    }
    if(INFO_SEX_ISVALID(pstCfgmem->enSex))
    {
        pstCfgtemp->enSex = pstCfgmem->enSex;
    }
    if(INFO_AGE_ISVALID(pstCfgmem->uiAge))
    {
        pstCfgtemp->uiAge = pstCfgmem->uiAge;
    }
    if(INFO_HEIGHT_ISVALID(pstCfgmem->uiHeight))
    {
        pstCfgtemp->uiHeight = pstCfgmem->uiHeight;
    }
    free(pstCfgmem);
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

