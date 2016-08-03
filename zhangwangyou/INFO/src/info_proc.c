/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2016-07-27
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
    Func Name: INFO_proc_DispSt[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: 显示单个结构体内数据
        Input: IN INFO_CFG_S stInputSt      数据结构体
       Output:

       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
VOID INFO_proc_DisStruct(IN INFO_CFG_S *pstInputStruct)
{
    printf("%u\t%s\t", pstInputStruct->uiId, pstInputStruct->szName);
    if (INFO_SEX_FEMALE == pstInputStruct->enSex)
    {
        printf("FEMALE\t");
    }
    else
    {
        printf("MALE\t");
    }
    printf("%u\t%u\r\n", pstInputStruct->uiAge, pstInputStruct->uiHeight);
}


/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 2016-07-28
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
    IGNORE_PARAM(pcInputStr);

    UINT uiId = INFO_ID_INVALID;
    ULONG ulHaveGotData = ERROR_FAILED;
    INFO_CFG_S stData = { 0 };

    /* 没有任何信息 */
    if (BOOL_TRUE == INFO_data_IsEmpty())
    {
        printf("No info.\r\n");
    }
    else
    {
        printf("ID\tName\tSex\tAge\tHeight\r\n");

        /* 依次获取所有有数据的工号及其他信息 */
        uiId = INFO_data_GetFirst();
        ulHaveGotData = INFO_data_GetData(uiId, &stData);
        if (ERROR_SUCCESS == ulHaveGotData)
        {
            INFO_proc_DisStruct(&stData);
        }

        uiId = INFO_data_GetNext(uiId);
        while (INFO_ID_INVALID != uiId)
        {
            ulHaveGotData = INFO_data_GetData(uiId, &stData);
            if (ERROR_SUCCESS == ulHaveGotData)
            {
                INFO_proc_DisStruct(&stData);
            }
            uiId = INFO_data_GetNext(uiId);
        }
    }
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 2016-07-29
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
    ULONG ulErrCode = ERROR_FAILED;
    const CHAR *pcErrInfo = NULL;
    INFO_CFG_S stCfg = { 0 };

    /* 解析输入字符串得到配置数据并存入stCfg中 */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ALL_ISVALID(&stCfg))
    {
        /* 数据输入不全或数据取值非法 */
        pcErrInfo = "The parameter is incorrect.\r\n";
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
    {
        /* 工号已经存在 */
        pcErrInfo = "The item already exists.\r\n";
        ulErrCode = ERROR_ALREADY_EXIST;
    }
    else
    {
        ulErrCode = INFO_data_AddData(&stCfg);

        if (ERROR_SUCCESS != ulErrCode)
        {
            /* 资源不足添加失败 */
            pcErrInfo = "Not enough resources are available to complete the operation.\r\n";
        }
        else
        {
            /* 添加成功 */
            pcErrInfo = "You can see the newly added info through Display function .\r\n";
        }
    }

    printf("%s", pcErrInfo);
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 2016-07-29
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
    ULONG ulErrCode = ERROR_FAILED;
    UINT uiId = INFO_ID_INVALID;
    const CHAR *pcErrInfo = NULL;


    if (0 != strncmp(pcInputStr, "id", strlen("id")))
    {
        /* 工号不合法 */
        pcErrInfo = "The parameter is incorrect.\r\n";
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* 提取字符串"id=xx"中的数值xx */
        if(-1 == sscanf(pcInputStr, "id=%u", &uiId))
        {
            /* 工号数值异常 */
            pcErrInfo = "The parameter is incorrect.\r\n";
            ulErrCode = ERROR_INVALID_PARAMETER;
        }
        else if (INFO_ID_ISVALID(uiId))
        {
            /* 工号数值异常 */
            pcErrInfo = "The parameter is incorrect.\r\n";
            ulErrCode = ERROR_INVALID_PARAMETER;
        }
        else if (BOOL_FALSE == INFO_data_IsExist(uiId))
        {
            /* 工号不存在 */
            pcErrInfo = "The specified item was not found.\r\n";
            ulErrCode = ERROR_NOT_FOUND;
        }
        else
        {
            /* 工号存在 */
            ulErrCode = INFO_data_DelData(uiId);

            if (ERROR_SUCCESS == ulErrCode)
            {
                pcErrInfo = "The info has been successfully deleted.\r\n";
            }
            else
            {
                pcErrInfo = "Fail to delete the info.\r\n";
            }
        }
    }

    printf("%s", pcErrInfo);
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 2016-07-30
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
    ULONG ulErrCode = ERROR_FAILED;
    const CHAR *pcErrInfo = NULL;
    INFO_CFG_S stCfg = { 0 };

    /* 解析输入字符串得到配置数据并存入stCfg中 */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (BOOL_FALSE == INFO_ID_ISVALID(stCfg.uiId))
    {
        /* 工号非法 */
        pcErrInfo = "The parameter is incorrect.\r\n";
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* 若工号合法，检查工号是否存在 */
        if (BOOL_FALSE == INFO_data_IsExist(stCfg.uiId))
        {
            /* 工号不存在 */
            pcErrInfo = "The specified item was not found.\r\n";
            ulErrCode = ERROR_NOT_FOUND;
        }
        else
        {
            (VOID) INFO_data_ModifyData(&stCfg);
            pcErrInfo = "Modification complete.\r\n";
            ulErrCode = ERROR_SUCCESS;
        }
    }

    printf("%s", pcErrInfo);

    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Exit
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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

