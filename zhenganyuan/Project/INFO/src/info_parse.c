/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_parse.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2011-12-29
        Author: guce 04152
   Description: 解析配置数据

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
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"

/*****************************************************************************
    Func Name: info_parse_Each
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 解析单个配置项
        Input: IN const CHAR *pcEach        单个配置项字符串
       Output: OUT INFO_CFG_S *pstCfg       配置数据
       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID info_parse_Each(IN const CHAR *pcEach, OUT INFO_CFG_S *pstCfg)
{
    DBGASSERT(NULL != pcEach);
    DBGASSERT(NULL != pstCfg);

    /* 工号 */
    if (0 == strncmp(pcEach, "id", strlen("id")))
    {
        sscanf(pcEach, "id=%u", &(pstCfg->uiId));
    }
    /* 姓名 */
    else if (0 == strncmp(pcEach, "name", strlen("name")))
    {
        INT iRet;
        CHAR szData[INFO_IOBUF_MAXLEN];

        iRet = sscanf(pcEach, "name=%s", szData);
        if ((1 == iRet) && (strlen(szData) <= INFO_NAME_MAXLEN))
        {
            strncpy(pstCfg->szName, szData, sizeof(pstCfg->szName));
        }
    }
    /* 性别 */
    else if (0 == strncmp(pcEach, "sex", strlen("sex")))
    {
        sscanf(pcEach, "sex=%u", &(pstCfg->enSex));
    }
    /* 年龄 */
    else if (0 == strncmp(pcEach, "age", strlen("age")))
    {
        sscanf(pcEach, "age=%u", &(pstCfg->uiAge));
    }
    /* 身高 */
    else if (0 == strncmp(pcEach, "height", strlen("height")))
    {
        sscanf(pcEach, "height=%u", &(pstCfg->uiHeight));
    }

    return;
}

/*****************************************************************************
    Func Name: INFO_parse_InputStr
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 解析输入字符串得到配置数据
        Input: IN const CHAR *pcInputStr    输入字符串
       Output: OUT INFO_CFG_S *pstCfg       配置数据
       Return: VOID
      Caution: 1.所有配置项解析前赋成非法值，调用者以此判断解析了哪些数据
               2.同一配置数据出现多次，之前解析的结果会被覆盖
               3.strsep会改写源字符串，需要先拷贝一份
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_parse_InputStr(IN const CHAR *pcInputStr, OUT INFO_CFG_S *pstCfg)
{
    CHAR szString[INFO_IOBUF_MAXLEN];
    CHAR *pcString;
    CHAR *pcEach;

    DBGASSERT(NULL != pcInputStr);
    DBGASSERT(strlen(pcInputStr) < INFO_IOBUF_MAXLEN);
    DBGASSERT(NULL != pstCfg);

    /* 出参初始化为非法值 */
    memset(pstCfg, 0, sizeof(INFO_CFG_S));

    /* 解析用户输入字符串 */
    strlcpy(szString, pcInputStr, sizeof(szString));
    pcString = szString;
    while (NULL != (pcEach = strsep(&pcString, " ")))   /* 以"空格"拆分各配置项 */
    {
        /* 依次解析各配置项 */
        info_parse_Each(pcEach, pstCfg);
    }

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

