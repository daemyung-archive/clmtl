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

#include "LibraryPool.h"

#include <sstream>
#include <Foundation/Foundation.hpp>

#include "Device.h"
#include "Program.h"

namespace cml {

LibraryPool::LibraryPool(Device *device)
    : mDevice{device}, mLibraries{} {
}

LibraryPool::~LibraryPool() {
    for (auto &[program, libraries] : mLibraries) {
        for (auto &[defines, library] : libraries) {
            library->release();
        }
    }
}

MTL::Library *LibraryPool::At(Program *program, const std::string &defines) {
    if (!mLibraries.count(program) || !mLibraries.at(program).count(defines)) {
        AddLibrary(program, defines);
    }

    return mLibraries[program][defines];
}

void LibraryPool::AddLibrary(Program *program, const std::string &source) {
    auto shader = NS::String::alloc()->init(source.c_str(), NS::UTF8StringEncoding);
    NS::Error *error;

    mLibraries[program][source] = mDevice->GetDevice()->newLibrary(shader, nullptr, &error);
    assert(mLibraries[program][source]);

    shader->release();

    if (error) {
        error->release();
    }
}

} //namespace cml
