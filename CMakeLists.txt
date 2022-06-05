########################################################################################################################
# Copyright (c) 2022-2022 Daemyung Jang.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
########################################################################################################################

cmake_minimum_required(VERSION 3.18)
project(clmtl CXX)

add_subdirectory(deps)

add_library(clmtl
        src/Driver.cpp
        src/Dispatch.h
        src/Dispatch.cpp
        src/Util.h
        src/Util.cpp
        src/Platform.h
        src/Platform.cpp
        src/Device.h
        src/Device.cpp
        src/Object.h
        src/Object.cpp
        src/Context.h
        src/Context.cpp
)

target_compile_features(clmtl
    PUBLIC
        cxx_std_20
)

target_compile_definitions(clmtl
    PRIVATE
        CL_TARGET_OPENCL_VERSION=100
)

target_link_libraries(clmtl
    PUBLIC
        khronos
        apple
)
