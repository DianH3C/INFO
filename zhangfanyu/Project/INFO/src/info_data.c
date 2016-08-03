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

/* 信息数据结构 */
typedef struct tagInfo_Data
{
    struct tagInfo_Data *Info_Data_next;/* 数据组织相关[*] */
    INFO_CFG_S stCfg;          /* 配置数据 */
}INFO_DATA_S;

/*全局变量的链表*/
INFO_DATA_S* pstHEAD;

/*******************************内部函数***********************************/
/*申请内存并将申请到的内存空间清零，返回空间首地址*/
INFO_DATA_S* info_data_Alloc()
{
    INFO_DATA_S* pstNode;
    pstNode=(INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    memset(pstNode,0,sizeof(INFO_DATA_S));
    if(pstNode == NULL)
    {
        return NULL;
    }
    return pstNode;
}

/*释放入参指针指向的内存空间*/
VOID info_data_Free(IN INFO_DATA_S* pstNode)
{
    free(pstNode);
    pstNode=NULL;
}

/*将入参指针指向的链表节点(按照工号从小到大的顺序)加入链表结构当中*/
/*思路:先判断是否为空链表，再循环判断链表节点的下一个节点UID是否大于插入节点
者为空，两种情况均可直接插入*/
VOID info_data_Add(IN INFO_DATA_S* pstNode)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    /*如过链表为空，则直接插入最后*/
    if(NULL == pstHEAD)
    {
        pstHEAD=pstNode;
        pstNode->Info_Data_next=NULL;
    }
    else
    {
        while(pstMvNode->Info_Data_next->stCfg.uiId < pstNode->stCfg.uiId
            &&pstMvNode->Info_Data_next != NULL)
        {
            pstMvNode=pstMvNode->Info_Data_next;
        }
        pstNode->Info_Data_next=pstMvNode->Info_Data_next;
        pstMvNode->Info_Data_next=pstNode;
    }
}

/*将入参指针指向的链表节点从链表结构中摘除*/
VOID info_data_Delete(IN INFO_DATA_S* pstNode)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode->Info_Data_next != NULL)
    {
        if(pstMvNode->Info_Data_next->stCfg.uiId == pstNode->stCfg.uiId)
        {
            pstMvNode->Info_Data_next=pstMvNode->Info_Data_next->Info_Data_next;
            return;
        }
        /*链表后移*/
        pstMvNode=pstMvNode->Info_Data_next;
    }
}

/*返回入参工号对应的链表节点的指针*/
INFO_DATA_S* info_data_Get(IN UINT uiId)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode != NULL)
    {
        if(pstMvNode->stCfg.uiId == uiId)
        {
            return pstMvNode;
        }
       /*链表后移*/
       pstMvNode=pstMvNode->Info_Data_next;
    }
    return NULL;
}
/**********************************修改数据************************************/
/*将入参工号的姓名改为入参姓名*/
VOID INFO_data_SetName(IN UINT uiId,IN CHAR szInName[])
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    strlcpy(pstCgNode->stCfg.szName,szInName,INFO_NAME_MAXLEN + 1);
}

/*将入参工号的性别改为入参性别*/
VOID INFO_data_SetSex(IN UINT uiId,IN INFO_SEX_E enInSex)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.enSex=enInSex;
}
/*将入参工号的年龄改为入参年龄*/
VOID INFO_data_SetAge(IN UINT uiId,IN UINT uiInAge)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.uiAge=uiInAge;
}
/*将入参工号的身高改为入参身高*/
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiInHeght)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.uiHeight=uiInHeght;
}


/*****************************************************************************
    Func Name: INFO_data_Create[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 为入参工号创建一个节点并将该节点加入链表
        Input: IN UINT uiId         工号
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_date_Create(IN UINT uiId)
{
    INFO_DATA_S* pstNode=info_data_Alloc();

    pstNode->stCfg.uiId=uiId;
    if(pstNode == NULL)
    {
        return ERROR_NO_ENOUGH_RESOURCE;
    }
    info_data_Add(pstNode);
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Destroy[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 为入参工号创建一个节点并将该节点加入链表
        Input: IN UINT uiId         工号
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Destroy(IN UINT uiId)
{
    INFO_DATA_S* pstNode=info_data_Get(uiId);
    if(pstNode == NULL)
    {
        return;
    }
    else
    {
        info_data_Delete(pstNode);
        info_data_Free(pstNode);
    }
}

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
    if(info_data_Get(uiId) == NULL)
    {
        return BOOL_FALSE;
    }
    else
    {
    return BOOL_TRUE;
    }
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
    if(pstHEAD == NULL)
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
    INFO_DATA_S *pstDataNode;
    pstDataNode=info_data_Get(uiId);

    if(pstDataNode == NULL)
    {
        return ERROR_FAILED;
    }
    else
    {
        *pstCfg=pstDataNode->stCfg;
        return ERROR_SUCCESS ;
    }

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
    INFO_DATA_S *pstFstNode=pstHEAD;
    if(pstFstNode == NULL)
    {
        return INFO_ID_INVALID;
    }
    else
    {
    return pstFstNode->stCfg.uiId;
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
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode != NULL)
     {
         if(pstMvNode->stCfg.uiId > uiId)
         {
             return pstMvNode->stCfg.uiId;
         }
         /*链表后移*/
         pstMvNode=pstMvNode->Info_Data_next;
     }

    return INFO_ID_INVALID;
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
    pstHEAD=NULL;

    if(pstHEAD==NULL)
    {
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
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
    INFO_DATA_S *pstMvNode=pstHEAD;
    INFO_DATA_S *pstTmpNode=pstHEAD;

    if(pstHEAD==NULL)
    {
        return;
    }

    while(pstMvNode->Info_Data_next != NULL)
    {
        pstTmpNode=pstMvNode;
        pstMvNode=pstMvNode->Info_Data_next;
        info_data_Free(pstTmpNode);
    }
    info_data_Free(pstMvNode);
    pstHEAD=NULL;
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

