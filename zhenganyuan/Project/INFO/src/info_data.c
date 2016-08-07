/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: �ڲ����ݲ���
                ����������ɾ�������á���ȡ������

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
#define RED BOOL_TRUE
#define BLACK BOOL_FALSE
#define VOLUME 100000
/* ��Ϣ���ݽṹ */
typedef struct tagInfo_Data
{
	BOOL_T bColor;
	struct tagInfo_Data *pstLeft;
	struct tagInfo_Data *pstRight;
    INFO_CFG_S stCfg;
}INFO_DATA_S;

INFO_DATA_S *g_pstRoot,*g_pstTmp;

STATIC BOOL_T IsRed(INFO_DATA_S *pNode)
{
	if (!pNode)
		return BOOL_FALSE;
	return pNode->bColor == RED;
}

STATIC INFO_DATA_S *RotateLeft(INFO_DATA_S *pNode)
{
	INFO_DATA_S *x = pNode->pstRight;
	pNode->pstRight = x->pstLeft;
	x->pstLeft = pNode;
	x->bColor = pNode->bColor;
	pNode->bColor = RED;
	return x;
}

STATIC INFO_DATA_S *RotateRight(INFO_DATA_S *pNode)
{
	INFO_DATA_S *x = pNode->pstLeft;
	pNode->pstLeft = x->pstRight;
	x->pstRight = pNode;
	x->bColor = pNode->bColor;
	pNode->bColor = RED;
	return x;
}

STATIC VOID FlipColors(INFO_DATA_S *pNode)
{
	pNode->bColor = !(pNode->bColor);
	pNode->pstLeft->bColor = !(pNode->pstLeft->bColor);
	pNode->pstRight->bColor = !(pNode->pstRight->bColor);
}

STATIC INFO_DATA_S *MoveRedLeft(INFO_DATA_S *pNode)
{
	// Assuming that p is red and both p->pstLeft and p->pstLeft->pstLeft
	// are black, make p->pstLeft or one of its children red
	FlipColors(pNode);
	if (IsRed(pNode->pstRight->pstLeft)) {
		pNode->pstRight = RotateRight(pNode->pstRight);
		pNode = RotateLeft(pNode);
		FlipColors(pNode);
	}
	return pNode;
}

STATIC INFO_DATA_S *MoveRedRight(INFO_DATA_S *pNode)
{
	// Assuming that p is red and both p->pstRight and p->pstRight->pstLeft
	// are black, make p->pstRight or one of its children red
	FlipColors(pNode);
	if (IsRed(pNode->pstLeft->pstLeft)) {
		pNode = RotateRight(pNode);
		FlipColors(pNode);
	}
	return pNode;
}

STATIC INFO_DATA_S *FixUp(INFO_DATA_S *pNode)
{
	if (IsRed(pNode->pstRight))
		pNode = RotateLeft(pNode);

	if (IsRed(pNode->pstLeft) && IsRed(pNode->pstLeft->pstLeft))
		pNode = RotateRight(pNode);

	if (IsRed(pNode->pstLeft) && IsRed(pNode->pstRight)) // four node
		FlipColors(pNode);

	return pNode;
}

STATIC INFO_DATA_S *getInOrderSuccessorINFO_DATA_S(INFO_DATA_S *pNode)
{
	pNode = pNode->pstRight;

	while (pNode->pstLeft != NULL) {

		pNode = pNode->pstLeft;
	}

	return pNode;
}

STATIC VOID FlipColors2(INFO_DATA_S *pNode)
{
	pNode->bColor = BLACK;
	pNode->pstLeft->bColor = RED;
	pNode->pstRight->bColor = RED;
}

STATIC VOID Init(INFO_DATA_S **ppRoot)
{
	*ppRoot = NULL;
}

STATIC INFO_DATA_S *CreateNode(INFO_CFG_S stValue , BOOL_T bColor)
{
	INFO_DATA_S *tmp = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    tmp->stCfg=stValue;
	tmp->bColor = bColor;
	tmp->pstLeft = tmp->pstRight = NULL;
	return tmp;
}

