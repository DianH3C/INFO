/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
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

#ifdef __cplusplus
extern "C"{
#endif

/* standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_parse.h"
#include "info_data.h"


/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 显示
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:

       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
ULONG INFO_proc_Display(IN const CHAR *pcInputStr)
{
    // display 10 user's information per page
    INT iOnePage = 10;
    if(INFO_data_IsEmpty())
    {
        printf("No info.\r\n");
        return ERROR_FAILED;
    }
    INFO_CFG_S * pstUserCfg = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));

    UINT uiId = INFO_data_GetFirst();

    do
    {
        printf("1 for female, and 2 for male.\r\n");
        if(ERROR_SUCCESS == INFO_data_GetData(uiId,&pstUserCfg))
        {
            printf("ID\tNAME\tSEX\tAGE\tHEIGHT\r\n");
            printf("%u\t%s\t%d\t%u\t%u\r\n",pstUserCfg->uiId,pstUserCfg->szName,pstUserCfg->enSex,pstUserCfg->uiAge,pstUserCfg->uiHeight);
        }
        else
        {
            return ERROR_FAILED;
        }
    
        uiId = INFO_data_GetNext(uiId);

    }while(uiId != INFO_ID_INVALID);

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 添加
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Add(IN const CHAR *pcInputStr)
{
    INFO_CFG_S * pstCfg=(INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    if(NULL == pstCfg)
    {
        return ERROR_NO_ENOUGH_RESOURCE;
    }
    INFO_parse_InputStr(pcInputStr,pstCfg);
    
    //better to tell user which datum is invalid
    if(!INFO_ALL_ISVALID(pstCfg))
    {   
        if(!INFO_ID_ISVALID(pstCfg->uiId))
        {
            printf("Sorry, the id you input are invalid.\r\n");
        }
        if(!INFO_AGE_ISVALID(pstCfg->uiAge))
        {
            printf("Sorry, the age you input are invalid.\r\n");
        }
        if(!INFO_SEX_ISVALID(pstCfg->enSex))
        {
            printf("Sorry, the sex you input are invalid.\r\n");
        }
        if(!INFO_HEIGHT_ISVALID(pstCfg->uiHeight))
        {
            printf("Sorry, the height you input are invalid.\r\n");
        }
        if(!INFO_NAME_ISVALID(pstCfg->szName))
        {
            printf("Sorry, the name you input are invalid.\r\n");
        }
        return ERROR_INVALID_PARAMETER;    
    }

    UINT uiId = pstCfg->uiId;
    if(INFO_data_IsExist(uiId))
    {
        printf("Sorry, id %u exists already, you can not add it.\r\n",uiId);
        return ERROR_ALREADY_EXIST;
    }

    INFO_data_Create(uiId);

    INFO_data_SetName(uiId,pstCfg->szName);
    INFO_data_SetAge(uiId,pstCfg->uiAge);
    INFO_data_SetHeight(uiId,pstCfg->uiHeight);
    INFO_data_SetSex(uiId,pstCfg->enSex);
    
    free(pstCfg);
    pstCfg = NULL;
    return ERROR_SUCCESS;


}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 删除
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Delete(IN const CHAR *pcInputStr)
{
    INFO_CFG_S * pstCfg = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));
    INFO_parse_InputStr(pcInputStr,pstCfg);
    UINT uiId = pstCfg->uiId;
    if(!INFO_ID_ISVALID(uiId))
    {
        printf("The id %u you want to delete is invalid!\r\n",pstCfg->uiId);
        return ERROR_INVALID_PARAMETER;
    }

    if(!INFO_data_IsExist(uiId))
    {
        printf("The id %u you want to delete does not exist!\r\n",uiId);
        return ERROR_SUCCESS;
    }

    INFO_data_Destroy(uiId);

    free(pstCfg);
    pstCfg = NULL;
    return ERROR_SUCCESS;

}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 修改
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution: 支持仅修改部分配置数据
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Modify(IN const CHAR *pcInputStr)
{

    INFO_CFG_S * pstCfg = (INFO_CFG_S*)malloc(sizeof(INFO_CFG_S));

    INFO_parse_InputStr(pcInputStr,pstCfg);

    UINT uiId = pstCfg->uiId;

    if(!INFO_ID_ISVALID(uiId))
    {
        printf("The id you want to modify is invalid!\r\n");
        return ERROR_INVALID_PARAMETER;
     }

    if(!INFO_data_IsExist(uiId))
    {
        printf("The id %u you want to modify does not exist!\r\n",uiId);
        return ERROR_NOT_FOUND;
    }

    if(INFO_SEX_ISVALID(pstCfg->enSex))
    {
        INFO_data_SetSex(uiId,pstCfg->enSex);
    }
    if(INFO_AGE_ISVALID(pstCfg->uiAge))
    {
        INFO_data_SetAge(uiId,pstCfg->uiAge);
    }
    if(INFO_NAME_ISVALID(pstCfg->szName))
    {
        INFO_data_SetName(uiId,pstCfg->szName);
    }
    if(INFO_HEIGHT_ISVALID(pstCfg->uiHeight))
    {
        INFO_data_SetHeight(uiId,pstCfg->uiHeight);
    }

    free(pstCfg);
    pstCfg = NULL;
    return ERROR_SUCCESS;

}

/*****************************************************************************
    Func Name: INFO_proc_Exit
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 退出
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Exit(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_OpenDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 打开调试开关
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_OpenDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_TRUE);

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_CloseDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: 关闭调试开关
        Input: IN const CHAR *pcInputStr    输入字符串
       Output:
       Return: ULONG, ERROR_SUCCESS         处理成功
                      OTHER                 处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_CloseDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_FALSE);

    return ERROR_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
