/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ÄÚ²¿ÊýŸÝ²Ù×÷
                °üÀšŽŽœš¡¢ÉŸ³ý¡¢ÉèÖÃ¡¢»ñÈ¡¡¢±éÀú

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
/* ÐÅÏ¢ÊýŸÝœá¹¹ */
typedef struct tagInfo_Data
{
	BOOL_T bColor;
	struct tagInfo_Data *pstLeft;
	struct tagInfo_Data *pstRight;
    INFO_CFG_S stCfg;
}INFO_DATA_S;

INFO_DATA_S *g_pstRoot,*g_pstTmp;

/*****************************************************************************
    Func Name: IsRed
 Date Created: 2016-08-07
       Author: zay
  Description: ÅÐ¶ÏœáµãÊÇ·ñÎªºìÉ«
        Input: IN INFO_DATA_S
       Output:
       Return: VOID
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC BOOL_T IsRed(IN INFO_DATA_S *pNode)
{
	if (!pNode)
		return BOOL_FALSE;
	return pNode->bColor == RED;
}

/*****************************************************************************
    Func Name: RotateLeft
 Date Created: 2016-08-07
       Author: zay
  Description: ÈôœáµãµÄÓÒ×ÓœáµãÎªºì£¬ÔòœøÐÐÓÒÐý
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DADA_S *
      Caution: µ÷ÓÃŽËœÓ¿ÚÇ°£¬±ØÐëÒÑŸ­³õÊŒ»¯¹ý
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/

    
STATIC INFO_DATA_S *RotateLeft(IN INFO_DATA_S *pNode)
{
	INFO_DATA_S *x = pNode->pstRight;
	pNode->pstRight = x->pstLeft;
	x->pstLeft = pNode;
	x->bColor = pNode->bColor;
	pNode->bColor = RED;
	return x;
}

/*****************************************************************************
    Func Name: RotateRight
 Date Created: 2016-08-07
       Author: zay
  Description: ÈôœáµãµÄ×óœáµãºÍ×óœáµãµÄ×óœáµãÎªºì£¬ÔòœøÐÐÓÒÐý
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DADA_S *
      Caution: µ÷ÓÃŽËœÓ¿ÚÇ°£¬±ØÐëÒÑŸ­³õÊŒ»¯¹ý
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
    
STATIC INFO_DATA_S *RotateRight(IN INFO_DATA_S *pNode)
{
	INFO_DATA_S *x = pNode->pstLeft;
	pNode->pstLeft = x->pstRight;
	x->pstRight = pNode;
	x->bColor = pNode->bColor;
	pNode->bColor = RED;
	return x;
}

/*****************************************************************************
    Func Name: FlipColors
 Date Created: 2016-08-07
       Author: zay
  Description: ÈôœáµãµÄ×óœáµãºÍÓÒœáµãÎªºì£¬Ôò±ä»»ÑÕÉ«
        Input: IN INFO_DATA_S *
       Output:
       Return: VOID
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
    
STATIC VOID FlipColors(IN INFO_DATA_S *pNode)
{
	pNode->bColor = !(pNode->bColor);
	pNode->pstLeft->bColor = !(pNode->pstLeft->bColor);
	pNode->pstRight->bColor = !(pNode->pstRight->bColor);
}

/*****************************************************************************
    Func Name: MoveRedLeft
 Date Created: 2016-08-07
       Author: zay
  Description: make p->pstLeft or one of its children red
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
    
STATIC INFO_DATA_S *MoveRedLeft(IN INFO_DATA_S *pNode)
{
	FlipColors(pNode);
	if (IsRed(pNode->pstRight->pstLeft)) {
		pNode->pstRight = RotateRight(pNode->pstRight);
		pNode = RotateLeft(pNode);
		FlipColors(pNode);
	}
	return pNode;
}
    
/*****************************************************************************
    Func Name: MoveRedRight
 Date Created: 2016-08-07
       Author: zay
  Description: make pNode->pstRight or one of its children red
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
    
STATIC INFO_DATA_S *MoveRedRight(IN INFO_DATA_S *pNode)
{
	FlipColors(pNode);
	if (IsRed(pNode->pstLeft->pstLeft)) {
		pNode = RotateRight(pNode);
		FlipColors(pNode);
	}
	return pNode;
}
/*****************************************************************************
    Func Name: FixUp
 Date Created: 2016-08-07
       Author: zay
  Description: 保持红黑树的平衡
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *FixUp(IN INFO_DATA_S *pNode)
{
	if (IsRed(pNode->pstRight))
		pNode = RotateLeft(pNode);

	if (IsRed(pNode->pstLeft) && IsRed(pNode->pstLeft->pstLeft))
		pNode = RotateRight(pNode);

	if (IsRed(pNode->pstLeft) && IsRed(pNode->pstRight)) // four node
		FlipColors(pNode);

	return pNode;
}

/*****************************************************************************
    Func Name: getInOrderSuccessorINFO_DATA_S(
 Date Created: 2016-08-07
       Author: zay
  Description: 获得当前节点的前一个节点
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *getInOrderSuccessorINFO_DATA_S(IN INFO_DATA_S *pNode)
{
	pNode = pNode->pstRight;

	while (pNode->pstLeft != NULL) {

		pNode = pNode->pstLeft;
	}

	return pNode;
}

/*****************************************************************************
    Func Name: FlipColors2
 Date Created: 2016-08-07
       Author: zay
  Description: 保持红黑树平衡的一种操作
        Input: IN INFO_DATA_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID FlipColors2(IN INFO_DATA_S *pNode)
{
	pNode->bColor = BLACK;
	pNode->pstLeft->bColor = RED;
	pNode->pstRight->bColor = RED;
}

/*****************************************************************************
    Func Name: Init
 Date Created: 2016-08-07
       Author: zay
  Description: 初始化根节点
        Input: IN INFO_DATA_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID Init(IN INFO_DATA_S **ppRoot)
{
	*ppRoot = NULL;
}

/*****************************************************************************
    Func Name: CreateNode
 Date Created: 2016-08-07
       Author: zay
  Description: 为结点分配空间，并返回指针
        Input: IN INFO_CFG_S, IN BOOL_T
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *CreateNode(IN INFO_CFG_S stValue , IN BOOL_T bColor)
{
	INFO_DATA_S *tmp = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    tmp->stCfg=stValue;
	tmp->bColor = bColor;
	tmp->pstLeft = tmp->pstRight = NULL;
	return tmp;
}

/*****************************************************************************
    Func Name: insert
 Date Created: 2016-08-07
       Author: zay
  Description: 插入一个结点到红黑树中
        Input: IN INFO_DATA_S *, IN INFO_CFG_S
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *insert(IN INFO_DATA_S *r,IN INFO_CFG_S stValue)
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

/*****************************************************************************
    Func Name: FixUp
 Date Created: 2016-08-07
       Author: zay
  Description: 插入一个结点到红黑树中
        Input: IN INFO_DATA_S **, IN INFO_CFG_S
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID Insert(IN INFO_DATA_S **ppRoot, IN INFO_CFG_S stValue)
{
	*ppRoot = insert(*ppRoot, stValue);
	(*ppRoot)->bColor = BLACK;
}

/*****************************************************************************
    Func Name: AtTheRootRotate
 Date Created: 2016-08-07
       Author: zay
  Description: 插入算法需用到的一种操作
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *AtTheRootRotate(IN INFO_DATA_S *h)
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

/*****************************************************************************
    Func Name: OnTheWayRotate1
 Date Created: 2016-08-07
       Author: zay
  Description: 插入算法需用到的一种操作
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *OnTheWayRotate1(IN INFO_DATA_S *h)
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

/*****************************************************************************
    Func Name: OnTheWayRotate2
 Date Created: 2016-08-07
       Author: zay
  Description: 插入算法需用到的一种操作
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *OnTheWayRotate2(IN INFO_DATA_S *h)
{
	h->bColor = BLACK;
	h->pstLeft->bColor = RED;
	h->pstRight->bColor = RED;
	return h;
}

/*****************************************************************************
    Func Name: delete_min
 Date Created: 2016-08-07
       Author: zay
  Description: 删除红黑树的最小结点
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *delete_min(IN INFO_DATA_S *r)
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
		// on the way down Çé¿öÒ»£ºÊ²ÃŽÒ²²»ÓÃ×ö
		if (IsRed(next_pstLeft->pstLeft))
		{
			r->pstLeft->pstLeft = delete_min(r->pstLeft->pstLeft);
		}
		//pstLeftÊÇÒ»žö2-node && pstRightÊÇÒ»žö3-node,ÒÆÒ»žö¹ýÈ¥
		else if (IsRed(next_pstRight->pstLeft))
		{
			r->pstLeft = OnTheWayRotate1(r->pstLeft);
			r->pstLeft->pstLeft = delete_min(r->pstLeft->pstLeft);
		}
		//pstLeftºÍpstRight¶ŒÊÇ2-node,ŽÓÉÏÃæÒÆÒ»žöÏÂÀŽºÏ²¢³É4-node
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

/*****************************************************************************
    Func Name: Find_Min
 Date Created: 2016-08-07
       Author: zay
  Description: 返回最小结点
        Input: IN INFO_DATA_S *
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *Find_Min(IN INFO_DATA_S *pRoot)
{
	if (!pRoot)
		return NULL;
	if (!(pRoot->pstLeft))
		return pRoot;
	return Find_Min(pRoot->pstLeft);
}

/*****************************************************************************
    Func Name: Delete_Min
 Date Created: 2016-08-07
       Author: zay
  Description: 删除红黑树的最小结点
        Input: IN INFO_DATA_S **
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID Delete_Min(IN INFO_DATA_S **root)
{
	if (!(*root))
		return;
	//Ö»ÓÐÒ»žöÔªËØ£¬Ö±œÓÉŸ³ý
	if (!((*root)->pstLeft))
	{
		free(*root);
		*root = NULL;
		return;
	}
	//Ö»ÓÐÁœžöÔªËØ
	if (IsRed((*root)->pstLeft))
		*root = delete_min(*root);
	//ÖÁÉÙÓÐÈýžöÔªËØ
	else if (IsRed((*root)->pstLeft->pstLeft))
		(*root)->pstLeft = delete_min((*root)->pstLeft);
	//at the root Çé¿öÒ»
	else if (!IsRed((*root)->pstRight->pstLeft))
	{
		FlipColors2(*root);
		*root = delete_min(*root);
	}
	//at the root Çé¿ö¶þ
	else
	{
		*root = AtTheRootRotate(*root);
		(*root)->pstLeft = delete_min((*root)->pstLeft);
	}
}

/*****************************************************************************
    Func Name: Remove
 Date Created: 2016-08-07
       Author: zay
  Description: 删除指定ID所在结点
        Input: IN INFO_DATA_S *, IN UINT
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC INFO_DATA_S *Remove(IN INFO_DATA_S *p, IN UINT uiId)
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

/*****************************************************************************
    Func Name: Destroy
 Date Created: 2016-08-07
       Author: zay
  Description: 销毁树
        Input: INFO_DATA_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID Destroy(IN INFO_DATA_S *pstTree)
{
    if(pstTree)
    {
        Destroy(pstTree->pstLeft);
        Destroy(pstTree->pstRight);
        free(pstTree);
    }
}

/*****************************************************************************
    Func Name: display
 Date Created: 2016-08-07
       Author: zay
  Description: 显示树
        Input: IN INFO_DATA_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
STATIC VOID display(IN INFO_DATA_S *r)
{
	if (r)
	{
		display(r->pstLeft);
        printf("%-8u%-8s%-8s%-8u%-8u\n", r->stCfg.uiId,r->stCfg.szName,\
               (r->stCfg.enSex==1?"female":"male"),r->stCfg.uiAge,r->stCfg.uiHeight);
		display(r->pstRight);
	}
}

/*****************************************************************************
    Func Name: Traverse
 Date Created: 2016-08-07
       Author: zay
  Description: 遍历树，讲结点按顺序保存在数组中
        Input: IN INFO_DATA_S *pstNode,IN UINT *puiIndex,OUT UINT *puiArr
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID Traverse(IN INFO_DATA_S *pstNode,IN UINT *puiIndex,OUT UINT *puiArr)
{
    if(pstNode)
    {
        Traverse(pstNode->pstLeft,puiIndex,puiArr);
        puiArr[*puiIndex]=pstNode->stCfg.uiId;
        ++(*puiIndex);
        Traverse(pstNode->pstRight,puiIndex,puiArr);
    }
}

/*****************************************************************************
    Func Name: find
 Date Created: 2016-08-07
       Author: zay
  Description: 返回指定ID所在结点的指针
        Input: IN INFO_DATA_S *,IN UINT
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INFO_DATA_S *find(IN INFO_DATA_S *pNode,IN UINT uiId)
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

/*****************************************************************************
    Func Name: Find
 Date Created: 2016-08-07
       Author: zay
  Description: 返回指定ID所在结点的指针
        Input: IN UINT
       Output:
       Return: INFO_DATA_S *

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INFO_DATA_S *Find(IN UINT uiId)
{
    return find(g_pstRoot,uiId);
}

/*****************************************************************************
    Func Name: INFO_data_Display
 Date Created: 2016-08-07
       Author: zay
  Description: 显示所有数据
        Input: VOID
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Display(VOID)
{
    display(g_pstRoot);
}

/*****************************************************************************
    Func Name: INFO_data_Insert
 Date Created: 2016-08-07
       Author: zay
  Description: 插入数据到红黑树中
        Input: IN INFO_CFG_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Insert(IN INFO_CFG_S *pstCfg)
{
    Insert(&g_pstRoot,*pstCfg);
}

/*****************************************************************************
    Func Name: INFO_data_Delete
 Date Created: 2016-08-07
       Author: zay
  Description: 删除指定ID的数据
        Input: IN UINT
       Output:
       Return: BOOL_T, BOOL_FALSE 删除失败
                       BOOL_TRUE  删除成功
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
BOOL_T INFO_data_Delete(IN UINT uiId)
{
    INFO_DATA_S *pstTmp=Find(uiId);
    if(NULL==pstTmp)
        return BOOL_FALSE;
    g_pstRoot=Remove(g_pstRoot,uiId);
    return BOOL_TRUE;
}

/*****************************************************************************
    Func Name: INFO_data_Modify
 Date Created: 2016-08-07
       Author: zay
  Description: 修改指定id的数据
        Input: IN INFO_CFG_S *
       Output:
       Return: VOID

      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
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
  Description: ÅÐ¶ÏÖž¶š¹€ºÅµÄÊýŸÝÊÇ·ñŽæÔÚ
        Input: IN UINT uiId         ¹€ºÅ
       Output:
       Return: BOOL_T, BOOL_TRUE    ŽæÔÚ
                       BOOL_T_FALSE   ²»ŽæÔÚ
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
  Description: ÅÐ¶ÏÕûžöÊýŸÝ×éÖ¯ÊÇ·ñÎª¿Õ
        Input:
       Output:
       Return: BOOL_T, BOOL_TRUE    ÊýŸÝ×éÖ¯Îª¿Õ
                       BOOL_T_FALSE   ÊýŸÝ×éÖ¯·Ç¿Õ
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
  Description: »ñÈ¡ÅäÖÃÊýŸÝ
        Input: IN UINT uiId             ¹€ºÅ
       Output: OUT INFO_CFG_S *pstCfg   ÅäÖÃÊýŸÝ
       Return: ULONG, ERROR_SUCCESS     ŽŠÀí³É¹Š
                      OTHER             ŽŠÀíÊ§°Ü
      Caution: ³ö²ÎœöÔÚ·µ»Ø³É¹ŠÊ±ÓÐÐ§
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
  Description: »ñÈ¡µÚÒ»žöÓÐÊýŸÝ¹€ºÅ
        Input: VOID
       Output:
       Return: UINT, != INFO_ID_INVALID     µÚÒ»žöÓÐÊýŸÝµÄ¹€ºÅ
                     == INFO_ID_INVALID     ÎŽÕÒµœ
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
  Description: »ñÈ¡ÏÂÒ»žöÓÐÊýŸÝ¹€ºÅ
        Input: IN UINT uiId                 µ±Ç°¹€ºÅ
       Output:
       Return: UINT, != INFO_ID_INVALID     ÏÂÒ»žö¹€ºÅ
                     == INFO_ID_INVALID     ÎŽÕÒµœ
      Caution: ŽËœÓ¿Ú»ñÈ¡ÏÂÒ»žö¹€ºÅ²»ÒÀÀµÓÚÈë²ÎuiId±ŸÉíÊÇ·ñÓÐÊýŸÝ
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
  Description: Ä£¿é³õÊŒ»¯
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     ŽŠÀí³É¹Š
                      OTHER             ŽŠÀíÊ§°Ü
      Caution: Ä¿Ç°ÊŒÖÕ³É¹Š
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
 Date Created: 2016-08-07
       Author: zay
  Description: Ä£¿éÍË³ö
        Input:
       Output:
       Return: VOID
      Caution: µ÷ÓÃŽËœÓ¿ÚÇ°£¬±ØÐëÒÑŸ­³õÊŒ»¯¹ý
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
