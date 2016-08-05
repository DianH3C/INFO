/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_parse.h
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

#ifndef _INFO_PARSE_H_
#define _INFO_PARSE_H_

#ifdef __cplusplus
extern "C"{
#endif

/* 解析输入字符串得到配置数据 */
VOID INFO_parse_InputStr(IN const CHAR *pcInputStr, OUT INFO_CFG_S *pstCfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_PARSE_H_ */

