/*
 * Copyright (C) 2013 Canonical Ltd
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
 *
 * Authored by: Michael Frey <michael.frey@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */
#include "binding.h"
#include "../include/hadk/native_window.h"
#include "../include/hadk/input.h"

#define HADK_LIBRARY_PATH "libhadk.so"
HADK_LIBRARY_INITIALIZE(hadk, HADK_LIBRARY_PATH)

/**
**********************************************************************
* native_window.h
**********************************************************************
*/

/**
* HDisplayInfo* 
* Hadk_GetDisplayInfo();
*/
HADK_IMPLEMENT_FUNCTION0( hadk,
                          HDisplayInfo*,
                          Hadk_GetDisplayInfo )

/**
*  void  
*  Hadk_ReleaseDisplayInfo(HDisplayInfo* info);
*/
HADK_IMPLEMENT_VOID_FUNCTION1( hadk,
                               Hadk_ReleaseDisplayInfo, 
                               HDisplayInfo* )

/**
* uint32_t 
* HDisplayInfo_getWidth(HDisplayInfo* info);
*/
HADK_IMPLEMENT_FUNCTION1( hadk,
                          uint32_t,
                          HDisplayInfo_getWidth,
                          HDisplayInfo* )

/**
* uint32_t 
* HDisplayInfo_getHeight(HDisplayInfo* info);
*/
HADK_IMPLEMENT_FUNCTION1( hadk,
                          uint32_t,
                          HDisplayInfo_getHeight,
                          HDisplayInfo* )

/**
* float 
* HDisplayInfo_getDensity(HDisplayInfo* info);
*/
HADK_IMPLEMENT_FUNCTION1( hadk,
                          float,
                          HDisplayInfo_getDensity,
                          HDisplayInfo* )

/**
*************************************************************
* NativeSurface API
*************************************************************
*/

/**
* HNativeSurface* 
* Hadk_CreateNativeSurface(const char* name ,uint32_t width,uint32_t height);
*/
HADK_IMPLEMENT_FUNCTION3( hadk,
                          HNativeSurface*,
                          Hadk_CreateNativeSurface,
                          const char*,uint32_t,uint32_t )

/**
* void* 
* HNativeWindow_fromSurface(HNativeSurface* surface);
*/
HADK_IMPLEMENT_FUNCTION1( hadk,
                          void*,
                          HNativeWindow_fromSurface,
                          HNativeSurface* )

/**
*  void  
*  Hadk_ReleaseNativeSurface(HNativeSurface* dispatcher);
*/
HADK_IMPLEMENT_VOID_FUNCTION1( hadk,
                               Hadk_ReleaseNativeSurface, 
                               HNativeSurface* )

/**
**********************************************************************
* input.h
**********************************************************************
*/

/**
* HInputDispatcher* 
* Hadk_CreateInputDispatcher(int32_t w,int32_t h,void* user_data,OnInputEvent callback);
*/
HADK_IMPLEMENT_FUNCTION4( hadk,
                          HInputDispatcher*,
                          Hadk_CreateInputDispatcher,
                          int32_t,int32_t,void*,OnInputEvent )
/**
* bool 
* HInputDispatcher_PumpEvent(HInputDispatcher* dispatcher);
*/
HADK_IMPLEMENT_FUNCTION1( hadk,
                          bool,
                          HInputDispatcher_PumpEvent,
                          HInputDispatcher* )

/**
*  void  
*  Hadk_ReleaseInputDispatcher(HInputDispatcher* dispatcher);
*/
HADK_IMPLEMENT_VOID_FUNCTION1( hadk,
                               Hadk_ReleaseInputDispatcher, 
                               HInputDispatcher* )