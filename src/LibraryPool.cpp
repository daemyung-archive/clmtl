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

#include <Foundation/Foundation.hpp>
#include <spirv_cross/spirv_msl.hpp>

#include "Device.h"
#include "Program.h"

namespace cml {

LibraryPool::LibraryPool(Device *device) :
        mDevice{device}, mLibraries{} {
}

LibraryPool::~LibraryPool() {
    for (auto &[program, library] : mLibraries) {
        library->release();
    }
}

MTL::Library *LibraryPool::At(Program *program) {
    if (!mLibraries.count(program)) {
        AddLibrary(program);
    }

    return mLibraries[program];
}

void LibraryPool::AddLibrary(Program *program) {
    auto source = NS::String::alloc()->init(spirv_cross::CompilerMSL(program->GetBinary()).compile().c_str(),
                                            NS::UTF8StringEncoding);
    NS::Error *error;

    mLibraries[program] = mDevice->GetDevice()->newLibrary(source, nullptr, &error);
    assert(mLibraries[program]);

    source->release();

    if (error) {
        error->release();
    }
}

} //namespace cml
