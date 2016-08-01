/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info.h
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2011-12-29
        Author: guce 04152
   Description: INFO模块对外接口和内部公共定义

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD  

*******************************************************************************/

#ifndef _INFO_H_
#define _INFO_H_

#ifdef __cplusplus
extern "C"{
#endif

/*************************************** 公共定义 ***************************************/
/* 工号 */
#define INFO_ID_MIN                     1U
#define INFO_ID_MAX                     100000U
#define INFO_ID_INVALID                 0U
#define INFO_ID_ISVALID(uiId)           ((INFO_ID_MIN <= (uiId)) && ((uiId) <= INFO_ID_MAX))

/* 姓名 */
#define INFO_NAME_MAXLEN                15UL
#define INFO_NAME_ISVALID(pcName)       (0 != ((pcName)[0]))

/* 性别 */
typedef enum tagInfo_Sex
{
    INFO_SEX_FEMALE  = 1,
    INFO_SEX_MALE    = 2
}INFO_SEX_E;
#define INFO_SEX_ISVALID(enSex)         ((INFO_SEX_FEMALE == (enSex)) || (INFO_SEX_MALE == (enSex)))

/* 年龄 */
#define INFO_AGE_MIN                    18U
#define INFO_AGE_MAX                    65U
#define INFO_AGE_ISVALID(uiAge)         ((INFO_AGE_MIN <= (uiAge)) && ((uiAge) <= INFO_AGE_MAX))

/* 身高 */
#define INFO_HEIGHT_MIN                 1U
#define INFO_HEIGHT_MAX                 300U
#define INFO_HEIGHT_ISVALID(uiHeight)   ((INFO_HEIGHT_MIN <= (uiHeight)) && ((uiHeight) <= INFO_HEIGHT_MAX))

/* 配置数据 */
typedef struct tagInfo_Cfg
{
    UINT uiId;                          /* 工号 */
    UINT uiAge;                         /* 年龄 */
    UINT uiHeight;                      /* 身高 */
    INFO_SEX_E enSex;                   /* 性别 */
    CHAR szName[INFO_NAME_MAXLEN + 1];  /* 姓名 */
}INFO_CFG_S;

/* 全部数据合法 */
#define INFO_ALL_ISVALID(pstCfg) \
            (INFO_ID_ISVALID((pstCfg)->uiId) && \
             INFO_NAME_ISVALID((pstCfg)->szName) && \
             INFO_SEX_ISVALID((pstCfg)->enSex) && \
             INFO_AGE_ISVALID((pstCfg)->uiAge) && \
             INFO_HEIGHT_ISVALID((pstCfg)->uiHeight))

/* 输入输出缓冲区最大长度 */
#define INFO_IOBUF_MAXLEN               512UL

/*************************************** 对外接口 ***************************************/
/* 初始化 */
ULONG INFO_Init(VOID);
/* 运行 */
VOID INFO_Run(VOID);
/* 退出 */
VOID INFO_Fini(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_H_ */

