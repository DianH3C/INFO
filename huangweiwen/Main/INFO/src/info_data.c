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
#include <malloc.h>
#include <stdlib.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_proc.h"


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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;

    while ((NULL != pstNode) && (BOOL_FALSE == pstNode->isEmpty))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            return BOOL_TRUE;
        }
        else if (uiId > pstNode->stCfg.uiId)
        {
            pstNode = pstNode->pNext;
        }
        else
        {
            /* 遍历时出现id较小的情况，说明链表中对应id的结点为空 */
            return BOOL_FALSE;
        }
    }
    /* 链表遍历至末尾仍未找到 */
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
    /* 判断头结点是否为空即可 */
    if (BOOL_TRUE == INFO_DATA_HEAD->isEmpty)
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetData[*]
 Date Created: 201x-xx-xx
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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;

    /* 遍历链表 */
    while ((BOOL_FALSE == pstNode->isEmpty) && (NULL != pstNode))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            *pstCfg = pstNode->stCfg;
            return ERROR_SUCCESS;
        }
        else
        {
            pstNode = pstNode->pNext;
        }
    }
    return ERROR_FAILED;
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 201x-xx-xx
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
    if (BOOL_TRUE == INFO_DATA_HEAD->isEmpty)
    {
        return INFO_ID_INVALID;
    }
    else
    {
        return INFO_DATA_HEAD->stCfg.uiId;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 201x-xx-xx
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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;
    UINT uiReId = INFO_ID_INVALID;

    /* 遍历链表 */
    while ((BOOL_FALSE == pstNode->isEmpty) && (NULL != pstNode))
    {
        if (uiId < pstNode->stCfg.uiId)
        {
            uiReId = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pNext;
        }
    }
    return uiReId;
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 模块初始化
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution: 目前始终成功
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    /* 给链表头结点分配内存空间 */
    INFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    if (NULL == INFO_DATA_HEAD)
    {
        return ERROR_FAILED;
    }
    else
    {
        /* 初始化构造链表结构 */
        INFO_DATA_HEAD->pPrior = NULL;
        INFO_DATA_HEAD->pNext = NULL;
        INFO_DATA_HEAD->isEmpty = BOOL_TRUE;
    }
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 201x-xx-xx
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
    INFO_DATA_S *pstNode = NULL;

    /* 从链头开始逐个结点释放内存 */
    while (NULL != INFO_DATA_HEAD)
    {
        pstNode = INFO_DATA_HEAD->pNext;
        free(INFO_DATA_HEAD);
        INFO_DATA_HEAD = pstNode;
    }
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

