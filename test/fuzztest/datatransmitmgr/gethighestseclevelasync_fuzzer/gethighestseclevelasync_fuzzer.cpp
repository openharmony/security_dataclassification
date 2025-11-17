/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include "gethighestseclevelasync_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <thread>

#include "file_ex.h"
#include "securec.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "accesstoken_kit.h"
#include "dev_slinfo_mgr.h"

#define DELAY_TIME 3000

namespace OHOS {
static bool g_isForcingFuzz1 = false;

static void NativeTokenGetFuzz1(void)
{
    uint64_t tokenId1;
    const char **permsFuzz1 = new const char *[1];
    permsFuzz1[0] = "ohos.permission.DISTRIBUTED_DATASYNC";
    NativeTokenInfoParams infoInstanceFuzz1 = {
        .dcapsNum = 0,
        .permsNum = 1,
        .aclsNum = 0,
        .dcaps = nullptr,
        .perms = permsFuzz1,
        .acls = nullptr,
        .aplStr = "system_basic",
    };

    infoInstanceFuzz1.processName = "DevSLMgrTest";
    tokenId1 = GetAccessTokenId(&infoInstanceFuzz1);
    SetSelfTokenID(tokenId1);
    OHOS::Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
    delete[] permsFuzz1;
}

static void BeginFuzzCase1(void)
{
    std::string isEnforcing;
    OHOS::LoadStringFromFile("/sys/fs/selinux/enforce", isEnforcing);
    if (isEnforcing.compare("1") == 0) {
        g_isForcingFuzz1 = true;
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "0");
    }
}

static void EndFuzzCase1(void)
{
    if (g_isForcingFuzz1) {
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "1");
    }
}

static int32_t g_cnt = 0;
static std::mutex g_mtx;
static std::condition_variable g_cv;

static void tmpCallbackFuzz1(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    g_cnt++;
    (void)queryParams;
    (void)result;
    (void)levelInfo;
    return;
}

void FuzzDoGetHighestSecLevelAsync(const uint8_t *data, size_t size)
{
    if (data == nullptr || size <= MAX_UDID_LENGTH) {
        return;
    }

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(DEVSLQueryParams), 0, sizeof(DEVSLQueryParams));
    queryParams.udidLen = MAX_UDID_LENGTH;
    (void)memcpy_s(queryParams.udid, MAX_UDID_LENGTH, data, MAX_UDID_LENGTH);
    BeginFuzzCase1();
    (void)DATASL_OnStart();
    (void)DATASL_GetHighestSecLevelAsync(&queryParams, tmpCallbackFuzz1);
    (void)DATASL_GetHighestSecLevelAsync(nullptr, tmpCallbackFuzz1);
    (void)DATASL_GetHighestSecLevelAsync(&queryParams, nullptr);

    std::unique_lock<std::mutex> lck(g_mtx);
    g_cv.wait_for(lck, std::chrono::milliseconds(DELAY_TIME), []() { return (g_cnt == 1); });
    DATASL_OnStop();
    EndFuzzCase1();
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerInitialize(int *argc, char ***argv)
{
    NativeTokenGetFuzz1();
    return 0;
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzDoGetHighestSecLevelAsync(data, size);
    return 0;
}