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
#include <spirv_cross/spirv_reflect.hpp>

#include "Dispatch.h"
#include "Device.h"
#include "Program.h"
#include "LibraryPool.h"

_cl_kernel::_cl_kernel(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace cml {

Kernel *Kernel::DownCast(cl_kernel kernel) {
    return dynamic_cast<Kernel *>(kernel);
}

Kernel::Kernel(Program *program, std::string name) :
        _cl_kernel{Dispatch::GetTable()}, Object{}, mProgram{program}, mName{std::move(name)},
        mFunction{nullptr}, mPipeline{nullptr} {
    InitFunction();
    InitPipeline();
}

Kernel::~Kernel() {
    mPipeline->release();
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

std::vector<Argument> Kernel::GetArgumentTable() const {
    return mArgumentTable;
}

size_t Kernel::GetWorkGroupSize() const {
    return mPipeline->maxTotalThreadsPerThreadgroup();
}

size_t Kernel::GetWorkItemExecutionWidth() const {
    return mPipeline->threadExecutionWidth();
}

void Kernel::InitFunction() {
    auto name = NS::String::alloc()->init(mName.c_str(), NS::UTF8StringEncoding);

    mFunction = Device::GetSingleton()->GetLibraryPool()->At(mProgram)->newFunction(name);
    assert(mFunction);

    name->release();
}


void Kernel::InitPipeline() {
    MTL::ComputePipelineReflection *reflection;
    NS::Error *error = nullptr;

    mPipeline = Device::GetSingleton()->GetDevice()->newComputePipelineState(mFunction, MTL::PipelineOptionArgumentInfo,
                                                                             &reflection, &error);
    assert(mPipeline);

    if (!error) {
        auto arguments = reflection->arguments();
        assert(arguments);

        for (auto i = 0; i != arguments->count(); ++i) {
            auto argument = (MTL::Argument *) arguments->object(i);
            assert(argument);

            mArgumentTable.push_back({argument->type()});
        }

        reflection->release();
    } else {
        error->release();
    }
}

} //namespace cml
