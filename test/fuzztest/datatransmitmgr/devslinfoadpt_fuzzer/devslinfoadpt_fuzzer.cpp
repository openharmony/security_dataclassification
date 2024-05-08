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

#include "devslinfoadpt_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "file_ex.h"
#include "securec.h"
#include "parameter.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "accesstoken_kit.h"

#include "dev_slinfo_adpt.h"
#include "dev_slinfo_mgr.h"

namespace OHOS {
static bool g_isForcingFuzz3 = false;

static uint8_t g_udidInvalid[MAX_UDID_LENGTH + 1] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

struct DeviceSecurityInfo {
    uint32_t magicNum {0};
    uint32_t result {0};
    uint32_t level {0};
};

extern "C" {
    extern void OnApiDeviceSecInfoCallback(const DeviceIdentify *identify, struct DeviceSecurityInfo *info);
}

static void NativeTokenGetFuzz3(void)
{
    uint64_t tokenId3;
    const char **permsFuzz3 = new const char *[1];
    permsFuzz3[0] = "ohos.permission.DISTRIBUTED_DATASYNC";
    NativeTokenInfoParams infoInstanceFuzz1 = {
        .dcapsNum = 0,
        .permsNum = 1,
        .aclsNum = 0,
        .dcaps = nullptr,
        .perms = permsFuzz3,
        .acls = nullptr,
        .aplStr = "system_basic",
    };

    infoInstanceFuzz1.processName = "DevSLMgrTest";
    tokenId3 = GetAccessTokenId(&infoInstanceFuzz1);
    SetSelfTokenID(tokenId3);
    OHOS::Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
    delete[] permsFuzz3;
}

static void BeginFuzzCase3(void)
{
    std::string isEnforcing;
    OHOS::LoadStringFromFile("/sys/fs/selinux/enforce", isEnforcing);
    if (isEnforcing.compare("1") == 0) {
        g_isForcingFuzz3 = true;
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "0");
    }
    NativeTokenGetFuzz3();
}

static void EndFuzzCase3(void)
{
    if (g_isForcingFuzz3) {
        OHOS::SaveStringToFile("/sys/fs/selinux/enforce", "1");
    }
}

static int32_t GetLocalUdidFuzz3(DEVSLQueryParams *queryParams)
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

void FuzzDoDevSlinfoAdpt(const uint8_t* data, size_t size)
{
    if (data == nullptr || size <= MAX_UDID_LENGTH) {
        return;
    }
    OnApiDeviceSecInfoCallback(nullptr, nullptr);
    (void)GetDeviceSecLevelByUdidAsync(nullptr, 0);

    DEVSLQueryParams queryParams;
    (void)memset_s(&queryParams, sizeof(DEVSLQueryParams), 0, sizeof(DEVSLQueryParams));
    queryParams.udidLen = MAX_UDID_LENGTH;
    (void)memcpy_s(queryParams.udid, MAX_UDID_LENGTH, data, MAX_UDID_LENGTH);
    BeginFuzzCase3();
    int32_t ret;
    uint32_t levelInfo = 0;
    int32_t devLevel = 0;
    DeviceIdentify devId;
    DeviceSecurityInfo devInfo;
    (void)DATASL_OnStart();

    do {
        ret = GetLocalUdidFuzz3(&queryParams);
        if (ret != DEVSL_SUCCESS) {
            break;
        }
        ret = GetDeviceSecLevelByUdid(static_cast<const uint8_t *>(g_udidInvalid), MAX_UDID_LENGTH + 1, &devLevel);
        if (ret == DEVSL_SUCCESS) {
            break;
        }
        (void)GetDeviceSecLevelByUdid(static_cast<const uint8_t *>(queryParams.udid), queryParams.udidLen, &devLevel);
        (void)DATASL_GetHighestSecLevel(&queryParams, &levelInfo);

        (void)memset_s(&devId, sizeof(devId), 0, sizeof(devId));
        (void)memcpy_s(devId.identity, MAX_UDID_LENGTH, queryParams.udid, queryParams.udidLen);
        devId.length = queryParams.udidLen;

        OnApiDeviceSecInfoCallback(&devId, nullptr);
        OnApiDeviceSecInfoCallback(&devId, &devInfo);
    } while (0);
    DATASL_OnStop();
    EndFuzzCase3();
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzDoDevSlinfoAdpt(data, size);
    return 0;
}