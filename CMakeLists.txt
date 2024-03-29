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
project(clmtl C CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(clmtl
        src/Metal.hpp
        src/Metal.cpp
        src/Driver.cpp
        src/Origin.h
        src/Size.h
        src/Dispatch.h
        src/Dispatch.cpp
        src/Util.h
        src/Util.cpp
        src/Platform.h
        src/Platform.cpp
        src/LibraryPool.h
        src/LibraryPool.cpp
        src/Device.h
        src/Device.cpp
        src/Object.h
        src/Object.cpp
        src/Context.h
        src/Context.cpp
        src/CommandQueue.h
        src/CommandQueue.cpp
        src/Memory.h
        src/Memory.cpp
        src/Buffer.h
        src/Buffer.cpp
        src/Image.h
        src/Image.cpp
        src/Program.h
        src/Program.cpp
        src/Reflector.h
        src/Reflector.cpp
        src/Kernel.h
        src/Kernel.cpp
        src/Event.h
        src/Event.cpp
        src/Sampler.h
        src/Sampler.cpp
)

target_compile_features(clmtl
    PUBLIC
        cxx_std_20
)

target_compile_definitions(clmtl
    PRIVATE
        CL_TARGET_OPENCL_VERSION=300
        CL_USE_DEPRECATED_OPENCL_1_0_APIS=1
        CL_USE_DEPRECATED_OPENCL_1_1_APIS=1
        CL_USE_DEPRECATED_OPENCL_1_2_APIS=1
        CL_USE_DEPRECATED_OPENCL_2_0_APIS=1
        CL_USE_DEPRECATED_OPENCL_2_1_APIS=1
        CL_USE_DEPRECATED_OPENCL_2_2_APIS=1
)

target_compile_options(clmtl
    PRIVATE
        -Wno-deprecated
)

target_link_libraries(clmtl
    PUBLIC
        "-framework Foundation"
        "-framework QuartzCore"
        "-framework Metal"
        ${CONAN_LIBS}
)

add_subdirectory(demo)
