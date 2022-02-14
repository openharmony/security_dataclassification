/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "dev_slinfo_list.h"
#include "dev_slinfo_adpt.h"
#include "dev_slinfo_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEVSL_API __attribute__ ((visibility ("default")))

static int32_t GetHigestSecLevelByUdid(DEVSLQueryParams *queryParams, uint32_t *dataLevel)
{
    int32_t ret = 0;
    int32_t devLevel = DEFAULT_DEV_SEC_LEVEL;

    DATA_SEC_LOG_INFO("GetHighestSecLevelByUdid by udid!");
    if ((queryParams->udidLen < 0u) || (queryParams->udidLen > 64u)) {
        return ERR_INVALID_PARA;
    }

    ret = GetDeviceSecLevelByUdid(queryParams->udid, queryParams->udidLen, &devLevel);
    if (ret != SUCCESS) {
        *dataLevel = DATA_SEC_LEVEL0;
        DATA_SEC_LOG_ERROR("GetHigestSecLevelByUdid, get device security level failed, error code: %d", ret);
        return ret;
    }

    *dataLevel  = GetDataSecLevelByDevSecLevel(devLevel);
    return ret;
}

DEVSL_API int32_t DATASL_OnStart(void)
{
    int32_t ret = 0;

    DATA_SEC_LOG_INFO("Enter DATASL_Onstart");
    ret = StartDevslEnv();
    if (ret != SUCCESS) {
        DATA_SEC_LOG_ERROR("StartDevslEnv - failed, error code is %d", ret);
        FinishDevslEnv();
        return DEVSL_ERROR;
    }
    DATA_SEC_LOG_INFO("DATASL_Onstart - success");
    return SUCCESS;
}

DEVSL_API void DATASL_OnStop(void)
{
    DATA_SEC_LOG_INFO("Enter DATASL_OnStop");
    FinishDevslEnv();
    DATA_SEC_LOG_INFO("DATASL_OnStop done");
    return;
}

DEVSL_API int32_t DATASL_GetHighestSecLevel(DEVSLQueryParams *queryParams, uint32_t *levelInfo)
{
    int32_t ret = 0;
    DATA_SEC_LOG_INFO("Enter DATASL_GetHighestSecLevel!");
    if ((queryParams == NULL) || (levelInfo == NULL)) {
        return ERR_INVALID_PARA;
    }

    ret = GetHigestSecLevelByUdid(queryParams, levelInfo);

    DATA_SEC_LOG_INFO("DATASL_GetHighestSecLevel done, ret %d!", ret);
    return ret;
}

// Async
static int32_t GetHighestSecLevelByUdidAsync(DEVSLQueryParams *queryParams)
{
    DATA_SEC_LOG_INFO("Enter GetHighestSecLevelByUdidAsync!");
    int32_t ret;

    if ((queryParams->udidLen < 0u) || (queryParams->udidLen > 64u)) {
        return ERR_INVALID_PARA;
    }

    ret = GetDeviceSecLevelByUdidAsync(queryParams->udid, queryParams->udidLen);
    if (ret != SUCCESS) {
        DATA_SEC_LOG_ERROR("GetHigestSecLevelByUdid, get device security level failed, error code: %d", ret);
        return ret;
    }
    
    DATA_SEC_LOG_INFO("GetHighestSecLevelByUdidAsync done, ret %d!", ret);
    return ret;
}

DEVSL_API int32_t DATASL_GetHighestSecLevelAsync(DEVSLQueryParams *queryParams, HigestSecInfoCallback *callback)
{
    DATA_SEC_LOG_INFO("Enter DATASL_GetHighestSecLevelAsync!");
    int32_t ret;

    if ((queryParams == NULL) || (callback == NULL)) {
        return ERR_INVALID_PARA;
    }

    ret = UpdateDATASLCallbackParams(queryParams, callback);
    if (ret != SUCCESS) {
        return ret;
    }

    ret = GetHighestSecLevelByUdidAsync(queryParams);
    DATA_SEC_LOG_INFO("DATASL_GetHighestSecLevelAsync done, ret %d!", ret);
    return ret;
}

#ifdef __cplusplus
}
#endif