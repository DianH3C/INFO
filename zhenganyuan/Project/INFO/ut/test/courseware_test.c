#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <sys/string_ex.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include "../../src/info.h"
#include "../../src/info_data.h"
#include "../../src/info_dbg.h"
#include "../../src/info_parse.h"
#include "../../src/info_proc.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include <gtest/gtest.h>
#include "../stub/stub.h"

#define UTC_INFO_proc_Add
/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_001
#测  试  项  : 功能测试
#测试用例标题: 添加一个成员信息成功
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 一个成员的信息：id=10 name=jack sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息成功
               查询成员已存在
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 001)
{
    ULONG ulErrCode;
    BOOL_T bIsExist;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    bIsExist = INFO_data_IsExist(10);
    EXPECT_EQ(BOOL_TRUE, bIsExist);
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_002
#测  试  项  : 功能测试
#测试用例标题: 添加五个成员信息成功
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 成员信息1：id=10 name=jack10 sex=1 age=30 height=100
               成员信息2：id=1  name=jack1  sex=2 age=40 height=250
               成员信息3：id=50 name=jack50 sex=1 age=50 height=200
               成员信息4：id=99 name=jack99 sex=2 age=33 height=111
               成员信息5：id=44 name=jack44 sex=1 age=22 height=222
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加五个成员信息成功
               可以按顺序获取五个成员信息
               成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 002)
{
    ULONG ulErrCode;
    UINT uiId;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode  = INFO_proc_Add("id=10 name=jack10 sex=1 age=30 height=100");
    ulErrCode |= INFO_proc_Add("id=1  name=jack1  sex=2 age=40 height=250");
    ulErrCode |= INFO_proc_Add("id=50 name=jack50 sex=1 age=50 height=200");
    ulErrCode |= INFO_proc_Add("id=99 name=jack99 sex=2 age=33 height=111");
    ulErrCode |= INFO_proc_Add("id=44 name=jack44 sex=1 age=22 height=222");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    uiId = INFO_data_GetFirst();
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(1, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(40, stCfg.uiAge);
    EXPECT_EQ(250, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack1"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(30, stCfg.uiAge);
    EXPECT_EQ(100, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack10"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(44, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(22, stCfg.uiAge);
    EXPECT_EQ(222, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack44"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(50, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(50, stCfg.uiAge);
    EXPECT_EQ(200, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack50"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(99, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(33, stCfg.uiAge);
    EXPECT_EQ(111, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack99"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_003
#测  试  项  : 边界测试
#测试用例标题: 按合法边界值添加用户信息成功
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 成员信息1(最小边界值)：id=1      name=1               sex=1 age=18 height=1
               成员信息2(最大边界值)：id=100000 name=123456789012345 sex=2 age=65 height=300
               成员信息3(小边界值1 )：id=2      name=11              sex=1 age=19 height=2
               成员信息4(小边界值2 )：id=3      name=111             sex=1 age=20 height=3
               成员信息5(大边界值1 )：id=99999  name=12345678901234  sex=2 age=64 height=299
               成员信息6(大边界值2 )：id=99998  name=1234567890123   sex=2 age=63 height=298
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息成功
               成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 003)
{
    ULONG ulErrCode;
    UINT uiId;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=1      name=1               sex=1 age=18 height=1  ");
    ulErrCode = INFO_proc_Add("id=100000 name=123456789012345 sex=2 age=65 height=300");
    ulErrCode = INFO_proc_Add("id=2      name=11              sex=1 age=19 height=2  ");
    ulErrCode = INFO_proc_Add("id=3      name=111             sex=1 age=20 height=3  ");
    ulErrCode = INFO_proc_Add("id=99999  name=12345678901234  sex=2 age=64 height=299");
    ulErrCode = INFO_proc_Add("id=99998  name=1234567890123   sex=2 age=63 height=298");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    uiId = INFO_data_GetFirst();
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(1, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(18, stCfg.uiAge);
    EXPECT_EQ(1, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "1"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(2, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(19, stCfg.uiAge);
    EXPECT_EQ(2, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "11"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(3, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(3, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "111"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(99998, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(63, stCfg.uiAge);
    EXPECT_EQ(298, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "1234567890123"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(99999, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(64, stCfg.uiAge);
    EXPECT_EQ(299, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "12345678901234"));
    uiId = INFO_data_GetNext(uiId);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(100000, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(65, stCfg.uiAge);
    EXPECT_EQ(300, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "123456789012345"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_004
#测  试  项  : 压力测试
#测试用例标题: 大规格测试，添加5000个员工信息
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 循环添加多个成员信息：id=x name=jack sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息成功
               抽查一个员工信息，与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 004)
{
    ULONG ulErrCode;
    UINT uiLoop;
    CHAR szInputStr[512];
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    for (uiLoop = 1; uiLoop < 100000; uiLoop = uiLoop + 20)
    {
        scnprintf(szInputStr, sizeof(szInputStr), "id=%u name=jack sex=2 age=20 height=175", uiLoop);
        ulErrCode |= INFO_proc_Add(szInputStr);
    }
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(5001, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(5001, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_005
#测  试  项  : 异常测试
#测试用例标题: 重复添加
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 成员信息：id=10 name=xxxx sex=1 age=55 height=222
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_ALREADY_EXIST
               查询成员已存在
               查询成员信息与预置条件输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 005)
{
    ULONG ulErrCode;
    BOOL_T bIsExist;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10 name=xxxx sex=1 age=55 height=222");
    EXPECT_EQ(ERROR_ALREADY_EXIST, ulErrCode);

    /* 检查预期结果 */
    bIsExist = INFO_data_IsExist(10);
    EXPECT_EQ(BOOL_TRUE, bIsExist);
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_006
#测  试  项  : 异常测试
#测试用例标题: 非法输入，用户信息输入空字符串
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：输入空字符串
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 006)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_007
#测  试  项  : 异常测试
#测试用例标题: 非法输入，工号为上限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=100001 name=jack             sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 007)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=100001 name=jack             sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_008
#测  试  项  : 异常测试
#测试用例标题: 非法输入，工号为下限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=0      name=jack             sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 008)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=0      name=jack             sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_009
#测  试  项  : 异常测试
#测试用例标题: 非法输入，性别为上限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=3 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 009)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=3 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_010
#测  试  项  : 异常测试
#测试用例标题: 非法输入，性别为下限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=0 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 010)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=0 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_011
#测  试  项  : 异常测试
#测试用例标题: 非法输入，姓名长度为上限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=1234567890123456 sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 011)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=1234567890123456 sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_012
#测  试  项  : 异常测试
#测试用例标题: 非法输入，姓名长度为下限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=                 sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 012)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=                 sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_013
#测  试  项  : 异常测试
#测试用例标题: 非法输入，年龄为上限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2 age=66 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 013)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2 age=66 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_014
#测  试  项  : 异常测试
#测试用例标题: 非法输入，年龄为下限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2 age=17 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 014)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2 age=17 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_015
#测  试  项  : 异常测试
#测试用例标题: 非法输入，身高为上限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2 age=20 height=301
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 015)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2 age=20 height=301");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_016
#测  试  项  : 异常测试
#测试用例标题: 非法输入，身高为下限
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2 age=20 height=0
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 016)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2 age=20 height=0");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_017
#测  试  项  : 异常测试
#测试用例标题: 非法输入，缺少工号
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：          name=jack             sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 017)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("          name=jack             sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_018
#测  试  项  : 异常测试
#测试用例标题: 非法输入，缺少姓名
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10                           sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 018)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10                           sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_019
#测  试  项  : 异常测试
#测试用例标题: 非法输入，缺少性别
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack                   age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 019)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack                   age=20 height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_020
#测  试  项  : 异常测试
#测试用例标题: 非法输入，缺少年龄
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2        height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 020)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2        height=175");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_021
#测  试  项  : 异常测试
#测试用例标题: 非法输入，缺少身高
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10     name=jack             sex=2 age=20
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 021)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10     name=jack             sex=2 age=20           ");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Add_022
#测  试  项  : 异常测试
#测试用例标题: 申请内存失败
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
#输       入 : 用户信息：id=10 name=jack sex=2 age=20 height=175
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 添加成员信息失败，返回错误码ERROR_NO_ENOUGH_RESOURCE
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Add, 022)
{
    ULONG ulErrCode;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 对malloc打桩，申请内存失败 */
    stub_set_one((VOID *)malloc, (VOID *)stub_malloc_ng);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_NO_ENOUGH_RESOURCE, ulErrCode);

    /* 清除桩 */
    stub_reset_all();

    /* 去初始化 */
    INFO_data_Fini();
}

#define UT_CORRECTMEMBERNUM 15UL
STATIC const CHAR *g_szCorrectMemberInfo[UT_CORRECTMEMBERNUM] = {
    "id=11 name=jack01 sex=1 age=21 height=170",    /* 0 */
    "id=12 name=jack02 sex=2 age=22 height=171",
    "id=13 name=jack03 sex=1 age=23 height=172",
    "id=14 name=jack04 sex=2 age=24 height=173",
    "id=15 name=jack05 sex=1 age=25 height=174",
    "id=16 name=jack06 sex=2 age=26 height=175",    /* 5 */
    /* 构造3组ID相同信息 */
    "id=17 name=jack07 sex=1 age=27 height=176",
    "id=18 name=jack08 sex=2 age=28 height=177",
    "id=19 name=jack09 sex=1 age=29 height=178",
    "id=17 name=jack10 sex=2 age=30 height=179",    /* 9 */
    "id=18 name=jack11 sex=1 age=31 height=180",
    "id=19 name=jack12 sex=2 age=32 height=181",
    /* 以下ID相同 */
    "id=20 name=jack13 sex=1 age=33 height=182",    /* 12 */
    "id=20 name=jack14 sex=2 age=34 height=183",
    "id=20 name=jack15 sex=1 age=35 height=184"     /* 14 */
};

/* 与g_szCorrectMemberInfo对应 */
INFO_CFG_S g_stMemberData[UT_CORRECTMEMBERNUM] = {
 {11, 21, 170, INFO_SEX_FEMALE, "jack01"},
 {12, 22, 171, INFO_SEX_MALE,   "jack02"},
 {13, 23, 172, INFO_SEX_FEMALE, "jack03"},
 {14, 24, 173, INFO_SEX_MALE,   "jack04"},
 {15, 25, 174, INFO_SEX_FEMALE, "jack05"},
 {16, 26, 175, INFO_SEX_MALE,   "jack06"},
 {17, 27, 176, INFO_SEX_FEMALE, "jack07"},
 {18, 28, 177, INFO_SEX_MALE,   "jack08"},
 {19, 29, 178, INFO_SEX_FEMALE, "jack09"},
 {17, 30, 179, INFO_SEX_MALE,   "jack10"},
 {18, 31, 180, INFO_SEX_FEMALE, "jack11"},
 {19, 32, 181, INFO_SEX_MALE,   "jack12"},
 {20, 33, 182, INFO_SEX_FEMALE, "jack13"},
 {20, 34, 183, INFO_SEX_MALE,   "jack14"},
 {20, 35, 184, INFO_SEX_FEMALE, "jack15"}
};

#define UTC_INFO_proc_Delete
/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_001
#测  试  项  : 功能测试
#测试用例标题: 删除1个成员成功
#重 要 级 别 : 高
#预 置 条 件 : 添加1个成员
#输       入 : pcInputStr=员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 成员删除
#              INFO_proc_Delete返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 001)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    ULONG ulFirstMember = 0;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加一个成员 */
    ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[ulFirstMember]);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    ulErrCode = INFO_data_GetData(g_stMemberData[ulFirstMember].uiId, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用被测函数 */
    ulErrCode = INFO_proc_Delete("id=11");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    ulErrCode = INFO_data_GetData(g_stMemberData[ulFirstMember].uiId, &stCfg);
    EXPECT_EQ(ERROR_FAILED, ulErrCode);


    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_002
#测  试  项  : 功能测试
#测试用例标题: 删除3个成员成功
#重 要 级 别 : 高
#预 置 条 件 : 添加3个成员
#输       入 : pcInputStr=员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 所有成员删除
#              INFO_proc_Delete返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 002)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iMemberNum = 3;
    INT iCurMember;
    BOOL_T bIsEmpty;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加成员 */
    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    for (iCurMember = iMemberNum - 1; iCurMember >= 0; iCurMember--)
    {
        /* 调用被测函数 */
        ulErrCode = INFO_proc_Delete(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_FAILED, ulErrCode);
    }

    bIsEmpty = INFO_data_IsEmpty();
    EXPECT_EQ(BOOL_TRUE, bIsEmpty);

    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_003
#测  试  项  : 功能测试
#测试用例标题: 删除5个成员成功
#重 要 级 别 : 高
#预 置 条 件 : 添加5个成员
#输       入 : pcInputStr=员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 所有成员删除
#              INFO_proc_Delete返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 003)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iMemberNum = 5;
    INT iCurMember;
    BOOL_T bIsEmpty;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加成员 */
    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    for (iCurMember = iMemberNum - 1; iCurMember >=0; iCurMember--)
    {
        /* 调用被测函数 */
        ulErrCode = INFO_proc_Delete(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_FAILED, ulErrCode);
    }

    bIsEmpty = INFO_data_IsEmpty();
    EXPECT_EQ(BOOL_TRUE, bIsEmpty);

    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_004
#测  试  项  : 功能测试
#测试用例标题: 删除的成员不存在
#重 要 级 别 : 高
#预 置 条 件 : 添加5个成员
#输       入 : pcInputStr=不存在的员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 没有成员被删除
#              INFO_proc_Delete返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 004)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iMemberNum = 5;
    INT iCurMember;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加成员 */
    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    /* 调用被测函数 */
    ulErrCode = INFO_proc_Delete("id=1000");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_005
#测  试  项  : 功能测试
#测试用例标题: 删除的成员失败，输入无效ID
#重 要 级 别 : 高
#预 置 条 件 : 添加5个成员
#输       入 : pcInputStr=无效员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 没有成员被删除
#              INFO_proc_Delete返回ERROR_INVALID_PARAMETER
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 005)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iMemberNum = 5;
    INT iCurMember;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加成员 */
    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[iCurMember]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    /* 调用被测函数 */
    ulErrCode = INFO_proc_Delete("id=0");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    /* 调用被测函数 */
    ulErrCode = INFO_proc_Delete("id=100001");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    for (iCurMember = 0; iCurMember < iMemberNum; iCurMember++)
    {
        ulErrCode = INFO_data_GetData(g_stMemberData[iCurMember].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Delete_006
#测  试  项  : 功能测试
#测试用例标题: 删除100000个成员成功
#重 要 级 别 : 高
#预 置 条 件 : 添加100000个成员
#输       入 : pcInputStr=员工ID
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 所有成员删除
#              INFO_proc_Delete返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Delete, 006)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iMemberNum = 10000;
    INT iCurMember;
    BOOL_T bIsEmpty;
    CHAR szInfo[INFO_IOBUF_MAXLEN];
    CHAR *pcString;
    UINT uiId;
    UINT uiCount;

    /* 初始化环境 */
    INFO_data_Init();

    pcString = szInfo;

    /* 添加成员 */
    for (iCurMember = 1; iCurMember <= iMemberNum; iCurMember++)
    {
        (VOID)scnprintf(pcString, sizeof(szInfo), "id=%d name=jack01 sex=1 age=21 height=170", iCurMember);
        ulErrCode = INFO_proc_Add(pcString);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    uiCount = 0;
    INFO_FOREACH(uiId)
    {
        uiCount++;
    }
    EXPECT_EQ(iMemberNum, uiCount);

    for (iCurMember = iMemberNum; iCurMember > 0; iCurMember--)
    {
        pcString = szInfo;
        (VOID)scnprintf(pcString, sizeof(szInfo), "id=%u", iCurMember);

        /* 调用被测函数 */
        ulErrCode = INFO_proc_Delete(pcString);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    bIsEmpty = INFO_data_IsEmpty();
    EXPECT_EQ(BOOL_TRUE, bIsEmpty);

    /* 去初始化环境 */
    INFO_data_Fini();
}

#define UTC_INFO_proc_Modify
/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_001
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改姓名
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 name=jackabc
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 001)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 name=jackabc");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jackabc"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_002
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改性别
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 sex=1
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 002)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 sex=1");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_003
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改年龄
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 age=35
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 003)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 age=35");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(35, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_004
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改身高
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 height=181
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 004)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 height=181");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(181, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_005
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改姓名和年龄
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 name=jack123456 age=45
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 005)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 name=jack123456 age=45");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(2, stCfg.enSex);
    EXPECT_EQ(45, stCfg.uiAge);
    EXPECT_EQ(175, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack123456"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_006
#测  试  项  : 功能测试
#测试用例标题: 部分修改，只修改性别+身高
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=10 sex=1 height=185
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询成员信息与输入一致
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 006)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=10 sex=1 height=185");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    ulErrCode = INFO_data_GetData(10, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(10, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(20, stCfg.uiAge);
    EXPECT_EQ(185, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jack"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_007
#测  试  项  : 异常测试
#测试用例标题: 工号不存在
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=100 sex=1 height=185
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息失败，返回ERROR_NOT_FOUND
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 007)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=100 sex=1 height=185");
    EXPECT_EQ(ERROR_NOT_FOUND, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_008
#测  试  项  : 异常测试
#测试用例标题: 工号为非法值
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               添加成员信息：id=10 name=jack sex=2 age=20 height=175
#输       入 : 修改成员信息：id=100005 sex=1 height=185
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息失败，返回ERROR_INVALID_PARAMETER
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 008)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode  = INFO_data_Init();
    ulErrCode |= INFO_proc_Add("id=10 name=jack sex=2 age=20 height=175");
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用添加函数 */
    ulErrCode = INFO_proc_Modify("id=100005 sex=1 height=185");
    EXPECT_EQ(ERROR_INVALID_PARAMETER, ulErrCode);

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_009
#测  试  项  : 压力测试
#测试用例标题: 修改大批量的用户信息
#重 要 级 别 : 高
#预 置 条 件 : 初始化链表结构
               循环输入多个用户信息：id=x name=jack sex=2 age=20 height=175
#输       入 : 循环修改成员信息：id=x name=jackabc sex=1 age=35 height=195
#操 作 步 骤 : 执行测试用例
#预 期 结 果 : 修改成员信息成功
               查询用户信息修改正确
#完  成  人  : wkf2298
#日      期  : 2011年12月31日
#######################################################################*/
TEST(UT_INFO_proc_Modify, 009)
{
    ULONG ulErrCode;
    UINT uiId;
    CHAR szInputStr[512];
    INFO_CFG_S stCfg;

    /* 初始化 */
    ulErrCode = INFO_data_Init();
    for (uiId = 100000; uiId > 0; uiId = uiId - 20)
    {
        scnprintf(szInputStr, sizeof(szInputStr), "id=%u name=jack sex=2 age=20 height=175", uiId);
        ulErrCode |= INFO_proc_Add(szInputStr);
    }
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 遍历所有工号，调用修改函数 */
    INFO_FOREACH(uiId)
    {
        scnprintf(szInputStr, sizeof(szInputStr), "id=%u name=jackabc sex=1 age=35 height=195", uiId);
        ulErrCode |= INFO_proc_Modify(szInputStr);
    }
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 检查预期结果 */
    uiId = INFO_data_GetNext(993);
    ulErrCode = INFO_data_GetData(uiId, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(uiId, stCfg.uiId);
    EXPECT_EQ(1, stCfg.enSex);
    EXPECT_EQ(35, stCfg.uiAge);
    EXPECT_EQ(195, stCfg.uiHeight);
    EXPECT_EQ(0, strcmp(stCfg.szName, "jackabc"));

    /* 去初始化 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_011
#测  试  项  : 功能测试
#测试用例标题: 修改1个成员所有信息成功
#重 要 级 别 : 高
#预 置 条 件 : 添加1个成员
#输       入 : pcInputStr=员工信息
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 成员数据修改
#              INFO_proc_Modify返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Modify, 011)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    ULONG ulFirstMember = 12;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加一个成员 */
    ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[ulFirstMember]);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    ulErrCode = INFO_data_GetData(g_stMemberData[ulFirstMember].uiId, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

    /* 调用被测函数 */
    ulErrCode = INFO_proc_Modify(g_szCorrectMemberInfo[ulFirstMember + 1]);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    ulErrCode = INFO_data_GetData(g_stMemberData[ulFirstMember].uiId, &stCfg);
    EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    EXPECT_EQ(g_stMemberData[ulFirstMember + 1].enSex, stCfg.enSex);
    EXPECT_EQ(g_stMemberData[ulFirstMember + 1].uiAge, stCfg.uiAge);
    EXPECT_EQ(g_stMemberData[ulFirstMember + 1].uiHeight, stCfg.uiHeight);
    EXPECT_STREQ(g_stMemberData[ulFirstMember + 1].szName, stCfg.szName);

    /* 去初始化环境 */
    INFO_data_Fini();
}

/*#######################################################################
#测试用例编号: UT_INFO_proc_Modify_012
#测  试  项  : 功能测试
#测试用例标题: 修改3个成员所有信息成功
#重 要 级 别 : 高
#预 置 条 件 : 添加3个成员
#输       入 : pcInputStr=员工信息
#操 作 步 骤 : 执行测试例
#预 期 结 果 : 所有成员删除
#              INFO_proc_Modify返回成功
#完  成  人  : liaohui kf2309
#日      期  : 2011-12-31
#######################################################################*/
TEST(UT_INFO_proc_Modify, 012)
{
    ULONG ulErrCode;
    INFO_CFG_S stCfg;
    INT iStart = 6;
    INT iEnd = 9;
    INT iModifyStart = 9;
    INT iModifyEnd = 12;
    INT iCur;
    BOOL_T bIsEmpty;

    /* 初始化环境 */
    INFO_data_Init();

    /* 添加成员 */
    for (iCur = iStart; iCur < iEnd; iCur++)
    {
        ulErrCode = INFO_proc_Add(g_szCorrectMemberInfo[iCur]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);

        ulErrCode = INFO_data_GetData(g_stMemberData[iCur].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
    }

    for (iCur = iModifyStart; iCur < iModifyEnd; iCur++)
    {
        /* 调用被测函数 */
        ulErrCode = INFO_proc_Modify(g_szCorrectMemberInfo[iCur]);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
        ulErrCode = INFO_data_GetData(g_stMemberData[iCur].uiId, &stCfg);
        EXPECT_EQ(ERROR_SUCCESS, ulErrCode);
        EXPECT_EQ(g_stMemberData[iCur].enSex, stCfg.enSex);
        EXPECT_EQ(g_stMemberData[iCur].uiAge, stCfg.uiAge);
        EXPECT_EQ(g_stMemberData[iCur].uiHeight, stCfg.uiHeight);
        EXPECT_STREQ(g_stMemberData[iCur].szName, stCfg.szName);
    }

    bIsEmpty = INFO_data_IsEmpty();
    EXPECT_EQ(BOOL_FALSE, bIsEmpty);

    /* 去初始化环境 */
    INFO_data_Fini();
}

