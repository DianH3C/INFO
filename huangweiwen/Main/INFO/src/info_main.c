/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_main.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2011-12-29
        Author: guce 04152
   Description:

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

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_data.h"
#include "info_proc.h"

/* 菜单功能 */
typedef enum tagInfo_Menu
{
    INFO_MENU_UNSPEC,
    INFO_MENU_ADD,              /* 添加 */
    INFO_MENU_DELETE,           /* 删除 */
    INFO_MENU_MODIFY,           /* 修改 */
    INFO_MENU_DISPLAY,          /* 显示 */
    INFO_MENU_EXIT,             /* 退出 */
    INFO_MENU_OPEN_DEBUG,       /* 打开调试开关 */
    INFO_MENU_CLOSE_DEBUG,      /* 关闭调试开关 */
    INFO_MENU_MAX
}INFO_MENU_E;
#define INFO_MENU_ISVALID(enMenu)   ((INFO_MENU_UNSPEC < (enMenu)) && ((enMenu) < INFO_MENU_MAX))

/* 功能数据结构 */
typedef struct tagInfo_Func
{
    const CHAR      *pcMenu;    /* 菜单名称 */
    INFO_PROC_PF    pfProc;     /* 处理函数 */
}INFO_FUNC_S;

/* 功能数据 */
STATIC INFO_FUNC_S g_astInfoFunc[] =
{
    [INFO_MENU_ADD]         = {"Add",           INFO_proc_Add},
    [INFO_MENU_DELETE]      = {"Delete",        INFO_proc_Delete},
    [INFO_MENU_MODIFY]      = {"Modify",        INFO_proc_Modify},
    [INFO_MENU_DISPLAY]     = {"Display",       INFO_proc_Display},
    [INFO_MENU_EXIT]        = {"Exit",          INFO_proc_Exit},
    [INFO_MENU_OPEN_DEBUG]  = {"Open debug",    INFO_proc_OpenDebug},
    [INFO_MENU_CLOSE_DEBUG] = {"Close debug",   INFO_proc_CloseDebug}
};

/*****************************************************************************
    Func Name: info_main_DisplayMenu
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 显示菜单
        Input:
       Output:
       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID info_main_DisplayMenu(VOID)
{
    INFO_MENU_E enEach;

    /* 显示每个菜单条目 */
    printf("\r\n");
    for (enEach = (INFO_MENU_E)(INFO_MENU_UNSPEC + 1); enEach < INFO_MENU_MAX; enEach++)
    {
        printf("%u.%s\r\n", enEach, g_astInfoFunc[enEach].pcMenu);
    }
    printf("\r\n");

    return;
}

/*****************************************************************************
    Func Name: info_main_DisplayErrInfo
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 输出错误提示信息
        Input: IN ULONG ulErrCode   错误码
       Output:
       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID info_main_DisplayErrInfo(IN ULONG ulErrCode)
{
    const CHAR *pcErrInfo;

    /* 区分错误提示信息 */
    switch (ulErrCode)
    {
        case ERROR_INVALID_PARAMETER:
        {
            pcErrInfo = "The parameter is incorrect.\r\n";
            break;
        }
        case ERROR_NOT_FOUND:
        {
            pcErrInfo = "The specified item was not found.\r\n";
            break;
        }
        case ERROR_ALREADY_EXIST:
        {
            pcErrInfo = "The item already exists.\r\n";
            break;
        }
        case ERROR_NOTSUPPORT:
        {
            pcErrInfo = "The operation is not supported.\r\n";
            break;
        }
        case ERROR_NO_ENOUGH_RESOURCE:
        {
            pcErrInfo = "Not enough resources are available to complete the operation.\r\n";
            break;
        }
        default:
        {
            pcErrInfo = "The operation completed unsuccessfully.\r\n";
            break;
        }
    }

    /* 输出 */
    printf("%s", pcErrInfo);

    return;
}

/*****************************************************************************
    Func Name: info_main_GetMenuChoice
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 和用户交互，获取"菜单选择"
        Input:
       Output: OUT INFO_MENU_E *penMenu 菜单选择
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution: 1.出参仅在处理成功时有效
               2.处理成功时，"菜单选择"已经保证为合法性
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC ULONG info_main_GetMenuChoice(OUT INFO_MENU_E *penMenu)
{
    CHAR szBuf[INFO_IOBUF_MAXLEN];
    INFO_MENU_E enMenu;
    ULONG ulErrCode;

    DBGASSERT(NULL != penMenu);

    /* 交互信息 */
    printf("Your choice:");

    /* 获取"菜单选择" */
    szBuf[0] = 0;
    (VOID)fgets(szBuf, (INT)sizeof(szBuf), stdin);

    /* 解析成功 */
    if ((1 == sscanf(szBuf, "%u", &enMenu)) && (INFO_MENU_ISVALID(enMenu)))
    {
        *penMenu = enMenu;

        INFO_DBG_PRINT("User choice %u.", enMenu);
        ulErrCode = ERROR_SUCCESS;
    }
    else /* 解析失败 */
    {
        INFO_DBG_PRINT("Input parse failed.");
        ulErrCode = ERROR_INVALID_PARAMETER;
    }

    return ulErrCode;
}

