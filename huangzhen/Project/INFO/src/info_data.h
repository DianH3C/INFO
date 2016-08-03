/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.h
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ÄÚ²¿Êý¾Ý²Ù×÷
                °üÀ¨´´½¨¡¢É¾³ý¡¢ÉèÖÃ¡¢»ñÈ¡¡¢±éÀú

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

/*************************************** Êý¾Ý²Ù×÷ ***************************************/
/* ÅÐ¶ÏÖ¸¶¨¹¤ºÅµÄÊý¾ÝÊÇ·ñ´æÔÚ */
BOOL_T INFO_data_IsExist(IN UINT uiId);
VOID INFO_data_Create(IN UINT uiId);
VOID INFO_data_SetName(IN UINT uiId, IN CHAR * szName);
VOID INFO_data_SetAge(IN UINT uiId, IN UINT uiAge);
VOID INFO_data_SetSex(IN UINT uiId, IN INFO_SEX_E enSex);
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiHeight);
VOID INFO_data_Destroy(UINT uiId);

/* »ñÈ¡Êý¾Ý */
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg);

/*************************************** ±éÀú ***************************************/
/* ÅÐ¶ÏÕû¸öÊý¾Ý×éÖ¯ÊÇ·ñÎª¿Õ */
BOOL_T INFO_data_IsEmpty(VOID);

/* »ñÈ¡"µÚÒ»¸ö/ÏÂÒ»¸ö"ÓÐÊý¾Ý¹¤ºÅ */
UINT INFO_data_GetFirst(VOID);
UINT INFO_data_GetNext(IN UINT uiId);
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg);
/* ±éÀú½Ó¿Ú */
#define INFO_FOREACH(uiId) \
for ((uiId)  = INFO_data_GetFirst(); \
     (uiId) != INFO_ID_INVALID; \
     (uiId)  = INFO_data_GetNext(uiId))

/*************************************** ³õÊ¼»¯ ***************************************/
ULONG INFO_data_Init(VOID);
VOID INFO_data_Fini(VOID);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_DATA_H_ */

