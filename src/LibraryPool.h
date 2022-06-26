/***********************************************************************************************************************
* Copyright (c) 2022-2022 Daemyung Jang.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***********************************************************************************************************************/

#ifndef CLMTL_LIBRARY_POOL_H
#define CLMTL_LIBRARY_POOL_H

#include <unordered_map>
#include <vector>
#include <spirv_cross/spirv_msl.hpp>
#include <Metal/Metal.hpp>

namespace cml {

class Device;
class Program;

class LibraryPool {
public:
    explicit LibraryPool(Device *device);
    ~LibraryPool();
    MTL::Library *At(Program *program, const std::string &defines);

private:
    Device *mDevice;
    std::unordered_map<Program *, std::unordered_map<std::string, MTL::Library *>> mLibraries;
    spirv_cross::CompilerMSL::Options mMslOptions;

    void InitMslOptions();
    void AddLibrary(Program *program, const std::string &defines);
};

} //namespace cml

#endif //CLMTL_LIBRARY_POOL_H