/*****************************************************************************
    Func Name: info_main_GetInputStr
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 根据菜单选择和用户交互，获取"输入字符串"
        Input: IN INFO_MENU_E enMenu    菜单选择
               IN ULONG ulInputLen      输入字符串缓冲区大小
       Output: OUT CHAR *pcInputStr     输入字符串
       Return: VOID
      Caution: "输入字符串"的合法性需要后续具体功能函数进行检查
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID info_main_GetInputStr(IN INFO_MENU_E enMenu, IN ULONG ulInputLen, OUT CHAR *pcInputStr)
{
    DBGASSERT(INFO_MENU_ISVALID(enMenu));
    DBGASSERT(0 < ulInputLen);
    DBGASSERT(NULL != pcInputStr);

    pcInputStr[0] = 0;

    /* 分别获取"输入字符串" */
    switch (enMenu)
    {
        /* 需要输入全部信息 */
        case INFO_MENU_ADD:             /* 添加 */
        {
            printf("Format :id=(1-100000) name=(len<=15) sex=(F=1,M=2) age=(18-65) height=(1-300)\r\n");
            printf("Example:id=10 name=jack sex=2 age=20 height=175\r\n");
            printf("Input Info:");
            (VOID)fgets(pcInputStr, (INT)ulInputLen, stdin);
            break;
        }
        /* 需要输入ID */
        case INFO_MENU_DELETE:          /* 删除 */
        {
            printf("Format :id=(1-100000)\r\n");
            printf("Example:id=10\r\n");
            printf("Input Id:");
            (VOID)fgets(pcInputStr, (INT)ulInputLen, stdin);
            break;
        }
        /* 需要输入ID+部分信息 */
        case INFO_MENU_MODIFY:          /* 修改 */
        {
            printf("Format :id=(1-100000) [name=(len<=15) sex=(F=1,M=2) age=(18-65) height=(1-300)]*\r\n");
            printf("Example:id=10 name=jack sex=2 \r\n");
            printf("Example:id=10 age=20 height=175\r\n");
            printf("Input Info:");
            (VOID)fgets(pcInputStr, (INT)ulInputLen, stdin);
            break;
        }
        /* 不需要输入 */
        case INFO_MENU_DISPLAY:         /* 显示 */
        case INFO_MENU_EXIT:            /* 退出 */
        case INFO_MENU_OPEN_DEBUG:      /* 打开调试开关 */
        case INFO_MENU_CLOSE_DEBUG:     /* 关闭调试开关 */
        {
            break;
        }
        default:
        {
            DBGASSERT(0);
            break;
        }
    }

    return;
}

/*****************************************************************************
    Func Name: INFO_Run
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 运行
        Input:
       Output:
       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_Run(VOID)
{
    ULONG ulErrCode;
    INFO_MENU_E enMenu;
    CHAR szInputStr[INFO_IOBUF_MAXLEN];

    /* 运行主体框架 */
    do
    {
        /* 显示菜单 */
        info_main_DisplayMenu();

        /* 获取"菜单选择" */
        enMenu = INFO_MENU_UNSPEC;
        szInputStr[0] = 0;
        ulErrCode = info_main_GetMenuChoice(&enMenu);
        if (ERROR_SUCCESS == ulErrCode)
        {
            /* 获取"输入字符串" */
            info_main_GetInputStr(enMenu, sizeof(szInputStr), szInputStr);

            /* 执行相应操作 */
            DBGASSERT(NULL != g_astInfoFunc[enMenu].pfProc);
            ulErrCode = g_astInfoFunc[enMenu].pfProc(szInputStr);
        }

        /* 输出提示信息 */
        if (ERROR_SUCCESS != ulErrCode)
        {
            info_main_DisplayErrInfo(ulErrCode);
        }
    }while (INFO_MENU_EXIT != enMenu); /* 不退出则继续 */

    return;
}

/*****************************************************************************
    Func Name: INFO_Init
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 初始化
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_Init(VOID)
{
    ULONG ulErrCode;

    ulErrCode = INFO_data_Init();

    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_Fini
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 退出
        Input:
       Output:
       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_Fini(VOID)
{
    INFO_data_Fini();

    return;
}

/*****************************************************************************
    Func Name: main
 Date Created: 2011-12-29
       Author: guce 04152
  Description: 程序入口
        Input: IN INT iArgc         入参个数
               IN CHAR *apcArgv[]   入参指针
       Output:
       Return: INT, ERROR_SUCCESS   处理成功
                    OTHER           处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INT main(IN INT iArgc, IN CHAR *apcArgv[])
{
    ULONG ulErrCode;

    IGNORE_PARAM(iArgc);
    IGNORE_PARAM(apcArgv);

    /* 初始化 */
    ulErrCode = INFO_Init();
    if (ERROR_SUCCESS == ulErrCode)
    {
        /* 运行 */
        INFO_Run();
    }

    /* 退出 */
    INFO_Fini();

    return (INT)(ulErrCode);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

