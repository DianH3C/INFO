/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
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

#ifdef __cplusplus
extern "C"{
#endif

/* standard library */
#include <stdlib.h>
#include <string.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"

/* ÐÅÏ¢Êý¾Ý½á¹¹ */
typedef struct tagInfo_Data
{
    /* Êý¾Ý×éÖ¯Ïà¹Ø[*] */
    DTQ_NODE_S stNode;
    INFO_CFG_S stCfg;       /* ÅäÖÃÊý¾Ý */
}INFO_DATA_S;


INFO_DATA_S * info_data_Get(IN UINT uiId);
INFO_DATA_S * info_data_Alloc();
VOID info_data_Add(IN INFO_DATA_S * pstUser, IN UINT uiId);
VOID info_data_Delete(INFO_DATA_S * pstUser);
VOID info_data_Free(INFO_DATA_S * pstUser);


static DTQ_HEAD_S * pstDataHead; 
static INFO_DATA_S * pstDataNode;

/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÅÐ¶ÏÖ¸¶¨¹¤ºÅµÄÊý¾ÝÊÇ·ñ´æÔÚ
        Input: IN UINT uiId         ¹¤ºÅ
       Output: 
       Return: BOOL_T, BOOL_TRUE    ´æÔÚ
                       BOOL_FALSE   ²»´æÔÚ
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsExist(IN UINT uiId)
{
    //not exist when info_data_Get() return NULL
    if(NULL == info_data_Get(uiId))
    {
        return BOOL_FALSE;
    }
    else
    {
        return BOOL_TRUE;
    }
}


VOID INFO_data_Create(IN UINT uiId)
{
    //ÉêÇëÐÂ½ÚµãµÄÄÚ´æ¿Õ¼ä
    INFO_DATA_S * pstUser = info_data_Alloc();
    //³õÊ¼»¯¸Ã½Úµã
    (*pstUser).stNode.pstPrev = &((*pstUser).stNode);
    (*pstUser).stNode.pstNext = &((*pstUser).stNode);
     
    (*pstUser).stCfg.uiId = (UINT)0;
    (*pstUser).stCfg.uiAge = (UINT)0;
    (*pstUser).stCfg.uiHeight = (UINT)0;
    (*pstUser).stCfg.enSex = INFO_SEX_FEMALE;
    (*pstUser).stCfg.szName[0] = 0;
    //½«¸Ã½Úµã¼ÓÈëÁ´±íÖÐ    
    info_data_Add(pstUser,uiId);
    ;
}

VOID INFO_data_SetName(IN UINT uiId, IN CHAR * szName)
{

    
    //»ñÈ¡Èë²Î¹¤ºÅ¶ÔÓ¦½ÚµãÖ¸Õë
    INFO_DATA_S * pstUser = (INFO_DATA_S*)info_data_Get(uiId);
    //¸ü¸ÄÐÕÃû
    
    int i;
    for(i=0;i<strlen(szName);i++)
    {
        pstUser->stCfg.szName[i] = szName[i];
    }
    pstUser->stCfg.szName[i] = '\0';
    //strlcpy(pstUser->stCfg.szName,szName,sizeof(pstUser->stCfg.szName));
    
}

VOID INFO_data_SetAge(IN UINT uiId, IN UINT uiAge)
{   
    //»ñÈ¡Èë²Î¹¤ºÅ¶ÔÓ¦½ÚµãÖ¸Õë
    INFO_DATA_S * pstUser = info_data_Get(uiId);
    //¸ü¸ÄÄêÁä
    (*pstUser).stCfg.uiAge = uiAge;

}
VOID INFO_data_SetSex(IN UINT uiId, IN INFO_SEX_E enSex)
{

    //»ñÈ¡Èë²Î¹¤ºÅ¶ÔÓ¦½ÚµãÖ¸Õë
    INFO_DATA_S * pstUser = info_data_Get(uiId);
    //¸ü¸ÄÐÔ±ð
    (*pstUser).stCfg.enSex = enSex;
}
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiHeight)
{
    //»ñÈ¡Èë²Î¹¤ºÅ¶ÔÓ¦½ÚµãÖ¸Õë
    INFO_DATA_S * pstUser = info_data_Get(uiId);
    //¸ü¸ÄÉí¸ß
    (*pstUser).stCfg.uiHeight = uiHeight;
}

