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
VOID info_data_Free(VOID * pstMem);


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




VOID INFO_data_SetName(IN UINT uiId, IN CHAR * szName)
{

    strlcpy(info_data_Get(uiId)->stCfg.szName,szName,INFO_NAME_MAXLEN);
/*
    INFO_DATA_S ** pstUser = (INFO_DATA_S**)malloc(sizeof(INFO_DATA_S*));
    *pstUser = (info_data_Get(uiId));

    strlcpy((*pstUser)->stCfg.szName,szName,sizeof((*pstUser)->stCfg.szName));
    info_data_Free(pstUser);
*/
    /*
    int i;
    if(pstUser)
    {
        for(i=0;i<strlen(szName);i++)
        {
            pstUser->stCfg.szName[i] = szName[i];
        }
        pstUser->stCfg.szName[i] = '\0';
        info_data_Free(pstUser);
    }
    else
    {
        info_data_Free(pstUser);
            printf("Failed to set name, something goes wrong...");
    }
    */
}

VOID INFO_data_SetAge(IN UINT uiId, IN UINT uiAge)
{   
    info_data_Get(uiId)->stCfg.uiHeight = uiAge;
/*
    INFO_DATA_S ** pstUser = (INFO_DATA_S**)malloc(sizeof(INFO_DATA_S*));
    *pstUser = (info_data_Get(uiId));

    (*pstUser)->stCfg.uiAge = uiAge;
    info_data_Free(pstUser);*/
}

VOID INFO_data_SetSex(IN UINT uiId, IN INFO_SEX_E enSex)
{
    info_data_Get(uiId)->stCfg.enSex = enSex;
/*
    INFO_DATA_S ** pstUser = (INFO_DATA_S**)malloc(sizeof(INFO_DATA_S*));
    *pstUser = (info_data_Get(uiId));

    (*pstUser)->stCfg.enSex = enSex;
    info_data_Free(pstUser);
    */
}
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiHeight)
{

    info_data_Get(uiId)->stCfg.uiHeight = uiHeight;
    /*
    INFO_DATA_S ** pstUser = (INFO_DATA_S**)malloc(sizeof(INFO_DATA_S*));
    *pstUser = (info_data_Get(uiId));

    (*pstUser)->stCfg.uiHeight = uiHeight;
    info_data_Free(pstUser);
    */
}

BOOL_T INFO_data_IsEmpty()
{

    if(DTQ_IsEmpty(pstDataHead))
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
}//end INFO_data_IsEmpty()


ULONG INFO_data_Create(IN UINT uiId)
{

    INFO_DATA_S * pstNewUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
//    memcpy(pstNewUser,info_data_Alloc(),sizeof(INFO_DATA_S));
    pstNewUser = info_data_Alloc();

    //initialize the pstNewUser
    (*pstNewUser).stNode.pstPrev = &((*pstNewUser).stNode);
    (*pstNewUser).stNode.pstNext = &((*pstNewUser).stNode);
     
    (*pstNewUser).stCfg.uiId = (UINT)0;
    (*pstNewUser).stCfg.uiAge = (UINT)0;
    (*pstNewUser).stCfg.uiHeight = (UINT)0;
    (*pstNewUser).stCfg.enSex = 0;
    (*pstNewUser).stCfg.szName[0] = 0;
    
    info_data_Add(pstNewUser,uiId);
    
    /*
    if(INFO_data_IsEmpty())
    {
        printf("Failed to reate user node, something goes wrong.\r\n"); 
        return ERROR_FAILED;
    }
    else
    {
        printf("Succeded to create user node.\r\n");
        return ERROR_SUCCESS;
    }
    */
}

VOID INFO_data_Destroy(UINT uiId)
{

    INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    pstUser= info_data_Get(uiId);
    if(NULL != pstUser)
    {
        info_data_Delete(pstUser);

        info_data_Free(pstUser);
    }

    else
    {   
        return;
    }
}//end INFO_data_Destroy()





UINT INFO_data_GetFirst()
{
    
    INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    pstUser = DTQ_ENTRY_FIRST(pstDataHead,INFO_DATA_S,stNode);
    UINT uiId = pstUser->stCfg.uiId;
    if(!INFO_data_IsExist(uiId))
    {
        uiId = INFO_ID_INVALID;
    }
    info_data_Free(pstUser);
    return uiId;
}//end INFO_data_GetFirst()


