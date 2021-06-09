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
 * @file native_window.h
 * @brief API for hadk accessing a display/surface/window.
 */

#ifndef HADK_NATIVE_WINDOW_H
#define HADK_NATIVE_WINDOW_H

#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
*************************************************************
* DisplayInfo API
*************************************************************
*/

struct HDisplayInfo;
/**
 * Opaque type that provides access to a display info.
 *
 * A pointer can be obtained using {@link Hadk_GetDisplayInfo()}.
 */
typedef struct HDisplayInfo HDisplayInfo;

/**
 * Get a pointer to display info.
 */
HDisplayInfo* Hadk_GetDisplayInfo();

/**
 * Release display info pointer.
 */
void  Hadk_ReleaseDisplayInfo(HDisplayInfo* info);

/**
 * Get display width.
 */
uint32_t HDisplayInfo_getWidth(HDisplayInfo* info);

/**
 * Get display height.
 */
uint32_t HDisplayInfo_getHeight(HDisplayInfo* info);

/**
 * Get display density.
 */
float HDisplayInfo_getDensity(HDisplayInfo* info);

/**
*************************************************************
* NativeSurface API
*************************************************************
*/

struct HNativeSurface;
typedef struct HNativeSurface HNativeSurface;

/**
 * Create a pointer to native surface.
 */
HNativeSurface* Hadk_CreateNativeSurface(const char* name ,uint32_t width,uint32_t height);

/**
 * Get a native window pointer by native surface.
 */
void* HNativeWindow_fromSurface(HNativeSurface* surface);

/**
 * Release a pointer to native surface.
 */
void  Hadk_ReleaseNativeSurface(HNativeSurface* surface);


#ifdef __cplusplus
}
#endif

#endif  // HADK_NATIVE_WINDOW_H