STATIC INFO_DATA_S *insert(INFO_DATA_S *r,INFO_CFG_S stValue)
{
	if (!r)
	{
		r = CreateNode(stValue, RED);
		return r;
	}
	if (stValue.uiId < r->stCfg.uiId)
		r->pstLeft = insert(r->pstLeft, stValue);
	else if (stValue.uiId > r->stCfg.uiId)
		r->pstRight = insert(r->pstRight, stValue);
	else
		r->stCfg = stValue;
	if (!IsRed(r->pstLeft) && IsRed(r->pstRight))
		r = RotateLeft(r);
	if (IsRed(r->pstLeft) && IsRed(r->pstLeft->pstLeft))
		r = RotateRight(r);
	if (IsRed(r->pstLeft) && IsRed(r->pstRight))
		FlipColors(r);
	return r;
}

STATIC VOID Insert(INFO_DATA_S **ppRoot, INFO_CFG_S stValue)
{
	*ppRoot = insert(*ppRoot, stValue);
	(*ppRoot)->bColor = BLACK;
}

STATIC INFO_DATA_S *AtTheRootRotate(INFO_DATA_S *h)
{
	INFO_DATA_S *x = h->pstRight->pstLeft;
	INFO_DATA_S *y = h->pstRight;
	h->pstRight = x->pstLeft;
	x->pstLeft = h;
	y->pstLeft = x->pstRight;
	x->pstRight = y;
	h->pstLeft->bColor = RED;
	h->bColor = BLACK;
	x->bColor = BLACK;
	return x;
}

STATIC INFO_DATA_S *OnTheWayRotate1(INFO_DATA_S *h)
{
	INFO_DATA_S *x = h->pstRight->pstLeft;
	INFO_DATA_S *y = h->pstRight;
	h->pstRight = x->pstLeft;
	x->pstLeft = h;
	y->pstLeft = x->pstRight;
	x->pstRight = y;
	x->bColor = RED;
	h->bColor = BLACK;
	h->pstLeft->bColor = RED;
	return x;
}

STATIC INFO_DATA_S *OnTheWayRotate2(INFO_DATA_S *h)
{
	h->bColor = BLACK;
	h->pstLeft->bColor = RED;
	h->pstRight->bColor = RED;
	return h;
}

STATIC INFO_DATA_S *delete_min(INFO_DATA_S *r)
{
	if (r->pstLeft->pstLeft == NULL)
	{
		free(r->pstLeft);
		r->pstLeft = NULL;
	}
	else
	{
		INFO_DATA_S *next_pstLeft = r->pstLeft->pstLeft;
		INFO_DATA_S *next_pstRight = r->pstLeft->pstRight;
		// on the way down ���һ��ʲôҲ������
		if (IsRed(next_pstLeft->pstLeft))
		{
			r->pstLeft->pstLeft = delete_min(r->pstLeft->pstLeft);
		}
		//pstLeft��һ��2-node && pstRight��һ��3-node,��һ����ȥ
		else if (IsRed(next_pstRight->pstLeft))
		{
			r->pstLeft = OnTheWayRotate1(r->pstLeft);
			r->pstLeft->pstLeft = delete_min(r->pstLeft->pstLeft);
		}
		//pstLeft��pstRight����2-node,��������һ�������ϲ���4-node
		else
		{
			r->pstLeft = OnTheWayRotate2(r->pstLeft);
			r->pstLeft = delete_min(r->pstLeft);
		}
	}
		if (!IsRed(r->pstLeft) && IsRed(r->pstRight))
			r = RotateLeft(r);
		if (IsRed(r->pstLeft) && IsRed(r->pstRight))
			r = RotateRight(r);
		if (IsRed(r->pstLeft) && IsRed(r->pstRight))
			FlipColors(r);
		return r;
}

