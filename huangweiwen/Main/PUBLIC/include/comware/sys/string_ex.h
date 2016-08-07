/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            string_ex.h
  Project Code: Comware V7
   Module Name:
  Date Created: 2008-4-7
        Author: c02254
   Description: commware ×Ö·û´®´¦Àí

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/

#ifndef _SYS_STRING_EX_H_
#define _SYS_STRING_EX_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifdef _STRING_EX_COMPILE
#define scnprintf                   snprintf
#define strlcpy                     strncpy
#endif

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */
#endif /* _SYS_STRING_EX_H_ */

