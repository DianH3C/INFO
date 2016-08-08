/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
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

/* ��Ϣ���ݽṹ */
typedef struct tagInfo_Data
{
    /* ������֯���--����[*] */
    struct tagInfo_Data *pstPrior;   /* ָ����һ����� */
    struct tagInfo_Data *pstNext;    /* ָ����һ����� */
    BOOL_T bIsEmpty;                /* ����Ƿ�Ϊ�յı�־ ��--BOOL_TRUE �ǿ�--BOOL_FALSE */
    INFO_CFG_S stCfg;              /* �������� */
}INFO_DATA_S;

/* ����ͷָ�� */
INFO_DATA_S *g_pstINFO_DATA_HEAD = NULL;

/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 201x-xx-xx
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
            /* ����ʱ����id��С�������˵�������ж�Ӧid�Ľ��Ϊ�� */
            return BOOL_FALSE;
        }
    }
    /* ���������ĩβ��δ�ҵ� */
    return BOOL_FALSE;
}

/*****************************************************************************
    Func Name: INFO_data_IsEmpty[*]
 Date Created: 201x-xx-xx
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
    /* �ж�ͷ����Ƿ�Ϊ�ռ��� */
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
  Description: ��ȡ��������
        Input: IN UINT uiId             ����
       Output: OUT INFO_CFG_S *pstCfg   ��������
       Return: ULONG, ERROR_SUCCESS     ����ɹ�
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
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* �������� */
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
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    UINT uiReId = INFO_ID_INVALID;

    /* �������� */
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
  Description: ���������������
        Input: IN INFO_CFG_S const *pstCfg
       Output:
       Return: LONG, == ERROR_SUCCESS    ��ӳɹ�
                     == ERROR_FAILED     ���ʧ��
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

    /* �������ݲ���������� */
    if (BOOL_TRUE == pstNode->bIsEmpty)
    {
        /* ��������Ϊ�� */
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
        /* ���ӵĽ�㹤�ű�ͷ���С */
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
        /* ���������ҵ������������λ�ã�����ֱ�Ӵ�ĩβ��� */
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
  Description: ��������ɾ������
        Input: IN UINT uiId
       Output:
       Return: ULONG, == ERROR_SUCCESS    ɾ���ɹ�
                      == ERROR_FAILED     ɾ��ʧ��
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

    /* ��λ��id���ڵĽ�� */
    while ((BOOL_FALSE == pstNode->bIsEmpty) && (NULL != pstNode))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            /* ��ǰ�����ͷ��� */
            if (NULL == pstNode->pstPrior)
            {
                /* �ж�ͷ�����Ƿ��н�� */
                if (NULL == pstNode->pstNext)
                {
                    /* ���ͷ�������� */
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
            /* ��ǰ��㲻��ͷ��� */
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
  Description: ���������޸�����
        Input: IN INFO_CFG_S *pstCfg
       Output:
       Return: ULONG, == ERROR_SUCCESS    �޸ĳɹ�
                      == ERROR_FAILED     �޸�ʧ��
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

    /* ���������ҵ�ָ��id��Ӧ���������� */
    ulErrCode = INFO_data_GetData(pstCfg->uiId, &(stCfg));

    /* �������� */
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
  Description: ģ���ʼ��
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     ����ɹ�
                      OTHER             ����ʧ��
      Caution: Ŀǰʼ�ճɹ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    /* ������ͷ�������ڴ�ռ� */
    g_pstINFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    if (NULL == g_pstINFO_DATA_HEAD)
    {
        return ERROR_FAILED;
    }
    else
    {
        /* ��ʼ����������ṹ */
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

    /* ����ͷ��ʼ�������ͷ��ڴ� */
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