//删除入参工号对应的链表节点
VOID INFO_data_Destroy(UINT uiId)
{
    //»ñÈ¡´ýÉ¾³ý½ÚµãµØÖ·
    INFO_DATA_S * pstUser;
    if(pstUser = info_data_Get(uiId))
    {
        //´ÓÁ´±íÖÐÕª³ý¸Ã½Úµã
    info_data_Delete(pstUser);
    //ÊÍ·ÅÄÚ´æ¿Õ¼ä
        info_data_Free(pstUser);
    }
    //Èô¸Ã¹¤ºÅÎÞÐÅÏ¢Ôò·µ»Ø
    else
    {
        return;
    }
}//end INFO_data_Destroy()

/*****************************************************************************
    Func Name: INFO_data_IsEmpty[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ÅÐ¶ÏÕû¸öÊý¾Ý×éÖ¯ÊÇ·ñÎª¿Õ
        Input: 
       Output: 
       Return: BOOL_T, BOOL_TRUE    Êý¾Ý×éÖ¯Îª¿Õ
                       BOOL_FALSE   Êý¾Ý×éÖ¯·Ç¿Õ
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsEmpty(VOID)
{
    DTQ_NODE_S  * pstHead = &((*pstDataHead).stHead);
    
    //both null when not initialized or point to itself when initialized 
    //if (((*pstHead).pstNext==NULL && (*pstHead).pstPrev==NULL)||((*pstHead).pstNext==pstHead && (*pstHead).pstPrev==pstHead)))
    if(DTQ_IsEmpty(pstDataHead))
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
}//end INFO_data_IsEmpty()


/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: »ñÈ¡µÚÒ»¸öÓÐÊý¾Ý¹¤ºÅ
        Input: VOID
       Output: 
       Return: UINT, != INFO_ID_INVALID     µÚÒ»¸öÓÐÊý¾ÝµÄ¹¤ºÅ
                     == INFO_ID_INVALID     Î´ÕÒµ½
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetFirst(VOID)
{
    
    INFO_DATA_S * pstFirstUser = DTQ_ENTRY_FIRST(pstDataHead,INFO_DATA_S,stNode);
    UINT uiId = (*pstFirstUser).stCfg.uiId;
    if(INFO_data_IsExist(uiId))
    {
        return uiId;
    }
    else
    {
        return INFO_ID_INVALID;
    }
}//end INFO_data_GetFirst()


/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: »ñÈ¡ÏÂÒ»¸öÓÐÊý¾Ý¹¤ºÅ
        Input: IN UINT uiId                 µ±Ç°¹¤ºÅ
       Output: 
       Return: UINT, != INFO_ID_INVALID     ÏÂÒ»¸ö¹¤ºÅ
                     == INFO_ID_INVALID     Î´ÕÒµ½
      Caution: ´Ë½Ó¿Ú»ñÈ¡ÏÂÒ»¸ö¹¤ºÅ²»ÒÀÀµÓÚÈë²ÎuiId±¾ÉíÊÇ·ñÓÐÊý¾Ý
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetNext(IN UINT uiId)
{
    if(INFO_data_IsExist(uiId))
    {
        INFO_DATA_S * pstUser = info_data_Get(uiId);
        INFO_DATA_S * pstNextUser;

        //if pstUser is not the last node, then return the next id
        if(pstNextUser = DTQ_ENTRY_NEXT(pstDataHead,pstUser,stNode))
        {
            return (*pstNextUser).stCfg.uiId;
        }
        //else pstUser is the last node, then no next id
        else
        {
            return INFO_ID_INVALID;
        }
    }
    else
    {
        return INFO_ID_INVALID;
    }
}

//end INFO_data_GetNext()



/*****************************************************************************
    Func Name: INFO_data_GetData[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: »ñÈ¡ÅäÖÃÊý¾Ý
        Input: IN UINT uiId             ¹¤ºÅ
       Output: OUT INFO_CFG_S *pstCfg   ÅäÖÃÊý¾Ý
       Return: ULONG, ERROR_SUCCESS     ´¦Àí³É¹¦
                      OTHER             ´¦ÀíÊ§°Ü
      Caution: ³ö²Î½öÔÚ·µ»Ø³É¹¦Ê±ÓÐÐ§
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    INFO_DATA_S * pstUser;
    if(pstUser = info_data_Get(uiId))
    {
        pstCfg = &((*pstUser).stCfg);
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}//end of INFO_data_GetData()








/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: Ä£¿é³õÊ¼»¯
        Input: 
       Output: 
       Return: ULONG, ERROR_SUCCESS     ´¦Àí³É¹¦
                      OTHER             ´¦ÀíÊ§°Ü
      Caution: Ä¿Ç°Ê¼ÖÕ³É¹¦
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    pstDataHead = (DTQ_HEAD_S*)malloc(sizeof(DTQ_NODE_S));
    pstDataNode = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    DTQ_Init(pstDataHead);
    /*(*pstDataHead).stHead.pstPrev = &((*pstDataHead).stHead);
    (*pstDataHead).stHead.pstNext = &((*pstDataHead).stHead);
*/
     return ERROR_SUCCESS;
  

}//end INFO_data_Init()


