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
    return INFO_data_Add(pcInputStr);
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
    INFO_CFG_S stCfg;
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if(INFO_ID_ISVALID(stCfg.uiId))
    {
        return INFO_data_Delete(stCfg.uiId);
    }
    else
    {
        return ERROR_INVALID_PARAMETER;
    }
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

