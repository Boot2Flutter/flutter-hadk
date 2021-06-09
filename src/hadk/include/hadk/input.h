/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file native_input.h
 * @brief API for hadk accessing input event.
 */

#ifndef HADK_NATIVE_INPUT_H
#define HADK_NATIVE_INPUT_H

#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
*************************************************************
* NativeInput API
*************************************************************
*/

struct HInputDispatcher;
typedef struct HInputDispatcher HInputDispatcher;

typedef int32_t (*OnInputEvent)(void* user_data, void* event);

/**
 * Create a pointer to intput dispatcher.
 */
HInputDispatcher* Hadk_CreateInputDispatcher(int32_t w,int32_t h,void* user_data,OnInputEvent callback);

/**
 * Pump input queue to get input event pointer.
 */
bool HInputDispatcher_PumpEvent(HInputDispatcher* dispatcher);

/**
 * Release a pointer to input queue.
 */
void  Hadk_ReleaseInputDispatcher(HInputDispatcher* dispatcher);

#ifdef __cplusplus
}
#endif

#endif  // HADK_NATIVE_INPUT_H