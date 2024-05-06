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

#include "gethighestseclevel_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "file_ex.h"
#include "securec.h"
#include "parameter.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "accesstoken_kit.h"

#include "dev_slinfo_mgr.h"

namespace OHOS {
static bool g_isForcingFuzz2 = false;

static void NativeTokenGetFuzz2(void)
{
    uint64_t tokenId2;
    const char **permsFuzz2 = new const char *[1];
    permsFuzz2[0] = "ohos.permission.DISTRIBUTED_DATASYNC";
    NativeTokenInfoParams infoInstanceFuzz1 = {
        .dcapsNum = 0,
        .permsNum = 1,
        .aclsNum = 0,
        .dcaps = nullptr,
        .perms = permsFuzz2,
        .acls = nullptr,
        .aplStr = "system_basic",
    };

    infoInstanceFuzz1.processName = "DevSLMgrTest";
    tokenId2 = GetAccessTokenId(&infoInstanceFuzz1);
    SetSelfTokenID(tokenId2);
    OHOS::Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
    delete[] permsFuzz2;
}

static void BeginFuzzCase2(void)
{
    std::string isEnforcing;
    OHOS::LoadStringFromFile("/sys/fs/selinux/enforce", isEnforcing);
    if (isEnforcing.compare("1") == 0) {
        g_isForcingFuzz2 = true;
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "0");
    }
    NativeTokenGetFuzz2();
}

static void EndFuzzCase2(void)
{
    if (g_isForcingFuzz2) {
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "1");
    }
}

static int32_t GetLocalUdidFuzz2(DEVSLQueryParams *queryParams)
{
    char udid[MAX_UDID_LENGTH + 1] = {0};
    int32_t ret = GetDevUdid(udid, MAX_UDID_LENGTH + 1);
    if (ret != DEVSL_SUCCESS) {
        return DEVSL_ERROR;
    }

    (void)memcpy_s(queryParams->udid, MAX_UDID_LENGTH, udid, MAX_UDID_LENGTH);
    queryParams->udidLen = MAX_UDID_LENGTH;
    return ret;
}

void FuzzDoGetHighestSecLevel(const uint8_t *data, size_t size)
{
    if (data == nullptr || size <= MAX_UDID_LENGTH) {
        return;
    }

    uint32_t levelInfo = 0;
    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(DEVSLQueryParams), 0, sizeof(DEVSLQueryParams));
    queryParams.udidLen = MAX_UDID_LENGTH;
    (void)memcpy_s(queryParams.udid, MAX_UDID_LENGTH, data, MAX_UDID_LENGTH);
    BeginFuzzCase2();
    (void)DATASL_OnStart();
    (void)DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
    (void)DATASL_GetHighestSecLevel(nullptr, &levelInfo);
    (void)DATASL_GetHighestSecLevel(&queryParams, nullptr);

    (void)GetLocalUdidFuzz2(&queryParams);

    (void)DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
    DATASL_OnStop();
    EndFuzzCase2();
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzDoGetHighestSecLevel(data, size);
    return 0;
}