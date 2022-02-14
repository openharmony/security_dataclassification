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

#ifndef DEV_SLINFO_LIST_H
#define DEV_SLINFO_LIST_H

#include "dev_slinfo_mgr.h"

#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

struct DATASLCallbackParams {
    DEVSLQueryParams queryParams;
    HigestSecInfoCallback *callback;
};

struct DATASLListParams {
    struct DATASLCallbackParams *callbackParams;
    struct DATASLListParams *prev;
    struct DATASLListParams *next;
};

struct DATASLListParams* ListInit(void);

int32_t PushList(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams);

void PopList(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams);

void ClearList(struct DATASLListParams *list);

int GetListLength(struct DATASLListParams *list);

int32_t FindList(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams);

#endif