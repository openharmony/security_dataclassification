/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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

#ifndef DEV_SLINFO_LOG_H
#define DEV_SLINFO_LOG_H

#ifdef HILOG_ENABLE

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3
} LogLevel;

#ifdef __cplusplus
extern "C" {
#endif

void DataSlLogPrint(LogLevel level, const char *funName, const char *fmt, ...);

#ifdef __cplusplus
}
#endif
 
#include "hilog/log.h"

#ifndef LOG_DOMAIN
#define LOG_DOMAIN 0xD002F10 /* Security subsystem's domain id */
#endif

#define DATA_SEC_LOG_DEBUG(fmt, ...) (DataSlLogPrint(LOG_LEVEL_DEBUG, __FUNCTION__, fmt, ##__VA_ARGS__))
#define DATA_SEC_LOG_INFO(fmt, ...) (DataSlLogPrint(LOG_LEVEL_INFO, __FUNCTION__, fmt, ##__VA_ARGS__))
#define DATA_SEC_LOG_WARN(fmt, ...) (DataSlLogPrint(LOG_LEVEL_WARN, __FUNCTION__, fmt, ##__VA_ARGS__))
#define DATA_SEC_LOG_ERROR(fmt, ...) (DataSlLogPrint(LOG_LEVEL_ERROR, __FUNCTION__, fmt, ##__VA_ARGS__))

#define DataSl_LOG_DEBUG(fmt, ...) HiLogPrint(LOG_CORE, LOG_DEBUG, LOG_DOMAIN, "[DataSl]", "%{public}s", buf)
#define DataSl_LOG_INFO(buf) HiLogPrint(LOG_CORE, LOG_INFO, LOG_DOMAIN, "[DataSl]", "%{public}s", buf)
#define DataSl_LOG_WARN(buf) HiLogPrint(LOG_CORE, LOG_WARN, LOG_DOMAIN, "[DataSl]", "%{public}s", buf)
#define DataSl_LOG_ERROR(buf) HiLogPrint(LOG_CORE, LOG_ERROR, LOG_DOMAIN, "[DataSl]", "%{public}s", buf)

#else

#include <stdio.h>

#define DATA_SEC_LOG_DEBUG(fmt, ...) printf("[DataSl][D][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_INFO(fmt, ...) printf("[DataSl][I][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_WARN(fmt, ...) printf("[DataSl][W][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_ERROR(fmt, ...) printf("[DataSl][E][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

#endif
#endif