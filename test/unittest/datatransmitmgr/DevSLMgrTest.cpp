/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
#include "dev_slinfo_mgr.h"
#include "securec.h"
#include "softbus_bus_center.h"
#include "dev_slinfo_adpt.h"
#include "DevSLMgrTest.h"

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

DevSLMgrTest::DevSLMgrTest() {}
DevSLMgrTest::~DevSLMgrTest() {}
void DevSLMgrTest::SetUpTestCase() {}
void DevSLMgrTest::TearDownTestCase() {};
void DevSLMgrTest::SetUp() {}
void DevSLMgrTest::TearDown() {}

static void DATASL_GetUdidByOpp(DEVSLQueryParams *queryParams)
{
    char udid[65] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    (void)memcpy_s(queryParams->udid, MAX_UDID_LENGTH, udid, MAX_UDID_LENGTH);
    queryParams->udidLen = MAX_UDID_LENGTH;
}

static HWTEST_F(DevSLMgrTest, TestOnstart, TestSize.Level1)
{
    int32_t ret;

    ret = DATASL_OnStart();
    EXPECT_EQ(DEVSL_SUCCESS, ret);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel001, TestSize.Level1)
{
    int32_t ret;
    uint32_t levelInfo = 0;

    ret = DATASL_OnStart();
    ret = DATASL_GetHighestSecLevel(nullptr, &levelInfo);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel002, TestSize.Level1)
{
    int32_t ret;

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    ret = DATASL_GetHighestSecLevel(&queryParams, nullptr);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevel003, TestSize.Level1)
{
    int32_t ret;
    uint32_t levelInfo = 0;

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_OnStart();
    ret = DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
    EXPECT_EQ(DATA_SEC_LEVEL0, static_cast<int32_t>(levelInfo));
    EXPECT_EQ(ERR_NOEXIST_DEVICE, ret);
}

static void tmpCallback000(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, result);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync001, TestSize.Level1)
{
    int32_t ret;

    ret = DATASL_OnStart();
    ret = DATASL_GetHighestSecLevelAsync(nullptr, &tmpCallback000);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
}

static HWTEST_F(DevSLMgrTest, TestGetHighestSecLevelAsync002, TestSize.Level1)
{
    int32_t ret;
    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(queryParams), 0, sizeof(queryParams));
    DATASL_GetUdidByOpp(&queryParams);

    ret = DATASL_GetHighestSecLevelAsync(&queryParams, nullptr);
    EXPECT_EQ(DEVSL_ERR_BAD_PARAMETERS, ret);
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

    ret = DATASL_GetHighestSecLevelAsync(&queryParams, &tmpCallback);
    EXPECT_EQ(ERR_NOEXIST_DEVICE, ret);
    DATASL_OnStop();
}