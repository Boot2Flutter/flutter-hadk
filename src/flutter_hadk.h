// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_HADK_H_
#define FLUTTER_HADK_H_

#include "common.h"
#include <EGL/egl.h>

#include <thread>
#include <functional>
#include <queue>
#include <vector>

class FlutterHadk
{
public:
  FlutterHadk(FlutterHadkDisplay* ,FlutterEngineLib* lib,FlutterAppArgs* args);
  ~FlutterHadk();

  void init();
  void run();
  void cleanup();

  std::thread::id mainThreadID() { return main_thread_id_; }

private:
  EGLDisplay egl_display_ = EGL_NO_DISPLAY;
  EGLContext egl_context_ = EGL_NO_CONTEXT;
  EGLSurface egl_window_surface_ = EGL_NO_SURFACE;
  EGLContext egl_resource_context_ = EGL_NO_CONTEXT;
  EGLSurface egl_resource_surface_= EGL_NO_SURFACE;
  EGLConfig  egl_config_;

  FlutterHadkDisplay*   display_info_ = nullptr;
  FlutterEngineLib*     engine_lib_ = nullptr;
  FlutterAppArgs*       app_args_ = nullptr;
  FlutterEngine         engine_ = nullptr;

  bool valid_ = false;
  std::thread::id main_thread_id_;

  bool initEGL(void* window);
  bool cleanupEGL();

  bool initApplication();

  bool GLMakeCurrent();
  bool GLClearCurrent();
  bool GLPresent();
  uint32_t GLFboCallback();
  bool GLMakeResourceCurrent();

  class CompareFlutterTask {
   public:
    bool operator()(std::pair<uint64_t, FlutterTask> n1,
                    std::pair<uint64_t, FlutterTask> n2) {
      return n1.first > n2.first;
    }
  };
  std::priority_queue<std::pair<uint64_t, FlutterTask>,
                      std::vector<std::pair<uint64_t, FlutterTask>>,
                      CompareFlutterTask> TaskRunner;

  void postTaskCallback(FlutterTask task, uint64_t target_time);

};

#endif