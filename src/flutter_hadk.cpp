// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common.h"
#include "flutter_hadk.h"
#include <dlfcn.h>

FlutterHadk::FlutterHadk(FlutterHadkDisplay* display,FlutterEngineLib* lib,FlutterAppArgs* args) 
            : display_info_(display)
            , engine_lib_(lib) 
            , app_args_(args)
            , main_thread_id_(std::this_thread::get_id()) {
}

FlutterHadk::~FlutterHadk() {

}

void FlutterHadk::init() {
  if(!initEGL(display_info_->window)) {
    LOGE("InitializeEGL failed.");
    return;
  }

  if(!initApplication()) {
    LOGE("initApplication failed.");
    return;
  }
  valid_ = true;
}

void FlutterHadk::run() {

}

void FlutterHadk::cleanup() {

}

void FlutterHadk::postTaskCallback(FlutterTask task, uint64_t target_time) {
  TaskRunner.push(std::make_pair(target_time, task));
}

bool FlutterHadk::initApplication() {
  // configure flutter rendering
  FlutterRendererConfig config = {};
  config.type = kOpenGL;
  config.open_gl.struct_size = sizeof(config.open_gl);
  config.open_gl.make_current = [](void* context) -> bool {
    return reinterpret_cast<FlutterHadk*>(context)->GLMakeCurrent();
  };
  config.open_gl.clear_current = [](void* context) -> bool {
    return reinterpret_cast<FlutterHadk*>(context)->GLClearCurrent();
  };
  config.open_gl.present = [](void* context) -> bool {
    return reinterpret_cast<FlutterHadk*>(context)->GLPresent();
  };
  config.open_gl.fbo_callback = [](void* context) -> uint32_t {
    return reinterpret_cast<FlutterHadk*>(context)->GLFboCallback();
  };
  config.open_gl.make_resource_current = [](void* context) -> bool {
    return reinterpret_cast<FlutterHadk*>(context)->GLMakeResourceCurrent();
  };
  config.open_gl.gl_proc_resolver = [](void* context,
                                       const char* name) -> void* {
    auto address = eglGetProcAddress(name);
    if (address != nullptr) {
      return reinterpret_cast<void*>(address);
    }
    LOGE("Tried unsuccessfully to resolve");
    return nullptr;
  };

  FlutterProjectArgs args = { 0 };
  args.struct_size = sizeof(FlutterProjectArgs);
  args.assets_path = app_args_->asset_bundle_path;
  args.icu_data_path = app_args_->icu_data_path;
  args.command_line_argc = app_args_->engine_argc;
  args.command_line_argv = (const char * const*) app_args_->engine_argv;
  args.shutdown_dart_vm_when_done = true;

  // Configure task runner interop
  FlutterTaskRunnerDescription platform_task_runner = {};
  platform_task_runner.struct_size = sizeof(FlutterTaskRunnerDescription);
  platform_task_runner.user_data = this;
  platform_task_runner.runs_task_on_current_thread_callback =
      [](void* context) -> bool {
        return reinterpret_cast<FlutterHadk*>(context)->mainThreadID() == std::this_thread::get_id();
      };
  platform_task_runner.post_task_callback =
      [](FlutterTask task, uint64_t target_time, void* context) -> void {
        reinterpret_cast<FlutterHadk*>(context)->postTaskCallback(task,
                                                                  target_time);
      };

  FlutterCustomTaskRunners custom_task_runners = {};
  custom_task_runners.struct_size = sizeof(FlutterCustomTaskRunners);
  custom_task_runners.platform_task_runner = &platform_task_runner;
  args.custom_task_runners = &custom_task_runners;

  // check flutter runtime mode.
  bool engine_is_aot = engine_lib_->FlutterEngineRunsAOTCompiledDartCode();
  if ((engine_is_aot == true) && app_args_->runtime_mode != kRelease) {
    LOGE("flutter engine is release, but app runtime_mode is kDebug, quit");
		return false;
	} else if ((engine_is_aot == false) && app_args_->runtime_mode != kDebug) {
    LOGE("flutter engine is debug, but app runtime_mode is kRelease, quit");
		return false;
	}

  FlutterEngineResult engine_result;
  FlutterEngineAOTDataSource aot_source;
  FlutterEngineAOTData aot_data;
  if(app_args_->runtime_mode == kRelease) {
    aot_source.elf_path = app_args_->app_elf_path;
    aot_source.type = kFlutterEngineAOTDataSourceTypeElfPath;

    engine_result = engine_lib_->FlutterEngineCreateAOTData(&aot_source, &aot_data);
    if (engine_result != kSuccess) {
      LOGE("Could not load AOT data. FlutterEngineCreateAOTData: %d",engine_result);
      return false;
    }

    args.aot_data = aot_data;
  }

  engine_result = engine_lib_->FlutterEngineRun(FLUTTER_ENGINE_VERSION, &config, &args, NULL, &engine_);
  if (engine_result != kSuccess) {
    LOGE( "Could not run the flutter engine. FlutterEngineRun: %d", engine_result);
    return false;
  }

  FlutterWindowMetricsEvent event = {};
  event.struct_size = sizeof(event);
  event.width = display_info_->width;
  event.height = display_info_->height;
  event.pixel_ratio = display_info_->scale;
  engine_result = engine_lib_->FlutterEngineSendWindowMetricsEvent(engine_, &event);
  if (engine_result != kSuccess) {
    LOGE("Could not send window metrics to flutter engine.");
    return false;
  }

  return true;
}

