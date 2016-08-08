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
#include "info_data.h"
#include "info_proc.h"

/* 信息数据结构 */
typedef struct tagInfo_Data
{
    /* 数据组织相关--链表[*] */
    struct tagInfo_Data *pstPrior;   /* 指向上一个结点 */
    struct tagInfo_Data *pstNext;    /* 指向下一个结点 */
    BOOL_T bIsEmpty;                /* 结点是否为空的标志 空--BOOL_TRUE 非空--BOOL_FALSE */
    INFO_CFG_S stCfg;              /* 配置数据 */
}INFO_DATA_S;

/* 链表头指针 */
INFO_DATA_S *g_pstINFO_DATA_HEAD = NULL;

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
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    while ((NULL != pstNode) && (BOOL_FALSE == pstNode->bIsEmpty))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            return BOOL_TRUE;
        }
        else if (uiId > pstNode->stCfg.uiId)
        {
            pstNode = pstNode->pstNext;
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
    if (BOOL_TRUE == g_pstINFO_DATA_HEAD->bIsEmpty)
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
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* 遍历链表 */
    while ((BOOL_FALSE == pstNode->bIsEmpty) && (NULL != pstNode))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            *pstCfg = pstNode->stCfg;
            return ERROR_SUCCESS;
        }
        else
        {
            pstNode = pstNode->pstNext;
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
    if (BOOL_TRUE == g_pstINFO_DATA_HEAD->bIsEmpty)
    {
        return INFO_ID_INVALID;
    }
    else
    {
        return g_pstINFO_DATA_HEAD->stCfg.uiId;
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
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    UINT uiReId = INFO_ID_INVALID;

    /* 遍历链表 */
    while ((NULL != pstNode) && (BOOL_FALSE == pstNode->bIsEmpty))
    {
        if (uiId < pstNode->stCfg.uiId)
        {
            uiReId = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }
    return uiReId;
}

/*****************************************************************************
    Func Name: INFO_data_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 向链表中添加数据
        Input: IN INFO_CFG_S const *pstCfg
       Output:
       Return: LONG, == ERROR_SUCCESS    添加成功
                     == ERROR_FAILED     添加失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Add(IN INFO_CFG_S *pstCfg)
{
    ULONG ulErrCode = ERROR_FAILED;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pNewNode = NULL;

    /* 将新数据插入的链表当中 */
    if (BOOL_TRUE == pstNode->bIsEmpty)
    {
        /* 链表内容为空 */
        pNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
        if (NULL == pNewNode)
        {
            ulErrCode = ERROR_FAILED;
        }
        else
        {
            g_pstINFO_DATA_HEAD = pNewNode;
            pNewNode->pstPrior = NULL;
            pNewNode->pstNext = NULL;
            pNewNode->bIsEmpty = BOOL_FALSE;
            pNewNode->stCfg = *pstCfg;
            ulErrCode = ERROR_SUCCESS;
        }
    }
    else if (pstCfg->uiId < INFO_data_GetFirst())
    {
        /* 增加的结点工号比头结点小 */
        pNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
        if (NULL == pNewNode)
        {
            ulErrCode = ERROR_FAILED;
        }
        else
        {
            g_pstINFO_DATA_HEAD = pNewNode;
            pNewNode->pstNext = pstNode;
            pNewNode->bIsEmpty = BOOL_FALSE;
            pNewNode->stCfg = *pstCfg;
            pstNode->pstPrior = pNewNode;
            ulErrCode = ERROR_SUCCESS;
        }
    }
    else
    {
        /* 遍历链表，找到新增结点插入的位置，否则直接从末尾添加 */
        while ((BOOL_FALSE == pstNode->bIsEmpty) && (NULL != pstNode))
        {
            if ((pstCfg->uiId > pstNode->stCfg.uiId)
                && ((NULL == pstNode->pstNext) || (pstCfg->uiId < pstNode->pstNext->stCfg.uiId)))
            {
                pNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
                if (NULL == pNewNode)
                {
                    ulErrCode = ERROR_FAILED;
                }
                else
                {
                    pNewNode->pstPrior = pstNode;
                    pNewNode->pstNext = pstNode->pstNext;
                    pNewNode->bIsEmpty = BOOL_FALSE;
                    pNewNode->stCfg = *pstCfg;
                    pstNode->pstNext = pNewNode;
                    if (NULL != pstNode->pstNext)
                    {
                        pstNode->pstNext->pstPrior = pNewNode;
                    }
                    ulErrCode = ERROR_SUCCESS;
                }
            }
            else
            {
                pstNode = pstNode->pstNext;
            }
        }
    }
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_data_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 从链表中删除数据
        Input: IN UINT uiId
       Output:
       Return: ULONG, == ERROR_SUCCESS    删除成功
                      == ERROR_FAILED     删除失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Delete(IN UINT uiId)
{
    ULONG ulErrCode = ERROR_FAILED;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pDelNode;

    /* 定位该id所在的结点 */
    while ((BOOL_FALSE == pstNode->bIsEmpty) && (NULL != pstNode))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            /* 当前结点是头结点 */
            if (NULL == pstNode->pstPrior)
            {
                /* 判断头结点后是否还有结点 */
                if (NULL == pstNode->pstNext)
                {
                    /* 清空头结点的内容 */
                    memset(&(pstNode->stCfg), 0, sizeof(INFO_CFG_S));
                    pstNode->bIsEmpty = BOOL_TRUE;
                }
                else
                {
                    pDelNode = pstNode;
                    pstNode->pstNext->pstPrior = NULL;
                    pstNode = pstNode->pstNext;
                    free(pDelNode);
                }
            }
            /* 当前结点不是头结点 */
            else
            {
                pDelNode = pstNode;
                pstNode->pstPrior->pstNext = pstNode->pstNext;
                if (NULL != pstNode->pstNext)
                {
                    pstNode->pstNext->pstPrior = pstNode->pstPrior;
                }
                free(pDelNode);
            }
            ulErrCode = ERROR_SUCCESS;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_data_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 从链表中修改数据
        Input: IN INFO_CFG_S *pstCfg
       Output:
       Return: ULONG, == ERROR_SUCCESS    修改成功
                      == ERROR_FAILED     修改失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Modify(IN INFO_CFG_S *pstCfg)
{
    ULONG ulErrCode = ERROR_FAILED;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_CFG_S stCfg = {0};

    /* 从链表中找到指定id对应的配置数据 */
    ulErrCode = INFO_data_GetData(pstCfg->uiId, &(stCfg));

    /* 遍历链表 */
    while ((BOOL_FALSE == pstNode->bIsEmpty) && (NULL != pstNode))
    {
        if (pstCfg->uiId != pstNode->stCfg.uiId)
        {
            pstNode = pstNode->pstNext;
        }
        else
        {
            if (BOOL_TRUE == INFO_NAME_ISVALID(pstCfg->szName))
            {
                strlcpy(pstNode->stCfg.szName, pstCfg->szName, sizeof(pstNode->stCfg.szName));
            }
            if (BOOL_TRUE == INFO_SEX_ISVALID(pstCfg->enSex))
            {
                pstNode->stCfg.enSex = pstCfg->enSex;
            }
            if (BOOL_TRUE == INFO_AGE_ISVALID(pstCfg->uiAge))
            {
                pstNode->stCfg.uiAge = pstCfg->uiAge;
            }
            if (BOOL_TRUE == INFO_HEIGHT_ISVALID(pstCfg->uiHeight))
            {
                pstNode->stCfg.uiHeight = pstCfg->uiHeight;
            }
            ulErrCode = ERROR_SUCCESS;
            break;
        }
    }
    return ulErrCode;
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
    g_pstINFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    if (NULL == g_pstINFO_DATA_HEAD)
    {
        return ERROR_FAILED;
    }
    else
    {
        /* 初始化构造链表结构 */
        g_pstINFO_DATA_HEAD->pstPrior = NULL;
        g_pstINFO_DATA_HEAD->pstNext = NULL;
        g_pstINFO_DATA_HEAD->bIsEmpty = BOOL_TRUE;
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
    while (NULL != g_pstINFO_DATA_HEAD)
    {
        pstNode = g_pstINFO_DATA_HEAD->pstNext;
        free(g_pstINFO_DATA_HEAD);
        g_pstINFO_DATA_HEAD = pstNode;
    }
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

