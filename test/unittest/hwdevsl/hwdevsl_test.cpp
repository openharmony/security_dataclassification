
/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "gtest/gtest.h"

#include "hwdevsl_test.h"
#include "dev_slinfo_mgr.h"

using namespace testing::ext;

class DeviceSecurityLevelTest : public testing::Test {
protected:
    DeviceSecurityLevelTest();
    ~DeviceSecurityLevelTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;

private:
};

DeviceSecurityLevelTest::DeviceSecurityLevelTest() {}
DeviceSecurityLevelTest::~DeviceSecurityLevelTest() {}
void DeviceSecurityLevelTest::SetUpTestCase() {}
void DeviceSecurityLevelTest::TearDownTestCase() {}
void DeviceSecurityLevelTest::SetUp() {}
void DeviceSecurityLevelTest::TearDown() {}

/**
 * @tc.name: TestOnStart
 * @tc.desc: Verify DEVSL_OnStart function
 * @tc.type: FUN
 */
HWTEST_F(DeviceSecurityLevelTest, TestOnStart, TestSize.Level0)
{
    int32_t ret;

    ret = DEVSL_OnStart(1);
    EXPECT_EQ(DEVSL_SUCCESS, ret);
    DEVSL_ToFinish();
}