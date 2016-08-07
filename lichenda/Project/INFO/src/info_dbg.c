/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_dbg.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2011-12-29
        Author: guce 04152
   Description: 调试信息

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
#include <stdarg.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/assert.h>

/* module   private */
#include "info.h"

/* 调试开关 */
STATIC BOOL_T g_bInfoDbg = BOOL_FALSE;

/*****************************************************************************
    Func Name: INFO_dbg_Set
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 设置调试开关
        Input: IN BOOL_T bSwitch    开关状态
       Output: 
       Return: VOID
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_dbg_Set(IN BOOL_T bSwitch)
{
    g_bInfoDbg = bSwitch;

    return;
}

/*****************************************************************************
    Func Name: INFO_dbg_Get
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 获取调试开关
        Input: VOID
       Output: 
       Return: BOOL_T, BOOL_TRUE    打开
                       BOOL_FALSE   关闭
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_dbg_Get(VOID)
{
    return g_bInfoDbg;
}

/*****************************************************************************
    Func Name: INFO_dbg_Print
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 打印调试信息
        Input: IN const CHAR *pcFunc    函数名
               IN const CHAR *pcFmt     格式串
               ...                      参数
       Output: 
       Return: VOID
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_dbg_Print(IN const CHAR *pcFunc, IN const CHAR *pcFmt, ...)
{
    CHAR szBuf[INFO_IOBUF_MAXLEN];
    va_list stArgs;

    DBGASSERT(NULL != pcFunc);
    DBGASSERT(NULL != pcFmt);

    /* 拼装可变参部分 */
    szBuf[0] = 0;
    va_start(stArgs, pcFmt);
    (VOID)vsnprintf(szBuf, sizeof(szBuf), pcFmt, stArgs);
    va_end(stArgs);

    /* 打印调试信息 */
    printf("*DEBUG[%s]: %s\r\n", pcFunc, szBuf);

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

