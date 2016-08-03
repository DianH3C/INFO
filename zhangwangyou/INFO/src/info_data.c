/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2016-07-28
        Author: xxxx 00000
   Description: �ڲ����ݲ���
                ����������ɾ�������á���ȡ������

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

/* ��Ϣ���ݽṹ */
typedef struct tagInfo_Data
{
    struct tagInfo_Data *pstNext;   /* ������֯���[*]��ָ����һ���ڵ� */
    BOOL_T bIsEmpty;            /* ������֯���[*]�������Ƿ�Ϊ�գ�TRUE��ʾ�� */
    INFO_CFG_S stCfg;           /* �������� */
}INFO_DATA_S;

/* ��������ͷָ�� */
INFO_DATA_S *g_pstINFO_DATA_HEAD = NULL;


/*****************************************************************************
    Func Name: INFO_data_FindNodeById[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ����ָ�����ŵĽڵ��Ƿ����
        Input: IN UINT uiId         ����
       Output:
       Return: INFO_DATA_S *, != NULL     �������ڽڵ�ָ��
                              == NULL     δ�ҵ�
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INFO_DATA_S *INFO_data_FindNodeById(IN UINT uiId)
{
    BOOL_T bRet = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* �����������������ҹ��� */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            bRet = BOOL_TRUE;
            break;
        }
        else if (uiId < pstNode->stCfg.uiId)
        {
            bRet = BOOL_FALSE;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    if (BOOL_TRUE == bRet)
    {
        return pstNode;
    }
    else
    {
        return NULL;
    }
}

