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
#include <malloc.h>
#include <stdlib.h>
/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_data.h"
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
    ULONG ulErrCode;
    UINT uiId;
    INFO_CFG_S stCfg = {0};

    IGNORE_PARAM(pcInputStr);

    if (BOOL_TRUE == INFO_data_IsEmpty())
    {
        printf("No info.\r\n");
    }
    else
    {
        printf("id \t Name \t Sex \t Age \t Height \r\n");
        uiId = INFO_data_GetFirst();
        ulErrCode = INFO_data_GetData(uiId, &stCfg);
        while (INFO_ID_INVALID != uiId)
        {
            printf("%u \t %s \t ", uiId, stCfg.szName);
            if (INFO_SEX_MALE == stCfg.enSex)
            {
                printf("MALE\t");
            }
            else
            {
                printf("FEMALE\t");
            }
            printf("%u \t %u \r\n", stCfg.uiAge, stCfg.uiHeight);
            uiId = INFO_data_GetNext(uiId);
        }
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
    ULONG ulErrCode;
    INFO_CFG_S stCfg = {0};

    INFO_DATA_S *pNode = INFO_DATA_HEAD;
    INFO_DATA_S *pNewNode = NULL;


    /* 解析输入的字符串并将得到的配置数据存入stCfg中 */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ALL_ISVALID(&stCfg))
    {
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
    {
        /* 该工号已经存在 */
        ulErrCode = ERROR_ALREADY_EXIST;
    }
    else
    {
        /* 将新数据插入的链表当中 */
        if (stCfg.uiId < INFO_data_GetFirst())
        {
            /* 增加的结点工号比头结点小 */
            pNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
            if (NULL == pNewNode)
            {
                ulErrCode = ERROR_FAILED;
            }
            else
            {
                INFO_DATA_HEAD = pNewNode;
                INFO_DATA_HEAD->pNext = pNode;
                INFO_DATA_HEAD->isEmpty = BOOL_FALSE;
                INFO_DATA_HEAD->stCfg = stCfg;
                pNode->pPrior = pNewNode;
                ulErrCode = ERROR_SUCCESS;
            }
        }
        else
        {
            /* 遍历链表，找到新增结点插入的位置，否则直接从末尾添加 */
            while ((BOOL_FALSE == pNode->isEmpty) && (NULL != pNode))
            {
                if ((stCfg.uiId > pNode->stCfg.uiId)
                    && ((NULL == pNode->pNext) || (stCfg.uiId < pNode->pNext->stCfg.uiId)))
                {
                    pNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
                    if (NULL == pNewNode)
                    {
                        ulErrCode = ERROR_FAILED;
                    }
                    else
                    {
                        pNewNode->pPrior = pNode->pPrior;
                        pNewNode->pNext = pNode->pNext;
                        pNewNode->isEmpty = BOOL_FALSE;
                        pNewNode->stCfg = stCfg;
                        pNode->pNext = pNewNode;
                        pNode->pPrior = pNewNode;
                        ulErrCode = ERROR_SUCCESS;
                    }
                }
                else
                {
                    pNode = pNode->pNext;
                }
            }
        }
    }
    return ulErrCode;
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
    ULONG ulErrCode;
    UINT uiId;
    INFO_DATA_S *pNode = INFO_DATA_HEAD;
    INFO_DATA_S *pDelNode;

    if (0 != strncmp(pcInputStr, "id", strlen("id")))
    {
        /* 输入字符串不合法 */
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        sscanf(pcInputStr, "id=%u", &uiId);
        if (INFO_ID_ISVALID(uiId))
        {
            /* 输入的id不合法 */
            ulErrCode = ERROR_INVALID_PARAMETER;
        }
        else if (BOOL_TRUE == INFO_data_IsExist(uiId))
        {
            /* 输入的id存在 */
            /* 定位该id所在的结点 */
            while ((BOOL_FALSE == pNode->isEmpty) && (NULL != pNode))
            {
                if (uiId == pNode->stCfg.uiId)
                {
                    /* 当前结点是头结点 */
                    if (uiId == INFO_DATA_HEAD->stCfg.uiId)
                    {
                        /* 判断头结点后是否还有结点 */
                        if (NULL == INFO_DATA_HEAD->pNext)
                        {
                            /* 清空头结点的内容 */
                            memset(&(pNode->stCfg), 0, sizeof(INFO_CFG_S));
                            pNode->isEmpty = BOOL_TRUE;
                        }
                        else
                        {
                            INFO_DATA_HEAD->pNext->pPrior = NULL;
                            INFO_DATA_HEAD = INFO_DATA_HEAD->pNext;
                            free(pNode);
                        }
                    }
                    else
                    {
                        /* 当前结点不是头结点 */
                        pDelNode = pNode;
                        pNode->pPrior->pNext = pNode->pNext;
                        if (NULL != pNode->pNext)
                        {
                            pNode->pNext->pPrior = pNode->pPrior;
                        }
                        free(pDelNode);
                    }
                }
                else
                {
                    pNode = pNode->pNext;
                }
            }
        }
        else
        {
            /* 输入的id不存在 */
            ulErrCode = ERROR_NOT_FOUND;
        }
    }
        return ulErrCode;
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

    ULONG ulErrCode;
    INFO_CFG_S stCfg = {0};

    /* 解析输入的字符串并将得到的配置数据存入stCfg中 */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ID_ISVALID(stCfg.uiId))
    {
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
        {
            /* 从链表中找到指定id对应的配置数据 */
            ulErrCode = INFO_data_GetData(stCfg.uiId, &(pNode->stCfg));
            if (BOOL_TRUE == INFO_NAME_ISVALID(stCfg.szName))
            {
                strlcpy(pNode->stCfg.szName, stCfg.szName, sizeof(pNode->stCfg.szName));
            }
            if (BOOL_TRUE == INFO_SEX_ISVALID(stCfg->enSex))
            {
                pNode->stCfg.enSex = stCfg->enSex;
            }
            if (BOOL_TRUE == INFO_AGE_ISVALID(stCfg->uiAge))
            {
                pNode->stCfg.uiAge = stCfg->uiAge;
            }
            if (BOOL_TRUE == INFO_HEIGHT_ISVALID(stCfg->uiHeight))
            {
                pNode->stCfg.uiHeight = stCfg->uiHeight;
            }
            ulErrCode = ERROR_SUCCESS;
        }
        else
        {
            ulErrCode = ERROR_NOT_FOUND;
        }
    }
    return ulErrCode;
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

