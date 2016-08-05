/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.h
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

#ifndef _INFO_PROC_H_
#define _INFO_PROC_H_

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
    Func Name: INFO_PROC_PF
 Date Created: 2011-12-29
       Author: xxxx 00000
  Description: 功能处理函数原型
        Input: IN const CHAR *pcInputStr    输入字符串
       Output: 
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution: 1.输入字符串在某些功能处理时可以忽略
               2.失败时需要返回具体错误码，供调用层次打印用户提示信息
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
typedef ULONG (*INFO_PROC_PF)(IN const CHAR *pcInputStr);

/* "添加"、"删除"、"修改"、"显示" */
ULONG INFO_proc_Add(IN const CHAR *pcInputStr);
ULONG INFO_proc_Delete(IN const CHAR *pcInputStr);
ULONG INFO_proc_Modify(IN const CHAR *pcInputStr);
ULONG INFO_proc_Display(IN const CHAR *pcInputStr);

/* 退出 */
ULONG INFO_proc_Exit(IN const CHAR *pcInputStr);

/* 调试开关操作 */
ULONG INFO_proc_OpenDebug(IN const CHAR *pcInputStr);
ULONG INFO_proc_CloseDebug(IN const CHAR *pcInputStr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_PROC_H_ */

