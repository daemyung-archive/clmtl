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

uint64_t GetHash(const std::unordered_map<uint32_t, std::string> &defines) {
    std::stringstream stream;

    for (auto &[ordinal, define] : defines) {
        stream << define;
    }

    return std::hash<std::string>{}(stream.str());
}

void KeepResourceBindings(spirv_cross::CompilerMSL &compiler) {
    const auto resources = compiler.get_shader_resources();
    const auto stage = compiler.get_execution_model();

    for (const auto &resource : resources.uniform_buffers) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                           .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource : resources.storage_buffers) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                              .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource : resources.storage_images) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                              .msl_texture = binding, .msl_sampler = binding});
    }

    for (auto &resource : resources.sampled_images) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                              .msl_texture = binding, .msl_sampler = binding});
    }

    for (auto &resource : resources.separate_images) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                              .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource  : resources.separate_samplers) {
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    
        compiler.add_msl_resource_binding({.stage = stage, .binding = binding, .msl_buffer = binding,
                                              .msl_texture = binding, .msl_sampler = binding});
    }
}

LibraryPool::LibraryPool(Device *device) :
    mDevice{device}, mLibraries{}, mMslOptions{} {
    InitMslOptions();
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

void LibraryPool::InitMslOptions() {
    mMslOptions.set_msl_version(2, 3);
}

void LibraryPool::AddLibrary(Program *program, const std::string &defines) {
    auto compiler = spirv_cross::CompilerMSL(program->GetBinary());

    compiler.set_msl_options(mMslOptions);
    KeepResourceBindings(compiler);

    auto shader = defines + compiler.compile();
    auto source = NS::String::alloc()->init(shader.c_str(), NS::UTF8StringEncoding);
    NS::Error *error;

    mLibraries[program][defines] = mDevice->GetDevice()->newLibrary(source, nullptr, &error);
    assert(mLibraries[program][defines]);

    source->release();

    if (error) {
        error->release();
    }
}

} //namespace cml
