/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ���幦�ܴ���

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

/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ��ʾ
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:

       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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
        ulErrCode = ERROR_SUCCESS;
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
                printf("MALE \t");
            }
            else
            {
                printf("FEMALE \t");
            }
            printf("%u \t %u \r\n", stCfg.uiAge, stCfg.uiHeight);
            uiId = INFO_data_GetNext(uiId);
        }
    }
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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

    /* ����������ַ��������õ����������ݴ���stCfg�� */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ALL_ISVALID(&stCfg))
    {
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
    {
        /* �ù����Ѿ����� */
        ulErrCode = ERROR_ALREADY_EXIST;
    }
    else
    {
        ulErrCode = INFO_data_Add(&stCfg);
    }
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ɾ��
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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

    if (0 != strncmp(pcInputStr, "id", strlen("id")))
    {
        /* �����ַ������Ϸ� */
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        sscanf(pcInputStr, "id=%u", &uiId);
        if (BOOL_FALSE == INFO_ID_ISVALID(uiId))
        {
            /* �����id���Ϸ� */
            ulErrCode = ERROR_INVALID_PARAMETER;
        }
        else if (BOOL_TRUE == INFO_data_IsExist(uiId))
        {
            ulErrCode = INFO_data_Delete(uiId);
        }
        else
        {
            /* �����id������ */
            ulErrCode = ERROR_NOT_FOUND;
        }
    }
        return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �޸�
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution: ֧�ֽ��޸Ĳ�����������
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

    /* ����������ַ��������õ����������ݴ���stCfg�� */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ID_ISVALID(stCfg.uiId))
    {
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
        {
            ulErrCode = INFO_data_Modify(&stCfg);
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
  Description: �˳�
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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
  Description: �򿪵��Կ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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
  Description: �رյ��Կ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
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

