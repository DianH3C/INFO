/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.h
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: 内部数据操作
                包括创建、删除、设置、获取、遍历

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/

#ifndef _INFO_DATA_H_
#define _INFO_DATA_H_

#ifdef __cplusplus
extern "C"{
#endif

/*************************************** 数据操作 ***************************************/
/* 判断指定工号的数据是否存在 */
BOOL_T INFO_data_IsExist(IN UINT uiId);

/* 创建数据 */
BOOL_T INFO_data_Create(IN UINT uiId);

/* 获取数据 */
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg);

/* 根据ID设置姓名 */
VOID INFO_data_SetName(IN UINT uiId,IN CHAR *szNAME);

/* 根据ID设置年龄 */
VOID INFO_data_SetAge(IN UINT uiId,IN UINT uiAge);

/* 根据ID设置身高 */
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiHeight);

/* 根据ID设置性别 */
VOID INFO_data_SetSex(IN UINT uiId,IN INFO_SEX_E enSex);

/* 获取相应工号对应节点指针 */
INFO_DATA_NODE * info_data_Get(IN UINT uiId);


/*************************************** 遍历 ***************************************/
/* 判断整个数据组织是否为空 */
BOOL_T INFO_data_IsEmpty(VOID);

/* 获取"第一个/下一个"有数据工号 */
UINT INFO_data_GetFirst(VOID);
UINT INFO_data_GetNext(IN UINT uiId);

/* 遍历接口 */
#define INFO_FOREACH(uiId) \
for ((uiId)  = INFO_data_GetFirst(); \
     (uiId) != INFO_ID_INVALID; \
     (uiId)  = INFO_data_GetNext(uiId))

/*************************************** 初始化 ***************************************/
ULONG INFO_data_Init(VOID);
VOID INFO_data_Fini(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_DATA_H_ */

