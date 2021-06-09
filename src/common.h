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
 *
 */

#ifndef FLUTTER_HADK_H
#define FLUTTER_HADK_H

#include <android/log.h>
#include "flutter/embedder.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "[fluttere-hadk]", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "[fluttere-hadk]", __VA_ARGS__))

#define FLWAY_DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete;

#define FLWAY_DISALLOW_ASSIGN(TypeName) \
  void operator=(const TypeName&) = delete;

#define FLWAY_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  FLWAY_DISALLOW_COPY(TypeName)                  \
  FLWAY_DISALLOW_ASSIGN(TypeName)

typedef struct {
  FlutterEngineResult (*FlutterEngineCreateAOTData)(const FlutterEngineAOTDataSource* source, FlutterEngineAOTData* data_out);
  FlutterEngineResult (*FlutterEngineCollectAOTData)(FlutterEngineAOTData data);
  FlutterEngineResult (*FlutterEngineRun)(size_t version, const FlutterRendererConfig* config, const FlutterProjectArgs* args, void* user_data, FlutterEngine *engine_out);
  FlutterEngineResult (*FlutterEngineShutdown)(FlutterEngine engine);
  FlutterEngineResult (*FlutterEngineInitialize)(size_t version, const FlutterRendererConfig* config, const FlutterProjectArgs* args, void* user_data, FlutterEngine *engine_out);
  FlutterEngineResult (*FlutterEngineDeinitialize)(FlutterEngine engine);
  FlutterEngineResult (*FlutterEngineRunInitialized)(FlutterEngine engine);
  FlutterEngineResult (*FlutterEngineSendWindowMetricsEvent)(FlutterEngine engine, const FlutterWindowMetricsEvent* event);
  FlutterEngineResult (*FlutterEngineSendPointerEvent)(FlutterEngine engine, const FlutterPointerEvent* events, size_t events_count);
  FlutterEngineResult (*FlutterEngineSendPlatformMessage)(FlutterEngine engine, const FlutterPlatformMessage* message);
  FlutterEngineResult (*FlutterPlatformMessageCreateResponseHandle)(FlutterEngine engine, FlutterDataCallback data_callback, void* user_data, FlutterPlatformMessageResponseHandle** response_out);
  FlutterEngineResult (*FlutterPlatformMessageReleaseResponseHandle)(FlutterEngine engine, FlutterPlatformMessageResponseHandle* response);
  FlutterEngineResult (*FlutterEngineSendPlatformMessageResponse)(FlutterEngine engine, const FlutterPlatformMessageResponseHandle* handle, const uint8_t* data, size_t data_length);
  FlutterEngineResult (*__FlutterEngineFlushPendingTasksNow)();
  FlutterEngineResult (*FlutterEngineRegisterExternalTexture)(FlutterEngine engine, int64_t texture_identifier);
  FlutterEngineResult (*FlutterEngineUnregisterExternalTexture)(FlutterEngine engine, int64_t texture_identifier);
  FlutterEngineResult (*FlutterEngineMarkExternalTextureFrameAvailable)(FlutterEngine engine, int64_t texture_identifier);
  FlutterEngineResult (*FlutterEngineUpdateSemanticsEnabled)(FlutterEngine engine, bool enabled);
  FlutterEngineResult (*FlutterEngineUpdateAccessibilityFeatures)(FlutterEngine engine, FlutterAccessibilityFeature features);
  FlutterEngineResult (*FlutterEngineDispatchSemanticsAction)(FlutterEngine engine, uint64_t id, FlutterSemanticsAction action, const uint8_t* data, size_t data_length);
  FlutterEngineResult (*FlutterEngineOnVsync)(FlutterEngine engine, intptr_t baton, uint64_t frame_start_time_nanos, uint64_t frame_target_time_nanos);
  FlutterEngineResult (*FlutterEngineReloadSystemFonts)(FlutterEngine engine);
  void (*FlutterEngineTraceEventDurationBegin)(const char* name);
  void (*FlutterEngineTraceEventDurationEnd)(const char* name);
  void (*FlutterEngineTraceEventInstant)(const char* name);
  FlutterEngineResult (*FlutterEnginePostRenderThreadTask)(FlutterEngine engine, VoidCallback callback, void* callback_data);
  uint64_t (*FlutterEngineGetCurrentTime)();
  FlutterEngineResult (*FlutterEngineRunTask)(FlutterEngine engine, const FlutterTask* task);
  FlutterEngineResult (*FlutterEngineUpdateLocales)(FlutterEngine engine, const FlutterLocale** locales, size_t locales_count);
  bool (*FlutterEngineRunsAOTCompiledDartCode)(void);
  FlutterEngineResult (*FlutterEnginePostDartObject)(FlutterEngine engine, FlutterEngineDartPort port, const FlutterEngineDartObject* object);
  FlutterEngineResult (*FlutterEngineNotifyLowMemoryWarning)(FlutterEngine engine);
  FlutterEngineResult (*FlutterEnginePostCallbackOnAllNativeThreads)(FlutterEngine engine, FlutterNativeThreadCallback callback, void* user_data);
} FlutterEngineLib;

typedef enum {
	kDebug, 
  kRelease
}FlutterRuntimeMode;

typedef struct {
  char *asset_bundle_path;
  char *kernel_blob_path;
  char *app_elf_path;
  void *app_elf_handle;
  char *icu_data_path;

  int engine_argc;
  char **engine_argv;
  FlutterRuntimeMode runtime_mode;
} FlutterAppArgs;

typedef struct {
  void* window;
  int32_t width;
  int32_t height;
  float scale;
} FlutterHadkDisplay;

#endif