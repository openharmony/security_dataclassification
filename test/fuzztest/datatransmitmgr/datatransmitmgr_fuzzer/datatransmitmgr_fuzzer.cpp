/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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

#include "datatransmitmgr_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "securec.h"
#include "dev_slinfo_mgr.h"

namespace OHOS {
    static void tmpCallback(DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
    {
        return;
    }

    void FuzzDoDataTransmitMgr(const uint8_t *data, size_t size)
    {
        if (data == nullptr || size <= MAX_UDID_LENGTH) {
            return;
        }

        uint32_t levelInfo = 0;
        DEVSLQueryParams queryParams;
        (void)memset_s(&queryParams, sizeof(DEVSLQueryParams), 0, sizeof(DEVSLQueryParams));
        queryParams.udidLen = size;
        (void)memcpy_s(queryParams.udid, MAX_UDID_LENGTH, data, MAX_UDID_LENGTH);

        (void)DATASL_OnStart();
        (void)DATASL_GetHighestSecLevelAsync(&queryParams, tmpCallback);
        (void)DATASL_GetHighestSecLevel(&queryParams, &levelInfo);
        DATASL_OnStop();
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzDoDataTransmitMgr(data, size);
    return 0;
}