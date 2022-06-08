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

#include "Program.h"

#include <clspv/Compiler.h>
#include <spirv-tools/linker.hpp>

#include "Dispatch.h"

_cl_program::_cl_program(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace cml {

Program *Program::DownCast(cl_program program) {
    return dynamic_cast<Program *>(program);
}

Program::Program(Context *context) :
        _cl_program{Dispatch::GetTable()}, Object{}, mContext{context}, mSources{}, mOptions{}, mBinary{},
        mLog{}, mBuildStatus{CL_BUILD_NONE} {
}

void Program::AddSource(const std::string &source) {
    mSources.push_back(source);
}

std::vector<std::vector<uint32_t>> Program::Compile() {
    std::vector<std::vector<uint32_t>> binaries;

    mBuildStatus = CL_BUILD_SUCCESS;
    for (auto &source : mSources) {
        std::vector<uint32_t> binary;

        if (!clspv::CompileFromSourceString(source, "", mOptions, &binary, &mLog)) {
            binaries.push_back(binary);
        } else {
            mBuildStatus = CL_BUILD_ERROR;

            return binaries;
        }
    }

    return binaries;
}

void Program::Link(const std::vector<std::vector<uint32_t>> &binaries) {
    auto spvContext = spvtools::Context(SPV_ENV_OPENCL_1_2);

    if (!spvtools::Link(spvContext, binaries, &mBinary)) {
        mBuildStatus = CL_BUILD_SUCCESS;
    } else {
        mBuildStatus = CL_BUILD_ERROR;
    }
}

void Program::SetOptions(const std::string &options) {
    mOptions = options;
}

void Program::SetBinary(const std::vector<uint32_t> &binary) {
    mBinary = binary;
}

Context *Program::GetContext() const {
    return mContext;
}

std::string Program::GetSource() const {
    return mSources[0];
}

std::string Program::GetOptions() const {
    return mOptions;
}

cl_build_status Program::GetBuildStatus() const {
    return mBuildStatus;
}

std::string Program::GetLog() const {
    return mLog;
}

std::vector<uint32_t> Program::GetBinary() const {
    return mBinary;
}

} //namespace cml
