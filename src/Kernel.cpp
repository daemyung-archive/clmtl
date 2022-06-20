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

#include "Kernel.h"

#include <cassert>
#include <Foundation/NSString.hpp>

#include "Dispatch.h"
#include "Device.h"
#include "Program.h"
#include "LibraryPool.h"

namespace cml {

uint64_t GetHash(const std::array<size_t, 3> &size) {
    return (size[0] << 42) | (size[1] << 21) | (size[2] << 0);
}

MTL::FunctionConstantValues *CreateConstantValues(const std::array<size_t, 3> &workGroupSize) {
    auto values = MTL::FunctionConstantValues::alloc()->init();

    values->setConstantValue(&workGroupSize[0], MTL::DataTypeUInt, 0ul);
    values->setConstantValue(&workGroupSize[1], MTL::DataTypeUInt, 1ul);
    values->setConstantValue(&workGroupSize[2], MTL::DataTypeUInt, 2ul);

    return values;
}

Kernel *Kernel::DownCast(cl_kernel kernel) {
    return (Kernel *) kernel;
}

Kernel::Kernel(Program *program, const std::string &name)
    : _cl_kernel{Dispatch::GetTable()}, Object{}, mProgram{program}, mName{name}
    , mBindings{program->GetReflection().at(name)}, mPipelineStates{}, mArgTable{} {
    InitPipelineState();
    InitArgTable();
}

Kernel::~Kernel() {
    for (auto &[hash, pipelineState] : mPipelineStates) {
        pipelineState->release();
    }
}

void Kernel::SetArg(size_t index, const void *data, size_t size) {
    if (data) {
        memcpy(mArgTable[index].Data, data, size);
    } else {
        memset(mArgTable[index].Data, 0, size);
    }

    mArgTable[index].Size = size;
}

Context *Kernel::GetContext() const {
    return mProgram->GetContext();
}

Program *Kernel::GetProgram() const {
    return mProgram;
}

std::string Kernel::GetName() const {
    return mName;
}

MTL::ComputePipelineState *Kernel::GetPipelineState(const std::array<size_t, 3> &workGroupSize) {
    auto hash = GetHash(workGroupSize);

    if (!mPipelineStates.count(hash)) {
        AddPipelineState(hash, workGroupSize);
    }

    return mPipelineStates[hash];
}

size_t Kernel::GetWorkGroupSize() const {
    return mPipelineStates.at(0)->maxTotalThreadsPerThreadgroup();
}

size_t Kernel::GetWorkItemExecutionWidth() const {
    return mPipelineStates.at(0)->threadExecutionWidth();
}

std::unordered_map<uint32_t, Arg> Kernel::GetArgTable() const {
    return mArgTable;
}

void Kernel::InitPipelineState() {
    try {
        AddPipelineState(0, {1, 1, 1});
    } catch (std::exception &e) {
        Release();

        throw e;
    }
}

void Kernel::InitArgTable() {
    for (auto &binding : mBindings) {
        mArgTable[binding.Ordinal] = {.Kind = binding.Kind, .Binding = binding.Index};
    }
}

MTL::Function *Kernel::CreateFunction(const std::array<size_t, 3> &workGroupSize) {
    auto name = NS::String::alloc()->init(mName.c_str(), NS::UTF8StringEncoding);
    auto constantValues = CreateConstantValues(workGroupSize);
    NS::Error *error = nullptr;

    auto function = Device::GetSingleton()->GetLibraryPool()->At(mProgram)->newFunction(name, constantValues, &error);
    assert(function);

    name->release();
    constantValues->release();

    if (error) {
        error->release();
    }

    return function;
}

void Kernel::AddPipelineState(uint64_t hash, const std::array<size_t, 3> &workGroupSize) {
    auto function = CreateFunction(workGroupSize);
    NS::Error *error = nullptr;

    mPipelineStates[hash] = Device::GetSingleton()->GetDevice()->newComputePipelineState(function, &error);

    function->release();

    if (error) {
        error->release();

        throw std::exception();
    }
}

} //namespace cml
