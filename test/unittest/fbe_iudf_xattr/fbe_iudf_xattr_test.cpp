
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

#include "fbe_iudf_xattr_test.h"
#include "fbe_sdp_policy.h"

using namespace testing::ext;

class IudfXattrTest : public testing::Test {
protected:
    IudfXattrTest();
    ~IudfXattrTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;

private:
};

IudfXattrTest::IudfXattrTest() {}
IudfXattrTest::~IudfXattrTest() {}
void IudfXattrTest::SetUpTestCase() {}
void IudfXattrTest::TearDownTestCase() {}
void IudfXattrTest::SetUp() {}
void IudfXattrTest::TearDown() {}

/**
 * @tc.name: TestIsSupportIudf
 * @tc.desc: Verify IsSupportIudf function
 * @tc.type: FUN
 */
HWTEST_F(IudfXattrTest, TestIsSupportIudf, TestSize.Level1)
{
    bool ret;

    ret = IsSupportIudf();
    EXPECT_EQ(false, ret);
}