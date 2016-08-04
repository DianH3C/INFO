/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_dbg.h
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

#ifndef _INFO_DBG_H_
#define _INFO_DBG_H_

#ifdef __cplusplus
extern "C"{
#endif

/*************************************** 调试开关 ***************************************/
VOID INFO_dbg_Set(IN BOOL_T bSwitch);
BOOL_T INFO_dbg_Get(VOID);

/*************************************** 打印信息 ***************************************/
VOID INFO_dbg_Print(IN const CHAR *pcFunc, IN const CHAR *pcFmt, ...);

/* 使用此宏打印调试信息 */
#define INFO_DBG_PRINT(pcFmt, ...)\
{\
    if (BOOL_TRUE == INFO_dbg_Get())\
    {\
        INFO_dbg_Print(__FUNCTION__, pcFmt, ##__VA_ARGS__);\
    }\
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_DBG_H_ */