/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: �ж�ָ�����ŵ������Ƿ����
        Input: IN UINT uiId         ����
       Output:
       Return: BOOL_T, BOOL_TRUE    ����
                       BOOL_FALSE   ������
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsExist(IN UINT uiId)
{
    if (NULL == INFO_data_FindNodeById(uiId))
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
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: �ж�����������֯�Ƿ�Ϊ��
        Input:
       Output:
       Return: BOOL_T, BOOL_TRUE    ������֯Ϊ��
                       BOOL_FALSE   ������֯�ǿ�
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsEmpty(VOID)
{
    /* �ж�ͷ�ڵ�������Ƿ�Ϊ�� */
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
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ��ȡ��������
        Input: IN UINT uiId             ����
       Output: OUT INFO_CFG_S *pstCfg   ��������
       Return: ULONG, ERROR_SUCCESS     �����ɹ�
                      OTHER             ����ʧ��
      Caution: ���ν��ڷ��سɹ�ʱ��Ч
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    INFO_DATA_S *pstNode = INFO_data_FindNodeById(uiId);

    if (NULL != pstNode)
    {
        *pstCfg = pstNode->stCfg;
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ��ȡ��һ�������ݹ���
        Input: VOID
       Output:
       Return: UINT, != INFO_ID_INVALID     ��һ�������ݵĹ���
                     == INFO_ID_INVALID     δ�ҵ�
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetFirst(VOID)
{
    if (BOOL_TRUE == INFO_data_IsEmpty())
    {
        return INFO_ID_INVALID;
    }
    else
    {
        /* ��һ�������ݵĹ�����ͷ�ڵ㴦 */
        return g_pstINFO_DATA_HEAD->stCfg.uiId;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ��ȡ��һ�������ݹ���
        Input: IN UINT uiId                 ��ǰ����
       Output:
       Return: UINT, != INFO_ID_INVALID     ��һ������
                     == INFO_ID_INVALID     δ�ҵ�
      Caution: �˽ӿڻ�ȡ��һ�����Ų����������uiId�����Ƿ�������
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetNext(IN UINT uiId)
{
    UINT uiRet = INFO_ID_INVALID;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* �����������������ҹ��� */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (uiId < pstNode->stCfg.uiId)
        {
            /* ��uiId��ĵ�һ�����ŵĽڵ� */
            uiRet = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    return uiRet;
}

/*****************************************************************************
    Func Name: INFO_data_GetLast[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ��ȡǰһ�������ݹ���
        Input: IN UINT uiId                 ��ǰ����
       Output:
       Return: UINT, != INFO_ID_INVALID     ǰһ������
                     == INFO_ID_INVALID     δ�ҵ�
      Caution: �˽ӿڻ�ȡ��һ�����Ų����������uiId�����Ƿ�������
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetLast(IN UINT uiId)
{
    UINT uiRet = INFO_ID_INVALID;
    BOOL_T bIs_uiId = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* �����������������ҹ��� */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        /* ��һ���ڵ�Ϊָ���������ڽڵ� */
        bIs_uiId = (NULL != pstNode->pstNext) \
            && (BOOL_TRUE != pstNode->pstNext->bIsEmpty) \
            && (uiId == pstNode->pstNext->stCfg.uiId);
        if ((uiId > pstNode->stCfg.uiId) && bIs_uiId)
        {
            /* ��uiIdС�ĵ�һ�����ŵĽڵ� */
            uiRet = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    return uiRet;
}

/*****************************************************************************
    Func Name: INFO_data_AddData[*]
 Date Created: 2016-07-29
       Author: xxxx 00000
  Description: ����һ�����ݽڵ�
        Input: IN INFO_CFG_S *pstCfg    ��������
       Output:
       Return: ULONG, ERROR_SUCCESS     �����ɹ�
                      OTHER             ����ʧ��
      Caution: �˽ӿڲ���鹤���Ƿ��Ѿ����ڣ����ⲿ�������м��
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_AddData(IN INFO_CFG_S *pstCfg)
{
    ULONG ulRet = ERROR_FAILED;
    BOOL_T bAddable = BOOL_FALSE;
    BOOL_T bAddable_next = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pstNewNode = NULL;

    if (INFO_data_GetFirst() > pstCfg->uiId)
    {
        /* ͷ�ڵ�Ĺ��ű�Ҫ���ӵĽڵ㹤�Ŵ� */
        pstNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
        if (NULL == pstNewNode)
        {
            ulRet = ERROR_NO_ENOUGH_RESOURCE;
        }
        else
        {
            ulRet = ERROR_SUCCESS;
            g_pstINFO_DATA_HEAD = pstNewNode;
            g_pstINFO_DATA_HEAD->pstNext = pstNode;
            g_pstINFO_DATA_HEAD->bIsEmpty = BOOL_FALSE;
            g_pstINFO_DATA_HEAD->stCfg = *pstCfg;
        }
    }
    else
    {
        /* ����������������λ���빤�ŵ�λ�� */
        while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
        {
            bAddable_next = (NULL == pstNode->pstNext) || (pstCfg->uiId < pstNode->pstNext->stCfg.uiId);
            bAddable = (pstCfg->uiId > pstNode->stCfg.uiId) && bAddable_next;
            if (bAddable)
            {
                pstNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
                if (NULL == pstNewNode)
                {
                    ulRet = ERROR_NO_ENOUGH_RESOURCE;
                }
                else
                {
                    ulRet = ERROR_SUCCESS;
                    pstNewNode->pstNext = pstNode->pstNext;
                    pstNewNode->bIsEmpty = BOOL_FALSE;
                    pstNewNode->stCfg = *pstCfg;
                    pstNode->pstNext = pstNewNode;
                }
                break;
            }
            else
            {
                pstNode = pstNode->pstNext;
            }
        }
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: INFO_data_DelData[*]
 Date Created: 2016-07-29
       Author: xxxx 00000
  Description: ɾ��һ�����ݽڵ�
        Input: IN UINT uiId                  Ҫɾ���Ľڵ㹤��
        Output:
        Return: ULONG, ERROR_SUCCESS         �����ɹ�
                       OTHER                 ����ʧ��
      Caution: �˽ӿڲ���鹤���Ƿ��Ѿ����ڣ����ⲿ�������м��
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_DelData(IN UINT uiId)
{
    UINT uiId_last = INFO_ID_INVALID;
    ULONG ulRet = ERROR_FAILED;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pstNode_last = NULL;

    if(uiId == pstNode->stCfg.uiId)
    {
        /* Ҫɾ���Ľڵ���ͷ�ڵ� */
        ulRet = ERROR_SUCCESS;
        if (NULL == pstNode->pstNext)
        {
            /* ��ͷ�ڵ����û�нڵ㣬��ֻ���ͷ�ڵ��ϵ����� */
            memset(&(pstNode->stCfg), 0, sizeof(INFO_CFG_S));
            pstNode->bIsEmpty = BOOL_TRUE;
        }
        else
        {
            g_pstINFO_DATA_HEAD = g_pstINFO_DATA_HEAD->pstNext;
            free(pstNode);
        }
    }
    else
    {
        /* Ҫɾ���Ľڵ㲻��ͷ�ڵ� */
        uiId_last = INFO_data_GetLast(uiId);
        if (INFO_ID_INVALID != uiId_last)
        {
            ulRet = ERROR_SUCCESS;
            pstNode_last = INFO_data_FindNodeById(uiId_last);
            if (NULL != pstNode_last)
            {
                pstNode = pstNode_last->pstNext;
                pstNode_last->pstNext = pstNode->pstNext;
                free(pstNode);
            }
        }
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: INFO_data_ModifyData[*]
 Date Created: 2016-07-30
       Author: xxxx 00000
  Description: �޸�һ���ڵ������
        Input: IN INFO_CFG_S *pstCfg         Ҫ�޸ĵ�����
        Output:
        Return: ULONG, ERROR_SUCCESS         �����ɹ�
                       OTHER                 ����ʧ��
      Caution: �˽ӿڲ���鹤���Ƿ��Ѿ����ڣ����ⲿ�������м��
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_ModifyData(IN INFO_CFG_S *pstCfg)
{
    INFO_DATA_S *pstNode = INFO_data_FindNodeById(pstCfg->uiId);
    if ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (BOOL_TRUE == INFO_NAME_ISVALID(pstCfg->szName))
        {
            (VOID) strlcpy(pstNode->stCfg.szName, pstCfg->szName, sizeof(pstNode->stCfg.szName));
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
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_NOT_FOUND;
    }
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ģ���ʼ��
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     �����ɹ�
                      OTHER             ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    ULONG ulRet = ERROR_SUCCESS;

    /* ������������ͷ�ڵ� */
    g_pstINFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));

    if (NULL == g_pstINFO_DATA_HEAD)
    {
        ulRet = ERROR_FAILED;
    }
    else
    {
        /* ��ʼ��ͷ�ڵ� */
        g_pstINFO_DATA_HEAD->pstNext = NULL;
        g_pstINFO_DATA_HEAD->bIsEmpty = BOOL_TRUE;
    }

    if (ERROR_SUCCESS != ulRet)
    {
        return ERROR_FAILED;
    }
    else
    {
        return ERROR_SUCCESS;
    }
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ģ���˳�
        Input:
       Output:
       Return: VOID
      Caution: ���ô˽ӿ�ǰ�������Ѿ���ʼ����
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Fini(VOID)
{
    INFO_DATA_S *pstNode = NULL;

    /* ���������������ͷ������������ڴ� */
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