/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: Ä£¿éÍË³ö
        Input: 
       Output: 
       Return: VOID
      Caution: µ÷ÓÃ´Ë½Ó¿ÚÇ°£¬±ØÐëÒÑ¾­³õÊ¼»¯¹ý
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Fini(VOID)
{
    if(INFO_data_Init())
    {
        free(pstDataHead);
        pstDataHead = NULL;
        free(pstDataNode);
        pstDataNode = NULL;
    }
    else
    {
        return;
    }
}//end INFO_data_Fini()









/*****************************************************************************
    Func Name: info_data_Allcoc[*]
 Date Created: 201x-xx-xx
       Author: Huangzhen
  Description: ÉêÇëÄÚ´æ²¢½«ÉêÇëµ½µÄÄÚ´æ¿Õ¼äÇåÁã£¬²¢·µ»Ø¿Õ¼äÊ×µØÖ·
        Input: VOID
       Output: 
       Return: INFO_DATA_S *,     
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INFO_DATA_S * info_data_Alloc()
{
    INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    memset(pstUser,0,sizeof(INFO_DATA_S));
    return pstUser;
}//end info_data_Alloc()



VOID info_data_Add(IN INFO_DATA_S * pstUser, IN UINT uiId)
{
    INFO_DATA_S * pstEntry, *pstNextEntry;
    DTQ_NODE_S * pstNode;
    INFO_DATA_S * pstLoopUser, * pstLoopNextUser;
    
    //if empty, then the pstUser will added as the fisrt node.
    //and return 
    if(INFO_data_IsEmpty())
    {
        DTQ_AddHead(pstDataHead,(&(*pstUser).stNode));
        return;
    }
    
    DTQ_FOREACH_ENTRY_SAFE(pstDataHead,pstEntry,pstNextEntry,stNode)
    {
        UINT uiId = (*pstUser).stCfg.uiId;
        if(pstNextEntry!=NULL)
        {
            if(uiId >= (*pstEntry).stCfg.uiId && uiId < (*pstNextEntry).stCfg.uiId)
            {
            DTQ_AddAfter((&(*pstEntry).stNode),&((*pstUser).stNode));
            }
        }
        else
        {
            DTQ_AddAfter((&(*pstEntry).stNode),&((*pstUser).stNode));
         
        }
    }
}

//end info_data_Add()

INFO_DATA_S * info_data_Get(IN UINT uiId)
{
    INFO_DATA_S * pstEntry = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));

    DTQ_FOREACH_ENTRY(pstDataHead,pstEntry,stNode)
    {
        if(pstEntry->stCfg.uiId == uiId)
        {
             return pstEntry;
        }
    }
    //end of the loop, no match, then return NULL
    return pstEntry;
    
}//end info_data_Get()


VOID info_data_Delete(INFO_DATA_S * pstUser)
{

    DTQ_Del(&((*pstUser).stNode));


    /*
    //INFO_DATA_S stNode = (*pstDataNode).stNode;
    //the inner function don't delete illegal argument, DTQ_HEAD_S* included.
    
    //if pstUser is the headdd of the list, then the pstPrevUser will be NULL
    INFO_DATA_S * pstPrevUser = DTQ_ENTRY_NEXT(pstDataHead,INFO_DATA_S,stNode);

    //if pstUser is the end of the list, then the pstNextUser will be NULL
    INFO_DATA_S * pstNextUser = DTQ_ENTRY_NEXT(pstDataHead,INFO_DATA_S,stNode);

    if(pstPrevUser == NULL && pstNextUser != NULL)
    {   
        (*pstDataHead).stHead.pstPrev= &((*pstNextUser).stNode);
        (*pstNextUser).stNode.pstPrev = &((*pstDataHead).stHead);
    }
    else if(pstPrevUser != NULL && pstNextUser == NULL)
    {
        (*pstPrevUser).stNode.pstNext = &((*pstDataHead).stHead);
        (*pstDataHead).stHead.pstPrev = &((*pstPrevUser).stNode);
    }
    //general condition
    else
    {
        (*pstPrevUser).stNode.pstNext = &((*pstNextUser).stNode);
        (*pstNextUser).stNode.pstPrev = &((*pstPrevUser).stNode);
    }
    */
}

VOID info_data_Free(INFO_DATA_S * pstUser)
{
    free(pstUser);
    pstUser = NULL;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