bool FlutterHadk::initEGL(void* window) {
  // 1.Get the display.
  egl_display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (egl_display_ == EGL_NO_DISPLAY) {
    LOGE("InitializeEGL failed phase 1");
    return false;
  }

  // 2.Initialize the display connection.
  if (eglInitialize(egl_display_, nullptr, nullptr) != EGL_TRUE) {
    LOGE("InitializeEGL failed phase 2");
    return false;
  }

  // 3.Choose egl configuration
  EGLint config_attributes[] = {
      // clang-format off
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
      EGL_RED_SIZE,        8,
      EGL_GREEN_SIZE,      8,
      EGL_BLUE_SIZE,       8,
      EGL_ALPHA_SIZE,      8,
      EGL_DEPTH_SIZE,      0,
      EGL_STENCIL_SIZE,    0,
      EGL_NONE,            // termination sentinel
      // clang-format on
  };

  EGLint config_count = 0;

  if (eglChooseConfig(egl_display_, config_attributes, &egl_config_, 1, &config_count) !=
      EGL_TRUE ||  !(config_count > 0 && egl_config_ != nullptr)) {
    LOGE("InitializeEGL failed phase 3");
    return false;
  }


  // 4. create egl resource context
  EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  egl_resource_context_ = eglCreateContext(
      egl_display_, egl_config_, EGL_NO_CONTEXT, context_attributes);

  if (egl_resource_context_ == EGL_NO_CONTEXT) {
    LOGE("InitializeEGL failed phase 4");
    return false;
  }

  egl_context_ = eglCreateContext(egl_display_, egl_config_, egl_resource_context_,
                                  context_attributes);
  if (egl_context_ == EGL_NO_CONTEXT) {
    LOGE("InitializeEGL failed phase 5");
    return false;
  }

  // 5. create egl window surface
  const EGLint surfaceAttributes[] = {EGL_NONE};

  egl_window_surface_ = eglCreateWindowSurface(egl_display_, egl_config_,
                                               static_cast<EGLNativeWindowType>(window),
                                               surfaceAttributes);
  if (egl_window_surface_ == EGL_NO_SURFACE) {
    LOGE("InitializeEGL failed phase 6");
    return false;
  }

  // 6. create egl resource pbuffer surface
  const EGLint attribs[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
  egl_resource_surface_ = eglCreatePbufferSurface(egl_display_, egl_config_, attribs);
  return egl_resource_surface_ != EGL_NO_SURFACE;
}

bool FlutterHadk::cleanupEGL(){
  EGLBoolean result = EGL_FALSE;

  // 1. destroy egl window surface
  if (egl_display_ != EGL_NO_DISPLAY && egl_window_surface_ != EGL_NO_SURFACE) {
    result = eglDestroySurface(egl_display_, egl_window_surface_);
    egl_window_surface_ = EGL_NO_SURFACE;

    if (result == EGL_FALSE) {
      LOGE("cleanupEGL failed phase 1");
      return false;
    }
  }

  // 2. destory egl context
  if (egl_display_ != EGL_NO_DISPLAY && egl_context_ != EGL_NO_CONTEXT) {
    result = eglDestroyContext(egl_display_, egl_context_);
    egl_context_ = EGL_NO_CONTEXT;

    if (result == EGL_FALSE) {
      LOGE("cleanupEGL failed phase 2");
      return false;
    }
  }

  // 3. destroy egl resource pbuffer surface
  if (egl_display_ != EGL_NO_DISPLAY && egl_resource_surface_ != EGL_NO_SURFACE) {
    result = eglDestroySurface(egl_display_, egl_resource_surface_);
    egl_resource_surface_ = EGL_NO_SURFACE;

    if (result == EGL_FALSE) {
      LOGE("cleanupEGL failed phase 3");
      return false;
    }
  }

  // 4. destroy egl resource context
  if (egl_display_ != EGL_NO_DISPLAY &&
      egl_resource_context_ != EGL_NO_CONTEXT) {
    result = eglDestroyContext(egl_display_, egl_resource_context_);
    egl_resource_context_ = EGL_NO_CONTEXT;

    if (result == EGL_FALSE) {
      LOGE("cleanupEGL failed phase 4");
      return false;
    }
  }

  if (egl_display_ != EGL_NO_DISPLAY) {
    eglTerminate(egl_display_);
    egl_display_ = EGL_NO_DISPLAY;
  }

  return true;
}


bool FlutterHadk::GLMakeCurrent() {
  if (!valid_) {
    return false;
  }

  return (eglMakeCurrent(egl_display_, egl_window_surface_, egl_window_surface_, egl_context_) ==
      EGL_TRUE);
}

bool FlutterHadk::GLClearCurrent() {
  if (!valid_) {
    return false;
  }

  return (eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE,
                         EGL_NO_CONTEXT) == EGL_TRUE);
}

bool FlutterHadk::GLPresent() {
  if (!valid_) {
    return false;
  }

  return (eglSwapBuffers(egl_display_, egl_window_surface_) == EGL_TRUE);
}

uint32_t FlutterHadk::GLFboCallback() {
  if (!valid_) {
    return -1;
  }

  return 0;  // FBO0
}

bool FlutterHadk::GLMakeResourceCurrent() {
  if (!valid_) {
    return false;
  }

  return (eglMakeCurrent(egl_display_, egl_resource_surface_, egl_resource_surface_,
                         egl_resource_context_) == EGL_TRUE);
}


