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
project(demo C)

add_executable(demo
        src/Hello.c
)

target_compile_definitions(demo
    PRIVATE
        CL_TARGET_OPENCL_VERSION=100
)

target_link_libraries(demo
    PUBLIC
        clmtl
)
