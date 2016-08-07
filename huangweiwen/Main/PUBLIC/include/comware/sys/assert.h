/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
-------------------------------------------------------------------------------- 
                            assert.h
  Project Code: Leopard
   Module Name:  
  Date Created: 2010-01-01
        Author: 
   Description: Comware Assert Definition
------------------------------------------------------------------------------  
Modification History
  DATE       NAME               DESCRIPTION
------------------------------------------------------------------------------
  
------------------------------------------------------------------------------ 
******************************************************************************/

#ifndef _SYS_ASSERT_H_
#define _SYS_ASSERT_H_

#ifdef  __cplusplus
extern "C" {
#endif


#ifdef _DEBUG_VERSION
#ifdef __KERNEL__
#define ASSERT_OUT    printk
#else
#include "stdio.h"
#include <execinfo.h>
#define ASSERT_OUT    printf
#endif

#ifndef __KERNEL__

#define MAX_STACK_DEPTH   32UL
#define PRINTNUM_PERLINE  4UL

#define BACKTRACE()                                                 \
do{                                                                 \
    if (NULL != backtrace)                                          \
    {                                                               \
        int iStackDep;                                              \
        int i;                                                      \
        void *apStackAddr[MAX_STACK_DEPTH];                         \
        iStackDep = backtrace(&apStackAddr[0], MAX_STACK_DEPTH);    \
        if (iStackDep > 0)                                          \
        {                                                           \
            printf("Call Stack:\n");                                \
                                                                    \
            for (i = 0; i < iStackDep; i++)                         \
            {                                                       \
                printf("%-19p",apStackAddr[i]);                     \
                if (((i + 1) % PRINTNUM_PERLINE) == 0)              \
                {                                                   \
                    printf("\n");                                   \
                }                                                   \
            }                                                       \
            if (i % PRINTNUM_PERLINE != 0)                          \
            {                                                       \
                printf("\n");                                       \
            }                                                       \
        }                                                           \
    }                                                               \
}while(0)    

#endif 

#ifdef __KERNEL__
#define DBGASSERT(expression)                                        \
    if (!(expression))                                                  \
    {                                                                   \
        ASSERT_OUT(KERN_EMERG "assertion failed:(%s), file: %s, line: %d:%s\n",    \
            #expression, __FILE__, __LINE__, __FUNCTION__);                \
        dump_stack();       \
    }
#else
#define DBGASSERT(expression)                                        \
    if (!(expression))                                                  \
    {                                                                   \
        ASSERT_OUT("assertion failed:(%s), file: %s, line: %d:%s\n",    \
            #expression, __FILE__, __LINE__, __FUNCTION__);             \
        BACKTRACE();                                                    \
    }
#endif
#else
#define DBGASSERT(expression)  (VOID)(expression)
#endif

#ifdef  __cplusplus
}
#endif

#endif  /* _SYS_ASSERT_H_ */

