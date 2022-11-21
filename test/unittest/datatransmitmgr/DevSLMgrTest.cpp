/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include "gtest/gtest.h"
#include "file_ex.h"
#include "dev_slinfo_mgr.h"
#include "securec.h"
#include "softbus_bus_center.h"
#include "dev_slinfo_adpt.h"
#include "DevSLMgrTest.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "accesstoken_kit.h"

using namespace testing::ext;

class DevSLMgrTest : public testing::Test {
protected:
    DevSLMgrTest();
    ~DevSLMgrTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
private:
};

static void NativeTokenGet()
{
    uint64_t tokenId;
    const char **perms = new const char *[1];
    perms[0] = "ohos.permission.DISTRIBUTED_DATASYNC";
    NativeTokenInfoParams infoInstance = {
        .dcapsNum = 0,
        .permsNum = 1,
        .aclsNum = 0,
        .dcaps = nullptr,
        .perms = perms,
        .acls = nullptr,
        .aplStr = "system_basic",
    };

    infoInstance.processName = "DevSLMgrTest";
    tokenId = GetAccessTokenId(&infoInstance);
    SetSelfTokenID(tokenId);
    OHOS::Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
    delete[] perms;
}

DevSLMgrTest::DevSLMgrTest() {}
DevSLMgrTest::~DevSLMgrTest() {}
void DevSLMgrTest::SetUpTestCase()
{
    OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "0");
    NativeTokenGet();
}
void DevSLMgrTest::TearDownTestCase()
{
    OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "1");
}
void DevSLMgrTest::SetUp() {}
void DevSLMgrTest::TearDown() {}

static void DATASL_GetUdidByOpp(DEVSLQueryParams *queryParams)
{
    char udid[65] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    (void)memcpy_s(queryParams->udid, MAX_UDID_LENGTH, udid, MAX_UDID_LENGTH);
    queryParams->udidLen = MAX_UDID_LENGTH;
}

static int32_t GetLocalUdid(DEVSLQueryParams *queryParams)
{
    const char *pkgName = "ohos.dslm";
    NodeBasicInfo info;
    int32_t ret = GetLocalNodeDeviceInfo(pkgName, &info);
    if (GetNodeKeyInfo(pkgName, info.networkId, NODE_KEY_UDID, (uint8_t *)(queryParams->udid), UDID_BUF_LEN) != 0) {
        return ret;
    }
    queryParams->udidLen = MAX_UDID_LENGTH;
    return DEVSL_SUCCESS;
}

static HWTEST_F(DevSLMgrTest, TestOnstart, TestSize.Level1)
{
    int32_t ret;

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    DATASL_OnStop();
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel001, TestSize.Level1)
{
    int32_t ret;
    uint32_t levelInfo = 0;

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevel(nullptr, &levelInfo);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
    DATASL_OnStop();
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel002, TestSize.Level1)
{
    int32_t ret;

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevel(&queryParams, nullptr);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
    DATASL_OnStop();
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel003, TestSize.Level1)
{
    int32_t ret;
    uint32_t levelInfo = 0;

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
    EXPECT_EQ(DATA_SEC_LEVEL0, static_cast<int32_t>(levelInfo));
    EXPECT_EQ(ERR_NOEXIST_DEVICE, ret);
    DATASL_OnStop();
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel004, TestSize.Level1)
{
    int32_t ret;
    uint32_t levelInfo = 0;

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    ret = GetLocalUdid(&queryParams);
    EXPECT_EQ(DEVSL_SUCCESS, ret);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    DATASL_OnStop();
}

static void tmpCallback000(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, result);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync001, TestSize.Level1)
{
    int32_t ret;

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevelAsync(nullptr, &tmpCallback000);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
    DATASL_OnStop();
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync002, TestSize.Level1)
{
    int32_t ret;
    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevelAsync(&queryParams, nullptr);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
    DATASL_OnStop();
}

static void tmpCallback(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    EXPECT_EQ(ERR_NOEXIST_DEVICE, result);
    EXPECT_EQ(DATA_SEC_LEVEL0, static_cast<int32_t>(levelInfo));
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync003, TestSize.Level1)
{
    int32_t ret;
    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevelAsync(&queryParams, &tmpCallback);
    EXPECT_EQ(ERR_NOEXIST_DEVICE, ret);
    DATASL_OnStop();
}

static void tmpCallbackLocal(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    EXPECT_EQ(DEVSL_SUCCESS, result);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync004, TestSize.Level1)
{
    int32_t ret;
    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    ret = GetLocalUdid(&queryParams);
    EXPECT_EQ(DEVSL_SUCCESS, ret);

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    ret = DATASL_GetHighestSecLevelAsync(&queryParams, &tmpCallbackLocal);
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    DATASL_OnStop();
}