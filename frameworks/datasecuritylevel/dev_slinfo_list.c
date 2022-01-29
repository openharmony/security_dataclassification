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

struct DATASLListParams* ListInit(void)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *list;
    list = (struct DATASLListParams *)malloc(sizeof(struct DATASLListParams));
    list->next = list;
    list->prev = list;
    pthread_mutex_unlock(&gMutex);
    return list;
}

void Update(struct DATASLListParams *new, struct DATASLListParams *prev, struct DATASLListParams *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

void ListPush(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams)
{
    DATA_SEC_LOG_INFO("ListPush, ret!");
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *newList = (struct DATASLListParams*)malloc(sizeof(struct DATASLListParams));
    if (list->prev == NULL) {
        list->prev = newList;
        list->next = newList;
        newList->prev = list;
        newList->next = list;
    } else {
        Update(newList, list->prev, list);
    }
    newList->callbackParams = (struct DATASLCallbackParams*)callbackParams;
    pthread_mutex_unlock(&gMutex);
    DATA_SEC_LOG_INFO("ListPush done, ret!");
}

void ListPop(struct DATASLListParams *list,  struct DATASLCallbackParams *callbackParams)
{
    DATA_SEC_LOG_INFO("ListPop, ret!");
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *pList = list->next;
    while (pList != NULL && pList != list) {
        if (UdidCmp(pList->callbackParams->queryParams,
            ((struct DATASLCallbackParams*)callbackParams)->queryParams) == SUCCESS) {
            pList->prev->next = pList->next;
            pList->next->prev = pList->prev;
            free(pList);
            break;
        }
        pList = pList->next;
    }
    pthread_mutex_unlock(&gMutex);
    DATA_SEC_LOG_INFO("ListPop done, ret!");
}

void ListClear(struct DATASLListParams *list)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *pList = list->next;
    while (pList == NULL || pList != list) {
        struct DATASLListParams *delList = pList;
        pList = pList->next;
        free(delList);
    }
    pthread_mutex_unlock(&gMutex);
}

int32_t ListLength(struct DATASLListParams *list)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *pList = list->next;
    int32_t listLength = 0;
    while (pList != NULL && pList != list) {
        listLength++;
        pList = pList->next;
    }
    pthread_mutex_unlock(&gMutex);
    return listLength;
}

int32_t ListFind(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams)
{
    pthread_mutex_lock(&gMutex);
    DATA_SEC_LOG_INFO("ListFind, ret!");
    struct DATASLListParams *pList = list->next;
    while (pList != NULL && pList != list) {
        if (UdidCmp(pList->callbackParams->queryParams,
            ((struct DATASLCallbackParams*)callbackParams)->queryParams) == SUCCESS) {
            DATA_SEC_LOG_INFO("ListFind fine done, ret!");
            pthread_mutex_unlock(&gMutex);
            return SUCCESS;
        }
        pList = pList->next;
    }
    DATA_SEC_LOG_INFO("ListFind not find, ret!");
    pthread_mutex_unlock(&gMutex);
    return DEVSL_ERROR;
}