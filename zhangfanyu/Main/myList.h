/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            list.h
  Project Code: Comware Leopard
   Module Name:
  Date Created: 2008-3-28
        Author: c02254
   Description: This file defines four types of data structures: singly-linked
                lists, singly-linked tail queues, doubly-linked lists and
                doubly-linked tail queues.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
2009-8-5      Y01604       Add RCU supporting
*****************************************************************************/
#ifndef _SYS_LIST_H_
#define _SYS_LIST_H_

#ifdef  __cplusplus
extern "C"{
#endif

/*
 * Singly-linked List
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
                     +--------------+      +--------------+
                     |user structure|      |user structure|
                     +--------------+      +--------------+
                     |   ......     |      |   ......     |
                     +--------------+      +--------------+
 +------------+  +-->|+------------+|  +-->|+------------+|
 | SL_HEAD_S  |  |   || SL_NODE_S  ||  |   || SL_NODE_S  ||
 +------------+  |   |+------------+|  |   |+------------+|
 | pstFirst   |--+   || pstNext    ||--+   || pstNext    ||----+
 +------------+      |+------------+|      |+------------+|   -+-
                     +--------------+      +--------------+
                     |   ......     |      |   ......     |
                     +--------------+      +--------------+
 */
typedef struct tagSL_NODE
{
    struct tagSL_NODE* pstNext;  /* the next element */
} SL_NODE_S;

#define SL_ENTRY(ptr, type, member) (container_of(ptr, type, member))

typedef struct tagSL_HEAD
{
    SL_NODE_S* pstFirst;
} SL_HEAD_S;

static inline VOID SL_Init(IN SL_HEAD_S* pstList);
static inline VOID SL_NodeInit(IN SL_NODE_S* pstNode);
static inline BOOL_T SL_IsEmpty(IN SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_First(IN SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_Next(IN SL_NODE_S* pstNode);
static inline VOID SL_AddHead(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode);
static inline SL_NODE_S* SL_DelHead(IN SL_HEAD_S* pstList);
static inline VOID SL_AddAfter(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst);
static inline SL_NODE_S* SL_DelAfter(IN SL_HEAD_S* pstList,
                                     IN SL_NODE_S* pstPrev);
static inline VOID SL_Del(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode);
static inline VOID SL_Append(IN SL_HEAD_S* pstDstList, IN SL_HEAD_S* pstSrcList);
static inline VOID SL_FreeAll(IN SL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INFO_PROC_H_ */


