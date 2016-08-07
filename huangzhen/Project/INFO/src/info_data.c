/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ?ú2?êy?Y2ù×÷
                °üà¨′′?¨?￠é?3y?￠éè???￠??è??￠±éàú

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

/* D??￠êy?Y?á11 */
typedef struct tagInfo_Data
{
    /* êy?Y×é?ˉ?à1?[*] */
    DTQ_NODE_S stNode;
    INFO_CFG_S stCfg;       /* ????êy?Y */
}INFO_DATA_S;


INFO_DATA_S * info_data_Get(IN UINT uiId);
INFO_DATA_S * info_data_Alloc(VOID);
VOID info_data_Add(IN INFO_DATA_S * pstUser, IN UINT uiId);
VOID info_data_Delete(IN INFO_DATA_S * pstUser);
VOID info_data_Free(VOID * pstMem);


static DTQ_HEAD_S * pstList;

//set the pstUserGet to return or IN argument value as user structure pointer
INFO_DATA_S * pstUserGet;

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

}

VOID INFO_data_SetAge(IN UINT uiId, IN UINT uiAge)
{
    info_data_Get(uiId)->stCfg.uiAge = uiAge;
}

VOID INFO_data_SetSex(IN UINT uiId, IN INFO_SEX_E enSex)
{
    info_data_Get(uiId)->stCfg.enSex = enSex;

}
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiHeight)
{

    info_data_Get(uiId)->stCfg.uiHeight = uiHeight;

}

BOOL_T INFO_data_IsEmpty(VOID)
{

    if(DTQ_IsEmpty(pstList))
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
}//end INFO_data_IsEmpty()


VOID INFO_data_Create(IN UINT uiId)
{

    INFO_DATA_S * pstNewUser;
    pstNewUser = info_data_Alloc();

    //initialize the pstNewUser
    (*pstNewUser).stNode.pstPrev = &((*pstNewUser).stNode);
    (*pstNewUser).stNode.pstNext = &((*pstNewUser).stNode);

    (*pstNewUser).stCfg.uiId = uiId;
    (*pstNewUser).stCfg.uiAge = (UINT)0;
    (*pstNewUser).stCfg.uiHeight = (UINT)0;
    (*pstNewUser).stCfg.enSex = 0;
    (*pstNewUser).stCfg.szName[0] = 0;

    pstUserGet = pstNewUser;
    info_data_Add(pstUserGet,uiId);
}

VOID INFO_data_Destroy(UINT uiId)
{
    INFO_DATA_S * pstDestroy = info_data_Get(uiId);
    info_data_Delete(pstDestroy);

    info_data_Free(pstDestroy);

}//end INFO_data_Destroy()





UINT INFO_data_GetFirst(VOID)
{
    pstUserGet = DTQ_ENTRY_FIRST(pstList,INFO_DATA_S,stNode);

    UINT uiId = pstUserGet->stCfg.uiId;

    if(INFO_data_IsExist(uiId))
    {
        return uiId;
    }
    else
    {
        return INFO_ID_INVALID;
    }
}//end INFO_data_GetFirst()


UINT INFO_data_GetNext(IN UINT uiId)
{
/*
    if(INFO_data_IsExist(uiId))
    {
        pstUserGet = info_data_Get(uiId);

        INFO_DATA_S * pstNextUser;
        //if pstUser is not the last node, say not equal to NULL,  then return the next id
        pstNextUser = DTQ_ENTRY_NEXT(pstList,pstUserGet,stNode);
        if(NULL != pstNextUser)
        {
            return pstNextUser->stCfg.uiId;
        }
        //else pstUser is the last node, then no next id
        else
        {
            return INFO_ID_INVALID;
        }
    }*/

    INFO_DATA_S * pstEntry;
    DTQ_FOREACH_ENTRY(pstList,pstEntry,stNode)
    {
        if(uiId < pstEntry->stCfg.uiId)
        {
            return pstEntry->stCfg.uiId;
        }
    }  
    return INFO_ID_INVALID;
}

//end INFO_data_GetNext()



ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S ** ppstCfg)
{
    pstUserGet = info_data_Get(uiId);

    if(NULL != pstUserGet)
    {
       *ppstCfg = &(pstUserGet->stCfg);
       return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}//end of INFO_data_GetData()





ULONG INFO_data_Init(VOID)
{
    pstList = (DTQ_HEAD_S*)malloc(sizeof(DTQ_HEAD_S));
    pstUserGet = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    DTQ_Init(pstList);

    return ERROR_SUCCESS;
}//end INFO_data_Init()


VOID INFO_data_Fini(VOID)
{
    if(INFO_data_Init())
    {

        UINT uiIdForDestroy;
        /*
        INFO_FOREACH(uiIdForDestroy)
        {
            INFO_data_Destroy(uiIdForDestroy);
        }*/


        for ((uiIdForDestroy) = INFO_data_GetFirst(); \
     (uiIdForDestroy) != INFO_ID_INVALID; \
     (uiIdForDestroy)  = INFO_data_GetNext(uiIdForDestroy))
        {
            INFO_data_Destroy(uiIdForDestroy);
        }
        info_data_Free(pstList);
        info_data_Free(pstUserGet);
    }
}//end INFO_data_Fini()






VOID info_data_Add(IN INFO_DATA_S * pstUser, IN UINT uiId)
{

    //if empty, then the pstUser will added as the fisrt node.
    //and return
    if(INFO_data_IsEmpty())
    {
        DTQ_AddHead(pstList,&(pstUser->stNode));

        return;
    }

    INFO_DATA_S * pstEntry ;
    INFO_DATA_S * pstNextEntry;

    DTQ_FOREACH_ENTRY_SAFE(pstList,pstEntry,pstNextEntry,stNode)
    {  
        //if current entry is the last node, then pstNextEntry is NULL, so add to list tail
        if(NULL == pstNextEntry)
        {
            if(uiId >= pstEntry->stCfg.uiId)
            {
                DTQ_AddAfter(&(pstEntry->stNode),&(pstUser->stNode));
                break;
            }
            else
            {
                DTQ_AddBefore(&(pstEntry->stNode),&(pstUser->stNode));
                break;
            }
        }
        if(NULL != pstNextEntry)
        {
            //make the prev uiId is smaller or equal, and the next uiId is bigger.
            if(uiId >= pstEntry->stCfg.uiId && uiId < pstNextEntry->stCfg.uiId)
            {
                DTQ_AddAfter(&(pstEntry->stNode),&(pstUser->stNode));
                break;
            }
            else
            {
                continue;
            }
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
    INFO_DATA_S * pstEntry;
    DTQ_FOREACH_ENTRY(pstList,pstEntry,stNode)
    {
        if(pstEntry->stCfg.uiId == uiId)
        {
            pstUserGet = pstEntry;
            return pstUserGet;
        }
    }
    return NULL;

}//end info_data_Get()



INFO_DATA_S * info_data_Alloc(VOID)
{
    INFO_DATA_S * pstNewUser = (INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    memset(pstNewUser,0,sizeof(INFO_DATA_S));
    return pstNewUser;
}//end info_data_Alloc()

VOID info_data_Free(VOID * pstMem)
{
    free(pstMem);
    pstMem = NULL;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
