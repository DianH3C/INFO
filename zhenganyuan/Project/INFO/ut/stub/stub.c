#ifdef __cplusplus
extern "C"{
#endif

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

#include <limits.h>    /* for PAGESIZE */
#include <sys/basetype.h>
#include "stub.h"

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

struct func_stub
{
    void *fn;
    char buf[5];
};

#define STUB_MAX   64

void stub_set(struct func_stub *stub, void *fn, void *fn_stub)
{
    stub->fn = fn;
    memcpy(&stub->buf, fn, 5);

    if(mprotect((void *)((int)fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
    *(char *)fn = (char)0xE9;
    *(int *)((char *)fn +1) = (char *)fn_stub - (char *)fn - 5;
    if(mprotect((void *)((int)fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
}

void stub_reset(struct func_stub *stub)
{
    if(mprotect((void *)((int)stub->fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
    memcpy(stub->fn, &stub->buf, 5);
    if(mprotect((void *)((int)stub->fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
}

STATIC struct func_stub g_astDldpFunc[STUB_MAX];
STATIC UINT g_uiFuncIndex;

VOID stub_init_all(VOID)
{
    memset(g_astDldpFunc, 0, sizeof(g_astDldpFunc));
    g_uiFuncIndex = 0;
    return;
}

VOID stub_set_one(IN VOID *pOldFn, IN VOID *pNewFn)
{
    if ((NULL == pOldFn) || (NULL == pNewFn))
    {
        return;
    }

    if (g_uiFuncIndex < STUB_MAX)
    {
        stub_set(&(g_astDldpFunc[g_uiFuncIndex]), pOldFn, pNewFn);
        g_uiFuncIndex++;
    }
    else
    {
        printf("\r\n stub_set ERROR!! \r\n");
    }

    return;
}

VOID stub_reset_all(VOID)
{
    int i;
    for(i = 0; i < STUB_MAX; i++)
    {
        if (NULL != g_astDldpFunc[i].fn)
        {
            stub_reset(&(g_astDldpFunc[i]));
        }
    }
    g_uiFuncIndex = 0;
    return;
}

static CHAR* g_acTmp[2][1024] = {0};

VOID *stub_malloc_ok(size_t ulLen)
{
    IGNORE_PARAM(ulLen);
    memset(g_acTmp[0], 0, 1024);
    return (VOID *)g_acTmp[0];
}

VOID *stub_malloc_ok_ng(size_t ulLen)
{
    static int iCount = 0;

    if (iCount == 0)
    {
        iCount++;
        memset(g_acTmp[0], 0, 1024);
        return (VOID *)g_acTmp[0];
    }
    else
    {
        iCount = 0;
        return NULL;
    }
}

VOID *stub_malloc_ok_ng_ok(size_t ulLen)
{
    static int iCount = 0;

    if (iCount == 0)
    {
        iCount++;
        memset(g_acTmp[0], 0, 1024);
        return (VOID *)g_acTmp[0];
    }

    if (1 == iCount)
    {
        iCount++;
        return NULL;
    }
    memset(g_acTmp[1], 0, 1024);
    return (VOID *)g_acTmp[1];

}

VOID *stub_malloc_ng(size_t ulLen)
{
    IGNORE_PARAM(ulLen);
    return NULL;
}

#ifdef __cplusplus
}
#endif /* end of __cplusplus */


