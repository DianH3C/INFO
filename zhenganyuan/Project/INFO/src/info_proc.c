/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ŸßÌå¹ŠÄÜŽŠÀí

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
extern BOOL_T g_bTmp;
/*****************************************************************************
    Func Name: INFO_proc_IsInputValid
 Date Created: 2016-08-07
       Author: ZAY
  Description: 检查输入的字符串是否合理
        Input: IN const CHAR *pcInputStr   
       Output:

       Return: INFO_CFG_S *, NULL 不合理
                            ！NULL 合理  
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
INFO_CFG_S *INFO_proc_IsInputValid(IN const CHAR *pcInputStr)
{
    INFO_CFG_S *pstCfg=(INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    DBGASSERT(pstCfg);
    INFO_parse_InputStr(pcInputStr,pstCfg);
    if(!INFO_ALL_ISVALID(pstCfg))
    {
        free(pstCfg);
        return NULL;
    }
    return pstCfg;
}

/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÏÔÊŸ
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:

       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
    if(INFO_data_IsEmpty())
    {
        printf("No info.\n");
        return ERROR_FAILED;
    }
    printf("ID      Name    Sex    Age     Height\n ");
    INFO_data_Display();
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÌíŒÓ
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
    INFO_CFG_S *pstCfg=INFO_proc_IsInputValid(pcInputStr);
    if(pstCfg==NULL)
    {
        printf("The parameter is incorrect.\n");
        return ERROR_INVALID_PARAMETER;
    }
    if(INFO_data_IsExist(pstCfg->uiId))
    {
        printf("The item already exists.\n");
        return ERROR_ALREADY_EXIST;
    }
    INFO_data_Insert(pstCfg);
    if(g_bTmp==BOOL_TRUE)
    {
        g_bTmp=BOOL_FALSE;
        return ERROR_NO_ENOUGH_RESOURCE;
    }
    printf("Íš¹ýDisplay¹ŠÄÜ£¬¿ÉÒÔ¿ŽµœÐÂÔöÐÅÏ¢\n");
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÉŸ³ý
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
    INFO_CFG_S *pstCfg=(INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    DBGASSERT(pstCfg);
    INFO_parse_InputStr(pcInputStr,pstCfg);
    if(!INFO_ID_ISVALID(pstCfg->uiId))
    {
        printf("The parameter is incorrect.\n");
        return ERROR_INVALID_PARAMETER;
    }
    if(BOOL_FALSE==INFO_data_IsExist(pstCfg->uiId))
        return ERROR_SUCCESS;
    if(BOOL_FALSE==INFO_data_Delete(pstCfg->uiId))
    {
        return ERROR_FAILED;
    }
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÐÞžÄ
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
      Caution: Ö§³ÖœöÐÞžÄ²¿·ÖÅäÖÃÊýŸÝ
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Modify(IN const CHAR *pcInputStr)
{
    INFO_CFG_S *pstCfg=(INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    DBGASSERT(pstCfg);
    INFO_parse_InputStr(pcInputStr,pstCfg);
    if(!INFO_ID_ISVALID(pstCfg->uiId))
    {
        printf("The parameter is incorrect.\n");
        return ERROR_INVALID_PARAMETER;
    }
    if(BOOL_FALSE==INFO_data_IsExist(pstCfg->uiId))
    {
        printf("The specified item was not found.\n");
        return ERROR_NOT_FOUND;
    }
    INFO_data_Modify(pstCfg);
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Exit
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÍË³ö
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
  Description: Žò¿ªµ÷ÊÔ¿ª¹Ø
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
  Description: ¹Ø±Õµ÷ÊÔ¿ª¹Ø
        Input: IN const CHAR *pcInputStr    ÊäÈë×Ö·ûŽ®
       Output:
       Return: ULONG, ERROR_SUCCESS         ŽŠÀí³É¹Š
                      OTHER                 ŽŠÀíÊ§°Ü
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
