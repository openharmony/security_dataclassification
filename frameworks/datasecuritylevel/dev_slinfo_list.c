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

#define WRITE_ONCE(var, val) \
       (*((volatile typeof(val) *)(&(var))) = (val))

DATASLListParams ListInit(void) {
    pthread_mutex_lock(&gMutex); // 加锁
    DATASLListParams list;
    list = (DATASLListParams)malloc(100 * sizeof(DATASLListParams));
    list->next = list;
    list->prev = list;
    pthread_mutex_unlock(&gMutex); // 解锁
    return list;
}


void Update(DATASLListParams new, DATASLListParams prev, DATASLListParams next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    WRITE_ONCE(prev->next, new);
}


void ListPush(DATASLListParams list, void *callbackCac) {
    DATA_SEC_LOG_INFO("ListPush, ret!");
    pthread_mutex_lock(&gMutex); // 加锁
    DATASLListParams newList = (DATASLListParams)malloc(100 * sizeof(DATASLListParams));
    if (list->prev == NULL) {
        list->prev = newList;
        list->next = newList;
        newList->prev = list;
        newList->next = list;
    } else {
        Update(newList, list->prev, list);
    }
    newList->callbackCac = (DATASLCallbackParams)callbackCac;
    pthread_mutex_unlock(&gMutex); // 解锁
    DATA_SEC_LOG_INFO("ListPush done, ret!");
}

void ListPop(DATASLListParams list,  void *callbackCac) {
    DATA_SEC_LOG_INFO("ListPop, ret!");
    pthread_mutex_lock(&gMutex); // 加锁
    DATASLListParams pList = list->next;
    while (pList != NULL && pList != list) {
        if (UdidCmp(pList->callbackCac->queryParams, ((DATASLCallbackParams)callbackCac)->queryParams) == SUCCESS) {
            pList->prev->next = pList->next;
            pList->next->prev = pList->prev;
            free(pList);
            break;
        }
        pList = pList->next;
    }
    pthread_mutex_unlock(&gMutex); // 解锁
    DATA_SEC_LOG_INFO("ListPop done, ret!");
}

void ListClear(DATASLListParams list) {
    pthread_mutex_lock(&gMutex); // 加锁
    DATASLListParams pList = list->next;
    while (pList == NULL || pList != list) {
        DATASLListParams delList = pList;
        pList = pList->next;
        free(delList);
    }
    pthread_mutex_unlock(&gMutex); // 解锁
}

int32_t ListLength(DATASLListParams list) {
    pthread_mutex_lock(&gMutex); // 加锁
    DATASLListParams pList = list->next;
    int32_t listLength = 0;
    while (pList != NULL && pList != list) {
        listLength++;
        pList = pList->next;
    }
    pthread_mutex_unlock(&gMutex); // 解锁
    return listLength;
}

int32_t ListFind(DATASLListParams list, void *callbackCac)
{
    pthread_mutex_lock(&gMutex); // 加锁
    DATA_SEC_LOG_INFO("ListFind, ret!");
    DATASLListParams pList = list->next;
    while (pList != NULL && pList != list) {
        if (UdidCmp(pList->callbackCac->queryParams, ((DATASLCallbackParams)callbackCac)->queryParams) == SUCCESS) {
            DATA_SEC_LOG_INFO("ListFind fine done, ret!");
            pthread_mutex_unlock(&gMutex); // 解锁
            return SUCCESS;
        }
        pList = pList->next;
    }
    DATA_SEC_LOG_INFO("ListFind not find, ret!");
    pthread_mutex_unlock(&gMutex); // 解锁
    return DEVSL_ERROR;
}
