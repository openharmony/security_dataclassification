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
#include <pthread.h>
#include "dev_slinfo_adpt.h"
#include "dev_slinfo_log.h"

pthread_mutex_t gMutex;

struct DATASLListParams* InitList(void)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *list = (struct DATASLListParams *)malloc(sizeof(struct DATASLListParams));
    if (list == NULL) {
        pthread_mutex_unlock(&gMutex);
        return NULL;
    }
    list->next = list;
    list->prev = list;
    pthread_mutex_unlock(&gMutex);
    return list;
}

static void Update(struct DATASLListParams *new, struct DATASLListParams *prev, struct DATASLListParams *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

int32_t PushList(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams)
{
    DATA_SEC_LOG_INFO("PushList, ret!");
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *newList = (struct DATASLListParams*)malloc(sizeof(struct DATASLListParams));
    if (newList == NULL) {
        pthread_mutex_unlock(&gMutex);
        return DEVSL_ERR_MALLOC_FAIL;
    }
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
    DATA_SEC_LOG_INFO("PushList done, ret!");
    return SUCCESS;
}

void PopList(struct DATASLListParams *list,  struct DATASLCallbackParams *callbackParams)
{
    DATA_SEC_LOG_INFO("PopList, ret!");
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *pList = list->next;
    while (pList != NULL && pList != list) {
        if (CompareUdid(&(pList->callbackParams->queryParams), &(callbackParams->queryParams)) == SUCCESS) {
            pList->prev->next = pList->next;
            pList->next->prev = pList->prev;
            free(pList->callbackParams);
            free(pList);
            break;
        }
        pList = pList->next;
    }
    pthread_mutex_unlock(&gMutex);
    DATA_SEC_LOG_INFO("PopList done, ret!");
}

void ClearList(struct DATASLListParams *list)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *pList = list->next;
    while (pList == NULL || pList != list) {
        struct DATASLListParams *delList = pList;
        pList = pList->next;
        free(delList->callbackParams);
        free(delList);
    }
    pthread_mutex_unlock(&gMutex);
}

int32_t GetListLength(struct DATASLListParams *list)
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

int32_t FindList(struct DATASLListParams *list, struct DATASLCallbackParams *callbackParams)
{
    pthread_mutex_lock(&gMutex);
    DATA_SEC_LOG_INFO("DATASL: FindList, ret!");
    struct DATASLListParams *pList = list->next;
    DATA_SEC_LOG_INFO("DATASL: list is not NULL!");
    while (pList != NULL && pList != list) {
        DATA_SEC_LOG_INFO("DATASL: while is ok!");
        if (CompareUdid(&(pList->callbackParams->queryParams), &(callbackParams->queryParams)) == SUCCESS) {
            DATA_SEC_LOG_INFO("FindList fine done, ret!");
            pthread_mutex_unlock(&gMutex);
            return SUCCESS;
        }
        pList = pList->next;
    }
    DATA_SEC_LOG_INFO("FindList not find, ret!");
    pthread_mutex_unlock(&gMutex);
    return DEVSL_ERROR;
}

void LookupCallback(struct DATASLListParams *list, DEVSLQueryParams *queryParams, int32_t result, uint32_t levelInfo)
{
    pthread_mutex_lock(&gMutex);
    struct DATASLListParams *tmpCallback = list->next;
    while (tmpCallback != NULL && tmpCallback != list) {
        struct DATASLListParams *nextCallback = tmpCallback->next;
        int32_t ret = CompareUdid(&(tmpCallback->callbackParams->queryParams), queryParams);
        if (ret == SUCCESS) {
            tmpCallback->callbackParams->callback(&(tmpCallback->callbackParams->queryParams), result, levelInfo);
            tmpCallback->prev->next = tmpCallback->next;
            tmpCallback->next->prev = tmpCallback->prev;
            free(tmpCallback->callbackParams);
            free(tmpCallback);
        }
        tmpCallback = nextCallback;
    }
    pthread_mutex_unlock(&gMutex);
}

int32_t InitPthreadMutex(void)
{
    int32_t ret;
    ret = pthread_mutex_init(&gMutex, NULL);
    return ret;
}

void DestroyPthreadMutex(void)
{
    pthread_mutex_destroy(&gMutex);
}