UINT INFO_data_GetNext(IN UINT uiId)
{
    if(INFO_data_IsExist(uiId))
    {
        INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
        pstUser = info_data_Get(uiId);
        INFO_DATA_S * pstNextUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));

    
        //if pstUser is not the last node, say not equal to NULL,  then return the next id
        pstNextUser = DTQ_ENTRY_NEXT(pstDataHead,pstUser,stNode);
        if(NULL != pstNextUser)
        {
            uiId = (*pstNextUser).stCfg.uiId;
        }
        //else pstUser is the last node, then no next id
        else
        {
            uiId = INFO_ID_INVALID;
        }
        info_data_Free(pstUser);
        info_data_Free(pstNextUser);
        return uiId;
    }
    else
    {
        return INFO_ID_INVALID;
    }
}

//end INFO_data_GetNext()



ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    pstUser = info_data_Get(uiId);
    
    if(NULL != pstUser)
    {
        memcpy(pstCfg,&(pstUser->stCfg),sizeof(INFO_CFG_S));
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}//end of INFO_data_GetData()








ULONG INFO_data_Init(VOID)
{
    pstDataHead = (DTQ_HEAD_S*)malloc(sizeof(DTQ_HEAD_S));
    pstDataNode = (DTQ_NODE_S*)malloc(sizeof(DTQ_HEAD_S));
    DTQ_Init(pstDataHead);
    
    return ERROR_SUCCESS;
  

}//end INFO_data_Init()


VOID INFO_data_Fini(VOID)
{
    if(INFO_data_Init())
    {
       //DTQ_FreeAll(pstDataHead,(* info_data_Free)());
        DTQ_NODE_S* pstCurNode;
        DTQ_NODE_S* pstNextNode;
        DTQ_FOREACH_SAFE(pstDataHead,pstCurNode,pstNextNode);
        {
            info_data_Free(pstCurNode);
        }
        info_data_Free(pstNextNode);
        DTQ_Init(pstDataHead);
        
        info_data_Free(pstDataHead);
        info_data_Free(pstDataNode);
    }
    else
    {
        return;
    }
}//end INFO_data_Fini()






VOID info_data_Add(IN INFO_DATA_S * pstUser, IN UINT uiId)
{

    //if empty, then the pstUser will added as the fisrt node.
    //and return 
    if(INFO_data_IsEmpty())
    {
        printf("Add the first data....\r\n");
        DTQ_AddHead(pstDataHead,&(pstUser->stNode));
        
        return;
    }

    /*
    INFO_DATA_S * pstEntry = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    INFO_DATA_S * pstNextEntry = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    */

    INFO_DATA_S * pstEntry ;
    INFO_DATA_S * pstNextEntry;
    DTQ_FOREACH_ENTRY_SAFE(pstDataHead,pstEntry,pstNextEntry,stNode)
    {
        
        if(NULL != pstNextEntry)
        {
            if(uiId >= pstEntry->stCfg.uiId && uiId < pstNextEntry->stCfg.uiId)
            {
                 ;
                DTQ_AddAfter(&(pstEntry->stNode),&(pstUser->stNode));
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            DTQ_AddTail(pstDataHead,&(pstUser->stNode));
         
        }
    }


    
}

//end info_data_Add()

VOID info_data_Delete(INFO_DATA_S * pstUser)
{
    DTQ_Del(&(pstUser->stNode));
}



INFO_DATA_S * info_data_Get(IN UINT uiId)
{
    INFO_DATA_S * pstUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    DTQ_FOREACH_ENTRY(pstDataHead,pstUser,stNode)
    {
        if(pstUser->stCfg.uiId == uiId)
        {
            pstDataNode = pstUser;
            info_data_Free(pstUser);
            return pstDataNode;
        }
    }
    return NULL;

    

}//end info_data_Get()



INFO_DATA_S * info_data_Alloc()
{
    INFO_DATA_S * pstNewNode = (INFO_DATA_S)malloc(sizeof(INFO_DATA_S));    
    memset(pstNewNode,0,sizeof(INFO_DATA_S));
    return pstNewNode;
}//end info_data_Alloc()

VOID info_data_Free(VOID * pstMem)
{
    free(pstMem);
    pstMem = NULL;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

