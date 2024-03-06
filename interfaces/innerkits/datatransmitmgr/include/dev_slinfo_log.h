/*
 * Copyright (C) 2022-2024 Huawei Device Co., Ltd.
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
 
#include "hilog/log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "DataSl"

#ifdef LOG_DOMAIN
#undef LOG_DOMAIN
#endif
#define LOG_DOMAIN 0xD002F04

#define DATA_SEC_LOG_DEBUG(fmt, ...) HILOG_DEBUG(LOG_CORE, fmt, ##__VA_ARGS__)
#define DATA_SEC_LOG_INFO(fmt, ...) HILOG_INFO(LOG_CORE, fmt, ##__VA_ARGS__)
#define DATA_SEC_LOG_WARN(fmt, ...) HILOG_WARN(LOG_CORE, fmt, ##__VA_ARGS__)
#define DATA_SEC_LOG_ERROR(fmt, ...) HILOG_ERROR(LOG_CORE, fmt, ##__VA_ARGS__)

#else

#include <stdio.h>
#include <stdlib.h>

#define DATA_SEC_LOG_DEBUG(fmt, ...) printf("[DataSl][D][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_INFO(fmt, ...) printf("[DataSl][I][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_WARN(fmt, ...) printf("[DataSl][W][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define DATA_SEC_LOG_ERROR(fmt, ...) printf("[DataSl][E][%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

#endif
#endif