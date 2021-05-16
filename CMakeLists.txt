#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID_STL=c++_static -std=c++14 -Wall -Werror")

add_executable(
    flutter-hadk
    src/main.cpp
    )

###########################################################################

#import flutter embedder
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# add_library(flutter_embedder SHARED IMPORTED)
# set_target_properties(flutter_embedder
#         PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libflutter_embedder.so)

###########################################################################


# add lib dependencies
# target_link_libraries(flutter-hadk
#     android
#     flutter_embedder
#     EGL
#     GLESv1_CM
#     log)