STATIC INFO_DATA_S *Find_Min(INFO_DATA_S *pRoot)
{
	if (!pRoot)
		return NULL;
	if (!(pRoot->pstLeft))
		return pRoot;
	return Find_Min(pRoot->pstLeft);
}

STATIC VOID Delete_Min(INFO_DATA_S **root)
{
	if (!(*root))
		return;
	//ֻ��һ��Ԫ�أ�ֱ��ɾ��
	if (!((*root)->pstLeft))
	{
		free(*root);
		*root = NULL;
		return;
	}
	//ֻ������Ԫ��
	if (IsRed((*root)->pstLeft))
		*root = delete_min(*root);
	//����������Ԫ��
	else if (IsRed((*root)->pstLeft->pstLeft))
		(*root)->pstLeft = delete_min((*root)->pstLeft);
	//at the root ���һ
	else if (!IsRed((*root)->pstRight->pstLeft))
	{
		FlipColors2(*root);
		*root = delete_min(*root);
	}
	//at the root �����
	else
	{
		*root = AtTheRootRotate(*root);
		(*root)->pstLeft = delete_min((*root)->pstLeft);
	}
}

STATIC INFO_DATA_S *Remove(INFO_DATA_S *p, int uiId)
{
	if (uiId < p->stCfg.uiId) {
		if (!IsRed(p->pstLeft) && !IsRed(p->pstLeft->pstLeft)) {
			p = MoveRedLeft(p);
		}
		p->pstLeft = Remove(p->pstLeft, uiId);
	}
	else {
		if (IsRed(p->pstLeft)) {
			p = RotateRight(p);
		}
		if ((uiId == p->stCfg.uiId) && (p->pstRight == NULL)) {
			return NULL;
		}
		if (!IsRed(p->pstRight) && !IsRed(p->pstRight->pstLeft)) {
			p = MoveRedRight(p);
		}
		if (uiId == p->stCfg.uiId) {
			/* added instead of code above */
			INFO_DATA_S *successor = getInOrderSuccessorINFO_DATA_S(p);
			p->stCfg = successor->stCfg;
			Delete_Min(&p->pstRight);
		}
		else {
			p->pstRight = Remove(p->pstRight, uiId);
		}
	}
	return FixUp(p);
}

STATIC VOID Destroy(INFO_DATA_S *pstTree)
{
    if(pstTree)
    {
        Destroy(pstTree->pstLeft);
        Destroy(pstTree->pstRight);
        free(pstTree);
    }
}

STATIC VOID display(INFO_DATA_S *r)
{
	if (r)
	{
		display(r->pstLeft);
        printf("%-8u%-8s%-8s%-8u%-8u\n", r->stCfg.uiId,r->stCfg.szName,\
               (r->stCfg.enSex==1?"female":"male"),r->stCfg.uiAge,r->stCfg.uiHeight);
		display(r->pstRight);
	}
}

VOID Traverse(INFO_DATA_S *pstNode,UINT *puiIndex,UINT *puiArr)
{
    if(pstNode)
    {
        Traverse(pstNode->pstLeft,puiIndex,puiArr);
        puiArr[*puiIndex]=pstNode->stCfg.uiId;
        ++(*puiIndex);
        Traverse(pstNode->pstRight,puiIndex,puiArr);
    }
}

INFO_DATA_S *find(INFO_DATA_S *pNode, UINT uiId)
{
    if (pNode)
    {
        if (pNode->stCfg.uiId == uiId)
            return pNode;
        else if (pNode->stCfg.uiId > uiId)
            return find(pNode->pstLeft, uiId);
        else
            return find(pNode->pstRight, uiId);
    }
    return NULL;
}

INFO_DATA_S *Find(UINT uiId)
{
    return find(g_pstRoot,uiId);
}

VOID INFO_data_Display(VOID)
{
    display(g_pstRoot);
}

VOID INFO_data_Insert(INFO_CFG_S *pstCfg)
{
    Insert(&g_pstRoot,*pstCfg);
}

