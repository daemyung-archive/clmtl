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
project(khronos C)

add_library(khronos INTERFACE
        include/CL/opencl.h
        include/CL/cl.h
        include/CL/cl_d3d10.h
        include/CL/cl_d3d11.h
        include/CL/cl_dx9_media_sharing.h
        include/CL/cl_dx9_media_sharing_intel.h
        include/CL/cl_egl.h
        include/CL/cl_ext.h
        include/CL/cl_ext_intel.h
        include/CL/cl_gl.h
        include/CL/cl_gl_ext.h
        include/CL/cl_half.h
        include/CL/cl_icd.h
        include/CL/cl_layer.h
        include/CL/cl_platform.h
        include/CL/cl_va_api_media_sharing_intel.h
        include/CL/cl_version.h
        include/spirv-tools/instrument.hpp
        include/spirv-tools/libspirv.h
        include/spirv-tools/libspirv.hpp
        include/spirv-tools/linker.hpp
        include/spirv-tools/optimizer.hpp
)

target_include_directories(khronos
    INTERFACE
        include
)

target_link_directories(khronos
    INTERFACE
        lib)

target_link_libraries(khronos
    INTERFACE
        libSPIRV-Tools.a
        libSPIRV-Tools-diff.a
        libSPIRV-Tools-link.a
        libSPIRV-Tools-lint.a
        libSPIRV-Tools-opt.a
        libSPIRV-Tools-reduce.a
)