BOOL_T INFO_data_Delete(IN UINT uiId)
{
    INFO_DATA_S *pstTmp=Find(uiId);
    if(NULL==pstTmp)
        return BOOL_FALSE;
    g_pstRoot=Remove(g_pstRoot,uiId);
    return BOOL_TRUE;
}

VOID INFO_data_Modify(IN INFO_CFG_S *pstCfg)
{
    INFO_DATA_S *pstTmp=Find(pstCfg->uiId);
    if(INFO_AGE_ISVALID(pstCfg->uiAge))
        pstTmp->stCfg.uiAge;
    if(INFO_HEIGHT_ISVALID(pstCfg->uiHeight))
        pstTmp->stCfg.uiHeight=pstCfg->uiHeight;
    if(INFO_SEX_ISVALID(pstCfg->enSex))
        pstTmp->stCfg.enSex=pstCfg->enSex;
    if(INFO_NAME_ISVALID(pstCfg->szName))
        strlcpy(pstTmp->stCfg.szName,pstCfg->szName,sizeof(pstCfg->szName));
}
/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �ж�ָ�����ŵ������Ƿ����
        Input: IN UINT uiId         ����
       Output:
       Return: BOOL_T, BOOL_TRUE    ����
                       BOOL_T_FALSE   ������
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsExist(IN UINT uiId)
{
    if (Find(uiId)==NULL)
        return BOOL_FALSE;
    return BOOL_TRUE;
}

/*****************************************************************************
    Func Name: INFO_data_IsEmpty[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �ж�����������֯�Ƿ�Ϊ��
        Input:
       Output:
       Return: BOOL_T, BOOL_TRUE    ������֯Ϊ��
                       BOOL_T_FALSE   ������֯�ǿ�
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_IsEmpty(VOID)
{
    return g_pstRoot == NULL;
}

/*****************************************************************************
    Func Name: INFO_data_GetData[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ��ȡ��������
        Input: IN UINT uiId             ����
       Output: OUT INFO_CFG_S *pstCfg   ��������
       Return: ULONG, ERROR_SUCCESS     ����ɹ�
                      OTHER             ����ʧ��
      Caution: ���ν��ڷ��سɹ�ʱ��Ч
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    INFO_DATA_S *tmp = Find(uiId);
    if (tmp)
    {
        *pstCfg = tmp->stCfg;
        return ERROR_SUCCESS;
    }
    return ERROR_FAILED;
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ��ȡ��һ�������ݹ���
        Input: VOID
       Output:
       Return: UINT, != INFO_ID_INVALID     ��һ�������ݵĹ���
                     == INFO_ID_INVALID     δ�ҵ�
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetFirst(VOID)
{
    INFO_DATA_S *pstFirst=Find_Min(g_pstRoot);
    if(NULL==pstFirst)
        return INFO_ID_INVALID;
    return pstFirst->stCfg.uiId;
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ��ȡ��һ�������ݹ���
        Input: IN UINT uiId                 ��ǰ����
       Output:
       Return: UINT, != INFO_ID_INVALID     ��һ������
                     == INFO_ID_INVALID     δ�ҵ�
      Caution: �˽ӿڻ�ȡ��һ�����Ų����������uiId�����Ƿ�������
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetNext(IN UINT uiId)
{
    UINT auiArr[VOLUME];
    UINT uiIndex=0;
    Traverse(g_pstRoot,&uiIndex,auiArr);
    UINT i=0;
    for(;i<uiIndex && auiArr[i]!=uiId;++i)
        ;
    if(uiIndex!=0 && i<uiIndex-1)
        return auiArr[i+1];
    return INFO_ID_INVALID;
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ģ���ʼ��
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     ����ɹ�
                      OTHER             ����ʧ��
      Caution: Ŀǰʼ�ճɹ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    g_pstRoot=NULL;
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ģ���˳�
        Input:
       Output:
       Return: VOID
      Caution: ���ô˽ӿ�ǰ�������Ѿ���ʼ����
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Fini(VOID)
{
    Destroy(g_pstRoot);
    g_pstRoot=NULL;
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
